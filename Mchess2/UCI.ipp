#pragma once

#include "Macros_and_lib.ipp"
#include "Constans.h"
#include "MoveGeneration.ipp"
#include "Prints.h"
#include "SearchBeastMove.ipp"

#include <cstdio>
#include <cstring>
#include <cctype>
#include <string>
#include <algorithm>

namespace UCI {

    // ---- вспомогалки ----
    static inline bool is_cmd(const char* s, const char* cmd) {
        // точное совпадение по префиксу + дальше либо конец строки, либо пробел/таб/перевод строки
        const size_t n = std::strlen(cmd);
        return std::strncmp(s, cmd, n) == 0 && (s[n] == '\0' || std::isspace(static_cast<unsigned char>(s[n])));
    }

    static inline void rstrip_crlf(char* s) {
        size_t len = std::strlen(s);
        while (len && (s[len - 1] == '\n' || s[len - 1] == '\r')) {
            s[--len] = '\0';
        }
    }

    // ---- UCI: парс одного хода в наш внутренний move ----
    int ParseMove(std::string move_string)
    {
        // базовая защита: нужно минимум 4 символа (e2e4)
        if (move_string.size() < 4) return 0;

        moves move_list[1];
        MoveGeneration::GenMoves(move_list);

        int source_square = (move_string[0] - 'a') + (8 - (move_string[1] - '0')) * 8;
        int target_square = (move_string[2] - 'a') + (8 - (move_string[3] - '0')) * 8;

        for (int move_count = 0; move_count < move_list->count; move_count++)
        {
            int move = move_list->llmoves[move_count];
            if (source_square == get_move_source(move) && target_square == get_move_target(move))
            {
                int promoted_piece = get_move_promoted(move);
                if (promoted_piece)
                {
                    if (move_string.size() >= 5) {
                        if ((promoted_piece == Q || promoted_piece == q) && move_string[4] == 'q') return move;
                        else if ((promoted_piece == R || promoted_piece == r) && move_string[4] == 'r') return move;
                        else if ((promoted_piece == B || promoted_piece == b) && move_string[4] == 'b') return move;
                        else if ((promoted_piece == N || promoted_piece == n) && move_string[4] == 'n') return move;
                    }
                    continue;
                }
                return move;
            }
        }
        return 0;
    }

    // ---- UCI: position ... ----
    void ParsePosition(std::string command)
    {
        // убираем префикс "position " в начале (ровно 9 символов), если есть
        if (command.rfind("position ", 0) == 0) {
            command.erase(0, 9);
        }

        // сделаем C-строку для удобства strstr/atoi
        char* cmd = new char[command.size() + 1];
        std::copy(command.begin(), command.end(), cmd);
        cmd[command.size()] = '\0';

        char* cur = cmd;

        // 1) установить позицию: либо стартовую, либо по FEN
        // "startpos"
        if (std::strncmp(cur, "startpos", 8) == 0) {
            MoveGeneration::ParseFEN(start_position);
            cur += 8; // сдвинулись за слово startpos
            while (*cur == ' ') ++cur;
        }
        // "fen <...>"
        else if (std::strncmp(cur, "fen", 3) == 0) {
            cur += 3; // сдвиг за "fen"
            while (*cur == ' ') ++cur;
            // FEN идёт до слова "moves" или до конца строки
            const char* moves_kw = std::strstr(cur, "moves");
            std::string fen = moves_kw
                ? std::string(cur, moves_kw - cur)
                : std::string(cur);
            // отрезаем хвостовые пробелы у FEN
            while (!fen.empty() && std::isspace(static_cast<unsigned char>(fen.back()))) fen.pop_back();

            if (!fen.empty()) {
                MoveGeneration::ParseFEN(fen);
            }
            // перенесём указатель на "moves" (если было)
            cur = const_cast<char*>(moves_kw ? moves_kw : (cmd + command.size()));
        }
        // иначе: если ни startpos, ни fen — не меняем позицию (остаётся как была)

        // 2) применить список ходов, если есть "moves"
        char* mkw = std::strstr(cur, "moves");
        if (mkw) {
            cur = mkw + 5;               // прошли слово "moves"
            while (*cur == ' ') ++cur;   // пропустили пробел

            while (*cur) {
                // взять один токен до пробела
                const char* sp = cur;
                while (*sp && !std::isspace(static_cast<unsigned char>(*sp))) ++sp;
                std::string mv(cur, sp - cur);

                if (!mv.empty()) {
                    int m = UCI::ParseMove(mv);
                    if (m) {
                        MoveGeneration::MakeMove(m, all_mv);
                        // если не хочется видеть применяемые ходы в логе, закомментируй:
                        std::printf("info string applied move %s\n", mv.c_str());
                    } else {
                        // ход не распознан — можно сообщить в лог
                        std::printf("info string bad move %s\n", mv.c_str());
                    }
                }

                cur = const_cast<char*>(sp);
                while (*cur && std::isspace(static_cast<unsigned char>(*cur))) ++cur;
            }
        }

        // минимальная диагностическая строка для GUI
        std::printf("info string position set\n");
        std::fflush(stdout);

        delete[] cmd;
    }

    // ---- UCI: go ... ----
    void ParseGo(std::string command)
    {
        int depth = -1;

        char* cmd = new char[command.size() + 1];
        std::copy(command.begin(), command.end(), cmd);
        cmd[command.size()] = '\0';

        char* current_depth = std::strstr(cmd, (char*)"depth");
        if (current_depth) {
            // "depth N" => взять число после пробела
            depth = std::atoi(current_depth + 6); // пропустим "depth "
        } else {
            depth = 5; // дефолтная глубина, если не указали
        }

        MoveSearcher::SearchBeastMove(depth);

        delete[] cmd;
    }

    // ---- главный цик UCI ----
    void Loop() {
        setbuf(stdin, NULL);
        setbuf(stdout, NULL);

        char input[2048];

        for (;;) {
            if (!std::fgets(input, sizeof(input), stdin)) {
                break; // EOF / ошибка ввода
            }

            rstrip_crlf(input); // аккуратно снимаем \n и \r (Windows-совместимо)

            // ---- обработка UCI-команд ----
            if (is_cmd(input, "uci")) {
                std::printf("id name Mchess\n");
                std::printf("id author GovnoCode\n");
                std::printf("uciok\n");
                std::fflush(stdout);
            }
            else if (is_cmd(input, "isready")) {
                std::printf("readyok\n");
                std::fflush(stdout);
            }
            else if (is_cmd(input, "ucinewgame")) {
                // при необходимости — сброс внутреннего состояния на новую партию
                // например: ClearHash(); ClearHistory(); и т.п.
            }
            else if (is_cmd(input, "position")) {
                ParsePosition(std::string(input));
            }
            else if (is_cmd(input, "go")) {
                ParseGo(std::string(input));
            }
            else if (is_cmd(input, "stop")) {
                // если есть флаг остановки поиска — поставь его здесь
            }
            else if (is_cmd(input, "quit")) {
                break;
            }
            else {
                // необязательная диагностика:
                // std::printf("info string unknown: %s\n", input);
                // std::fflush(stdout);
            }
        }
    }
} // namespace UCI

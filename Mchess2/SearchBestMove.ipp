#pragma once

#include <cstdio>

namespace MoveSearcher {

    void PrintMove(int move) {
        if (get_move_promoted(move))
            std::printf("%s%s%c\n",
                        square_to_coordinates[get_move_source(move)],
                        square_to_coordinates[get_move_target(move)],
                        promoted_pieces[get_move_promoted(move)]);
        else
            std::printf("%s%s\n",
                        square_to_coordinates[get_move_source(move)],
                        square_to_coordinates[get_move_target(move)]);
    }

    //P, N, B, R, Q, K, p, n, b, r, q, k
    int material_score[12]{
        100, 320, 330, 500, 900, 20000, -100, -320, -330, -500, -900, -20000
    };

    int Evalute() {
        BRD_U64 BRD;
        int piece, sq, score = 0;
        for (int i = P; i <= k; i++)
        {
            BRD = ALL_BRDS[i];
            while (BRD)
            {
                piece = i;
                sq = get_ls1b_index(BRD);
                score += material_score[piece];

                switch (piece) {
                case P: score += P_score[sq]; break;
                case N: score += N_score[sq]; break;
                case R: score += R_score[sq]; break;
                case B: score += B_score[sq]; break;
                case K: score += K_score[sq]; break;

                case p: score -= P_score[MirrorScore[sq]]; break;
                case n: score -= N_score[MirrorScore[sq]]; break;
                case r: score -= R_score[MirrorScore[sq]]; break;
                case b: score -= B_score[MirrorScore[sq]]; break;
                case k: score -= K_score[MirrorScore[sq]]; break;
                }

                pp_bt(BRD, sq);
            }
        }
        return (side == WHT) ? score : -score;
    }

    BRD_U64 nodes = 0, ply = 0;
    int best_move = 0;

    int NegaMax(int alpha, int beta, int depth)
    {
        if (depth == 0)
            return Evalute();

        nodes++;
        int in_check = MoveGeneration::square_is_attacked(
            (side == WHT) ? get_ls1b_index(ALL_BRDS[K]) : get_ls1b_index(ALL_BRDS[k]),
            side ^= 1
        );
        side ^= 1;

        int best_sofar = -1000, legal_moves = 0;
        int old_alpha = alpha;

        moves move_list[1];
        MoveGeneration::GenMoves(move_list);

        for (int count = 0; count < move_list->count; count++)
        {
            Backup();
            ply++;

            if (MoveGeneration::MakeMove(move_list->llmoves[count], all_mv) == 0)
            {
                ply--;
                continue;
            }
            legal_moves++;

            int score = -NegaMax(-beta, -alpha, depth - 1);

            ply--;
            RecoveryBRD();

            if (score >= beta) {
                return beta;
            }

            if (score > alpha) {
                alpha = score;
                if (ply == 0)
                    best_sofar = move_list->llmoves[count];
            }
        }

        // Если надо, можно вернуть маты/паты, закомментировано в твоём исходнике
         if (legal_moves == 0) {
             if (in_check)   return -49000 + ply;
             else            return 0;
         }

        if (old_alpha != alpha)
            best_move = best_sofar;

        return alpha;
    }

    void SearchBeastMove(int depth) {
        // сбрасываем счётчики перед КАЖДЫМ поиском
        nodes = 0;
        ply   = 0;
        best_move = 0;

        int score = NegaMax(-50000, 50000, depth);

        if (best_move) {
            // Сформируем ход "e2e4" или "e7e8q"
            char mv[6]; // максимум 5 + '\0'
            std::snprintf(mv, sizeof(mv), "%s%s",
                          square_to_coordinates[get_move_source(best_move)],
                          square_to_coordinates[get_move_target(best_move)]);
            if (get_move_promoted(best_move)) {
                mv[4] = promoted_pieces[get_move_promoted(best_move)]; // q/r/b/n
                mv[5] = '\0';
            }

            // информативный info для GUI
            std::printf("info score cp %d depth %d nodes %llu pv %s\n",
                        score, depth, (unsigned long long)nodes, mv);

            // собственно лучший ход
            std::printf("bestmove %s\n", mv);
            std::fflush(stdout);
        } else {
            // на случай отсутствия легальных ходов / ошибки
            std::printf("bestmove 0000\n");
            std::fflush(stdout);
        }
    }
}

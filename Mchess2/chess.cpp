#define _CRT_SECURE_NO_WARNINGS
#include "Attacks.ipp"
#include "Tests.h"
#include "UCI.ipp"
#include "SearchBeastMove.ipp"
#include<thread>
#include<chrono>

int main()
{
    attacks::init_all();
    bool debug = false;
    if (debug) {
        MoveGeneration::ParseFEN(start_position);
        int i = 0;
        while (i < 20) {
            MoveSearcher::SearchBeastMove(3);
            MoveGeneration::MakeMove(MoveSearcher::best_move, all_mv);
            PrintAllPosition();
            ++i;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            system("cls");
        }
    }
    else UCI::Loop();
    return 0;
}
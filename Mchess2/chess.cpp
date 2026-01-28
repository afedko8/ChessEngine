#define _CRT_SECURE_NO_WARNINGS
#include "Attacks.ipp"
#include "Tests.h"
#include "UCI.ipp"
#include "SearchBestMove.ipp"
#include<thread>
#include<chrono>
//#include"PerftDriver.ipp"
#include<ctime>

int main()
{
    attacks::init_all();
    bool debug = 0;
    if (debug) {
        MoveGeneration::ParseFEN(Lichess_bot1);
        std::cout << MoveGeneration::MakeMove(encode_move(e1, c1, K, 0, 0, 0, 0, 1), all_mv) << '\n';
        PrintAllPosition();
    }
    else UCI::Loop();
    return 0;
}
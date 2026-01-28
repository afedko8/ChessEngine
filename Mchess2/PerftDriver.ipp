#pragma once
#include "Constants.h"
#include "MacrosAndLib.ipp"
#include "MoveGeneration.ipp"
namespace PerftDriver {
    static inline BRD_U64 PerftDriver(int depth)
    {
        if (depth == 0)
        {
            return 1ULL;
        }
        BRD_U64 nodes = 0;
        moves move_list[1];
        MoveGeneration::GenMoves(move_list);
        for (int move_count = 0; move_count < move_list->count; move_count++)
        {
            Backup();
            if (!MoveGeneration::MakeMove(move_list->llmoves[move_count], all_mv))
                continue;
            nodes += PerftDriver(depth - 1);
            RecoveryBRD();
        }
        return nodes;
    }

    static inline BRD_U64 PerftDriverWithPrint(int depth)
    {
        if (depth == 0)
        {
            return 1ULL;
        }
        BRD_U64 nodes = 0, last_nodes = 0;
        moves move_list[1];
        MoveGeneration::GenMoves(move_list);
        for (int move_count = 0; move_count < move_list->count; move_count++)
        {
            Backup();
            int move = move_list->llmoves[move_count];
            if (!MoveGeneration::MakeMove(move, all_mv)) continue;
            last_nodes = PerftDriver(depth - 1);
            nodes += last_nodes;
            printf("%s%s: %llu\n", square_to_coordinates[get_move_source(move)], square_to_coordinates[get_move_target(move)], last_nodes);
            RecoveryBRD();
        }
        return nodes;
    }
}

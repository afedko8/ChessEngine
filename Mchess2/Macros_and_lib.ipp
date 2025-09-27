#pragma once
#include <iostream>
#include <cstring>
#include <unordered_map>
#include <vector>

typedef unsigned long long BRD_U64;

#define gt_bt(bitboard, square) ((bitboard) & (1ULL << (square)))
#define st_bt1(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define gt_bt(bitboard, square) ((bitboard) & (1ULL << (square)))
#define pp_bt(bitboard, square) (gt_bt(bitboard, square) ? (bitboard) ^= (1ULL << (square)) : 0)

#define encode_move(source, target, piece, promoted, capture, DDpush, enpssp, castling) \
    (source) |          \
    (target << 6) |     \
    (piece << 12) |     \
    (promoted << 16) |  \
    (capture << 20) |   \
    (DDpush << 21) |    \
    (enpssp << 22) | \
    (castling << 23)    \

#define get_move_source(move) (move & 0x3f)
#define get_move_target(move) ((move & 0xfc0) >> 6ULL)
#define get_move_piece(move) ((move & 0xf000) >> 12ULL)
#define get_move_promoted(move) ((move & 0xf0000) >> 16ULL)
#define get_move_capture(move) (move & 0x100000)
#define get_move_double(move) (move & 0x200000)
#define get_move_enpssp(move) (move & 0x400000)
#define get_move_castling(move) (move & 0x800000)



static inline BRD_U64 cntBTon(BRD_U64 a)
{
    a = (a & 0x5555555555555555) + ((a & 0xAAAAAAAAAAAAAAAA) >> 1);
    a = (a & 0x3333333333333333) + ((a & 0xCCCCCCCCCCCCCCCC) >> 2);
    a = (a & 0x0F0F0F0F0F0F0F0F) + ((a & 0xF0F0F0F0F0F0F0F0) >> 4);
    a = (a & 0x00FF00FF00FF00FF) + ((a & 0xFF00FF00FF00FF00) >> 8);
    a = (a & 0x0000FFFF0000FFFF) + ((a & 0xFFFF0000FFFF0000) >> 16);
    a = (a & 0x00000000FFFFFFFF) + ((a & 0xFFFFFFFF00000000) >> 32);
    return a;
}

static inline BRD_U64 get_ls1b_index(BRD_U64 BRD)
{
    if (BRD)
    {
        return cntBTon((BRD & -1 * BRD) - 1);
    }

    else
        return -1;
}


BRD_U64 SToccup(int index, int bits_in_mask, BRD_U64 attack_mask)
{
    BRD_U64 occupancy = 0ULL;

    for (int count = 0; count < bits_in_mask; count++)
    {
        int square = get_ls1b_index(attack_mask);

        pp_bt(attack_mask, square);

        if (index & (1 << count))
            occupancy |= (1ULL << square);
    }

    return occupancy;
}



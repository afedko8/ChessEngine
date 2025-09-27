#pragma once
#include "Constans.h"
#include "Macros_and_lib.ipp"
#include "Attacks.ipp"
namespace MoveGeneration {
    void ParseFEN(std::string FEN) {

        memset(ALL_BRDS, 0ULL, sizeof(ALL_BRDS));

        memset(ALL_OCCUP, 0ULL, sizeof(ALL_OCCUP));

        side = 0;
        enpssp = no_sq;
        cstl = 0;

        bool agree = true;
        if (agree) {
            int fen = 0;

            for (int rank = 0; rank < 8; rank++)
            {
                for (int file = 0; file < 8; file++)
                {
                    int square = rank * 8 + file;

                    if ((FEN[fen] >= 'a' && FEN[fen] <= 'z') || (FEN[fen] >= 'A' && FEN[fen] <= 'Z')) {

                        int piece = char_pieces[FEN[fen]];
                        st_bt1(ALL_BRDS[piece], square);

                        if ((FEN[fen] >= 'a' && FEN[fen] <= 'z')) st_bt1(ALL_OCCUP[BLCK], square);
                        else st_bt1(ALL_OCCUP[WHT], square);

                        fen++;
                    }

                    if (FEN[fen] >= '0' && FEN[fen] <= '9') {

                        int piece = -1;

                        for (int bb_p = P; bb_p <= k; bb_p++)
                        {
                            if (gt_bt(ALL_BRDS[bb_p], square)) piece = bb_p;
                        }

                        if (piece == -1) file--;

                        file += FEN[fen] - '0';

                        fen++;

                    }
                    if (FEN[fen] == '/') fen++;
                }
            }
            fen++;
            if (FEN[fen] == 'b') side = BLCK;

            fen += 2;

            while (FEN[fen] != ' ')
            {
                switch (FEN[fen])
                {
                case 'K': cstl |= wk; break;
                case 'Q': cstl |= wq; break;
                case 'k': cstl |= bk; break;
                case 'q': cstl |= bq; break;
                case '-': break;
                }

                fen++;
            }

            fen++;

            //if (FEN[fen] != '-')
            //{
            //    // parse enpssp file & rank
            //    int file = FEN[0] - 'a';
            //    int rank = 8 - (FEN[1] - '0');

            //    // init enpssp square
            //    enpssp = rank * 8 + file;
            //}

            //// no enpssp square
            //else
            //    enpssp = no_sq;

            for (int piece = P; piece <= K; piece++)
                ALL_OCCUP[WHT] |= ALL_BRDS[piece];

            for (int piece = p; piece <= k; piece++)
                ALL_OCCUP[BLCK] |= ALL_BRDS[piece];

            ALL_OCCUP[BTH] = ALL_OCCUP[WHT] | ALL_OCCUP[BLCK];


        }
        else {
            //printf("Invalid syntax of FEN string\n %s",errors);
        }

    }


    /*
              binary move bits

        0000 0000 0000 0000 0011 1111    source square
        0000 0000 0000 1111 1100 0000    target square
        0000 0000 1111 0000 0000 0000    piece
        0000 1111 0000 0000 0000 0000    promoted piece
        0001 0000 0000 0000 0000 0000    capture flag
        0010 0000 0000 0000 0000 0000    double push flag
        0100 0000 0000 0000 0000 0000    enpssp flag
        1000 0000 0000 0000 0000 0000    castling flag
    */


    bool square_is_attacked(int sq, int side) {
        if ((side == WHT) && (P_attacks[BLCK][sq] & ALL_BRDS[P])) return 1;
        if ((side == BLCK) && (P_attacks[WHT][sq] & ALL_BRDS[p])) return 1;
        if (N_attacks[sq] & ((side == WHT) ? ALL_BRDS[N] : ALL_BRDS[n])) return 1;
        if (K_attacks[sq] & ((side == WHT) ? ALL_BRDS[K] : ALL_BRDS[k])) return 1;
        if (attacks::get_bishop_attacks(sq, ALL_OCCUP[BTH]) & ((side == WHT) ? ALL_BRDS[B] : ALL_BRDS[b])) return 1;
        if (attacks::get_rook_attacks(sq, ALL_OCCUP[BTH]) & ((side == WHT) ? ALL_BRDS[R] : ALL_BRDS[r])) return 1;
        if (attacks::get_queen_attacks(sq, ALL_OCCUP[BTH]) & ((side == WHT) ? ALL_BRDS[Q] : ALL_BRDS[q])) return 1;
        return 0;
    }

    void add_move(moves* strmoves, int move) {
        strmoves->llmoves[strmoves->count] = move;
        strmoves->count++;
    }

#define Backup()                                                      \
    BRD_U64 ALL_BRDS_copy[12], ALL_OCCUP_copy[3];                          \
    int side_copy, enpssp_copy, castle_copy;                           \
    memcpy(ALL_BRDS_copy, ALL_BRDS, 96);                                \
    memcpy(ALL_OCCUP_copy, ALL_OCCUP, 24);                            \
    side_copy = side, enpssp_copy = enpssp, castle_copy = cstl;   \

#define RecoveryBRD()                                                       \
    memcpy(ALL_BRDS, ALL_BRDS_copy, 96);                                \
    memcpy(ALL_OCCUP, ALL_OCCUP_copy, 24);                            \
    side = side_copy, enpssp = enpssp_copy, cstl = castle_copy;   \


    void GenMoves(moves* move_list)
    {

        move_list->count = 0;

        int source_square, target_square;

        BRD_U64 bitboard, attacks;

        for (int piece = P; piece <= k; piece++)
        {

            bitboard = ALL_BRDS[piece];

            if (side == WHT)
            {

                if (piece == P)
                {

                    while (bitboard)
                    {

                        source_square = get_ls1b_index(bitboard);

                        target_square = source_square - 8;

                        if (!(target_square < a8) && !gt_bt(ALL_OCCUP[BTH], target_square))
                        {
                            if (source_square >= a7 && source_square <= h7)
                            {
                                add_move(move_list, encode_move(source_square, target_square, piece, Q, 0, 0, 0, 0));
                                add_move(move_list, encode_move(source_square, target_square, piece, R, 0, 0, 0, 0));
                                add_move(move_list, encode_move(source_square, target_square, piece, B, 0, 0, 0, 0));
                                add_move(move_list, encode_move(source_square, target_square, piece, N, 0, 0, 0, 0));
                            }
                            else
                            {
                                add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

                                if ((source_square >= a2 && source_square <= h2) && !gt_bt(ALL_OCCUP[BTH], target_square - 8))
                                    add_move(move_list, encode_move(source_square, target_square - 8, piece, 0, 0, 1, 0, 0));
                            }
                        }

                        attacks = P_attacks[side][source_square] & ALL_OCCUP[BLCK];

                        while (attacks)
                        {

                            target_square = get_ls1b_index(attacks);

                            if (source_square >= a7 && source_square <= h7)
                            {
                                add_move(move_list, encode_move(source_square, target_square, piece, Q, 1, 0, 0, 0));
                                add_move(move_list, encode_move(source_square, target_square, piece, R, 1, 0, 0, 0));
                                add_move(move_list, encode_move(source_square, target_square, piece, B, 1, 0, 0, 0));
                                add_move(move_list, encode_move(source_square, target_square, piece, N, 1, 0, 0, 0));
                            }

                            else

                                add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

                            pp_bt(attacks, target_square);
                        }

                        if (enpssp != no_sq)
                        {

                            BRD_U64 enpssp_attacks = P_attacks[side][source_square] & (1ULL << enpssp);

                            if (enpssp_attacks)
                            {

                                int target_enpssp = get_ls1b_index(enpssp_attacks);
                                add_move(move_list, encode_move(source_square, target_enpssp, piece, 0, 1, 0, 1, 0));
                            }
                        }

                        pp_bt(bitboard, source_square);
                    }
                }

                if (piece == K)
                {

                    if (cstl & wk)
                    {

                        if (!gt_bt(ALL_OCCUP[BTH], f1) && !gt_bt(ALL_OCCUP[BTH], g1))
                        {

                            if (!square_is_attacked(e1, BLCK) && !square_is_attacked(f1, BLCK))
                                add_move(move_list, encode_move(e1, g1, piece, 0, 0, 0, 0, 1));
                        }
                    }

                    if (cstl & wq)
                    {

                        if (!gt_bt(ALL_OCCUP[BTH], d1) && !gt_bt(ALL_OCCUP[BTH], c1) && gt_bt(ALL_OCCUP[BTH], b1))
                        {

                            if (!square_is_attacked(e1, BLCK) && !square_is_attacked(d1, BLCK))
                                add_move(move_list, encode_move(e1, c1, piece, 0, 0, 0, 0, 1));
                        }
                    }
                }
            }

            else
            {

                if (piece == p)
                {

                    while (bitboard)
                    {

                        source_square = get_ls1b_index(bitboard);

                        target_square = source_square + 8;

                        if (!(target_square > h1) && !gt_bt(ALL_OCCUP[BTH], target_square))
                        {

                            if (source_square >= a2 && source_square <= h2)
                            {
                                add_move(move_list, encode_move(source_square, target_square, piece, q, 0, 0, 0, 0));
                                add_move(move_list, encode_move(source_square, target_square, piece, r, 0, 0, 0, 0));
                                add_move(move_list, encode_move(source_square, target_square, piece, b, 0, 0, 0, 0));
                                add_move(move_list, encode_move(source_square, target_square, piece, n, 0, 0, 0, 0));
                            }

                            else
                            {

                                add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

                                if ((source_square >= a7 && source_square <= h7) && !gt_bt(ALL_OCCUP[BTH], target_square + 8))
                                    add_move(move_list, encode_move(source_square, target_square + 8, piece, 0, 0, 1, 0, 0));
                            }
                        }

                        attacks = P_attacks[side][source_square] & ALL_OCCUP[WHT];

                        while (attacks)
                        {

                            target_square = get_ls1b_index(attacks);

                            if (source_square >= a2 && source_square <= h2)
                            {
                                add_move(move_list, encode_move(source_square, target_square, piece, q, 1, 0, 0, 0));
                                add_move(move_list, encode_move(source_square, target_square, piece, r, 1, 0, 0, 0));
                                add_move(move_list, encode_move(source_square, target_square, piece, b, 1, 0, 0, 0));
                                add_move(move_list, encode_move(source_square, target_square, piece, n, 1, 0, 0, 0));
                            }

                            else

                                add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

                            pp_bt(attacks, target_square);
                        }

                        if (enpssp != no_sq)
                        {

                            BRD_U64 enpssp_attacks = P_attacks[side][source_square] & (1ULL << enpssp);

                            if (enpssp_attacks)
                            {

                                int target_enpssp = get_ls1b_index(enpssp_attacks);
                                add_move(move_list, encode_move(source_square, target_enpssp, piece, 0, 1, 0, 1, 0));
                            }
                        }

                        pp_bt(bitboard, source_square);
                    }
                }

                if (piece == k)
                {

                    if (cstl & bk)
                    {

                        if (!gt_bt(ALL_OCCUP[BTH], f8) && !gt_bt(ALL_OCCUP[BTH], g8))
                        {

                            if (!square_is_attacked(e8, WHT) && !square_is_attacked(f8, WHT))
                                add_move(move_list, encode_move(e8, g8, piece, 0, 0, 0, 0, 1));
                        }
                    }

                    if (cstl & bq)
                    {

                        if (!gt_bt(ALL_OCCUP[BTH], d8) && !gt_bt(ALL_OCCUP[BTH], c8) && !gt_bt(ALL_OCCUP[BTH], b8))
                        {

                            if (!square_is_attacked(e8, WHT) && !square_is_attacked(d8, WHT))
                                add_move(move_list, encode_move(e8, c8, piece, 0, 0, 0, 0, 1));
                        }
                    }
                }
            }

            if ((side == WHT) ? piece == N : piece == n)
            {

                while (bitboard)
                {
                    source_square = get_ls1b_index(bitboard);

                    attacks = N_attacks[source_square] & ((side == WHT) ? ~ALL_OCCUP[WHT] : ~ALL_OCCUP[BLCK]);

                    while (attacks)
                    {

                        target_square = get_ls1b_index(attacks);

                        if (!gt_bt(((side == WHT) ? ALL_OCCUP[BLCK] : ALL_OCCUP[WHT]), target_square))
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

                        else

                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

                        pp_bt(attacks, target_square);
                    }

                    pp_bt(bitboard, source_square);
                }
            }

            if ((side == WHT) ? piece == B : piece == b)
            {

                while (bitboard)
                {

                    source_square = get_ls1b_index(bitboard);

                    attacks = attacks::get_bishop_attacks(source_square, ALL_OCCUP[BTH]) & ((side == WHT) ? ~ALL_OCCUP[WHT] : ~ALL_OCCUP[BLCK]);

                    while (attacks)
                    {

                        target_square = get_ls1b_index(attacks);

                        if (!gt_bt(((side == WHT) ? ALL_OCCUP[BLCK] : ALL_OCCUP[WHT]), target_square))
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

                        else

                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

                        pp_bt(attacks, target_square);
                    }

                    pp_bt(bitboard, source_square);
                }
            }

            if ((side == WHT) ? piece == R : piece == r)
            {

                while (bitboard)
                {

                    source_square = get_ls1b_index(bitboard);

                    attacks = attacks::get_rook_attacks(source_square, ALL_OCCUP[BTH]) & ((side == WHT) ? ~ALL_OCCUP[WHT] : ~ALL_OCCUP[BLCK]);

                    while (attacks)
                    {

                        target_square = get_ls1b_index(attacks);

                        if (!gt_bt(((side == WHT) ? ALL_OCCUP[BLCK] : ALL_OCCUP[WHT]), target_square))
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

                        else

                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

                        pp_bt(attacks, target_square);
                    }

                    pp_bt(bitboard, source_square);
                }
            }

            if ((side == WHT) ? piece == Q : piece == q)
            {

                while (bitboard)
                {

                    source_square = get_ls1b_index(bitboard);

                    attacks = attacks::get_queen_attacks(source_square, ALL_OCCUP[BTH]) & ((side == WHT) ? ~ALL_OCCUP[WHT] : ~ALL_OCCUP[BLCK]);

                    while (attacks)
                    {

                        target_square = get_ls1b_index(attacks);

                        if (!gt_bt(((side == WHT) ? ALL_OCCUP[BLCK] : ALL_OCCUP[WHT]), target_square))
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

                        else

                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

                        pp_bt(attacks, target_square);
                    }

                    pp_bt(bitboard, source_square);
                }
            }

            if ((side == WHT) ? piece == K : piece == k)
            {

                while (bitboard)
                {

                    source_square = get_ls1b_index(bitboard);

                    attacks = K_attacks[source_square] & ((side == WHT) ? ~ALL_OCCUP[WHT] : ~ALL_OCCUP[BLCK]);

                    while (attacks)
                    {

                        target_square = get_ls1b_index(attacks);

                        if (!gt_bt(((side == WHT) ? ALL_OCCUP[BLCK] : ALL_OCCUP[WHT]), target_square))
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));

                        else

                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

                        pp_bt(attacks, target_square);
                    }

                    pp_bt(bitboard, source_square);
                }
            }
        }
    }

    int MakeMove(int move, int move_flag)
    {
        if (move_flag == all_mv)
        {
            Backup();

            int source_square = get_move_source(move);
            int target_square = get_move_target(move);
            int piece = get_move_piece(move);
            int promoted_piece = get_move_promoted(move);
            int capture = get_move_capture(move);
            int double_push = get_move_double(move);
            int enpass = get_move_enpssp(move);
            int castling = get_move_castling(move);

            pp_bt(ALL_BRDS[piece], source_square);
            st_bt1(ALL_BRDS[piece], target_square);

            if (capture)
            {
                int start_piece, end_piece;

                if (side == WHT)
                {
                    start_piece = p;
                    end_piece = k;
                }

                else
                {
                    start_piece = P;
                    end_piece = K;
                }

                for (int bb_piece = start_piece; bb_piece <= end_piece; bb_piece++)
                {
                    if (gt_bt(ALL_BRDS[bb_piece], target_square))
                    {
                        pp_bt(ALL_BRDS[bb_piece], target_square);
                        break;
                    }
                }
            }

            if (promoted_piece)
            {
                pp_bt(ALL_BRDS[(side == WHT) ? P : p], target_square);

                st_bt1(ALL_BRDS[promoted_piece], target_square);
            }

            if (enpass)
            {
                (side == WHT) ? pp_bt(ALL_BRDS[p], target_square + 8) :
                    pp_bt(ALL_BRDS[P], target_square - 8);
            }

            enpssp = no_sq;

            if (double_push)
            {
                (side == WHT) ? (enpssp = target_square + 8) :
                    (enpssp = target_square - 8);
            }

            if (castling)
            {
                switch (target_square)
                {
                case (g1):
                    pp_bt(ALL_BRDS[R], h1);
                    st_bt1(ALL_BRDS[R], f1);
                    break;

                case (c1):
                    pp_bt(ALL_BRDS[R], a1);
                    st_bt1(ALL_BRDS[R], d1);
                    break;

                case (g8):
                    pp_bt(ALL_BRDS[r], h8);
                    st_bt1(ALL_BRDS[r], f8);
                    break;

                case (c8):
                    pp_bt(ALL_BRDS[r], a8);
                    st_bt1(ALL_BRDS[r], d8);
                    break;
                }
            }

            cstl &= castling_rights[source_square];
            cstl &= castling_rights[target_square];

            memset(ALL_OCCUP, 0ULL, 24);

            for (int bb_piece = P; bb_piece <= K; bb_piece++)
                ALL_OCCUP[WHT] |= ALL_BRDS[bb_piece];

            for (int bb_piece = p; bb_piece <= k; bb_piece++)
                ALL_OCCUP[BLCK] |= ALL_BRDS[bb_piece];

            ALL_OCCUP[BTH] |= ALL_OCCUP[WHT];
            ALL_OCCUP[BTH] |= ALL_OCCUP[BLCK];
            side ^= 1;

            if (square_is_attacked((side == WHT) ? get_ls1b_index(ALL_BRDS[k]) : get_ls1b_index(ALL_BRDS[K]), side))
            {
                RecoveryBRD();
                return 0;
            }
            else
                return 1;
        }
        else
        {
            if (get_move_capture(move))
                MakeMove(move, all_mv);
            else
                return 0;
        }
    }
}
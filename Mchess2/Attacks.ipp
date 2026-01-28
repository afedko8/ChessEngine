#pragma once
#include"Constants.h"

namespace attacks {

    BRD_U64 P_msk_att(int side, int square)
    {
        BRD_U64 attacks = 0ULL;
        BRD_U64 BRD = 0ULL;
        st_bt1(BRD, square);
        if (!side)
        {
            if ((BRD >> 7) & not_a_file) attacks |= (BRD >> 7);
            if ((BRD >> 9) & not_h_file) attacks |= (BRD >> 9);
        }
        else
        {
            if ((BRD << 7) & not_h_file) attacks |= (BRD << 7);
            if ((BRD << 9) & not_a_file) attacks |= (BRD << 9);
        }

        return attacks;
    }

    BRD_U64 N_msk_att(int square)
    {
        BRD_U64 attacks = 0ULL;
        BRD_U64 BRD = 0ULL;
        st_bt1(BRD, square);
        if ((BRD >> 17) & not_h_file) attacks |= (BRD >> 17);
        if ((BRD >> 15) & not_a_file) attacks |= (BRD >> 15);
        if ((BRD >> 10) & not_hg_file) attacks |= (BRD >> 10);
        if ((BRD >> 6) & not_ab_file) attacks |= (BRD >> 6);

        if ((BRD << 17) & not_a_file) attacks |= (BRD << 17);
        if ((BRD << 15) & not_h_file) attacks |= (BRD << 15);
        if ((BRD << 10) & not_ab_file) attacks |= (BRD << 10);
        if ((BRD << 6) & not_hg_file) attacks |= (BRD << 6);
        return attacks;
    }

    BRD_U64 K_msk_att(int square)
    {
        BRD_U64 attacks = 0ULL;

        BRD_U64 BRD = 0ULL;

        st_bt1(BRD, square);

        if (BRD >> 8) attacks |= (BRD >> 8);
        if ((BRD >> 9) & not_h_file) attacks |= (BRD >> 9);
        if ((BRD >> 7) & not_a_file) attacks |= (BRD >> 7);
        if ((BRD >> 1) & not_h_file) attacks |= (BRD >> 1);

        if (BRD << 8) attacks |= (BRD << 8);
        if ((BRD << 9) & not_a_file) attacks |= (BRD << 9);
        if ((BRD << 7) & not_h_file) attacks |= (BRD << 7);
        if ((BRD << 1) & not_a_file) attacks |= (BRD << 1);

        return attacks;
    }

    BRD_U64 B_msk_att(int square)
    {
        BRD_U64 attacks = 0ULL;

        int r, f;

        int tr = square / 8;
        int tf = square % 8;

        for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++) attacks |= (1ULL << (r * 8 + f));
        for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++) attacks |= (1ULL << (r * 8 + f));
        for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--) attacks |= (1ULL << (r * 8 + f));
        for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--) attacks |= (1ULL << (r * 8 + f));

        return attacks;
    }

    BRD_U64 R_msk_att(int square)
    {
        BRD_U64 attacks = 0ULL;

        int r, f;

        int tr = square / 8;
        int tf = square % 8;

        for (r = tr + 1; r <= 6; r++) attacks |= (1ULL << (r * 8 + tf));
        for (r = tr - 1; r >= 1; r--) attacks |= (1ULL << (r * 8 + tf));
        for (f = tf + 1; f <= 6; f++) attacks |= (1ULL << (tr * 8 + f));
        for (f = tf - 1; f >= 1; f--) attacks |= (1ULL << (tr * 8 + f));

        return attacks;
    }

    BRD_U64 B_attacks(int square, BRD_U64 block)
    {
        BRD_U64 attacks = 0ULL;

        int r, f;

        int tr = square / 8;
        int tf = square % 8;

        for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++)
        {
            attacks |= (1ULL << (r * 8 + f));
            if ((1ULL << (r * 8 + f)) & block) break;
        }

        for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++)
        {
            attacks |= (1ULL << (r * 8 + f));
            if ((1ULL << (r * 8 + f)) & block) break;
        }

        for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--)
        {
            attacks |= (1ULL << (r * 8 + f));
            if ((1ULL << (r * 8 + f)) & block) break;
        }

        for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--)
        {
            attacks |= (1ULL << (r * 8 + f));
            if ((1ULL << (r * 8 + f)) & block) break;
        }

        return attacks;
    }

    BRD_U64 R_attacks(int square, BRD_U64 block)
    {
        BRD_U64 attacks = 0ULL;

        int r, f;

        int tr = square / 8;
        int tf = square % 8;

        for (r = tr + 1; r <= 7; r++)
        {
            attacks |= (1ULL << (r * 8 + tf));
            if ((1ULL << (r * 8 + tf)) & block) break;
        }

        for (r = tr - 1; r >= 0; r--)
        {
            attacks |= (1ULL << (r * 8 + tf));
            if ((1ULL << (r * 8 + tf)) & block) break;
        }

        for (f = tf + 1; f <= 7; f++)
        {
            attacks |= (1ULL << (tr * 8 + f));
            if ((1ULL << (tr * 8 + f)) & block) break;
        }

        for (f = tf - 1; f >= 0; f--)
        {
            attacks |= (1ULL << (tr * 8 + f));
            if ((1ULL << (tr * 8 + f)) & block) break;
        }

        return attacks;
    }

    BRD_U64 find_magic_number(int square, int relevant_bits, int bishop)
    {
        BRD_U64 ALL_OCCUP[4096];

        BRD_U64 attacks[4096];

        BRD_U64 used_attacks[4096];

        BRD_U64 attack_mask = bishop ? B_msk_att(square) : R_msk_att(square);

        int occupancy_indicies = 1 << relevant_bits;

        for (int index = 0; index < occupancy_indicies; index++)
        {
            ALL_OCCUP[index] = SToccup(index, relevant_bits, attack_mask);

            attacks[index] = bishop ? B_attacks(square, ALL_OCCUP[index]) :
                R_attacks(square, ALL_OCCUP[index]);
        }

        for (int random_count = 0; random_count < 100000000; random_count++)
        {
            BRD_U64 magic_number = generate_magic_number();

            if (cntBTon((attack_mask * magic_number) & 0xFF00000000000000) < 6) continue;

            memset(used_attacks, 0ULL, sizeof(used_attacks));

            int index, fail;

            for (index = 0, fail = 0; !fail && index < occupancy_indicies; index++)
            {
                int magic_index = (int)((ALL_OCCUP[index] * magic_number) >> (64 - relevant_bits));

                if (used_attacks[magic_index] == 0ULL)
                    used_attacks[magic_index] = attacks[index];

                else if (used_attacks[magic_index] != attacks[index])
                    fail = 1;
            }

            if (!fail)
                return magic_number;
        }

        printf("  Magic number fails!\n");
        return 0ULL;
    }

    void init_magic_numbers()
    {
        for (int square = 0; square < 64; square++)
            R_MN[square] = find_magic_number(square, R_relev[square], R);

        for (int square = 0; square < 64; square++)
            B_MN[square] = find_magic_number(square, B_relev[square], B);
    }

    void init_leapers_attacks()
    {
        for (int square = 0; square < 64; square++)
        {
            P_attacks[WHT][square] = P_msk_att(WHT, square);
            P_attacks[BLCK][square] = P_msk_att(BLCK, square);

            N_attacks[square] = N_msk_att(square);

            K_attacks[square] = K_msk_att(square);
        }
    }

    void init_sliders_attacks(int bishop)
    {
        for (int square = 0; square < 64; square++)
        {
            B_msk[square] = B_msk_att(square);
            R_msk[square] = R_msk_att(square);

            BRD_U64 attack_mask = bishop ? B_msk[square] : R_msk[square];

            BRD_U64 relevant_bits_count = cntBTon(attack_mask);

            BRD_U64 occupancy_indicies = (1ULL << relevant_bits_count);

            for (int index = 0; index < occupancy_indicies; index++)
            {
                if (bishop)
                {
                    BRD_U64 occupancy = SToccup(index, relevant_bits_count, attack_mask);

                    BRD_U64 magic_index = (occupancy * B_MN[square]) >> (64 - B_relev[square]);

                    B_att[square][magic_index] = B_attacks(square, occupancy);
                }

                else
                {
                    BRD_U64 occupancy = SToccup(index, relevant_bits_count, attack_mask);

                    BRD_U64 magic_index = (occupancy * R_MN[square]) >> (64 - R_relev[square]);

                    R_att[square][magic_index] = R_attacks(square, occupancy);

                }
            }
        }
    }

    static inline BRD_U64 get_bishop_attacks(int square, BRD_U64 occupancy)
    {
        occupancy &= B_msk[square];
        occupancy *= B_MN[square];
        occupancy >>= 64 - B_relev[square];

        return B_att[square][occupancy];
    }

    static inline BRD_U64 get_rook_attacks(int square, BRD_U64 occupancy)
    {
        occupancy &= R_msk[square];
        occupancy *= R_MN[square];
        occupancy >>= 64 - R_relev[square];

        return R_att[square][occupancy];
    }

    static inline BRD_U64 get_queen_attacks(int square, BRD_U64 occupancy) {
        return get_bishop_attacks(square, occupancy) | get_rook_attacks(square, occupancy);
    }


    void init_all()
    {
        init_leapers_attacks();

        init_sliders_attacks(Bishop);
        init_sliders_attacks(Rook);
    }

}

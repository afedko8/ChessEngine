#pragma once
#include"Macros_and_lib.ipp"
#include"Constans.h"


void prnt_brd(BRD_U64 BRD)
{
    printf("\n");
    for (int rank = 0; rank < 8; rank++)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            if (!file)
                printf("  %d ", 8 - rank);
            printf(" %d", gt_bt(BRD, square) ? 1 : 0);

        }
        printf("\n");
    }
    printf("\n     a b c d e f g h\n\n");
    printf("     BRD: %llud\n\n", BRD);
}

void prnt_brd2(BRD_U64 BRD, int squareC, char c) {
    for (int rank = 0; rank < 8; rank++)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            if (file == 0) printf("%d ", 8 - rank);
            if (square == squareC) printf(" %c", c);
            else printf(" %c", (gt_bt(BRD, square) ? '1' : ' '));
        }
        std::cout << '\n';
    }
    printf("\n   A B C D E F G H \n");
    printf("BitBoard:%llu\n\n", BRD);
}

void prnt_att(BRD_U64 BRD, int squareC, char c, int squareO) {
    char o = '*';
    for (int rank = 0; rank < 8; rank++)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            if (file == 0) printf("%d ", 8 - rank);
            if (square == squareC) printf(" %c", c);
            else if (square == squareO) printf(" %c", o);
            else printf(" %c", (gt_bt(BRD, square) ? '1' : ' '));
        }
        std::cout << '\n';
    }
    printf("\n   A B C D E F G H \n");
    printf("BitBoard:%llu\n\n", BRD);
}

void PrintAllPosition() {
    printf("\n");
    for (int rank = 0; rank < 8; rank++)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            int piece = -1;

            if (!file) printf(" %d ", 8 - rank);

            for (int bb_qq = P; bb_qq <= k; bb_qq++)
            {
                if (gt_bt(ALL_BRDS[bb_qq], square)) piece = bb_qq;
            }

            printf(" %c", (piece == -1) ? '.' : ascii_pieces[piece]);
        }
        printf("\n");
    }
    printf("\n    a b c d e f g h\n\n");

    printf("     Side:     %s\n", !side ? "WHT" : "BLCK");

    printf("     Castling:  %c%c%c%c\n", (cstl & wk) ? 'K' : '-',
        (cstl & wq) ? 'Q' : '-',
        (cstl & bk) ? 'k' : '-',
        (cstl & bq) ? 'q' : '-');

    printf("     enpssp:   %s\n", (enpssp != no_sq) ? square_to_coordinates[enpssp] : "no");


}


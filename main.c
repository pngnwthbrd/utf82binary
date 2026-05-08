#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#define COLOR_YELLOW "\e[0;33m"
#define COLOR_BLUE "\e[0;34m"
#define COLOR_GREEN "\e[0;32m"
#define COLOR_RED "\e[0;31m"
#define COLOR_CYAN "\e[0;36m"
#define COLOR_PURPLE "\e[0;35m"
#define COLOR_BOLD_WHITE "\e[1;37m"
#define COLOR_RESET "\e[0m"

int colored = 0;
int infoprint = 0;
int lineprint = 0;

// utf-8 prefixes
struct {
    char byte1[2];
    char byte2[4];
    char byte3[5];
    char byte4[6];
    char bytef[3];
} UTF8Pre = {"0", "110", "1110", "11110", "10"};

// check byte prefix
int bPre(char * prefix, int prefixSize, char byte[9]) {
    for (int i = 0; i < (prefixSize -1); i++) {
        if (prefix[i] != byte[i]) {
            return 0;
        }
    }
    return 1;
}

void color(char * code) {
    if (colored) {
        printf(code);
    }
}

int main(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "il")) != -1) {
        switch (opt) {
            case 'i':
                infoprint = 1;
                colored = 1;
                break;
            case 'l':
                lineprint = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-i,l] <string>\n",
                       argv[0]);
                return 1;
        }
    }

    if (argc > 1) {
        char * input = argv[argc - 1];
        for (int i = 0; input[i] != '\0'; i++) {
            char byte[9];
            for (int j = 7; j >= 0; j--) {
                byte[7 - j] = ((input[i] >> j) & 1) + '0';
            }
            byte[8] = '\0';

            // WIP
            if (infoprint) {
                char lbits[5];
                char rbits[5];
                sprintf(lbits, "%.4s", byte);
                sprintf(rbits, "%.4s", &byte[4]); // byte + 4

                // TODO: convert to hex -> utf-8 code point

                if (bPre(UTF8Pre.byte1, sizeof(UTF8Pre.byte1), byte) == 1) {
                    color(COLOR_BOLD_WHITE);
                    printf("%c ", input[i]);

                    if (lineprint) printf("\t");

                    color(COLOR_GREEN);
                    printf("<1 byte char>");
                } else if (bPre(UTF8Pre.byte2, sizeof(UTF8Pre.byte2), byte) == 1) {
                    color(COLOR_BOLD_WHITE);
                    printf("%c%c ", input[i], input[i + 1]);

                    if (lineprint) printf("\t");

                    color(COLOR_YELLOW);
                    printf("<2 byte char>");
                } else if (bPre(UTF8Pre.byte3, sizeof(UTF8Pre.byte3), byte) == 1) {
                    color(COLOR_BOLD_WHITE);
                    printf("%c%c%c ", input[i], input[i + 1], input[i + 2]);

                    if (lineprint) printf("\t");

                    color(COLOR_BLUE);
                    printf("<3 byte char>");
                } else if (bPre(UTF8Pre.byte4, sizeof(UTF8Pre.byte4), byte) == 1) {
                    color(COLOR_BOLD_WHITE);
                    printf("%c%c%c%c ", input[i], input[i + 1], input[i + 2], input[i + 3]);

                    if (lineprint) printf("\t");

                    color(COLOR_CYAN);
                    printf("<4 byte char>");
                } else if (bPre(UTF8Pre.bytef, sizeof(UTF8Pre.bytef), byte) == 1) {
                    if (lineprint) printf(" \t");

                    color(COLOR_PURPLE);
                    printf("<Continuation>");
                }

                if (lineprint) printf("\t");

                color(COLOR_RESET);
            }
            // END_WIP

            printf("%s", byte);

            if (infoprint) printf(" ");
            if (lineprint) printf("\n");

            color(COLOR_RESET);
        }
        printf("\n");

        return 0;
    } else {
        printf("Usage: %s <string>\n", argv[0]);
        printf("Options:\n");
        printf("-i  Show colored byte information\n");
        printf("-l  Show one line per character\n");
        return 1;
    }
}


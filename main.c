#include <stdio.h>
#include <stdint.h>

struct {
    char byte1[2];
    char byte2[4];
    char byte3[5];
    char byte4[6];
    char bytef[3];
} Utf8Prefixes = {"0", "110", "1110", "11110", "10"};

int bytePrefix(char * prefix, int prefixSize, char byte[9]) {
    for (int i = 0; i < (prefixSize -1); i++) {
        if (prefix[i] != byte[i]) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        char * input = argv[1];
        for (int i = 0; input[i] != '\0'; i++) {
            char byte[9];
            for (int j = 7; j >= 0; j--) {
                byte[7 - j] = ((input[i] >> j) & 1) + '0';
            }
            byte[8] = '\0';

            if (input[i] == ' ') {
                printf("\n");
                printf("%s", byte);
                printf("\n");
            }
            printf("%s", byte);
            printf(" ");


            // WIP
            if (argc > 2) {
                char lbits[5];
                char rbits[5];
                sprintf(lbits, "%.4s", byte);
                sprintf(rbits, "%.4s", &byte[4]); // byte + 4

                // TODO: check what utf-8 prefix has byte, then loop to lbits and rbits
                // and convert to hex -> utf-8 code point


                if (bytePrefix(Utf8Prefixes.byte1, sizeof(Utf8Prefixes.byte1), byte) == 1) {
                    printf("1 byte Character found!\n");
                } else if (bytePrefix(Utf8Prefixes.byte2, sizeof(Utf8Prefixes.byte2), byte) == 1) {
                    printf("2 byte Character found!\n");
                } else if (bytePrefix(Utf8Prefixes.byte3, sizeof(Utf8Prefixes.byte3), byte) == 1) {
                    printf("3 byte Character found!\n");
                } else if (bytePrefix(Utf8Prefixes.byte4, sizeof(Utf8Prefixes.byte4), byte) == 1) {
                    printf("4 byte Character found!\n");
                } else if (bytePrefix(Utf8Prefixes.bytef, sizeof(Utf8Prefixes.bytef), byte) == 1) {
                    printf("Follow up byte found!\n");
                } else {
                    printf("String contains non utf-8 binary: %s\n", byte);
                }
            }
            // END_WIP
        }
        printf("\n");

        return 0;
    } else {
        printf("Usage: %s <string>\n", argv[0]);
        return 1;
    }
}


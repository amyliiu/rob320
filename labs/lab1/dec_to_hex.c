#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

// TODO: Declare any necessary helper functions

int get_max_power(int decimal_int) {
    int max_power = 0;
    int val = 1;
    while(val * 16 <= decimal_int) {
        val *= 16;
        max_power++;
    }
    return max_power;
}

char get_hex(int i){
    if (i >= 0 && i <= 9) {
        return '0' + i;
    }
    return 'A' + (i - 10);
}

void dec_to_hex(char* decimal_str, char* hex_str) {
    // TODO: Convert decimal string to hexadecimal string
    // Example: decimal_str = "255", hex_str = "FF"
    //          decimal_str = "4096", hex_str = "1000"
    int decimal_int = atoi(decimal_str);
    int max_power = 0;
    int val = 1;
    while(val * 16 <= decimal_int) {
        val *= 16;
        max_power++;
    }
    int str_i = 0;

    for(; max_power >= 0; max_power--) {
        int curr = (int) (decimal_int / val);
        // printf("curr: %d\n", curr);
        // printf("max power %d\n", max_power);
        hex_str[str_i++] = get_hex(curr);
        // printf("%s\n", hex_str);
        // printf("%d\n", decimal_int);
        decimal_int = decimal_int - curr *val;
        val /= 16;
    }
    hex_str[str_i] = '\0';
}

int main() {
    // Read decimal value as string from STDIN
    char decimal_str[8] = {0};
    fgets(decimal_str, sizeof(decimal_str), stdin);

    // Convert decimal string to hexadecimal string
    char hex_str[8] = {0};
    dec_to_hex(decimal_str, hex_str);

    // Write hexadecimal value to STDOUT
    printf("%s", hex_str);
}
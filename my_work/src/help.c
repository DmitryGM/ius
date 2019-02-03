#include <help.h>
#include <sio.h>


void int_to_string(int number, char *string, int length)
{
    bit sign;
    int i, j;
    int d_count;
    char temp;

    j = 0;

    for (i = 0; i < length; ++i)
        string[i] = '\0';

    if (number < 0) {
        number *= -1;
        sign = 1;
    }
    else {
        sign = 0;
    }

    if (number == 0) {
        string[j++] = '0';
    }


    while (number > 0) {
        string[j++] = (number % 10) + '0';
        number /= 10;
    }

    if (sign) {
        string[j++] = '-';
    }

    d_count = j;

    // reverse:
    for (i = 0; i < d_count / 2; ++i) {
        temp = string[i];
        string[i] = string[d_count - 1 - i];
        string[d_count - 1 - i] = temp;
    }
}

void print_byte(char byte) {
    int i;

    for (i = 0; i < 8; ++i) {
        if (byte & 1)
            type("1");
        else
            type("0");

        byte >>= 1;
    }
    type("\n");
}
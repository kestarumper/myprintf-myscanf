#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int mystrlen(const char * str) {
    int result = 0;
    while(*str != '\0') {
        str++;
        result++;
    }
    return result;
}

int myfind(char c, const char * str) {
    char * ptr = str;

    while(*ptr != '\0' && *ptr != c) {
        ptr++;
    }

    if(*ptr == '\0') {
        return -1;
    }
    
    return ptr - str;
}

#define DIGITS_BUFSIZE 32

char * inttostring(int a, int base) {
    char * result = malloc(DIGITS_BUFSIZE * sizeof(char));

    int * digits = malloc(DIGITS_BUFSIZE * sizeof(int));

    if(result == NULL || digits == NULL) {
        perror("inttostring:malloc");
        exit(1);
    }

    int sign = 1;
    if(a < 0) {
        sign = -1;
    }

    a *= sign;

    int i = 0;
    while(a > 0) {
        digits[i] = a % base;
        a /= base;
        i++;
    }

    if( sign == -1 ) {
        digits[i] = -3;
        i++;
    }

    char tmp;
    int digit;
    for(int j = i; j > 0; j--) {
        digit = digits[j-1];

        if(digit > 9) {
            tmp = 'A';
            digit -= 10;
        } else {
            tmp = '0';
        }

        result[i-j] = (char)(tmp + digit);
    }

    result[i] = '\0';

    free(digits);
    return result;
}

void myprintf(char * format, ... ) {
    char * p = (((char*)&format) + sizeof(format));

    int arg1;
    char * arg2;

    char * formatbegin = format;
    int percent = myfind('%', format);

    char option;


    while(percent != -1) {
        write(1, format, percent);
        // printf("%% found on %i pos\n", percent);
        option = *(format + percent + 1);
        // printf("%c", option);

        switch(option) {
            case 'd':
                arg1 = *((int*)p);
                p += sizeof(int);
                write(1, inttostring(arg1, 10), mystrlen(inttostring(arg1, 10)));
                break;
            case 'x':
                arg1 = *((int*)p);
                p += sizeof(int);
                write(1, inttostring(arg1, 16), mystrlen(inttostring(arg1, 16)));
                break;
            case 'b':
                arg1 = *((int*)p);
                p += sizeof(int);
                write(1, inttostring(arg1, 2), mystrlen(inttostring(arg1, 2)));
                break;
            case 's':
                arg2 = *((char**)p);
                p += sizeof(char*);
                write(1, arg2, mystrlen(arg2));
                break;
        }

        format += percent + 2;

        percent = myfind('%', format);
    }
    write(1, format, mystrlen(format));
}

int main() {
    myprintf("%s ma %d lat (hex: %x), a jego tata %s ma %d lat (bin: %b)\n", "Adrian", 21, 21, "Damian", 45, 45);
    return 0;
}
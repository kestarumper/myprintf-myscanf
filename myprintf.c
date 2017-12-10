#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

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

int stringToInt(char * a, int base) {
    int result = 0;
    int sign = 1;

    int length = mystrlen(a);

    if(a[length-1] == '\n') {
        a[length-1] = '\0';
        length--;
    }

    if(a[0] == '-') {
        a++;
        sign = -1;
        length--;
    }

    char digit;
    int power;

    for(int i = 0; i < length; i++) {
        digit = a[length-i-1];
        if(digit >= 'A') {
            digit -= 'A';
        } else {
            digit -= '0';
        }

        result += pow(base, i) * ((int)digit);
    }

    return result * sign;
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

void myscanf(char * format, ... ) {
    char * p = (((char*)&format) + sizeof(format));

    char * input = malloc(1024 * sizeof(char));

    int * arg1;
    char ** arg2;
    
    int percent = myfind('%', format);
    int whitechar;
    char option;

    read(STDIN_FILENO, input, 1024);

    while(percent != -1) {
        option = *(format + percent + 1);

        whitechar = myfind(' ', input);

        if(whitechar >= 0) {
            *(input+whitechar) = '\0';
        }

        switch(option) {
            case 'd':
                arg1 = *((int*)p);
                p += sizeof(int);
                *arg1 = stringToInt(input, 10);
                break;
            case 's':
                arg2 = *((char**)p);
                p += sizeof(char*);
                *arg2 = input;
                break;
        }

        format += percent + 2;
        input += whitechar + 1;

        percent = myfind('%', format);
    }
}

int main() {
    char * tekst = malloc(1024 * sizeof(char));
    int wiek;
    int wzrost;
    myprintf("%s ma %d lat (hex: %x), a jego tata %s ma %d lat (bin: %b)\n", "Adrian", 21, 21, "Damian", 45, 45);

    myscanf("%d%s%d", &wiek, &tekst, &wzrost);
    myprintf("%d lat %s i wzrostu %d\n", wiek, tekst, wzrost);
    return 0;
}
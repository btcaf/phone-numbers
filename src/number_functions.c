/** @file
 * Implementacja podstawowych operacji na numerach telefonów
 * i ich pojedynczych cyfrach.
 *
 * @author Cezary Botta <cb439922@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include <stdlib.h>
#include <ctype.h>

#include "number_functions.h"

int sortValue(int ch) {
    if (ch == '\0')
        return -1;

    return (int) charToDigit(ch);
}

unsigned int charToDigit(int ch) {
    if (ch == '*')
        return 10;
    if (ch == '#')
        return 11;
    return (size_t) (ch - (int) '0');
}

char digitToChar(unsigned int digit) {
    if (digit < 10) {
        // dokonujemy tu celowo zawężającej konwersji, wartość zawsze
        // mieści się w zakresie typu char
        return (char) (digit + (unsigned int) '0');
    }
    if (digit == 10)
        return '*';
    return '#';

}

bool isPhNumDigit(int ch) {
    return isdigit(ch) || ch == '*' || ch == '#';
}

bool isCorrect(char const *num) {
    if (num == NULL)
        return false;

    size_t i = 0;
    while (isPhNumDigit(num[i]))
        ++i;

    return (num[i] == '\0' && i > 0);
}
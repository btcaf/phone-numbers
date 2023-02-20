/** @file
 * Interfejs klasy implementującej podstawowe operacje na numerach telefonów
 * i ich pojedynczych cyfrach.
 *
 * @author Cezary Botta <cb439922@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef NUMBER_FUNCTIONS_H
#define NUMBER_FUNCTIONS_H

#include <stdbool.h>

/**
 * Wyznacza wartość znaku w celu porównywania go z innymi znakami.
 * @param[in] ch – kod znaku.
 * @return Wartość cyfry, jeśli @p ch jest legalną cyfrą w rozumieniu
 *         treści zadania.
 *         Wartość -1, jeśli @p ch jest znakiem kończącym napis.
 */
int sortValue(int ch);

/**
 * Wyznacza wartość cyfry w rozumieniu treści zadania.
 * @param[in] ch – kod znaku reprezentującego cyfrę.
 * @return Wartość cyfry reprezentowanej przez @p ch.
 */
unsigned int charToDigit(int ch);

/**
 * Wyznacza znak reprezentujący cyfrę na podstawie jej wartości.
 * @param[in] digit – wartość cyfry.
 * @return Znak reprezentujący cyfrę @p digit.
 */
char digitToChar(unsigned int digit);

/** @brief Sprawdza, czy znak o danym kodzie reprezentuje cyfrę.
 * Przez znak reprezentujący cyfrę rozumiemy tu jeden ze znaków 0, 1, ..., 9,
 * *, #.
 * @param[in] ch – kod znaku.
 * @return Wartość @p true, jeśli znak reprezentuje cyfrę.
 *         Wartość @p false, jeśli znak nie reprezentuje cyfry.
 */
bool isPhNumDigit(int ch);

/**
 * Sprawdza, czy napis reprezentuje numer.
 * @param[in] num – wskaźnik na napis.
 * @return Wartość @p true, jeśli napis reprezentuje numer.
 *         Wartość @p false, jeśli napis nie reprezentuje numeru.
 */
bool isCorrect(char const *num);

#endif /* NUMBER_FUNCTIONS_H */

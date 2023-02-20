/** @file
 * Implementacja klasy przechowującej przekierowania numerów telefonicznych
 *
 * @author Cezary Botta <cb439922@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "phone_forward.h"
#include "number_functions.h"
#include "trie.h"
#include "list.h"

/**
 * Struktura przechowująca przekierowania numerów telefonów składa się z dwóch
 * drzew trie odpowiadającym odpowiednio numerom przekierowanym oraz ich
 * przekierowaniom. Korzenie drzew odpowiadają pustym napisom.
 */
struct PhoneForward {
    TrieNode *rootFwd; ///< korzeń drzewa przechowującego przekierowania
    TrieNode *rootReverse; /**< korzeń drzewa przechowującego odwrotności
                           przekierowań */
};

/**
 * Struktura przechowująca ciąg numerów telefonów jest dynamicznie powiększaną
 * tablicą.
 */
struct PhoneNumbers {
    char **numbers; ///< tablica napisów
    size_t numberCount; ///< liczba napisów przechowywanych w tablicy
    size_t capacity; /**< maksymalna liczba napisów, którą można pomieścić
                     w tablicy bez dodatkowej alokacji */
};

/* Funkcje struktury PhoneNumbers */

/**
 * Tworzy nową strukturę reprezentującą pusty ciąg.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         alokować pamięci.
 */
static PhoneNumbers *phnumNew(void) {
    PhoneNumbers *newStruct = malloc(sizeof(struct PhoneNumbers));

    if (newStruct != NULL) {
        newStruct->numbers = NULL;
        newStruct->numberCount = 0;
        newStruct->capacity = 0;
    }

    return newStruct;
}

void phnumDelete(PhoneNumbers *pnum) {
    if (pnum == NULL)
        return;

    for (size_t i = 0; i < pnum->numberCount; ++i)
        free(pnum->numbers[i]);
    free(pnum->numbers);
    free(pnum);
}

/**
 * Dodaje numer @p num do struktury @p pnum.
 * @param[in, out] pnum – wskaźnik na strukturę przechowującą ciąg numerów;
 * @param[in] num       – wskaźnik na napis reprezentujący numer.
 * @return Wartość @p true, jeśli napis został dodany.
 *         Wartość @p false, jeśli nie udało się alokować pamięci
 *         lub jeden z parametrów ma wartość NULL.
 */
static bool phnumAdd(PhoneNumbers *pnum, char *num) {
    if (pnum == NULL || num == NULL)
        return false;

    if (pnum->numberCount == pnum->capacity) {
        size_t maxCapacity = SIZE_MAX / sizeof(char *);

        // sprawdzamy, czy rozmiar nie miałby wykroczyć poza zakres
        // typu size_t
        if (pnum->capacity == maxCapacity)
            return false;

        // ustawiamy nowy rozmiar jako w przybliżeniu 3/2 poprzedniego
        // lub maksymalny dopuszczany przez typ size_t, jeśli ten drugi
        // jest mniejszy
        size_t newCapacity;

        if (pnum->capacity > maxCapacity / 3 * 2)
            newCapacity = maxCapacity;
        else
            newCapacity = pnum->capacity * 3 / 2 + 1;

        char **tmp = realloc(pnum->numbers, newCapacity * sizeof(char *));

        if (tmp == NULL)
            return false;
        pnum->capacity = newCapacity;
        pnum->numbers = tmp;
    }

    pnum->numbers[pnum->numberCount] = num;
    ++pnum->numberCount;
    return true;
}

/** @brief Dodaje numer do struktury obsługując błędy.
 * Dodaje numer @p num do struktury @p pnum, a w przypadku niepowodzenia
 * usuwa strukturę @p pnum i zwalnia pamięć zaalokowaną numerowi @p num.
 * @param[in, out] pnum – wskaźnik na strukturę przechowującą ciąg numerów;
 * @param[in, out] num  – wskaźnik na napis reprezentujący numer.
 * @return Wartość @p true, jeśli napis został dodany.
 *         Wartość @p false, jeśli nie udało się alokować pamięci
 *         lub jeden z parametrów ma wartość NULL i usunięte zostały
 *         struktura i napis.
 */
static bool phnumSafeAdd(PhoneNumbers *pnum, char *num) {
    if (!phnumAdd(pnum, num)) {
        free(num);
        phnumDelete(pnum);
        return false;
    }

    return true;
}

/** @brief Porównuje leksykograficznie dwa numery.
 * Porównuje leksykograficznie dwa numery tak, aby używać jej jako
 * komparatora w funkcji qsort.
 * @param[in] a – wskaźnik na pierwszy numer do porównania;
 * @param[in] b – wskaźnik na drugi numer do porównania.
 * @return Wartość ujemna, jeśli @p a jest mniejsze niż @p b (w porządku
 *         leksykograficznym).
 *         Wartość zero, jeśli @p a jest równe @p b.
 *         Wartość dodatnia, jeśli @p a jest większe niż @p b.
 *
 */
static int lexCompare(const void *a, const void *b) {
    char const *num1 = *(char const **) a;
    char const *num2 = *(char const **) b;

    size_t i = 0;
    while (num1[i] != '\0' && num2[i] != '\0' && num1[i] == num2[i])
        ++i;

    return sortValue(num1[i]) - sortValue(num2[i]);
}

/**
 * Sortuje strukturę @p pnum leksykograficznie.
 * @param[in, out] pnum – wskaźnik na strukturę reprezentującą ciąg.
 */
static void lexSort(PhoneNumbers *pnum) {
    qsort(pnum->numbers, pnum->numberCount, sizeof(char *), lexCompare);
}

/**
 * Usuwa powtarzające się numery z posortowanej struktury.
 * @param[in, out] pnum – wskaźnik na wskaźnik na strukturę
 *                        reprezentującą ciąg.
 * @return Wartość @p true, jeśli usuwanie się powiodło.
 *         Wartość @p false, jeśli nie udało się alokować pamięci.
 */
static bool removeDuplicates(PhoneNumbers **pnum) {
    PhoneNumbers *new = phnumNew();
    if (new == NULL)
        return false;

    if (!phnumAdd(new, (*pnum)->numbers[0])) {
        free(new);
        return false;
    }

    for (size_t i = 1; i < (*pnum)->numberCount; ++i) {
        if (strcmp((*pnum)->numbers[i], (*pnum)->numbers[i - 1]) != 0) {
            if (!phnumAdd(new, (*pnum)->numbers[i])) {
                free(new->numbers);
                free(new);
                return false;
            }
        } else {
            free((*pnum)->numbers[i]);
            (*pnum)->numbers[i] = (*pnum)->numbers[i - 1];
        }
    }

    free((*pnum)->numbers);
    free(*pnum);
    *pnum = new;
    return true;
}

char const *phnumGet(PhoneNumbers const *pnum, size_t idx) {
    if (pnum == NULL || idx >= pnum->numberCount)
        return NULL;
    return pnum->numbers[idx];
}

/* Funkcje struktury PhoneForward */

PhoneForward *phfwdNew(void) {
    PhoneForward *newStruct = malloc(sizeof(struct PhoneForward));

    if (newStruct != NULL) {
        newStruct->rootFwd = trieNew();

        if (newStruct->rootFwd == NULL) {
            free(newStruct);
            return NULL;
        }

        newStruct->rootReverse = trieNew();

        if (newStruct->rootReverse == NULL) {
            free(newStruct->rootFwd);
            free(newStruct);
            return NULL;
        }
    }

    return newStruct;
}

void phfwdDelete(PhoneForward *pf) {
    if (pf == NULL)
        return;

    trieDelete(pf->rootFwd);

    // wywołanie trieDelete na korzeniu drzewa przekierowań usuwa wszystkie
    // węzły drzewa odwrotności przekierowań z wyjątkiem korzenia, więc
    // wystarczy zwolnić pamięć zaalokowaną dla jego korzenia
    free(pf->rootReverse);
    free(pf);
}

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2) {
    if (pf == NULL)
        return false;

    if (!isCorrect(num1) || !isCorrect(num2) || !strcmp(num1, num2))
        return false;

    TrieNode *fwd = trieAdd(pf->rootFwd, num1);
    if (fwd == NULL)
        return false;

    TrieNode *reverse = trieAdd(pf->rootReverse, num2);
    if (reverse == NULL) {
        deleteDeadBranch(fwd);
        return false;
    }

    if (!addToReverseFwdList(reverse, fwd)) {
        deleteDeadBranch(fwd);
        deleteDeadBranch(reverse);
        return false;
    }

    deleteFwdData(fwd);
    setFwdNode(fwd, reverse);
    setListNode(fwd, getListNode(reverse));
    return true;
}

void phfwdRemove(PhoneForward *pf, char const *num) {
    if (pf == NULL || !isCorrect(num))
        return;

    trieRemove(pf->rootFwd, num);
}

PhoneNumbers *phfwdGet(PhoneForward const *pf, char const *num) {
    if (pf == NULL)
        return NULL;

    if (!isCorrect(num))
        return phnumNew();

    size_t i = 0;

    TrieNode *maxPrefix = pf->rootFwd;
    TrieNode *maxCandidate = trieFindNextNonEmpty(maxPrefix, num, &i);

    // znajdujemy najdłuższy przekierowany prefiks
    while (maxCandidate != NULL) {
        maxPrefix = maxCandidate;
        maxCandidate = trieFindNextNonEmpty(maxPrefix, num, &i);
    }

    char *fwdNum = changePrefix(num, getFwdNode(maxPrefix), i);
    PhoneNumbers *result = phnumNew();

    if (!phnumSafeAdd(result, fwdNum))
        return NULL;

    return result;
}

PhoneNumbers *phfwdReverse(PhoneForward const *pf, char const *num) {
    if (pf == NULL)
        return NULL;

    if (!isCorrect(num))
        return phnumNew();

    char *numCopy = malloc((strlen(num) + 1) * sizeof(char));
    if (numCopy == NULL)
        return NULL;

    PhoneNumbers *result = phnumNew();

    // dodajemy numer num do wyniku
    strcpy(numCopy, num);
    if (!phnumSafeAdd(result, numCopy))
        return NULL;

    size_t i = 0;
    TrieNode *currPrefix = trieFindNextNonEmpty(pf->rootReverse, num, &i);

    // przechodzimy przez wszystkie węzły odpowiadające prefiksom,
    // na które został przekierowany przynajmniej jeden prefiks
    while (currPrefix != NULL) {
        // przechodzimy przez wszystkie prefiksy, które zostały przekierowane
        // na aktualny i dodajemy odpowiednie numery do wyniku
        ListNode *currListNode = getListNode(currPrefix);
        while (currListNode != NULL) {
            char *reverseNum = changePrefix(num, getKey(currListNode), i);

            if (!phnumSafeAdd(result, reverseNum))
                return NULL;

            currListNode = getNext(currListNode);
        }

        currPrefix = trieFindNextNonEmpty(currPrefix, num, &i);
    }

    lexSort(result);
    if (!removeDuplicates(&result)) {
        phnumDelete(result);
        return NULL;
    }
    return result;
}

PhoneNumbers *phfwdGetReverse(PhoneForward const *pf, char const *num) {
    if (pf == NULL)
        return NULL;

    if (!isCorrect(num))
        return phnumNew();

    size_t i = 0;
    PhoneNumbers *result = phnumNew();
    if (result == NULL)
        return NULL;

    // sprawdzamy, czy numer num został przekierowany i jeśli nie,
    // to dodajemy go do wyniku
    if (trieFindNextNonEmpty(pf->rootFwd, num, &i) == NULL) {
        char *numCopy = malloc((strlen(num) + 1) * sizeof(char));
        if (numCopy == NULL) {
            phnumDelete(result);
            return NULL;
        }

        strcpy(numCopy, num);
        if (!phnumSafeAdd(result, numCopy))
            return NULL;
    }

    i = 0;
    TrieNode *currPrefix = trieFindNextNonEmpty(pf->rootReverse, num, &i);

    // przechodzimy przez wszystkie węzły odpowiadające prefiksom,
    // na które został przekierowany przynajmniej jeden prefiks
    while (currPrefix != NULL) {
        // przechodzimy przez wszystkie prefiksy, które zostały przekierowane
        // na aktualny
        ListNode *currListNode = getListNode(currPrefix);
        while (currListNode != NULL) {
            size_t j = i;
            TrieNode *fwdNode = getKey(currListNode);

            // sprawdzamy, czy numer nie został dalej przekierowany i jeśli
            // nie, to dodajemy go do wyniku
            if (trieFindNextNonEmpty(fwdNode, num, &j) == NULL) {
                char *reverseNum = changePrefix(num, fwdNode, i);

                if (!phnumSafeAdd(result, reverseNum))
                    return NULL;
            }

            currListNode = getNext(currListNode);
        }

        currPrefix = trieFindNextNonEmpty(currPrefix, num, &i);
    }

    // powyższy algorytm nigdy nie dodaje do wyniku dwa razy tego samego
    // numeru, więc nie trzeba usuwać duplikatów
    lexSort(result);
    return result;
}
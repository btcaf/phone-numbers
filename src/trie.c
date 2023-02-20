/** @file
 * Implementacja drzewa trie dla celów implementacji struktury
 * PhoneForward z interfejsu phone_forward.h.
 *
 * @author Cezary Botta <cb439922@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include <stdlib.h>

#include "trie.h"
#include "list.h"
#include "number_functions.h"

/**
 * Struktura reprezentująca węzeł drzewa trie poza wskaźnikami do
 * przechowywanych danych i synów zawiera również wskaźnik do ojca.
 */
struct TrieNode {
    TrieNode *fwdNode; /**< Wskaźnik na węzeł drzewa. W drzewie przekierowań
                       wskazuje na węzeł drzewa odwrotności przekierowań, na
                       które dany węzeł jest przekierowany (lub NULL, jeśli
                       węzeł przekierowany nie jest), a w drzewie odwrotności
                       przekierowań jest równy NULL */
    ListNode *listNode; /**< Wskaźnik na węzeł listy. W drzewie odwrotności
                        przekierowań wskazuje na początek listy węzłów, które
                        zostały przekierowane na dany węzeł, a w drzewie
                        przekierowań wskazuje na element listy w węźle @p
                        fwdNode, który przechowuje wskaźnik na dany węzeł */

    TrieNode *children[12]; /**< wskaźniki do synów węzła drzewa odpowiadające
                            odpowiednim cyfrom, jeśli dany syn nie istnieje,
                            to odpowiedni wskaźnik wynosi NULL */
    TrieNode *parent; /**< wskaźnik na ojca węzła drzewa,
                      NULL w przypadku korzenia */
};

TrieNode *trieNew(void) {
    TrieNode *newStruct = malloc(sizeof(struct TrieNode));

    if (newStruct != NULL) {
        newStruct->fwdNode = NULL;
        newStruct->listNode = NULL;

        for (unsigned int i = 0; i < 12; ++i)
            newStruct->children[i] = NULL;
        newStruct->parent = NULL;
    }

    return newStruct;
}

/** @brief Sprawdza, czy węzeł jest pusty.
 * Sprawdza, czy węzeł @p node nie został przekierowany lub czy nie został
 * na niego przekierowany inny węzeł.
 * @param[in] node – wskaźnik na węzeł drzewa
 * @return Wartość @p true, jeśli węzeł @p num jest pusty lub jest równy NULL.
 *         Wartość @p false, jeśli węzeł @p num jest korzeniem drzewa lub
 *         nie jest pusty.
 */
static bool isEmpty(TrieNode *node) {
    if (node == NULL)
        return true;
    if (node->parent == NULL)
        return false;
    return node->fwdNode == NULL && node->listNode == NULL;
}

void deleteDeadBranch(TrieNode *node) {
    for (unsigned int i = 0; i < 12; ++i)
        if (node->children[i] != NULL)
            return;

    TrieNode *current = node;
    bool isLeaf = true;

    while (isEmpty(current) && isLeaf) {
        TrieNode *currentParent = current->parent;

        for (unsigned int i = 0; i < 12; ++i) {
            if (currentParent->children[i] == current)
                currentParent->children[i] = NULL;
            else if (currentParent->children[i] != NULL)
                isLeaf = false;
        }

        free(current);
        current = currentParent;
    }
}

TrieNode *trieAdd(TrieNode *t, char const *num) {
    TrieNode *current = t;
    size_t i = 0;

    // znajdujemy najdłuższy prefiks num w drzewie
    while (num[i] != '\0' && current->children[charToDigit(num[i])] != NULL) {
        current = current->children[charToDigit(num[i])];
        ++i;
    }

    // jeśli w drzewie jest cały numer, to zwracamy wskaźnik na odpowiedni
    // węzeł drzewa
    if (num[i] == '\0')
        return current;

    while (num[i] != '\0') {
        TrieNode *newNode = trieNew();

        // gdy zabraknie pamięci usuwamy dodane węzły
        if (newNode == NULL) {
            deleteDeadBranch(current);
            return NULL;
        }

        current->children[charToDigit(num[i])] = newNode;
        newNode->parent = current;
        current = newNode;
        ++i;
    }

    return current;
}

void deleteFwdData(TrieNode *node) {
    if (node->fwdNode == NULL)
        return;

    if (node->fwdNode->listNode == node->listNode)
        node->fwdNode->listNode = getNext(node->fwdNode->listNode);

    listRemove(node->listNode);
    deleteDeadBranch(node->fwdNode);
}

/**
 * Usuwanie poddrzewa zaimplementowane zostało iteracyjnie w celu uniknięcia
 * przepełnienia stosu przy przechowywaniu długich numerów. Używamy przy tym
 * następującego algorytmu: przemieszczamy się od korzenia poddrzewa do
 * pierwszego syna aż nie dojdziemy do węzła, który pierwszego syna nie ma.
 * Wówczas ustawiamy jako jego pierwszego syna drugiego syna korzenia
 * i powtarzamy tę procedurę, tworząc dowiązania do kolejnych synów korzenia.
 * W momencie, w którym mamy dowiązania do wszystkich synów korzenia możemy
 * usunąć korzeń i ustawić jego pierwszego syna (w już potencjalnie
 * zmodyfikowanym drzewie) jako nowy korzeń. W ten sposób otrzymaliśmy drzewo
 * o jednym węźle mniej, więc powtarzając tę procedurę w końcu usuniemy całe
 * poddrzewo.
 */
void trieDelete(TrieNode *node) {
    TrieNode *root = node;
    TrieNode *current = root;

    while (root != NULL) {
        while (current->children[0] != NULL)
            current = current->children[0];

        for (unsigned int i = 1; i < 12; ++i) {
            current->children[0] = root->children[i];
            while (current->children[0] != NULL)
                current = current->children[0];
        }

        TrieNode *tmp = root;
        root = root->children[0];
        deleteFwdData(tmp);
        free(tmp);
    }
}

TrieNode *trieFind(TrieNode *t, char const *num) {
    TrieNode *current = t;
    size_t i = 0;

    while (num[i] != '\0') {
        if (current == NULL)
            return NULL;

        current = current->children[charToDigit(num[i])];
        ++i;
    }

    return current;
}

TrieNode *trieFindNextNonEmpty(TrieNode *node, char const *num,
                               size_t *currIndex) {
    if (num[*currIndex] == '\0')
        return NULL;

    size_t originalIndex = *currIndex;

    TrieNode *current = node->children[charToDigit(num[*currIndex])];
    ++(*currIndex);

    while (num[*currIndex] != '\0' && current != NULL) {
        if (!isEmpty(current)) {
            return current;
        }

        current = current->children[charToDigit(num[*currIndex])];
        ++(*currIndex);
    }

    if (isEmpty(current)) {
        *currIndex = originalIndex;
        return NULL;
    }
    return current;
}

void trieRemove(TrieNode *t, char const *num) {
    TrieNode *nodeToDelete = trieFind(t, num);

    if (nodeToDelete != NULL) {
        TrieNode *parent = nodeToDelete->parent;

        for (unsigned int i = 0; i < 12; ++i)
            if (parent->children[i] == nodeToDelete)
                parent->children[i] = NULL;

        trieDelete(nodeToDelete);
        deleteDeadBranch(parent);
    }
}

bool addToReverseFwdList(TrieNode *node, TrieNode *nodeToAdd) {
    return listAdd(&node->listNode, nodeToAdd);
}

TrieNode *getFwdNode(TrieNode *node) {
    return node->fwdNode;
}

void setFwdNode(TrieNode *node, TrieNode *nodeToAdd) {
    node->fwdNode = nodeToAdd;
}

ListNode *getListNode(TrieNode *node) {
    return node->listNode;
}

void setListNode(TrieNode *node, ListNode *nodeToAdd) {
    node->listNode = nodeToAdd;
}

/**
 * Znajduje długość numeru odpowiadającego węzłowi drzewa.
 * @param[in] node – wskaźnik na węzeł drzewa.
 * @return Długość numeru odpowiadającego węzłowi @p node.
 */
static size_t length(TrieNode *node) {
    if (node == NULL)
        return 0;

    size_t result = 0;

    while (node->parent != NULL) {
        ++result;
        node = node->parent;
    }

    return result;
}

/**
 * Odwraca prefiks numeru.
 * @param[in, out] num – wskaźnik na numer;
 * @param[in] index    – długość prefiksu do odwrócenia.
 */
static void mirror(char *num, size_t index) {
    if (index == 0)
        return;

    for (size_t i = 0; 2 * i < index - 1; ++i) {
        char tmp = num[i];
        num[i] = num[index - i - 1];
        num[index - i - 1] = tmp;
    }
}

char *changePrefix(char const *num, TrieNode *newPrefixNode,
                   size_t index) {
    char *result;
    size_t numLength = index;

    while (num[numLength] != '\0')
        ++numLength;

    size_t newPrefLength = length(newPrefixNode);

    result = malloc((newPrefLength + numLength - index + 1) * sizeof(char));

    if (result == NULL)
        return NULL;

    size_t i = 0;
    if (newPrefLength > 0) {
        TrieNode *current = newPrefixNode;

        while (i < newPrefLength) {
            TrieNode *parent = current->parent;

            for (unsigned int j = 0; j < 12; ++j) {
                if (parent->children[j] == current)
                    result[i] = digitToChar(j);
            }

            current = parent;
            ++i;
        }

        // dodawaliśmy kolejne znaki do wyniku w odwrotnej kolejności,
        // więc odwracamy ten fragment wyniku
        mirror(result, newPrefLength);
    }

    while (i < newPrefLength + numLength - index + 1) {
        result[i] = num[i - newPrefLength + index];
        ++i;
    }

    return result;
}
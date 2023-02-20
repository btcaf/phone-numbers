/** @file
 * Implementacja listy wskaźnikowej przechowującej
 * wskaźniki do obiektów typu @p TrieNode.
 *
 * @author Cezary Botta <cb439922@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include <stdlib.h>

#include "list.h"

/**
 * Struktura @p ListNode przechowuje wartość w niej przechowywaną oraz
 * wskaźniki do elementów sąsiednich.
 */
struct ListNode {
    TrieNode *key; ///< wartość elementu listy
    ListNode *next; /**< wskaźnik na element następny lub NULL, jeśli
                           taki nie istnieje */
    ListNode *prev; /**< wskaźnik na element poprzedni lub NULL, jeśli
                           taki nie istnieje */
};

bool listAdd(ListNode **list, TrieNode *node) {
    ListNode *new = malloc(sizeof(struct ListNode));

    if (new == NULL)
        return false;

    new->key = node;
    new->next = *list;
    new->prev = NULL;
    if (*list != NULL)
        (*list)->prev = new;
    *list = new;

    return true;
}

void listRemove(ListNode *node) {
    if (node->next != NULL)
        node->next->prev = node->prev;
    if (node->prev != NULL)
        node->prev->next = node->next;

    free(node);
}

ListNode *getNext(ListNode *node) {
    return node->next;
}

TrieNode *getKey(ListNode *node) {
    return node->key;
}
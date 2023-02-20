/** @file
 * Interfejs klasy implementującej listę wskaźnikową przechowującą
 * wskaźniki do obiektów typu @p TrieNode.
 *
 * @author Cezary Botta <cb439922@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

/**
 * Deklarujemy typ @p TrieNode, aby móc z niego korzystać.
 */
typedef struct TrieNode TrieNode;

/**
 * Struktura reprezentująca węzeł listy.
 */
struct ListNode;

/**
 * Typ @p ListNode reprezentuje strukturę @p ListNode.
 */
typedef struct ListNode ListNode;

/** @brief Dodaje element na początek listy.
 * Dodaje element @p node na początek listy, na który wskazuje @p *list.
 * Jeśli @p *list wynosi NULL, to tworzy nową listę i ustawia @p *list
 * jako wskaźnik na jej jedyny element.
 * @param[in, out] list – wskaźnik na wskaźnik na początek listy;
 * @param[in] node      – element do dodania.
 * @return Wartość @p true, jeśli element został dodany.
 *         Wartość @p false, jeśli nie udało się alokować pamięci.
 */
bool listAdd(ListNode **list, TrieNode *node);

/**
 * Usuwa element listy wskazywany przez @p node.
 * @param[in, out] node – wskaźnik na element listy.
 */
void listRemove(ListNode *node);

/**
 * Znajduje następnik elementu listy.
 * @param[in] node – wskaźnik na element listy.
 * @return Następnik elementu listy lub NULL, jeśli @p node wskazuje
 * na ostatni element.
 */
ListNode *getNext(ListNode *node);

/**
 * Znajduje wartość przechowywaną w elemencie listy.
 * @param[in] node – wskaźnik na element listy.
 * @return Wartość przechowywana w elemencie @p node.
 */
TrieNode *getKey(ListNode *node);

#endif /* LIST_H */

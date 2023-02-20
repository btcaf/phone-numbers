/** @file
 * Interfejs klasy implementującej drzewo trie dla celów implementacji
 * struktury PhoneForward z interfejsu phone_forward.h.
 *
 * @author Cezary Botta <cb439922@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef TRIE_H
#define TRIE_H

#include <stdbool.h>

#include "list.h"

/**
 * Struktura reprezentująca węzeł drzewa trie. Przechowuje wskaźnik do
 * innego obiektu typu @p TrieNode oraz wskaźnik do obiektu typu @p ListNode.
 * Opisy zastosowań tych wskaźników znajdują się w pliku trie.c.
 */
struct TrieNode;

/**
 * Typ @p TrieNode reprezentuje strukturę @p TrieNode.
 */
typedef struct TrieNode TrieNode;

/**
 * Tworzy nowy, pusty węzeł drzewa.
 * @return Wskaźnik na utworzoną strukturę lub NULL, jeśli nie udało się
 *         alokować pamięci.
 */
TrieNode *trieNew(void);

/** @brief Usuwa martwą gałąź drzewa.
 * Jeśli parametr @p node jest liściem drzewa, to usuwa maksymalną gałąź
 * składającą się z pustych węzłów zawierającą @p node za wyjątkiem korzenia.
 * W szczególności jeśli @p node jest niepusty, to nie usuwa nic.
 * @param[in, out] node – wskaźnik na węzeł drzewa.
 */
void deleteDeadBranch(TrieNode *node);

/** @brief Dodaje nowy element do drzewa.
 * Jeśli w drzewie o korzeniu @p t znajduje się węzeł odpowiadający numerowi
 * @p num, to nie modyfikuje drzewa. W przeciwnym wypadku dodaje do @p t
 * nowy węzeł odpowiadający numerowi @p num.
 * @param[in, out] t – wskaźnik na korzeń drzewa;
 * @param[in] num    – wskaźnik na napis reprezentujący numer.
 * @return Wskaźnik na węzeł drzewa odpowiadający numerowi @p num lub NULL,
 *         jeśli nie udało się alokować pamięci.
 */
TrieNode *trieAdd(TrieNode *t, char const *num);

/** @brief Usuwa dane w węźle drzewa przekierowań.
 * Usuwa dane przechowane w węźle drzewa przekierowań oraz odpowiadający
 * mu element w liście w drzewie odwrotności przekierowań (i w razie potrzeby
 * również nieużywane węzły w tym drzewie).
 * @param[in, out] node – wskaźnik na węzeł drzewa przekierowań.
 */
void deleteFwdData(TrieNode *node);

/** @brief Usuwa poddrzewo drzewa przekierowań.
 * Usuwa poddrzewo węzła @p node (włącznie z tym węzłem) drzewa przekierowań
 * usuwając przy tym odpowiednie elementy list w drzewie odwrotności
 * przekierowań i nieużywane po tej operacji węzły tego drzewa.
 * @param[in, out] node – wskaźnik na węzeł drzewa przekierowań.
 */
void trieDelete(TrieNode *node);

/** Znajduje węzeł drzewa odpowiadający numerowi.
 * Znajduje węzeł drzewa o korzeniu @p t odpowiadający numerowi @p num.
 * @param[in] t   – wskaźnik na korzeń drzewa;
 * @param[in] num – wskaźnik na napis reprezentujący numer.
 * @return Węzeł drzewa @p t odpowiadający numerowi @p num lub NULL, jeśli
 *         taki nie istnieje.
 */
TrieNode *trieFind(TrieNode *t, char const *num);

/** @brief Znajduje pierwszy niepusty węzeł odpowiadający fragmentowi numeru.
 * Znajduje pierwszy niepusty węzeł (tzn. taki, w którym jeden
 * z przechowywanych wskaźników jest różny od NULL) odpowiadający prefiksowi
 * sufiksu numeru @p num od pozycji @p *currIndex poczynając od @p node
 * (jeśli @p node jest niepusty, to go nie wliczamy). Wartość @p *currIndex
 * jest przy tym odpowiednio zmieniana na indeks, dla którego znaleziono
 * niepusty węzeł.
 * @param[in] node           – wskaźnik na węzeł drzewa;
 * @param[in] num            – wskaźnik na napis reprezentujący numer.
 * @param[in, out] currIndex – wskaźnik na aktualny indeks @p num
 * @return Znaleziony węzeł lub NULL, jeśli taki nie istnieje.
 */
TrieNode *trieFindNextNonEmpty(TrieNode *node, char const *num,
                               size_t *currIndex);

/** @brief Usuwa węzły z drzewa przekierowań.
 * Usuwa wszystkie węzły z drzewa @p t, które odpowiadają numerom, który @p
 * num jest prefiksem. Usuwa przy tym odpowiednie elementy list w drzewie
 * odwrotności przekierowań i nieużywane po tej operacji węzły tego drzewa.
 * @param[in, out] t – wskaźnik na korzeń drzewa przekierowań;
 * @param[in] num    – wskaźnik na napis reprezentujący numer.
 */
void trieRemove(TrieNode *t, char const *num);

/**
 * Dodaje element do listy w węźle drzewa odwrotności przekierowań.
 * @param[in, out] node – wskaźnik na węzeł odwrotności drzewa przekierowań.
 * @param[in] nodeToAdd – wskaźnik na węzeł, który ma być dodany do listy.
 * @return Wartość @p true, jeśli element został dodany.
 *         Wartość @p false, jeśli nie udało się alokować pamięci.
 */
bool addToReverseFwdList(TrieNode *node, TrieNode *nodeToAdd);

/**
 * Znajduje węzeł, na który przekierowany jest @p node.
 * @param[in] node – wskaźnik na węzeł drzewa przekierowań.
 * @return Wskaźnik na węzeł, na który przekierowany jest @p node.
 */
TrieNode *getFwdNode(TrieNode *node);

/**
 * Ustawia nowy węzeł, na który przekierowany jest @p node.
 * @param[out] node     – wskaźnik na węzeł drzewa przekierowań;
 * @param[in] nodeToAdd – wskaźnik na nowy węzeł, na który przekierowany ma
 *                        być @p node.
 */
void setFwdNode(TrieNode *node, TrieNode *nodeToAdd);

/**
 * Znajduje węzeł listy przechowywany w @p node.
 * @param[in] node – wskaźnik na węzeł drzewa.
 * @return Wskaźnik na węzeł listy przechowywany w @p node.
 */
ListNode *getListNode(TrieNode *node);

/**
 * Ustawia nowy węzeł listy przechowywany w @p node.
 * @param[out] node     – wskaźnik na węzeł drzewa;
 * @param[in] nodeToAdd – wskaźnik na węzeł listy.
 */
void setListNode(TrieNode *node, ListNode *nodeToAdd);

/** @brief Zamienia prefiks numeru.
 * Zamienia prefiks numeru @p num złożony z jego pierwszych @p index cyfr
 * na prefiks odpowiadający węzłowi @p newPrefix i zwraca otrzymany numer
 * (nie zmieniając przy tym @p num).
 * @param[in] num           – wskaźnik na numer, którego prefiks ma
 *                            być zastąpiony;
 * @param[in] newPrefixNode – wskaźnik na węzeł drzewa;
 * @param[in] index         – indeks, do którego rozważamy prefiks do
 *                            zastąpienia.
 * @return Wskaźnik na napis z podmienionym prefiksem lub NULL, jeśli nie
 *         udało się alokować pamięci.
 */
char *changePrefix(char const *num, TrieNode *newPrefixNode,
                   size_t index);

#endif /* TRIE_H */

/*
 * algoritmos.h - Ordenacao (Insertion Sort) e Busca Binaria
 *
 * Ambos escritos a mao (sem qsort/bsearch), pela chave primaria (ISBN).
 */
#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "fila.h"

/* Insertion Sort sobre a fila encadeada: religa os nos, sem copiar dados.
 * Estavel. O(n) se ja estiver ordenada, O(n^2) no pior caso. */
void ordenar_insertion(struct Fila *f);

/* Cria um vetor de ponteiros para os nos, na ordem atual da fila.
 * Permite acesso por indice, necessario para a busca binaria (uma lista
 * encadeada nao tem acesso direto). Liberar com free(). NULL = sem memoria. */
struct No **indexar_fila(const struct Fila *f);

/* Busca binaria no vetor ordenado por ISBN. Retorna o indice ou -1. */
int busca_binaria(struct No *const *v, int n, const char *isbn);

#endif

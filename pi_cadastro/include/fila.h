/*
 * fila.h - Fila encadeada com alocacao dinamica
 *
 * Cada no e alocado com malloc e guarda um struct Livro por valor.
 * Novos registros entram no fim (enfileirar); a ordem final por chave
 * primaria e garantida por ordenar_insertion() (ver algoritmos.h).
 */
#ifndef FILA_H
#define FILA_H

#include "livro.h"

struct No {
    struct Livro livro;     /* o objeto guardado no no */
    struct No   *prox;
};

struct Fila {
    struct No *inicio;      /* frente da fila */
    struct No *fim;         /* final da fila */
    int tamanho;
};

struct Fila *fila_criar(void);                 /* NULL se faltar memoria */
void         fila_destruir(struct Fila *f);    /* libera todos os nos */
int          fila_vazia(const struct Fila *f);
int          fila_tamanho(const struct Fila *f);

/* Copia o livro para um no novo no fim. Retorna 0 se faltar memoria. */
int fila_enfileirar(struct Fila *f, const struct Livro *l);

/* Remove o inicio. Copia para 'saida' se nao for NULL. 0 se vazia. */
int fila_desenfileirar(struct Fila *f, struct Livro *saida);

/* Remove o no logo apos 'anterior' (ou o inicio, se anterior == NULL).
 * Usada na exclusao de um registro qualquer. 0 se nao houver no. */
int fila_remover_apos(struct Fila *f, struct No *anterior, struct Livro *saida);

#endif

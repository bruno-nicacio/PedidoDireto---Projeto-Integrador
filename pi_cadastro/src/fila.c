/* fila.c - implementacao da fila encadeada */
#include <stdlib.h>
#include "fila.h"

struct Fila *fila_criar(void)
{
    struct Fila *f = (struct Fila *)malloc(sizeof(struct Fila));
    if (f == NULL) return NULL;
    f->inicio = NULL;
    f->fim = NULL;
    f->tamanho = 0;
    return f;
}

void fila_destruir(struct Fila *f)
{
    if (f == NULL) return;
    while (fila_desenfileirar(f, NULL)) { /* libera no a no */ }
    free(f);
}

int fila_vazia(const struct Fila *f)
{
    return f->inicio == NULL;
}

int fila_tamanho(const struct Fila *f)
{
    return f->tamanho;
}

int fila_enfileirar(struct Fila *f, const struct Livro *l)
{
    struct No *novo = (struct No *)malloc(sizeof(struct No));
    if (novo == NULL) return 0;

    novo->livro = *l;       /* copia a struct para dentro do no */
    novo->prox = NULL;
    if (f->fim == NULL) f->inicio = novo;
    else                f->fim->prox = novo;
    f->fim = novo;
    f->tamanho++;
    return 1;
}

int fila_remover_apos(struct Fila *f, struct No *anterior, struct Livro *saida)
{
    struct No *alvo;

    if (f->inicio == NULL) return 0;
    if (anterior == NULL) {
        alvo = f->inicio;
        f->inicio = alvo->prox;
        if (f->inicio == NULL) f->fim = NULL;
    } else {
        alvo = anterior->prox;
        if (alvo == NULL) return 0;
        anterior->prox = alvo->prox;
        if (alvo == f->fim) f->fim = anterior;
    }
    if (saida != NULL) *saida = alvo->livro;
    free(alvo);
    f->tamanho--;
    return 1;
}

int fila_desenfileirar(struct Fila *f, struct Livro *saida)
{
    return fila_remover_apos(f, NULL, saida);
}

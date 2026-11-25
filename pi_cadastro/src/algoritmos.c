/* algoritmos.c - Insertion Sort e Busca Binaria */
#include <stdlib.h>
#include <string.h>
#include "algoritmos.h"

void ordenar_insertion(struct Fila *f)
{
    struct No *ordenada = NULL;     /* parte ja ordenada */
    struct No *atual = f->inicio;

    while (atual != NULL) {
        struct No *prox = atual->prox;

        /* Encaixa 'atual' na posicao certa da lista ordenada. */
        if (ordenada == NULL ||
            livro_comparar_chave(&atual->livro, &ordenada->livro) < 0) {
            atual->prox = ordenada;
            ordenada = atual;
        } else {
            struct No *p = ordenada;
            while (p->prox != NULL &&
                   livro_comparar_chave(&p->prox->livro, &atual->livro) <= 0)
                p = p->prox;
            atual->prox = p->prox;
            p->prox = atual;
        }
        atual = prox;
    }

    /* Atualiza inicio e fim da fila. */
    f->inicio = ordenada;
    f->fim = ordenada;
    while (f->fim != NULL && f->fim->prox != NULL)
        f->fim = f->fim->prox;
}

struct No **indexar_fila(const struct Fila *f)
{
    int n = f->tamanho, i = 0;
    struct No *p;
    struct No **v = (struct No **)malloc(sizeof(struct No *) * (n > 0 ? n : 1));

    if (v == NULL) return NULL;
    for (p = f->inicio; p != NULL; p = p->prox) v[i++] = p;
    return v;
}

int busca_binaria(struct No *const *v, int n, const char *isbn)
{
    int baixo = 0, alto = n - 1;

    while (baixo <= alto) {
        int meio = baixo + (alto - baixo) / 2;
        int cmp = strcmp(isbn, livro_get_isbn(&v[meio]->livro));

        if (cmp == 0) return meio;
        if (cmp < 0)  alto = meio - 1;
        else          baixo = meio + 1;
    }
    return -1;
}

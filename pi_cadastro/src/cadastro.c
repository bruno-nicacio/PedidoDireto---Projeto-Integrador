/* cadastro.c - operacoes de CRUD sobre a fila de livros */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cadastro.h"
#include "fila.h"
#include "algoritmos.h"
#include "arquivo.h"
#include "entrada.h"

#define TAM_BUF 256     /* maior que qualquer campo: texto longo e rejeitado */

struct Cadastro {
    struct Fila *fila;
    char caminho[256];
};

/* ---------- metodos privados ---------- */

/* Localiza um ISBN por busca binaria sobre a fila ordenada.
 * Retorna 1 (achou), 0 (nao achou) ou -1 (sem memoria).
 * Se achou, *no recebe o no e *anterior o no anterior (NULL se for o 1o). */
static int localizar(const struct Fila *f, const char *isbn,
                     struct No **no, struct No **anterior)
{
    struct No **vet = indexar_fila(f);
    int pos;

    if (vet == NULL) return -1;
    pos = busca_binaria(vet, fila_tamanho(f), isbn);
    if (pos >= 0) {
        if (no != NULL)       *no = vet[pos];
        if (anterior != NULL) *anterior = (pos > 0) ? vet[pos - 1] : NULL;
    }
    free(vet);
    return (pos >= 0) ? 1 : 0;
}

/* Ordena e grava no arquivo (chamada apos incluir, alterar e excluir). */
static void persistir(struct Cadastro *c)
{
    ordenar_insertion(c->fila);
    if (arquivo_salvar(c->fila, c->caminho))
        printf("Arquivo atualizado.\n");
    else
        printf("ERRO: nao foi possivel gravar '%s'. Dados so em memoria.\n",
               c->caminho);
}

/* ---------- construtor e destrutor ---------- */

struct Cadastro *cadastro_criar(const char *caminho_arquivo)
{
    struct Cadastro *c = (struct Cadastro *)malloc(sizeof(struct Cadastro));
    int n;

    if (c == NULL) return NULL;
    c->fila = fila_criar();
    if (c->fila == NULL) { free(c); return NULL; }
    snprintf(c->caminho, sizeof(c->caminho), "%s", caminho_arquivo);

    n = arquivo_carregar(c->fila, c->caminho);
    if (n < 0)       printf("Aviso: nao foi possivel ler '%s'.\n", c->caminho);
    else if (n == 0) printf("Nenhum registro carregado (arquivo novo ou vazio).\n");
    else             printf("%d registro(s) carregado(s) de '%s'.\n", n, c->caminho);

    ordenar_insertion(c->fila);     /* garante a ordem mesmo se o .txt foi editado */
    return c;
}

void cadastro_destruir(struct Cadastro *c)
{
    if (c == NULL) return;
    fila_destruir(c->fila);
    free(c);
}

/* ---------- CRUD ---------- */

void cadastro_incluir(struct Cadastro *c)
{
    char buf[TAM_BUF];
    int ano, r;
    struct Livro l;

    printf("\n--- Incluir livro ---\n");
    livro_iniciar(&l);

    ler_linha("ISBN (chave primaria): ", buf, sizeof(buf));
    if (!livro_set_isbn(&l, buf)) {
        printf("ISBN invalido (apenas digitos, '-' e 'X', ate %d caracteres).\n",
               TAM_ISBN - 1);
        return;
    }
    r = localizar(c->fila, livro_get_isbn(&l), NULL, NULL);
    if (r < 0) { printf("Erro: sem memoria.\n"); return; }
    if (r == 1) { printf("ISBN ja cadastrado. Inclusao cancelada.\n"); return; }

    ler_linha("Titulo: ", buf, sizeof(buf));
    if (!livro_set_titulo(&l, buf)) {
        printf("Titulo invalido (vazio, com ';' ou maior que %d).\n", TAM_TITULO - 1);
        return;
    }
    ler_linha("Autor : ", buf, sizeof(buf));
    if (!livro_set_autor(&l, buf)) {
        printf("Autor invalido (vazio, com ';' ou maior que %d).\n", TAM_AUTOR - 1);
        return;
    }
    if (!ler_inteiro("Ano   : ", &ano) || !livro_set_ano(&l, ano)) {
        printf("Ano invalido (use um numero entre %d e %d).\n", ANO_MIN, ANO_MAX);
        return;
    }

    printf("\nNovo registro:\n");
    livro_exibir(&l);
    if (!ler_confirmacao("Confirmar inclusao (s/n)? ")) {
        printf("Inclusao cancelada.\n");
        return;
    }
    if (!fila_enfileirar(c->fila, &l)) {
        printf("Erro: sem memoria.\n");
        return;
    }
    printf("Livro incluido.\n");
    persistir(c);                   /* ordena e salva */
}

void cadastro_alterar(struct Cadastro *c)
{
    char buf[TAM_BUF];
    struct No *no, *outro;
    struct Livro novo;
    int r, ano;

    printf("\n--- Alterar livro ---\n");
    ler_linha("ISBN do livro: ", buf, sizeof(buf));
    r = localizar(c->fila, buf, &no, NULL);
    if (r < 0) { printf("Erro: sem memoria.\n"); return; }
    if (r == 0) { printf("Livro nao encontrado.\n"); return; }

    printf("\nRegistro atual:\n");
    livro_exibir(&no->livro);
    printf("\n(Enter = manter o valor atual)\n");

    novo = no->livro;               /* edita uma copia; so grava se confirmar */

    ler_linha("Novo ISBN  : ", buf, sizeof(buf));
    if (buf[0] != '\0') {
        if (!livro_set_isbn(&novo, buf)) { printf("ISBN invalido. Alteracao cancelada.\n"); return; }
        r = localizar(c->fila, livro_get_isbn(&novo), &outro, NULL);
        if (r < 0) { printf("Erro: sem memoria.\n"); return; }
        if (r == 1 && outro != no) {
            printf("Ja existe outro livro com esse ISBN. Alteracao cancelada.\n");
            return;
        }
    }
    ler_linha("Novo titulo: ", buf, sizeof(buf));
    if (buf[0] != '\0' && !livro_set_titulo(&novo, buf)) {
        printf("Titulo invalido. Alteracao cancelada.\n");
        return;
    }
    ler_linha("Novo autor : ", buf, sizeof(buf));
    if (buf[0] != '\0' && !livro_set_autor(&novo, buf)) {
        printf("Autor invalido. Alteracao cancelada.\n");
        return;
    }
    ler_linha("Novo ano   : ", buf, sizeof(buf));
    if (buf[0] != '\0' &&
        (!texto_para_inteiro(buf, &ano) || !livro_set_ano(&novo, ano))) {
        printf("Ano invalido. Alteracao cancelada.\n");
        return;
    }

    printf("\nRegistro depois da alteracao:\n");
    livro_exibir(&novo);
    if (!ler_confirmacao("Confirmar alteracao (s/n)? ")) {
        printf("Alteracao cancelada.\n");
        return;
    }
    no->livro = novo;
    printf("Livro alterado.\n");
    persistir(c);                   /* reordena (a chave pode ter mudado) e salva */
}

void cadastro_excluir(struct Cadastro *c)
{
    char buf[TAM_BUF];
    struct No *no, *anterior;
    int r;

    printf("\n--- Excluir livro ---\n");
    ler_linha("ISBN do livro: ", buf, sizeof(buf));
    r = localizar(c->fila, buf, &no, &anterior);
    if (r < 0) { printf("Erro: sem memoria.\n"); return; }
    if (r == 0) { printf("Livro nao encontrado.\n"); return; }

    printf("\nRegistro encontrado:\n");
    livro_exibir(&no->livro);
    if (!ler_confirmacao("Confirmar exclusao (s/n)? ")) {
        printf("Exclusao cancelada.\n");
        return;
    }
    fila_remover_apos(c->fila, anterior, NULL);
    printf("Livro excluido.\n");
    persistir(c);
}

void cadastro_consultar(struct Cadastro *c)
{
    char buf[TAM_BUF];
    struct No *no;
    int r;

    printf("\n--- Consultar livro ---\n");
    ler_linha("ISBN do livro: ", buf, sizeof(buf));
    r = localizar(c->fila, buf, &no, NULL);     /* busca binaria */
    if (r < 0) { printf("Erro: sem memoria.\n"); return; }
    if (r == 0) { printf("Livro nao encontrado.\n"); return; }

    printf("\nRegistro encontrado:\n");
    livro_exibir(&no->livro);
}

void cadastro_listar(const struct Cadastro *c)
{
    const struct No *p;
    int pos = 1;

    if (fila_vazia(c->fila)) {
        printf("\nNenhum registro cadastrado.\n");
        return;
    }
    printf("\n=== LIVROS CADASTRADOS (%d), ordenados por ISBN ===\n",
           fila_tamanho(c->fila));
    livro_imprimir_cabecalho();
    for (p = c->fila->inicio; p != NULL; p = p->prox)
        livro_imprimir_linha(&p->livro, pos++);
    printf("--------------------------------------------------------------------------------\n");
}

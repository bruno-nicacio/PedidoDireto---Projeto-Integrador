/* arquivo.c - leitura e gravacao do arquivo texto */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "arquivo.h"
#include "entrada.h"

#define TAM_LINHA 512

/* Busca linear (so usada ao carregar, quando a fila ainda nao esta ordenada). */
static int ja_existe(const struct Fila *f, const char *isbn)
{
    const struct No *p;

    for (p = f->inicio; p != NULL; p = p->prox)
        if (strcmp(livro_get_isbn(&p->livro), isbn) == 0) return 1;
    return 0;
}

/* Converte uma linha em Livro e enfileira.
 * Retorna NULL se deu certo, ou o motivo do erro. */
static const char *processar_linha(struct Fila *f, char *linha)
{
    char *campo[4];
    char *p = linha;
    int i, ano;
    struct Livro l;

    for (i = 0; i < 4; i++) {
        campo[i] = p;
        if (i < 3) {
            p = strchr(p, ';');
            if (p == NULL) return "campos faltando";
            *p++ = '\0';
        }
    }
    if (strchr(campo[3], ';') != NULL)       return "campos demais";
    if (!texto_para_inteiro(campo[3], &ano)) return "ano nao e numero";
    if (!livro_criar(&l, campo[0], campo[1], campo[2], ano))
        return "dado invalido";
    if (ja_existe(f, livro_get_isbn(&l)))    return "ISBN repetido";
    if (!fila_enfileirar(f, &l))             return "sem memoria";
    return NULL;
}

int arquivo_carregar(struct Fila *f, const char *caminho)
{
    FILE *fp = fopen(caminho, "r");
    char linha[TAM_LINHA];
    int num = 0, carregados = 0;

    if (fp == NULL) return (errno == ENOENT) ? 0 : -1;

    while (fgets(linha, sizeof(linha), fp) != NULL) {
        size_t n = strlen(linha);
        const char *erro;

        num++;
        if (n > 0 && linha[n - 1] == '\n') {
            linha[--n] = '\0';
        } else if (!feof(fp)) {         /* linha maior que o buffer */
            int c;
            while ((c = fgetc(fp)) != '\n' && c != EOF) { /* descarta */ }
            printf("Aviso: linha %d ignorada (linha muito longa).\n", num);
            continue;
        }
        if (n > 0 && linha[n - 1] == '\r') linha[--n] = '\0';
        if (linha[0] == '\0' || linha[0] == '#') continue;

        erro = processar_linha(f, linha);
        if (erro != NULL) printf("Aviso: linha %d ignorada (%s).\n", num, erro);
        else carregados++;
    }
    fclose(fp);
    return carregados;
}

int arquivo_salvar(const struct Fila *f, const char *caminho)
{
    char tmp[300];
    FILE *fp;
    const struct No *p;
    int ok;

    if (snprintf(tmp, sizeof(tmp), "%s.tmp", caminho) >= (int)sizeof(tmp))
        return 0;
    fp = fopen(tmp, "w");
    if (fp == NULL) return 0;

    fprintf(fp, "# ISBN;TITULO;AUTOR;ANO\n");
    for (p = f->inicio; p != NULL; p = p->prox)
        fprintf(fp, "%s;%s;%s;%d\n",
                livro_get_isbn(&p->livro), livro_get_titulo(&p->livro),
                livro_get_autor(&p->livro), livro_get_ano(&p->livro));

    ok = (ferror(fp) == 0);
    if (fclose(fp) != 0) ok = 0;
    if (!ok) { remove(tmp); return 0; }

    if (rename(tmp, caminho) != 0) {    /* Windows nao sobrescreve no rename */
        remove(caminho);
        if (rename(tmp, caminho) != 0) { remove(tmp); return 0; }
    }
    return 1;
}

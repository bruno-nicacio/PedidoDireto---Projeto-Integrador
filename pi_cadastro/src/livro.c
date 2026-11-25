/* livro.c - implementacao da classe Livro */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "livro.h"

#define COL_ISBN   17
#define COL_TITULO 32
#define COL_AUTOR  22

/* ---------- metodos privados (static) ---------- */

/* Texto valido: nao vazio, so espacos nao vale, cabe no campo, sem ';'
 * (separador do arquivo) e sem caracteres de controle. */
static int texto_valido(const char *s, size_t max)
{
    size_t i, n;
    int tem_visivel = 0;

    if (s == NULL) return 0;
    n = strlen(s);
    if (n == 0 || n >= max) return 0;
    for (i = 0; i < n; i++) {
        unsigned char c = (unsigned char)s[i];
        if (c == ';' || c < 0x20 || c == 0x7F) return 0;
        if (!isspace(c)) tem_visivel = 1;
    }
    return tem_visivel;
}

/* ISBN: apenas digitos, '-' e 'X', com pelo menos um digito. */
static int isbn_valido(const char *s)
{
    size_t i, n;
    int digitos = 0;

    if (s == NULL) return 0;
    n = strlen(s);
    if (n == 0 || n >= TAM_ISBN) return 0;
    for (i = 0; i < n; i++) {
        if (isdigit((unsigned char)s[i])) digitos++;
        else if (s[i] != '-' && s[i] != 'X') return 0;
    }
    return digitos > 0;
}

/* Imprime texto em uma coluna de largura fixa. Conta caracteres UTF-8
 * (nao bytes) para nao cortar acento ao meio nem desalinhar a tabela. */
static void imprimir_campo(const char *s, int largura)
{
    const unsigned char *p = (const unsigned char *)s;
    int cols = 0;

    while (*p && cols < largura) {
        putchar(*p++);
        while ((*p & 0xC0) == 0x80) putchar(*p++);
        cols++;
    }
    while (cols++ < largura) putchar(' ');
}

/* Implementacao do metodo exibir para Livro (sobrescrita). */
static void livro_exibir_impl(const struct Publicacao *self)
{
    /* Seguro: base e o primeiro campo de struct Livro. */
    const struct Livro *l = (const struct Livro *)self;

    printf("  Tipo   : %s\n", self->vt->nome_tipo);
    printf("  ISBN   : %s\n", livro_get_isbn(l));
    printf("  Titulo : %s\n", livro_get_titulo(l));
    printf("  Autor  : %s\n", livro_get_autor(l));
    printf("  Ano    : %d\n", livro_get_ano(l));
}

/* Tabela de metodos unica de Livro (compartilhada por todos os objetos). */
static const struct PublicacaoVTable vtable_livro = {
    "Livro",
    livro_exibir_impl
};

/* ---------- construtores ---------- */

void livro_iniciar(struct Livro *l)
{
    memset(l, 0, sizeof(*l));
    l->base.vt = &vtable_livro;
}

int livro_criar(struct Livro *l, const char *isbn, const char *titulo,
                const char *autor, int ano)
{
    livro_iniciar(l);
    if (!livro_set_isbn(l, isbn))     return 0;
    if (!livro_set_titulo(l, titulo)) return 0;
    if (!livro_set_autor(l, autor))   return 0;
    if (!livro_set_ano(l, ano))       return 0;
    return 1;
}

/* ---------- setters e getters ---------- */

int livro_set_isbn(struct Livro *l, const char *isbn)
{
    if (!isbn_valido(isbn)) return 0;
    strcpy(l->base.isbn, isbn);
    return 1;
}

int livro_set_titulo(struct Livro *l, const char *titulo)
{
    if (!texto_valido(titulo, TAM_TITULO)) return 0;
    strcpy(l->base.titulo, titulo);
    return 1;
}

int livro_set_autor(struct Livro *l, const char *autor)
{
    if (!texto_valido(autor, TAM_AUTOR)) return 0;
    strcpy(l->autor, autor);
    return 1;
}

int livro_set_ano(struct Livro *l, int ano)
{
    if (ano < ANO_MIN || ano > ANO_MAX) return 0;
    l->base.ano = ano;
    return 1;
}

const char *livro_get_isbn(const struct Livro *l)   { return l->base.isbn; }
const char *livro_get_titulo(const struct Livro *l) { return l->base.titulo; }
const char *livro_get_autor(const struct Livro *l)  { return l->autor; }
int         livro_get_ano(const struct Livro *l)    { return l->base.ano; }

/* ---------- comparacao e exibicao ---------- */

int livro_comparar_chave(const struct Livro *a, const struct Livro *b)
{
    return strcmp(a->base.isbn, b->base.isbn);
}

/* Chamada polimorfica: executa o exibir do tipo real do objeto. */
void publicacao_exibir(const struct Publicacao *p)
{
    p->vt->exibir(p);
}

void livro_exibir(const struct Livro *l)
{
    publicacao_exibir(&l->base);
}

void livro_imprimir_cabecalho(void)
{
    printf("%-4s ", "No");
    imprimir_campo("ISBN", COL_ISBN);   putchar(' ');
    imprimir_campo("Titulo", COL_TITULO); putchar(' ');
    imprimir_campo("Autor", COL_AUTOR);   putchar(' ');
    printf("Ano\n");
    printf("--------------------------------------------------------------------------------\n");
}

void livro_imprimir_linha(const struct Livro *l, int pos)
{
    printf("%-4d ", pos);
    imprimir_campo(livro_get_isbn(l), COL_ISBN);     putchar(' ');
    imprimir_campo(livro_get_titulo(l), COL_TITULO); putchar(' ');
    imprimir_campo(livro_get_autor(l), COL_AUTOR);   putchar(' ');
    printf("%d\n", livro_get_ano(l));
}

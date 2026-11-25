/*
 * livro.h - "Classe" Livro (POO simulada em C)
 *
 * Conceitos de POO aplicados:
 *  - Classe/objeto ...... struct Livro (cada variavel e um objeto)
 *  - Heranca ............ struct Livro "herda" de struct Publicacao por
 *                         composicao (base e SEMPRE o primeiro campo)
 *  - Polimorfismo ....... tabela de metodos (vtable) na Publicacao; cada
 *                         tipo aponta para a sua propria implementacao
 *  - Sobrescrita ........ livro_exibir_impl() implementa o metodo exibir
 *  - Construtor ......... livro_iniciar() / livro_criar()
 *  - Encapsulamento ..... campos lidos/alterados so por getters e setters
 *                         com validacao
 */
#ifndef LIVRO_H
#define LIVRO_H

#define TAM_ISBN   18      /* 17 caracteres + '\0' */
#define TAM_TITULO 80
#define TAM_AUTOR  60
#define ANO_MIN    1000
#define ANO_MAX    2100

struct Publicacao;

/* Tabela de metodos: "interface" que cada tipo de publicacao preenche. */
struct PublicacaoVTable {
    const char *nome_tipo;
    void (*exibir)(const struct Publicacao *self);
};

/* Classe base. */
struct Publicacao {
    const struct PublicacaoVTable *vt;
    char isbn[TAM_ISBN];        /* chave primaria */
    char titulo[TAM_TITULO];
    int  ano;
};

/* Classe derivada. Guardada em memoria, ordenada e gravada em arquivo. */
struct Livro {
    struct Publicacao base;     /* heranca: deve ser o primeiro campo */
    char autor[TAM_AUTOR];
};

/* Construtores */
void livro_iniciar(struct Livro *l);
int  livro_criar(struct Livro *l, const char *isbn, const char *titulo,
                 const char *autor, int ano);

/* Setters: validam e retornam 1 (ok) ou 0 (valor invalido). */
int livro_set_isbn(struct Livro *l, const char *isbn);
int livro_set_titulo(struct Livro *l, const char *titulo);
int livro_set_autor(struct Livro *l, const char *autor);
int livro_set_ano(struct Livro *l, int ano);

/* Getters */
const char *livro_get_isbn(const struct Livro *l);
const char *livro_get_titulo(const struct Livro *l);
const char *livro_get_autor(const struct Livro *l);
int         livro_get_ano(const struct Livro *l);

/* Compara pela chave primaria (ISBN). Mesmo retorno de strcmp. */
int livro_comparar_chave(const struct Livro *a, const struct Livro *b);

/* Metodos de exibicao */
void publicacao_exibir(const struct Publicacao *p);   /* polimorfico */
void livro_exibir(const struct Livro *l);             /* dados completos */
void livro_imprimir_cabecalho(void);
void livro_imprimir_linha(const struct Livro *l, int pos);

#endif

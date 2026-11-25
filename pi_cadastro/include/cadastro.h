/*
 * cadastro.h - Cadastro de livros (CRUD)
 *
 * Encapsulamento: struct Cadastro e OPACA. Sua definicao fica em
 * cadastro.c, entao quem usa este .h so consegue usar os metodos abaixo.
 */
#ifndef CADASTRO_H
#define CADASTRO_H

struct Cadastro;

/* Construtor: cria o cadastro e carrega o arquivo (se existir).
 * Retorna NULL se faltar memoria. */
struct Cadastro *cadastro_criar(const char *caminho_arquivo);

/* Destrutor: libera a fila e o proprio cadastro. */
void cadastro_destruir(struct Cadastro *c);

/* Operacoes do menu (cada uma interage com o usuario). */
void cadastro_incluir(struct Cadastro *c);
void cadastro_alterar(struct Cadastro *c);
void cadastro_excluir(struct Cadastro *c);
void cadastro_consultar(struct Cadastro *c);
void cadastro_listar(const struct Cadastro *c);

#endif

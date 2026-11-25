/*
 * arquivo.h - Persistencia em arquivo texto (.txt)
 *
 * Formato: uma linha por registro, campos separados por ';'
 *     ISBN;TITULO;AUTOR;ANO
 * Linhas vazias e linhas iniciadas por '#' sao ignoradas (comentarios).
 */
#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "fila.h"

/* Le o arquivo e enfileira os registros validos. Linhas invalidas ou com
 * ISBN repetido sao ignoradas com aviso.
 * Retorna: quantidade carregada; 0 se o arquivo nao existe (1a execucao);
 * -1 se existe mas nao pode ser lido. */
int arquivo_carregar(struct Fila *f, const char *caminho);

/* Grava toda a fila (na ordem em que esta). Escreve em arquivo temporario
 * e troca no final, para nao perder dados se der erro no meio.
 * Retorna 1 (ok) ou 0 (erro). */
int arquivo_salvar(const struct Fila *f, const char *caminho);

#endif

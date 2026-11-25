/* entrada.h - leitura segura do teclado */
#ifndef ENTRADA_H
#define ENTRADA_H

#include <stddef.h>

/* Le uma linha, remove '\n' e espacos nas pontas. Retorna 0 no fim da
 * entrada (EOF). Se a linha for maior que o buffer, o excesso e descartado
 * (o texto fica com tam-1 caracteres; os setters rejeitam por tamanho). */
int ler_linha(const char *msg, char *dest, size_t tam);

/* Converte texto em inteiro. Retorna 1 se todo o texto for um numero. */
int texto_para_inteiro(const char *texto, int *valor);

/* Le um inteiro. Retorna 1 se valido, 0 caso contrario. */
int ler_inteiro(const char *msg, int *valor);

/* Pergunta s/n ate receber resposta valida. 1 = sim, 0 = nao (ou EOF). */
int ler_confirmacao(const char *msg);

/* 1 se a entrada acabou. */
int entrada_encerrada(void);

#endif

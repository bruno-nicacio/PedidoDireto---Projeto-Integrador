/* entrada.c - leitura segura do teclado */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "entrada.h"

/* Remove espacos no inicio e no fim, no proprio buffer. */
static void aparar(char *s)
{
    size_t ini = 0, fim = strlen(s);

    while (fim > 0 && isspace((unsigned char)s[fim - 1])) fim--;
    s[fim] = '\0';
    while (s[ini] != '\0' && isspace((unsigned char)s[ini])) ini++;
    if (ini > 0) memmove(s, s + ini, fim - ini + 1);
}

int ler_linha(const char *msg, char *dest, size_t tam)
{
    size_t n;
    int c;

    printf("%s", msg);
    fflush(stdout);
    if (fgets(dest, (int)tam, stdin) == NULL) {
        dest[0] = '\0';
        return 0;
    }
    n = strlen(dest);
    if (n > 0 && dest[n - 1] == '\n') {
        dest[n - 1] = '\0';
    } else {
        while ((c = getchar()) != '\n' && c != EOF) { /* descarta o resto */ }
    }
    aparar(dest);
    return 1;
}

int texto_para_inteiro(const char *texto, int *valor)
{
    char *fim;
    long v;

    if (texto == NULL || texto[0] == '\0') return 0;
    v = strtol(texto, &fim, 10);
    if (*fim != '\0' || v < INT_MIN || v > INT_MAX) return 0;
    *valor = (int)v;
    return 1;
}

int ler_inteiro(const char *msg, int *valor)
{
    char buf[32];

    if (!ler_linha(msg, buf, sizeof(buf))) return 0;
    return texto_para_inteiro(buf, valor);
}

int ler_confirmacao(const char *msg)
{
    char buf[16];

    for (;;) {
        if (!ler_linha(msg, buf, sizeof(buf))) return 0;
        if ((buf[0] == 's' || buf[0] == 'S') && buf[1] == '\0') return 1;
        if ((buf[0] == 'n' || buf[0] == 'N') && buf[1] == '\0') return 0;
        printf("Responda s ou n.\n");
    }
}

int entrada_encerrada(void)
{
    return feof(stdin);
}

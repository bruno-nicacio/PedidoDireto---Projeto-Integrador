/*
 * main.c - Sistema de cadastro de livros
 * Projeto Integrador: Estrutura de Dados + POO (em C)
 *
 * Uso: ./cadastro [arquivo.txt]     (padrao: data/livros.txt)
 */
#include <stdio.h>
#include "cadastro.h"
#include "entrada.h"

#define ARQUIVO_PADRAO "data/livros.txt"

static void exibir_menu(void)
{
    printf("\n====================================\n");
    printf("        SISTEMA DE CADASTRO\n");
    printf("====================================\n");
    printf("1 - Incluir\n");
    printf("2 - Alterar\n");
    printf("3 - Excluir\n");
    printf("4 - Consultar\n");
    printf("5 - Listar\n");
    printf("6 - Sair\n");
}

int main(int argc, char **argv)
{
    const char *caminho = (argc > 1) ? argv[1] : ARQUIVO_PADRAO;
    struct Cadastro *cad = cadastro_criar(caminho);
    int opcao = 0;

    if (cad == NULL) {
        printf("Erro: sem memoria.\n");
        return 1;
    }

    do {
        exibir_menu();
        if (!ler_inteiro("Escolha uma opcao: ", &opcao)) {
            if (entrada_encerrada()) break;
            opcao = 0;
        }
        switch (opcao) {
            case 1: cadastro_incluir(cad);   break;
            case 2: cadastro_alterar(cad);   break;
            case 3: cadastro_excluir(cad);   break;
            case 4: cadastro_consultar(cad); break;
            case 5: cadastro_listar(cad);    break;
            case 6: printf("\nEncerrando...\n"); break;
            default: printf("\nOpcao invalida.\n");
        }
    } while (opcao != 6);

    cadastro_destruir(cad);
    return 0;
}

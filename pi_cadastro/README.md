# Cadastro de Livros — Projeto Integrador (Estrutura de Dados + POO, em C)

Sistema de cadastro com **fila encadeada** de `struct Livro`, CRUD, arquivo `.txt`,
**Insertion Sort** e **busca binária** pela chave primária (ISBN).

## Compilar e executar

```
make
./cadastro                 # usa data/livros.txt
./cadastro outro.txt       # usa outro arquivo
```

Sem `make`:

```
gcc -Wall -Wextra -pedantic -std=c99 -Iinclude -o cadastro src/*.c
```

## Estrutura de pastas

```
include/   cabeçalhos (.h)
src/       código-fonte (.c)
data/      livros.txt (registros de demonstração)
```

| Arquivo | Papel |
|---|---|
| `livro.c/.h` | "classe" Livro: struct, getters/setters com validação, exibição |
| `fila.c/.h` | fila encadeada com `malloc`/`free` (enfileirar, desenfileirar, remover) |
| `algoritmos.c/.h` | Insertion Sort sobre a fila e busca binária (escritos à mão) |
| `arquivo.c/.h` | leitura e gravação do `.txt` |
| `cadastro.c/.h` | operações do CRUD (struct `Cadastro` opaca) |
| `entrada.c/.h` | leitura segura do teclado |
| `main.c` | menu com as 6 opções |

## Como cada requisito do PDF é atendido

- **Struct**: `struct Livro` guarda o registro em memória, é ordenada e gravada em arquivo.
- **Chave primária**: ISBN (único; duplicidade é bloqueada na inclusão, na alteração e ao carregar o arquivo).
- **Ordenação**: `ordenar_insertion()` roda após incluir, após alterar e antes de gravar (`persistir()` em `cadastro.c`) e ao carregar o arquivo.
- **Busca binária**: `busca_binaria()` é usada em consultar, alterar, excluir e na checagem de duplicidade. Como uma lista encadeada não tem acesso por índice, `indexar_fila()` monta um vetor de ponteiros para os nós e a busca roda sobre ele.
- **Persistência**: carrega ao iniciar; grava após incluir, alterar e excluir. Arquivo inexistente na 1ª execução é tratado. A gravação usa arquivo temporário para não corromper os dados se der erro.
- **Formato do arquivo**: uma linha por registro, `ISBN;TITULO;AUTOR;ANO`. Linhas vazias e iniciadas por `#` são ignoradas. Campos não podem conter `;`.
- **Confirmação**: pede `s/n` antes de incluir, alterar e excluir.

## Conceitos de POO aplicados (simulados em C)

| Conceito | Onde |
|---|---|
| Classe e objeto | `struct Livro`, `struct Fila`, `struct Cadastro` |
| Herança (composição) | `struct Livro` tem `struct Publicacao base` como primeiro campo |
| Polimorfismo e sobrescrita | `struct PublicacaoVTable` (ponteiro para função `exibir`); `livro_exibir_impl` sobrescreve; `publicacao_exibir` chama o método do tipo real |
| Construtor e destrutor | `livro_iniciar`/`livro_criar`, `fila_criar`/`fila_destruir`, `cadastro_criar`/`cadastro_destruir` |
| Encapsulamento | getters/setters validados; `struct Cadastro` opaca (definida só em `cadastro.c`); funções privadas `static` |
| `const` | parâmetros que não são alterados |

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// Aqui eu crio a struct da peça, bem simples mesmo
typedef struct {
    char nome;
    int id;
} Peca;

// Fila circular (estou usando essa porque a lista gira sozinha)
typedef struct {
    Peca dados[TAM_FILA];
    int inicio;
    int fim;
    int qtd;
} Fila;

// Pilha (é só o topo que importa)
typedef struct {
    Peca dados[TAM_PILHA];
    int topo;
} Pilha;

/*
    Essa função gera uma peça nova com tipo aleatório.
    O ID vai aumentando sempre pra cada peça criada.
*/
Peca gerarPeca() {
    static int idAtual = 0;
    char tipos[] = {'I', 'O', 'T', 'L'};

    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = idAtual++;

    return p;
}

// Começo a fila zerada
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->qtd = 0;
}

// Começo a pilha vazia (topo = -1)
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Enfileirar: colocar peça no final
void enfileirar(Fila *f, Peca x) {
    if (f->qtd == TAM_FILA) {
        // fila cheia, mas pelo enunciado isso não deve acontecer
        return;
    }

    f->dados[f->fim] = x;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->qtd++;
}

// Tirar peça da frente da fila
Peca desenfileirar(Fila *f) {
    Peca aux = f->dados[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->qtd--;
    return aux;
}

// Colocar peça no topo da pilha
void empilhar(Pilha *p, Peca x) {
    if (p->topo == TAM_PILHA - 1) {
        // pilha cheia
        return;
    }

    p->topo++;
    p->dados[p->topo] = x;
}

// Tirar peça do topo
Peca desempilhar(Pilha *p) {
    Peca aux = p->dados[p->topo];
    p->topo--;
    return aux;
}

/*
    Só imprime o estado da fila e da pilha.
    Fiz questão de deixar bonitinho porque ajuda a visualizar.
*/
void mostrarEstado(Fila *f, Pilha *p) {
    printf("\n------------------------------\n");
    printf(" Estado atual das peças\n");
    printf("------------------------------\n");

    printf("Fila: ");
    int idx = f->inicio;
    for (int i = 0; i < f->qtd; i++) {
        printf("[%c %d] ", f->dados[idx].nome, f->dados[idx].id);
        idx = (idx + 1) % TAM_FILA;
    }
    printf("\n");

    printf("Pilha (Topo -> Base): ");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->dados[i].nome, p->dados[i].id);
    }
    printf("\n\n");
}

// Troca simples entre a peça da fila e a da pilha
void trocarSimples(Fila *f, Pilha *p) {
    if (p->topo == -1) {
        // pilha vazia, não tem o que trocar
        return;
    }

    Peca temp = f->dados[f->inicio];
    f->dados[f->inicio] = p->dados[p->topo];
    p->dados[p->topo] = temp;
}

// Troca as 3 primeiras peças da fila com as 3 da pilha
void trocarTres(Fila *f, Pilha *p) {
    if (f->qtd < 3 || p->topo < 2) {
        // não tem 3 peças em algum dos lados
        return;
    }

    int idx = f->inicio;

    for (int i = 0; i < 3; i++) {
        Peca temp = f->dados[idx];
        f->dados[idx] = p->dados[p->topo - i];
        p->dados[p->topo - i] = temp;

        idx = (idx + 1) % TAM_FILA;
    }
}

int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preencho a fila com peças iniciais
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca());
    }

    int opcao;

    do {
        mostrarEstado(&fila, &pilha);

        printf("Opções:\n");
        printf("1 - Jogar peça da fila\n");
        printf("2 - Reservar peça (fila → pilha)\n");
        printf("3 - Usar peça reservada (pilha)\n");
        printf("4 - Trocar peça da fila com o topo da pilha\n");
        printf("5 - Trocar 3 peças entre fila e pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                if (fila.qtd > 0) {
                    desenfileirar(&fila);
                    enfileirar(&fila, gerarPeca());
                }
                break;
            }
            case 2: {
                if (fila.qtd > 0 && pilha.topo < TAM_PILHA - 1) {
                    Peca retirada = desenfileirar(&fila);
                    empilhar(&pilha, retirada);
                    enfileirar(&fila, gerarPeca());
                }
                break;
            }
            case 3: {
                if (pilha.topo >= 0) {
                    desempilhar(&pilha);
                }
                break;
            }
            case 4:
                trocarSimples(&fila, &pilha);
                break;

            case 5:
                trocarTres(&fila, &pilha);
                break;

            case 0:
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("Opção inválida, tente outra.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}

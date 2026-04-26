#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// Estrutura da Peça
typedef struct {
    char nome;
    int id;
} Peca;

// Estrutura da Fila Circular
typedef struct {
    Peca itens[TAM_FILA];
    int frente, fim, total;
} FilaCircular;

// Estrutura da Pilha Linear
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} PilhaReserva;

int contadorId = 0;

// --- Protótipos ---
void inicializar(FilaCircular *f, PilhaReserva *p);
Peca gerarPeca();
void enfileirar(FilaCircular *f);
Peca desenfileirar(FilaCircular *f);
void empilhar(PilhaReserva *p, Peca peca);
Peca desempilhar(PilhaReserva *p);
void exibirEstado(FilaCircular f, PilhaReserva p);

int main() {
    srand(time(NULL));
    FilaCircular fila;
    PilhaReserva pilha;
    
    inicializar(&fila, &pilha);

    int opcao = -1;
    while (opcao != 0) {
        exibirEstado(fila, pilha);
        printf("\nOpcoes de Acao:\n");
        printf("1 - Jogar peca\n");
        printf("2 - Reservar peca\n");
        printf("3 - Usar peca reservada\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: // Jogar peça: Sai da fila, entra nova na fila
                desenfileirar(&fila);
                enfileirar(&fila);
                break;

            case 2: // Reservar: Sai da fila -> Pilha, entra nova na fila
                if (pilha.topo < TAM_PILHA - 1) {
                    Peca p = desenfileirar(&fila);
                    empilhar(&pilha, p);
                    enfileirar(&fila);
                } else {
                    printf("\n[AVISO] Pilha de reserva cheia!\n");
                }
                break;

            case 3: // Usar reservada: Sai da pilha, nada acontece na fila
                if (pilha.topo != -1) {
                    Peca p = desempilhar(&pilha);
                    printf("\n>> Voce usou a peca reservada [%c %d]!\n", p.nome, p.id);
                } else {
                    printf("\n[ERRO] Pilha vazia!\n");
                }
                break;
        }
    }
    return 0;
}

// --- Implementações ---

void inicializar(FilaCircular *f, PilhaReserva *p) {
    f->frente = 0;
    f->fim = -1;
    f->total = 0;
    p->topo = -1;
    // Preenchimento inicial da fila
    for (int i = 0; i < TAM_FILA; i++) enfileirar(f);
}

Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova = {tipos[rand() % 4], contadorId++};
    return nova;
}

void enfileirar(FilaCircular *f) {
    if (f->total < TAM_FILA) {
        f->fim = (f->fim + 1) % TAM_FILA;
        f->itens[f->fim] = gerarPeca();
        f->total++;
    }
}

Peca desenfileirar(FilaCircular *f) {
    Peca p = f->itens[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->total--;
    return p;
}

void empilhar(PilhaReserva *p, Peca peca) {
    p->itens[++(p->topo)] = peca;
}

Peca desempilhar(PilhaReserva *p) {
    return p->itens[(p->topo)--];
}

void exibirEstado(FilaCircular f, PilhaReserva p) {
    printf("\nEstado atual:\n");
    
    // Exibição da Fila
    printf("Fila de pecas: ");
    for (int i = 0; i < f.total; i++) {
        printf("[%c %d] ", f.itens[(f.frente + i) % TAM_FILA].nome, f.itens[(f.frente + i) % TAM_FILA].id);
    }

    // Exibição da Pilha (LIFO: Topo para Base)
    printf("\nPilha de reserva (Topo -> Base): ");
    if (p.topo == -1) printf("[Vazia]");
    for (int i = p.topo; i >= 0; i--) {
        printf("[%c %d] ", p.itens[i].nome, p.itens[i].id);
    }
    printf("\n------------------------------------------\n");
}
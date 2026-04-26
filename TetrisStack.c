#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// Estrutura que representa uma peça
typedef struct {
    char nome;
    int id;
} Peca;

// Estrutura da Fila Circular
typedef struct {
    Peca itens[TAM_FILA];
    int frente, fim, total;
} FilaCircular;

// Estrutura da Pilha
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
void trocarTopo(FilaCircular *f, PilhaReserva *p);
void trocaMultipla(FilaCircular *f, PilhaReserva *p);
void exibirEstado(FilaCircular f, PilhaReserva p);

int main() {
    srand(time(NULL));
    FilaCircular fila;
    PilhaReserva pilha;
    
    inicializar(&fila, &pilha);

    int opcao = -1;
    while (opcao != 0) {
        exibirEstado(fila, pilha);
        printf("\nOpcoes disponiveis:\n");
        printf("1 - Jogar peca da frente da fila\n");
        printf("2 - Enviar peca da fila para a reserva\n");
        printf("3 - Usar peca da reserva\n");
        printf("4 - Trocar frente da fila com topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com os 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Opcao escolhida: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                desenfileirar(&fila);
                enfileirar(&fila);
                break;
            case 2:
                if (pilha.topo < TAM_PILHA - 1) {
                    empilhar(&pilha, desenfileirar(&fila));
                    enfileirar(&fila);
                } else printf("\n[AVISO] Reserva cheia!\n");
                break;
            case 3:
                if (pilha.topo != -1) desempilhar(&pilha);
                else printf("\n[AVISO] Reserva vazia!\n");
                break;
            case 4:
                trocarTopo(&fila, &pilha);
                break;
            case 5:
                trocaMultipla(&fila, &pilha);
                break;
            case 0:
                printf("Encerrando...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    }
    return 0;
}

// --- Funções de Inicialização e Geração ---

void inicializar(FilaCircular *f, PilhaReserva *p) {
    f->frente = 0; f->fim = -1; f->total = 0;
    p->topo = -1;
    for (int i = 0; i < TAM_FILA; i++) enfileirar(f);
}

Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova = {tipos[rand() % 4], contadorId++};
    return nova;
}

// --- Operações de Estruturas ---

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

// --- Lógica de Trocas (Requisitos Avançados) ---

void trocarTopo(FilaCircular *f, PilhaReserva *p) {
    if (f->total > 0 && p->topo != -1) {
        Peca temp = f->itens[f->frente];
        f->itens[f->frente] = p->itens[p->topo];
        p->itens[p->topo] = temp;
        printf("\n[SUCESSO] Troca simples realizada!\n");
    } else {
        printf("\n[ERRO] Fila ou Pilha insuficientes para troca.\n");
    }
}

void trocaMultipla(FilaCircular *f, PilhaReserva *p) {
    // Requisito: Ambas precisam ter pelo menos 3 peças
    if (f->total >= 3 && p->topo >= 2) {
        for (int i = 0; i < 3; i++) {
            // O índice da fila deve respeitar a circularidade a partir da frente
            int idxFila = (f->frente + i) % TAM_FILA;
            // O índice da pilha deve ser do topo para baixo (LIFO)
            int idxPilha = p->topo - i;

            Peca temp = f->itens[idxFila];
            f->itens[idxFila] = p->itens[idxPilha];
            p->itens[idxPilha] = temp;
        }
        printf("\n[SUCESSO] Troca múltipla (3 peças) concluída!\n");
    } else {
        printf("\n[ERRO] Necessário 3 peças na fila e 3 na reserva.\n");
    }
}

// --- Exibição ---

void exibirEstado(FilaCircular f, PilhaReserva p) {
    printf("\nEstado atual:\n");
    printf("Fila de pecas: ");
    for (int i = 0; i < f.total; i++) {
        int idx = (f.frente + i) % TAM_FILA;
        printf("[%c %d] ", f.itens[idx].nome, f.itens[idx].id);
    }
    printf("\nPilha de reserva (Topo -> Base): ");
    if (p.topo == -1) printf("[Vazia]");
    for (int i = p.topo; i >= 0; i--) {
        printf("[%c %d] ", p.itens[i].nome, p.itens[i].id);
    }
    printf("\n------------------------------------------------\n");
}
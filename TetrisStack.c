#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO 5 // Capacidade máxima da fila de peças futuras

// Estrutura que define uma peça de Tetris
typedef struct {
    char nome; // 'I', 'O', 'T', 'L'
    int id;    // Identificador único
} Peca;

// Estrutura da Fila Circular
typedef struct {
    Peca itens[TAMANHO];
    int frente;
    int fim;
    int total;
} FilaPecas;

// Variável global para controle de IDs únicos
int contadorId = 0;

// Protótipos das funções
void inicializarFila(FilaPecas *f);
Peca gerarPeca();
int enfileirar(FilaPecas *f);
int desenfileirar(FilaPecas *f);
void exibirFila(FilaPecas f);

int main() {
    srand(time(NULL)); // Inicializa a semente para geração aleatória
    FilaPecas filaFutura;
    inicializarFila(&filaFutura);

    // Preenchimento inicial da fila conforme requisito (5 peças)
    for (int i = 0; i < TAMANHO; i++) {
        enfileirar(&filaFutura);
    }

    int opcao = -1;
    while (opcao != 0) {
        printf("\n--- TETRIS STACK: CONTROLE DE PECAS ---\n");
        exibirFila(filaFutura);
        
        printf("\nOpcoes de acao:\n");
        printf("1 - Jogar peca (dequeue)\n");
        printf("2 - Inserir nova peca (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                desenfileirar(&filaFutura);
                break;
            case 2:
                if (!enfileirar(&filaFutura)) {
                    printf("\n[AVISO] A fila esta cheia! Jogue uma peca primeiro.\n");
                }
                break;
            case 0:
                printf("Encerrando sistema...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    }

    return 0;
}

// Inicializa os índices da fila
void inicializarFila(FilaPecas *f) {
    f->frente = 0;
    f->fim = -1;
    f->total = 0;
}

// Gera uma peça automaticamente com tipo aleatório e ID sequencial
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = contadorId++;
    return nova;
}

// Adiciona uma peça ao final da fila (Lógica Circular)
int enfileirar(FilaPecas *f) {
    if (f->total < TAMANHO) {
        f->fim = (f->fim + 1) % TAMANHO; // Incremento circular
        f->itens[f->fim] = gerarPeca();
        f->total++;
        return 1; // Sucesso
    }
    return 0; // Fila cheia
}

// Remove a peça da frente da fila (Lógica Circular)
int desenfileirar(FilaPecas *f) {
    if (f->total > 0) {
        Peca removida = f->itens[f->frente];
        printf("\n>> Voce jogou a peca [%c %d]!\n", removida.nome, removida.id);
        
        f->frente = (f->frente + 1) % TAMANHO; // Incremento circular
        f->total--;
        return 1;
    }
    printf("\n[ERRO] Nao ha pecas para jogar!\n");
    return 0;
}

// Percorre a fila da frente ao fim para exibição
void exibirFila(FilaPecas f) {
    printf("Fila de pecas: ");
    if (f.total == 0) {
        printf("[Vazia]");
    } else {
        for (int i = 0; i < f.total; i++) {
            int indice = (f.frente + i) % TAMANHO;
            printf("[%c %d] ", f.itens[indice].nome, f.itens[indice].id);
        }
    }
    printf("\n");
}
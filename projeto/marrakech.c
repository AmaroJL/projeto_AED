#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "marrakech.h"

typedef struct noPilha{
    int tapete;
    int jogador;
    struct noPilha* prox;
}NoPilha;

typedef struct{
    int tam;
    NoPilha* topo;
}Topo;

typedef struct node{
    Topo* tapetes;
    struct node* norte;
    struct node* sul;
    struct node* leste;
    struct node* oeste;
}Node;

typedef struct assam{
    int linha;
    int coluna;
    char orientacao;
    Node* posicao;
}Assam;

typedef struct jogador{
    int dinheiro;
    int tapetes;
    struct jogador *prox;
}Jogador;

typedef Node* Tabuleiro;

Topo* criarPilhaTapetes(){
    Topo* novoTopo = (Topo*)malloc(sizeof(Topo));
    novoTopo->tam = 0;
    novoTopo->topo = NULL;

    return novoTopo;
}

Node* criarNo(){
    Node* novoNo = (Node*)malloc(sizeof(Node));
    novoNo->norte = NULL;
    novoNo->sul = NULL;
    novoNo->leste = NULL;
    novoNo->oeste = NULL;
    novoNo->tapetes = criarPilhaTapetes();

    return novoNo;
}

Assam* inicializarAssam(Tabuleiro *tabuleiro){
    Assam *novoAssam = (Assam*)malloc(sizeof(Assam));

    novoAssam->linha = (TAM + 1)/2;
    novoAssam->coluna = (TAM + 1)/2;

    srand(time(NULL));
    int or = rand() % 4;
    if(or == 0)
        novoAssam->orientacao = 'N';
    else if(or == 1)
        novoAssam->orientacao = 'S';
    else if(or == 2)
        novoAssam->orientacao = 'L';
    else if(or == 3)
        novoAssam->orientacao = 'O';

    Node *aux = *tabuleiro;
    for(int i = 1; i < (TAM + 1)/2; i++){
        aux = aux->leste;
        aux = aux->sul;
    }
    novoAssam->posicao = aux;

    return novoAssam;
}

Tabuleiro* criar(){
    Tabuleiro* tabuleiro = (Tabuleiro*)malloc(sizeof(Tabuleiro));
    Node* anterior = NULL;
    Node* linhaAnterior = NULL;
    Node* inicioLinhaAnterior = NULL;
    Node* inicioLinhaAtual = NULL;

    for(int i = 0; i < TAM; i++){
        for(int j = 0; j < TAM; j++){
            Node* novoNo = criarNo();
            if(i == 0 && j == 0){
                *tabuleiro = novoNo;
            }

            if(j > 0){
                anterior->leste = novoNo;
                novoNo->oeste = anterior;
            }
            
            if(i > 0){
                novoNo->norte = linhaAnterior;
                linhaAnterior->sul = novoNo;
                linhaAnterior = linhaAnterior->leste;
            }

            novoNo->tapetes = criarPilhaTapetes();

            if(j == 0)
                inicioLinhaAtual = novoNo;
            anterior = novoNo;
        }
        inicioLinhaAnterior = inicioLinhaAtual;
        linhaAnterior = inicioLinhaAnterior;
    }

    Node* atual = *tabuleiro;
    while(atual->leste != NULL){
        if(atual->norte == NULL){
            atual->norte = atual->leste;
            atual->leste->norte = atual;
            atual = atual->leste;
        }else
            atual = atual->leste;
    }
    if(atual->norte == NULL){
        atual->norte = atual;
        atual->leste = atual;
    }else{
        atual->leste = atual->sul;
        atual->sul->leste = atual;
    }

    atual = atual->sul;
    while(atual->sul != NULL){
        if(atual->leste == NULL){
            atual->leste = atual->sul;
            atual->sul->leste = atual;
            atual = atual->sul;
        }else
            atual = atual->sul;
    }
    if(atual->leste == NULL){
        atual->leste = atual;
        atual->sul = atual;
    }else{
        atual->sul = atual->oeste;
        atual->oeste->sul = atual;
    }

    atual = atual->oeste;
    while(atual->oeste != NULL){
        if(atual->sul == NULL){
            atual->sul = atual->oeste;
            atual->oeste->sul = atual;
            atual = atual->oeste;
        }else
            atual = atual->oeste;
    }
    if(atual->sul == NULL){
        atual->sul = atual;
        atual->oeste = atual;
    }else{
        atual->oeste = atual->norte;
        atual->norte->oeste = atual;
    }

    atual = atual->norte;
    while(atual->norte != atual->leste){
        if(atual->oeste == NULL){
            atual->oeste = atual->norte;
            atual->norte->oeste = atual;
            atual = atual->norte;
        }else{
            atual = atual->norte;
        }
    }

    return tabuleiro;
}

/*
void imprimirTeste(Tabuleiro* tabuleiro){
    Node* atual = *tabuleiro;
    Node* anterior = atual;
    for(int i = 0; i < TAM; i++){
        for(int j = 0; j < TAM; j++){
            printf("V: %d ", atual->valor);
            if(atual->norte)
                printf("N: %d ", atual->norte->valor);
            else
                printf("N: nulo ");
            if(atual->sul)
                printf("S: %d ", atual->sul->valor);
            else
                printf("S: nulo ");
            if(atual->leste)
                printf("L: %d ", atual->leste->valor);
            else
                printf("L: nulo ");
            if(atual->oeste)
                printf("O: %d ", atual->oeste->valor);
            else
                printf("O: nulo ");
            printf("| ");
            atual = atual->leste;
        }
        printf("\n");
        atual = anterior->sul;
        anterior = anterior->sul;
    }
}
*/
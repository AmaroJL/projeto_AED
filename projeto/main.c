#include <stdio.h>
#include <stdlib.h>
#include "marrakech.h"

int main()
{   
    Tabuleiro *tabuleiro = criar();
    Assam *assam = inicializarAssam(tabuleiro);
    listaJogadores* jogadores = inicializarJogadores();
    Jogador *jogadorAtual = *jogadores;
    int jogoEncerrado = 0;
    while(jogoEncerrado == 0){
        imprimirTabuleiro(tabuleiro, assam);
        imprimirJogadorAtual(jogadorAtual);
        fazerJogada(tabuleiro, &assam, &jogadorAtual);
        if(FimDeJogo(jogadorAtual) == 1) jogoEncerrado = 1;
    }
    printf("\njogo encerrado!\n");
    condicaoVitoria(tabuleiro, jogadorAtual);
    
    return 0;
}
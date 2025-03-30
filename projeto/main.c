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
    }
    
    return 0;
}
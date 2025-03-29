#include <stdio.h>
#include <stdlib.h>
#include "marrakech.h"

int main()
{   
    Tabuleiro *tabuleiro = criar();
    Assam *assam = inicializarAssam(tabuleiro);
    listaJogadores* jogadores = inicializarJogadores();
    imprimirTabuleiro(tabuleiro, assam);
    system("pause");
    
    return 0;
}
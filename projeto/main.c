#include <stdio.h>
#include <stdlib.h>
#include "marrakech.h"

int main()
{   
    Tabuleiro *tabuleiro = criar();
    Assam *assam = inicializarAssam(tabuleiro);
    //imprimirTeste(tabuleiro);
    system("pause");
    
    return 0;
}
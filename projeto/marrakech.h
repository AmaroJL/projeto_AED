#ifndef MARRAKECH_H_INCLUDED
#define MARRAKECH_H_INCLUDED
#define TAM 7

typedef struct node Node;
typedef Node* Tabuleiro;
typedef struct assam Assam;

Node* criarNo();
Tabuleiro* criar();
void imprimirTeste(Tabuleiro*);
Assam* inicializarAssam(Tabuleiro*);

#endif // MARRAKECH_H_INCLUDED
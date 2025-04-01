#ifndef MARRAKECH_H_INCLUDED
#define MARRAKECH_H_INCLUDED
#define TAM 7

typedef struct node Node;
typedef Node* Tabuleiro;
typedef struct assam Assam;
typedef struct jogador Jogador;
typedef Jogador* listaJogadores;

Node* criarNo();
Tabuleiro* criar();
Assam* inicializarAssam(Tabuleiro*);
void imprimirTabuleiro(Tabuleiro*, Assam*);
Jogador* criarJogador();
listaJogadores* inicializarJogadores();
void girarAssamHorario(Assam**);
void girarAssamAntiHorario(Assam**);
void imprimirJogadorAtual(Jogador*);
void avancarJogador(Jogador**);
void fazerJogada(Tabuleiro*, Assam**, Jogador**);
void moverAssam(Assam**, int);
void inserirNaPilha(char cor[10], Node *node);

#endif // MARRAKECH_H_INCLUDED
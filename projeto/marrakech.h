#ifndef MARRAKECH_H_INCLUDED
#define MARRAKECH_H_INCLUDED
#define TAM 7

typedef struct node Node;
typedef Node* Tabuleiro;
typedef struct assam Assam;
typedef Jogador* listaJogadores;

Node* criarNo();
Tabuleiro* criar();
//void imprimirTeste(Tabuleiro*);
Assam* inicializarAssam(Tabuleiro*);
void imprimirTabuleiro(Tabuleiro*, Assam*);
Jogador* criarJogador();
listaJogadores* inicializarJogadores();
void girarAssamHorario(Assam*);
void girarAssamAntiHorario(Assam*);
void pushPilhaTapetes(Topo*, NoPilha*);

#endif // MARRAKECH_H_INCLUDED
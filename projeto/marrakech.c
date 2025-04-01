#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include "marrakech.h"

typedef struct noPilha{
    //int tapete; -> creio eu que eu preciso saber nao da qtd, mas da cor do tapete que ta ali
    int jogador;
    char cor[10];
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
    char orientacao[6];
    Node* posicao;
}Assam;

typedef struct jogador{
    int dinheiro;
    int tapetes;
    char cor[10];
    struct jogador *prox;
}Jogador;

typedef Node* Tabuleiro;
typedef Jogador* listaJogadores;

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
        strcpy(novoAssam->orientacao, "Norte");
    else if(or == 1)
        strcpy(novoAssam->orientacao, "Sul");
    else if(or == 2)
        strcpy(novoAssam->orientacao, "Leste");
    else if(or == 3)
        strcpy(novoAssam->orientacao, "Oeste");

    Node *atual = *tabuleiro;
    for(int i = 1; i < (TAM + 1)/2; i++){
        atual = atual->leste;
        atual = atual->sul;
    }
    novoAssam->posicao = atual;

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

Jogador* criarJogador(){
    Jogador *novoJogador = (Jogador*)malloc(sizeof(Jogador));
    strcpy(novoJogador->cor, "\0");
    novoJogador->dinheiro = 30;
    novoJogador->prox = NULL;
    novoJogador->tapetes = 12;

    return novoJogador;
}

listaJogadores* inicializarJogadores(){
    listaJogadores *novosJogadores = (listaJogadores*)malloc(sizeof(listaJogadores));

    Jogador *jogador1 = criarJogador();
    Jogador *jogador2 = criarJogador();
    Jogador *jogador3 = criarJogador();
    Jogador *jogador4 = criarJogador();
    
    strcpy(jogador1->cor, "Vermelho");
    strcpy(jogador2->cor, "Azul");
    strcpy(jogador3->cor, "Amarelo");
    strcpy(jogador4->cor, "Verde");
    
    
    jogador1->prox = jogador2;
    jogador2->prox = jogador3;
    jogador3->prox = jogador4;
    jogador4->prox = jogador1;

    *novosJogadores = jogador1;

    return novosJogadores;
}

void girarAssamHorario(Assam **assam){
    if(strcmp((*assam)->orientacao, "Norte") == 0){
        strcpy((*assam)->orientacao, "Leste");
    }else if(strcmp((*assam)->orientacao, "Leste") == 0){
        strcpy((*assam)->orientacao, "Sul");
    }else if(strcmp((*assam)->orientacao, "Sul") == 0){
        strcpy((*assam)->orientacao, "Oeste");
    }else if(strcmp((*assam)->orientacao, "Oeste") == 0){
        strcpy((*assam)->orientacao, "Norte");
    }
}

void girarAssamAntiHorario(Assam **assam){
    if(strcmp((*assam)->orientacao, "Norte") == 0){
        strcpy((*assam)->orientacao, "Oeste");
    }else if(strcmp((*assam)->orientacao, "Oeste") == 0){
        strcpy((*assam)->orientacao, "Sul");
    }else if(strcmp((*assam)->orientacao, "Sul") == 0){
        strcpy((*assam)->orientacao, "Leste");
    }else if(strcmp((*assam)->orientacao, "Leste") == 0){
        strcpy((*assam)->orientacao, "Norte");
    }
}

void imprimirTabuleiro(Tabuleiro* tabuleiro, Assam* assam){
    Node *inicioLinha = *tabuleiro;
    Node *atual = NULL;

    printf("\n");
    for(int i = 0; i < TAM; i++)
        printf("+----");
    printf("+");
    for(int i = 0; i < TAM; i++){
        printf("\n");
        atual = inicioLinha;
        inicioLinha = inicioLinha->sul;
        for(int j = 0; j < TAM; j++){
            printf("|");
            if(assam->posicao == atual)
                printf(" X  ");
            else if(atual->tapetes->tam != 0)
                printf("  %d", atual->tapetes->tam);
            else
                printf("    ");
            atual = atual->leste;
        }
        printf("|");
        printf("\n");
        for(int i = 0; i < TAM; i++)
            printf("+----");
        printf("+");
    }
    printf("\n\nOrientacao atual: %s\n\n", assam->orientacao);
}

void imprimirJogadorAtual(Jogador *jogadorAtual){
    printf("\nVez do jogador %s\nTapetes restantes: %d\nMoedas restantes: %d\n\n", jogadorAtual->cor, jogadorAtual->tapetes, jogadorAtual->dinheiro);
}

void avancarJogador(Jogador **jogadorAtual){
    *jogadorAtual = (*jogadorAtual)->prox;
}

void moverAssam(Assam **assam, int resultado){
    for(int i = 0; i < resultado; i++){
        if((*assam)->linha == 1 && strcmp((*assam)->orientacao, "Norte") == 0){
            if((*assam)->posicao != (*assam)->posicao->norte){
                if((*assam)->posicao->norte == (*assam)->posicao->oeste){
                    (*assam)->posicao = (*assam)->posicao->norte;
                    strcpy((*assam)->orientacao, "Sul");
                    (*assam)->coluna--;
                }else if((*assam)->posicao->norte == (*assam)->posicao->leste){
                    (*assam)->posicao = (*assam)->posicao->norte;
                    strcpy((*assam)->orientacao, "Sul");
                    (*assam)->coluna++;
                }
            }else{
                if((*assam)->posicao->norte == (*assam)->posicao->oeste){
                    strcpy((*assam)->orientacao, "Leste");
                }else if((*assam)->posicao->norte == (*assam)->posicao->leste){
                    strcpy((*assam)->orientacao, "Oeste");
                }
            }
        }else if((*assam)->coluna == TAM && strcmp((*assam)->orientacao, "Leste") == 0){
            if((*assam)->posicao != (*assam)->posicao->leste){
                if((*assam)->posicao->leste == (*assam)->posicao->norte){
                    (*assam)->posicao = (*assam)->posicao->leste;
                    strcpy((*assam)->orientacao, "Oeste");
                    (*assam)->linha--;
                }else if((*assam)->posicao->leste == (*assam)->posicao->sul){
                    (*assam)->posicao = (*assam)->posicao->leste;
                    strcpy((*assam)->orientacao, "Oeste");
                    (*assam)->linha++;
                }
            }else{
                if((*assam)->posicao->leste == (*assam)->posicao->norte){
                    strcpy((*assam)->orientacao, "Sul");
                }else if((*assam)->posicao->leste == (*assam)->posicao->sul){
                    strcpy((*assam)->orientacao, "Norte");
                }
            }
        }else if((*assam)->linha == TAM && strcmp((*assam)->orientacao, "Sul") == 0){
            if((*assam)->posicao != (*assam)->posicao->sul){
                if((*assam)->posicao->sul == (*assam)->posicao->oeste){
                    (*assam)->posicao = (*assam)->posicao->sul;
                    strcpy((*assam)->orientacao, "Norte");
                    (*assam)->coluna--;
                }else if((*assam)->posicao->sul == (*assam)->posicao->leste){
                    (*assam)->posicao = (*assam)->posicao->sul;
                    strcpy((*assam)->orientacao, "Norte");
                    (*assam)->coluna++;
                }
            }else{
                if((*assam)->posicao->sul == (*assam)->posicao->oeste){
                    strcpy((*assam)->orientacao, "Leste");
                }else if((*assam)->posicao->sul == (*assam)->posicao->leste){
                    strcpy((*assam)->orientacao, "Oeste");
                }
            }
        }else if((*assam)->coluna == 1 && strcmp((*assam)->orientacao, "Oeste") == 0){
            if((*assam)->posicao != (*assam)->posicao->oeste){
                if((*assam)->posicao->oeste == (*assam)->posicao->norte){
                    (*assam)->posicao = (*assam)->posicao->oeste;
                    strcpy((*assam)->orientacao, "Leste");
                    (*assam)->linha--;
                }else if((*assam)->posicao->oeste == (*assam)->posicao->sul){
                    (*assam)->posicao = (*assam)->posicao->oeste;
                    strcpy((*assam)->orientacao, "Leste");
                    (*assam)->linha++;
                }
            }else{
                if((*assam)->posicao->oeste == (*assam)->posicao->norte){
                    strcpy((*assam)->orientacao, "Sul");
                }else if((*assam)->posicao->oeste == (*assam)->posicao->sul){
                    strcpy((*assam)->orientacao, "Norte");
                }
            }
        }else{
            if(strcmp((*assam)->orientacao, "Norte") == 0){
                (*assam)->posicao = (*assam)->posicao->norte;
                (*assam)->linha--;
            }else if(strcmp((*assam)->orientacao, "Leste") == 0){
                (*assam)->posicao = (*assam)->posicao->leste;
                (*assam)->coluna++;
            }else if(strcmp((*assam)->orientacao, "Sul") == 0){
                (*assam)->posicao = (*assam)->posicao->sul;
                (*assam)->linha++;
            }else if(strcmp((*assam)->orientacao, "Oeste") == 0){
                (*assam)->posicao = (*assam)->posicao->oeste;
                (*assam)->coluna--;
            }
        }
    }
}

void fazerJogada(Tabuleiro *tabuleiro, Assam **assam, Jogador **jogadorAtual){
    char girarAssam, direcao;
    printf("\nGirar assam?(S para sim e N para nao) ");
    scanf("%c", &girarAssam);
    while(getchar() != '\n');

    if(girarAssam != 's' && girarAssam != 'S' && girarAssam != 'n' && girarAssam != 'N'){
        printf("\nDigite uma opcao valida: ");
        do{
            scanf("%c", &girarAssam);
            while(getchar() != '\n');
        }while(girarAssam != 's' && girarAssam != 'S' && girarAssam != 'n' && girarAssam != 'N');
    }

    if(girarAssam == 's' || girarAssam == 'S'){
        printf("\nPara qual direcao?(D ou d para direita, E ou epara esuqerda) ");
        scanf("%c", &direcao);
        while(getchar() != '\n');

        if(direcao != 'd' && direcao != 'D' && direcao != 'e' && direcao != 'E'){
            printf("\nDigite uma opcao valida: ");
            do{
                scanf("%c", &direcao);
                while(getchar() != '\n');
            }while(direcao != 'd' && direcao != 'D' && direcao != 'e' && direcao != 'E');
        }

        if(direcao == 'D' || direcao == 'd')
            girarAssamHorario(assam);
        if(direcao == 'E' || direcao == 'e')
            girarAssamAntiHorario(assam);
    }
    printf("\nPressione enter para rolar o dado");
    getchar();
    printf("\nRolando dado...");
    Sleep(1000);
    int resultado = (rand() % 6) / 2 + 1;
    printf("\nResultado: %d\n", resultado);
    
    moverAssam(assam, resultado);
    avancarJogador(jogadorAtual);
}

void inserirNaPilha(char cor[10], Node *node){
    if(node == NULL || cor == NULL){
        return;
    }
    
    NoPilha *novo = (NoPilha*)malloc(sizeof(NoPilha));
    if(novo == NULL){
        return;
    }

    novo->jogador = 0;
    strcpy(novo->cor, cor);

    if(node->tapetes->tam == 0){
        node->tapetes->topo = novo;
        novo->prox = NULL;
    }else{
        novo->prox = node->tapetes->topo;
        node->tapetes->topo = novo;
    }

    node->tapetes->tam++;
}

//verificam se o nó está dentro do limite do tabuleiro
int nodeNorthIsExist(Assam **assam){
    if((*assam) == NULL) return -1;
    if(((*assam)->linha) - 1 <= TAM && ((*assam)->linha) - 1 >= 1){
        return 1;
    }
}

int nodeSouthIsExist(Assam **assam){
    if((*assam) == NULL) return -1;
    if(((*assam)->linha) + 1 <= TAM && ((*assam)->linha) + 1 >= 1){
        return 1;
    }
}
//oeste
int nodeWestIsExist(Assam **assam){
    if((*assam) == NULL) return -1;
    if(((*assam)->coluna) - 1 <= TAM && ((*assam)->coluna) - 1 >= 1){
        return 1;
    }
}
//lest
int nodeEastIsExist(Assam **assam){
    if((*assam) == NULL) return -1;
    if(((*assam)->coluna) + 1 <= TAM && ((*assam)->coluna) + 1 >= 1){
        return 1;
    }
}
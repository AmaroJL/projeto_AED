#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>
#include "marrakech.h"

typedef struct tapete{
    char cor[10];
    int numTap;
    struct tapete *metade;
}Tapete;

typedef struct noPilha{
    Tapete tapete;
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
    int r, g, b;
    Node *inicioLinha = *tabuleiro;
    Node *atual = NULL;

    printf("\n     ");
    for(int i = 0; i < TAM; i++)
        printf("%c    ", i + 65);
    printf("\n   ");

    for(int i = 0; i < TAM; i++)
        printf("+----");
    printf("+");
    for(int i = 0; i < TAM; i++){
        printf("\n");
        if(i < 9)
            printf(" %d ", i + 1);
        else
        printf("%d ", i + 1);
        atual = inicioLinha;
        inicioLinha = inicioLinha->sul;
        for(int j = 0; j < TAM; j++){
            printf("|");
            if(assam->posicao == atual){
                if(atual->tapetes->topo != NULL)
                    corRGB(atual->tapetes->topo->tapete.cor, &r, &g, &b);
                else
                    corRGB("", &r, &g, &b);
                if(strcmp(assam->orientacao, "Norte") == 0){
                    printf("\x1b[38;2;%d;%d;%dm \u2191  \x1b[0m", r, g, b);
                }else if(strcmp(assam->orientacao, "Leste") == 0){
                    printf("\x1b[38;2;%d;%d;%dm \u2192  \x1b[0m", r, g, b);
                }else if(strcmp(assam->orientacao, "Sul") == 0){
                    printf("\x1b[38;2;%d;%d;%dm \u2193  \x1b[0m", r, g, b);
                }else if(strcmp(assam->orientacao, "Oeste") == 0){
                    printf("\x1b[38;2;%d;%d;%dm \u2190  \x1b[0m", r, g, b);
                }
            }else if(atual->tapetes->tam != 0){
                corRGB(atual->tapetes->topo->tapete.cor, &r, &g, &b);
                printf("  \x1b[38;2;%d;%d;%dm%d \x1b[0m", r, g, b, atual->tapetes->topo->tapete.numTap);
            }else
                printf("    ");
            atual = atual->leste;
        }
        printf("|");
        printf("\n   ");
        for(int i = 0; i < TAM; i++)
            printf("+----");
        printf("+");
    }
}

void imprimirJogadorAtual(Jogador *jogadorAtual, Assam *assam){
    int r, g, b;
    corRGB(jogadorAtual->cor, &r, &g, &b);

    printf("\n\n  ╔═══════════════════════════════════╗");
    printf("\n  ║");
    for(int i = 0; i < (35 - 15 - strlen(jogadorAtual->cor))/2; i++)
        printf(" ");
    printf("\x1b[38;2;%d;%d;%dmVez do jogador %s\x1b[0m", r, g, b, jogadorAtual->cor);
    for(int i = 0; i < (36 - 15 - strlen(jogadorAtual->cor))/2; i++)
        printf(" ");
    printf("║");
    printf("\n  ╠═══════════════════════════════════╣");
    printf("\n  ║ \x1b[38;2;%d;%d;%dmMoedas restantes\x1b[0m: %-16d║", r, g, b, jogadorAtual->dinheiro);
    printf("\n  ║ \x1b[38;2;%d;%d;%dmTapetes restantes\x1b[0m: %-15d║", r, g, b, jogadorAtual->tapetes);
    printf("\n  ║ \x1b[38;2;%d;%d;%dmOrientação do Assam\x1b[0m: %-13s║", r, g, b, assam->orientacao);
    printf("\n  ╚═══════════════════════════════════╝");
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
    int girarAssam, direcao, r, g, b;
    corRGB((*jogadorAtual)->cor, &r, &g, &b);
    printf("\n\n  >>>>>>>>>\x1b[38;2;%d;%d;%dmRotacionar Assam?\x1b[0m<<<<<<<<<\n", r, g, b);
    printf("  [0] \x1b[38;2;%d;%d;%dmNão\x1b[0m\n  [1] \x1b[38;2;%d;%d;%dmSim\n  Opção\x1b[0m: ", r, g, b, r, g, b);
    scanf("%d", &girarAssam);
    while(getchar() != '\n');

    if(girarAssam != 0 && girarAssam != 1){
        printf("\n  Digite uma opção válida: ");
        do{
            scanf("%d", &girarAssam);
            while(getchar() != '\n');
        }while(girarAssam != 0 && girarAssam != 1);
    }

    if(girarAssam == 1){
        printf("\n  >>>>>>>>>\x1b[38;2;%d;%d;%dmEm qual sentido?\x1b[0m<<<<<<<<<", r, g, b);
        printf("\n  [0] \x1b[38;2;%d;%d;%dmAnti horário\x1b[0m\n  [1] \x1b[38;2;%d;%d;%dmHorário\x1b[0m", r, g, b, r, g, b);
        printf("\n  \x1b[38;2;%d;%d;%dmOpção\x1b[0m: ", r, g, b);
        scanf("%d", &direcao);
        while(getchar() != '\n');

        if(girarAssam != 0 && girarAssam != 1){
            printf("\n  Digite uma opção válida: ");
            do{
                scanf("%d", &direcao);
                while(getchar() != '\n');
            }while(girarAssam != 0 && girarAssam != 1);
        }

        if(direcao == 0)
            girarAssamAntiHorario(assam);
        if(direcao == 1)
            girarAssamHorario(assam);
    }
    system("cls");
    imprimirTabuleiro(tabuleiro, *assam);
    imprimirJogadorAtual(*jogadorAtual, *assam);
    printf("\n\n  Pressione enter para rolar o dado");
    getchar();
    printf("\n  Rolando dado...");
    Sleep(800);
    int resultado = (rand() % 6) / 2 + 1;
    printf("\n  Resultado: %d\n", resultado);
    Sleep(600);
    for(int i = 0; i < resultado; i++){
        system("cls");
        printf("\n  Rolando dado...");
        printf("\n  Resultado: %d\n", resultado);
        moverAssam(assam, 1);
        imprimirTabuleiro(tabuleiro, *assam);
        Sleep(600);
    }
    imprimirJogadorAtual(*jogadorAtual, *assam);
    if((*assam)->posicao->tapetes->topo != NULL){
        pagarJogador(*assam, jogadorAtual);
    }
    colocarTapete(tabuleiro, assam, jogadorAtual);
    system("cls");
    avancarJogador(jogadorAtual);
}

void inserirNaPilha(char cor[], Node *node, Jogador *jogador){
    if(node == NULL || cor == NULL){
        return;
    }
    
    NoPilha *novo = (NoPilha*)malloc(sizeof(NoPilha));
    if(novo == NULL){
        return;
    }

    strcpy(novo->tapete.cor, cor);
    novo->tapete.numTap = 12 - jogador->tapetes + 1;

    if(node->tapetes->topo == NULL){
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

void colocarTapete(Tabuleiro *tabuleiro, Assam **assam, Jogador **jogador) {
    int l1, l2;
    char c1, c2;
    Node *auxL = *tabuleiro;
    Node *auxC, *tap1, *tap2;
    while(1) {
        printf("\nEscolha da primeira coordenada:\nLinha: ");
        scanf(" %d", &l1);
        while(l1 < 1 || l1 > TAM) {
            printf("Informe uma linha valida: ");
            getchar();
            scanf(" %d", &l1);
        }
        printf("Coluna: ");
        scanf(" %c", &c1);
        c1 = toupper(c1);
        while(c1-64 < 1 || c1-64 > TAM) {
            printf("Informe uma coluna valida: ");
            getchar();
            scanf(" %c", &c1);
        }
        printf("\nEscolha da segunda coordenada:\nLinha: ");
        scanf(" %d", &l2);
        while(l2 < 1 || l2 > TAM) {
            printf("Informe uma linha valida: ");
            getchar();
            scanf(" %d", &l2);
        }
        printf("Coluna: ");
        scanf(" %c", &c2);
        c2 = toupper(c2);
        while(c2-64 < 1 || c2-64 > TAM) {
            printf("Informe uma coluna valida: ");
            getchar();
            scanf(" %c", &c2);
        } 
        while(getchar() != '\n');


        auxL = *tabuleiro;
        for(int i=1; i<=TAM; i++) {
            auxC = auxL;
            if(l1 == i) {
                for(int j=1; j<=TAM; j++) {
                    if(j == c1-64){
                        tap1 = auxC;
                    }
                    auxC = auxC->leste;
                }    
            }
            auxL = auxL->sul;
        }
    
        auxL = *tabuleiro;
        for(int i=1; i<=TAM; i++) {
            auxC = auxL;
            if(l2 == i) {
                for(int j=1; j<=TAM; j++) {
                    if(j == c2-64){
                        tap2 = auxC;
                    }
                    auxC = auxC->leste;
                }    
            }
            auxL = auxL->sul;
        }
        if((*assam)->posicao->norte != tap1 && (*assam)->posicao->leste != tap1 && (*assam)->posicao->sul != tap1 && (*assam)->posicao->oeste != tap1) {
            if((*assam)->posicao->norte != tap2 && (*assam)->posicao->leste != tap2 && (*assam)->posicao->sul != tap2 && (*assam)->posicao->oeste != tap2) {
                printf("\nPelo menos uma das informacoes passadas deve ser adjacente ao Asaam. Tente novamente:");
                continue;
            }
        }
        if(tap1->norte != tap2 && tap1->leste != tap2 && tap1->sul != tap2 && tap1->oeste != tap2) {
            printf("\nO tapete deve ser colocado em duas posicoes adjacentes. Tente novamente:");
            continue;
        }
        if(tap1->tapetes->topo != NULL) {
            if(tap1->tapetes->topo->prox == tap2->tapetes->topo && tap2->tapetes->topo->prox == tap1->tapetes->topo) {
                printf("\nO tapete nao pode cobrir totalmete outro tapete. Tente novamente:");
                continue;
            } 
        }
        if(tap1 == (*assam)->posicao || tap2 == (*assam)->posicao) {
            printf("\nO tapete nao pode ser colocado de baixo do Assam. Tente novamente:");
            continue;
        }
    
        inserirNaPilha((*jogador)->cor, tap1, *jogador);
        inserirNaPilha((*jogador)->cor, tap2, *jogador);
        tap1->tapetes->topo->tapete.metade = &(tap2->tapetes->topo->tapete);
        tap2->tapetes->topo->tapete.metade = &(tap1->tapetes->topo->tapete);
        tap1->tapetes->tam++;
        tap2->tapetes->tam++;

        break;
    }
    
    (*jogador)->tapetes--;
}

void areaTapete(Node *espacoAtual, Topo *tapAdj){
    NoPilha *novo = (NoPilha*)malloc(sizeof(NoPilha));
    strcpy(novo->tapete.cor, espacoAtual->tapetes->topo->tapete.cor);
    novo->tapete.metade = espacoAtual->tapetes->topo->tapete.metade;
    novo->tapete.numTap = espacoAtual->tapetes->topo->tapete.numTap;
    if(tapAdj->topo == NULL){
        tapAdj->topo = novo;
        novo->prox = NULL;
    }else{
        novo->prox = tapAdj->topo;
        tapAdj->topo = novo;
    }
    tapAdj->tam++;
    NoPilha *aux, *atual;
    if(espacoAtual->norte->tapetes->topo != NULL){
        if(strcmp(espacoAtual->tapetes->topo->tapete.cor, espacoAtual->norte->tapetes->topo->tapete.cor) == 0){
            aux = espacoAtual->norte->tapetes->topo;
            atual = tapAdj->topo;
            while(atual != NULL){
                if(atual->tapete.metade != aux->tapete.metade)
                    atual = atual->prox;
                else
                    break;
            }
            if(atual == NULL)
                areaTapete(espacoAtual->norte, tapAdj);
        }
    }
    if(espacoAtual->leste->tapetes->topo != NULL){
        if(strcmp(espacoAtual->tapetes->topo->tapete.cor, espacoAtual->leste->tapetes->topo->tapete.cor) == 0){
            aux = espacoAtual->leste->tapetes->topo;
            atual = tapAdj->topo;
            while(atual != NULL){
                if(atual->tapete.metade != aux->tapete.metade)
                    atual = atual->prox;
                else
                    break;
            }
            if(atual == NULL){
                areaTapete(espacoAtual->leste, tapAdj);
            }
        }
    }
    if(espacoAtual->sul->tapetes->topo != NULL){
        if(strcmp(espacoAtual->tapetes->topo->tapete.cor, espacoAtual->sul->tapetes->topo->tapete.cor) == 0){
            aux = espacoAtual->sul->tapetes->topo;
            atual = tapAdj->topo;
            while(atual != NULL){
                if(atual->tapete.metade != aux->tapete.metade)
                    atual = atual->prox;
                else
                    break;
            }
            if(atual == NULL)
                areaTapete(espacoAtual->sul, tapAdj);
        }
    }
    if(espacoAtual->oeste->tapetes->topo != NULL){
        if(strcmp(espacoAtual->tapetes->topo->tapete.cor, espacoAtual->oeste->tapetes->topo->tapete.cor) == 0){
            aux = espacoAtual->oeste->tapetes->topo;
            atual = tapAdj->topo;
            while(atual != NULL){
                if(atual->tapete.metade != aux->tapete.metade)
                    atual = atual->prox;
                else
                    break;
            }
            if(atual == NULL)
                areaTapete(espacoAtual->oeste, tapAdj);
        }
    }
}

void pagarJogador(Assam *assam, Jogador **jogadorAtual) {
    int r, g, b;
    Jogador *donoTapete = *jogadorAtual;
    while(strcmp(donoTapete->cor, assam->posicao->tapetes->topo->tapete.cor) != 0)
        donoTapete = donoTapete->prox;
    corRGB((*jogadorAtual)->cor, &r, &g, &b);
    printf("\n\n  O \x1b[38;2;%d;%d;%dmJogador %s\x1b[0m pisou em cima do tapete do ", r, g, b, (*jogadorAtual)->cor);
    corRGB(assam->posicao->tapetes->topo->tapete.cor, &r, &g, &b);
    printf("\x1b[38;2;%d;%d;%dmJogador %s\x1b[0m... hora de pagar o preço", r, g, b, donoTapete->cor);

    Topo *tapAdj = criarPilhaTapetes();
    areaTapete(assam->posicao, tapAdj);
    int moedas = tapAdj->tam;
    NoPilha *aux, *atual = tapAdj->topo;
    tapAdj->topo = NULL;
    free(tapAdj);
    while(atual != NULL){
        aux = atual;
        atual = atual->prox;
        free(aux);
    }

    printf("\n  Calculando quantas moedas devem ser pagas");
    for(int i = 0; i < 3; i++){
        Sleep(500);
        printf(".");
    }
    printf(" Pronto");
    corRGB((*jogadorAtual)->cor, &r, &g, &b);
    printf("\n  Transferindo %d moedas do \x1b[38;2;%d;%d;%dmJogador %s\x1b[0m para o ", moedas, r, g, b, (*jogadorAtual)->cor);
    corRGB(donoTapete->cor, &r, &g, &b);
    printf("\x1b[38;2;%d;%d;%dmJogador %s\x1b[0m", r, g, b, donoTapete->cor);
    (*jogadorAtual)->dinheiro -= moedas;
    donoTapete->dinheiro += moedas;
    for(int i = 0; i < 3; i++){
        Sleep(500);
        printf(".");
    }
    printf(" Pronto");

}

int continuarJogadondo(Jogador **jogador) {
    if((*jogador)->dinheiro <= 0) {
        Jogador *aux = (*jogador)->prox;
        Jogador *ant = (*jogador);
        while(aux != (*jogador)) {
            ant = aux;
            aux = aux->prox;
        }
        ant->prox = aux->prox;
        printf("Jogador %s esta fora\n", (*jogador)->cor);
        (*jogador) = (*jogador)->prox;
        free(aux);
        return 1;
    }
    return 0;
}

int FimDeJogo(Jogador *jogador) {
    int tap=1;
    Jogador *aux = jogador;
    do {
        aux = aux->prox;
        if(aux->tapetes > 0) {
            tap = 0;
        }            
    } while(aux != jogador);
    
    return tap;
}

void condicaoVitoria(Tabuleiro *tabuleiro, Jogador *jogador) {
    int din=0, empate=0;
    char vencendor[10];
    Jogador *aux = jogador;
    do {
        aux = aux->prox;
        if(aux->dinheiro > din) {
            din = aux->dinheiro;
            strcpy(vencendor, aux->cor);
        } else if(aux->dinheiro == din) {
            empate = 1;
        }
    } while(aux != jogador);
    
    if(empate == 0) {
        printf("\nVENCEDOR: Jogador %s", vencendor);
        return;
    }

    int tapdin=0;
    empate = 0;
    do {
        if((aux->dinheiro + aux->tapetes) > tapdin) {
            tapdin = aux->dinheiro + aux->tapetes;
            strcpy(vencendor, aux->cor);
        } else if((aux->dinheiro + aux->tapetes) == tapdin) {
            empate = 1;
        }
    } while(aux != jogador);
    if(empate == 0) {
        printf("\nVENCEDOR: Jogador %s", vencendor);
    
    } else {
        printf("\nEMPATE");
    }

}

void corRGB(char cor[], int *r, int *g, int *b){
    if(strcmp(cor, "Vermelho") == 0){
        *r = 255;
        *g = 0;
        *b = 0;
    }else if(strcmp(cor, "Azul") == 0){
        *r = 0;
        *g = 0;
        *b = 255;
    }else if(strcmp(cor, "Amarelo") == 0){
        *r = 255;
        *g = 255;
        *b = 0;
    }else if(strcmp(cor, "Verde") == 0){
        *r = 0;
        *g = 255;
        *b = 0;
    }else{
        *r = 255;
        *g = 255;
        *b = 255;
    }
}


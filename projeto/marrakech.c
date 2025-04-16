#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>
#include "marrakech.h"

typedef struct noPilha{
    //int tapete; -> creio eu que eu preciso saber da cor do tapete que ta ali
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

    printf("\n     ");
    for(int i = 0; i < TAM; i++)
        printf("%c    ", i+65);
    printf("\n   ");

    for(int i = 0; i < TAM; i++)
        printf("+----");
    printf("+");
    for(int i = 0; i < TAM; i++){
        printf("\n");
        if(i >= 9)
            printf("%d ", i+1);
        else
            printf(" %d ", i+1);
        atual = inicioLinha;
        inicioLinha = inicioLinha->sul;
        for(int j = 0; j < TAM; j++){
            printf("|");
            if(assam->posicao == atual){
                if(strcmp(assam->orientacao, "Norte") == 0)
                    printf(" ⮝  ");
                else if(strcmp(assam->orientacao, "Leste") == 0)
                    printf(" ⮞  ");
                else if(strcmp(assam->orientacao, "Sul") == 0)
                    printf(" ⮟  ");
                else if(strcmp(assam->orientacao, "Oeste") == 0)
                    printf(" ⮜  ");
            }else if(atual->tapetes->tam != 0)
                printf("  %d ", atual->tapetes->tam);
            else
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
    if(strcmp(jogadorAtual->cor, "Vermelho") == 0){
        r = 255;
        g = 0;
        b = 0;
    }else if(strcmp(jogadorAtual->cor, "Azul") == 0){
        r = 0;
        g = 0;
        b = 128;
    }else if(strcmp(jogadorAtual->cor, "Amarelo") == 0){
        r = 128;
        g = 128;
        b = 0;
    }else if(strcmp(jogadorAtual->cor, "Verde") == 0){
        r = 0;
        g = 128;
        b = 0;
    }

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
    int girarAssam, direcao;
    printf("\n\n  >>>>>>>>>Rotacionar Assam?<<<<<<<<<\n  [0] Não\n  [1] Sim\n  Opção: ");
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
        printf("\n  Em qual sentido?\n  [0] Anti horário\n  [1] Horário\nOpção: ");
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
    printf("\n  Pressione enter para rolar o dado");
    getchar();
    printf("\n  Rolando dado...");
    Sleep(1000);
    int resultado = (rand() % 6) / 2 + 1;
    printf("\n  Resultado: %d\n", resultado);
    
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
    
        inserirNaPilha((*jogador)->cor, tap1);
        inserirNaPilha((*jogador)->cor, tap2);
        tap1->tapetes->topo->prox = tap2->tapetes->topo;
        tap2->tapetes->topo->prox = tap1->tapetes->topo;

        break;
    }
    
    (*jogador)->tapetes--;
}

int areaTapete(Assam **assam) {
    if((*assam)->posicao->tapetes->topo == NULL) return 0;
    return (*assam)->posicao->tapetes->tam;
}

int pagarJogador(Assam **assam, Jogador **jogador) {
    int area = areaTapete(assam);

    Jogador *aux = (*jogador);
    while(strcmp(aux->cor, (*assam)->posicao->tapetes->topo->cor) != 0) {
        aux = aux->prox;
    }
    aux->dinheiro += area;
    return area;
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

/*
int enableAnsiColors() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return 0;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return 0;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode)) return 0;

    return 1;
}
*/
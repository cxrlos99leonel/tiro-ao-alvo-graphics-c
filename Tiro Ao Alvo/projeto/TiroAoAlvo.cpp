#include <graphics.h>
#include <stdio.h>
#include <math.h>

typedef struct {
    int pontuacaoRodadas[3];
    int pontuacaoTotal;
} Placar;

void moverAlvo(int *xAlvo, int *direcao, int *x2Alvo);
void layoutJogo(Placar *placar, int rodada);
void projetil();
int calcularPontos(int xElipse, int xAlvo);
int colisao(int xElipse, int yElipse, int raioElipse, int xAlvo, int yAlvo);
void exibirPontuacaoRodadas(Placar *placar);
int tentativas = 0;

main() {
    int alturaJan = 835;
    int larguraJan = 1530;
    int rodada = 0;

    Placar placar = {{0}, 0};

    initwindow(larguraJan, alturaJan, "Tiro ao Alvo", 0, 0);

    //controle de rodadas
    for (rodada = 0; rodada < 3; rodada++) {
        tentativas = 0;
        layoutJogo(&placar, rodada);
        while (tentativas < 3) {

        }

        cleardevice();
    }

    //mostra a pontuação no final
    exibirPontuacaoRodadas(&placar);

    while (1) {
        if (kbhit()) {
            int tecla = getch();
            if (tecla == 27) {
                closegraph();
                break;
            }
        }
    }
}

void layoutJogo(Placar *placar, int rodada) {
    int larguraJan = 1530;
    int tecla;
    int xAlvo = 690;
    int yAlvo = 200;
    int direcao = 1;
    int xAlvoAnterior = xAlvo;
    int yAlvoAnterior = yAlvo;
    int x2Alvo = xAlvo + 150;
    int yAlvoBaixo = yAlvo + 150;
    int disparoAtivo = 0;
    char pontuacaoStr[50];
    int pontos = 0;
    
    int x2 = ((getmaxwidth() - 226) / 2) - 15;
    int y2 = ((getmaxheight() - 283) / 2) + 283;

    readimagefile("c:\\temp\\gun.jpg", x2, y2, x2 + 226, y2 + 283);
    readimagefile("c:\\temp\\spacebar.jpg", 0, 767, 120, 835);
    readimagefile("c:\\temp\\esc.jpg", 125, 767, 197, 835);

    while (tentativas < 3) {
        if (kbhit()) {
            tecla = getch();
            if (tecla == 27) {
                closegraph();
                exit(0);
            }
            if (tecla == 32) {
                disparoAtivo = 1;
                tentativas = tentativas + 1;
            }
        }

        //apaga o rastro do alvo
        setfillstyle(SOLID_FILL, BLACK);
        bar(xAlvoAnterior - 5, yAlvoAnterior, x2Alvo + 5, yAlvoBaixo+1);

        moverAlvo(&xAlvo, &direcao, &x2Alvo);

        readimagefile("c:\\temp\\target.jpg", xAlvo, yAlvo, x2Alvo, yAlvoBaixo);

        xAlvoAnterior = xAlvo;
        yAlvoAnterior = yAlvo;
        x2Alvo = xAlvo + 150;
        yAlvoBaixo = yAlvo + 150;

        delay(50);

        if (disparoAtivo) {
            projetil();
            pontos = calcularPontos(739, xAlvo);
            (*placar).pontuacaoRodadas[rodada] = (*placar).pontuacaoRodadas[rodada] + pontos;
            (*placar).pontuacaoTotal = (*placar).pontuacaoTotal + pontos;
            disparoAtivo = 0;

            if (colisao(739, 549, 10, xAlvo, yAlvo) == 1) {
                readimagefile("c:\\temp\\target.jpg", xAlvo, yAlvo, x2Alvo, yAlvoBaixo);
                delay(500);
            }
        }

        setcolor(WHITE);
        settextjustify(RIGHT_TEXT, TOP_TEXT);
        sprintf(pontuacaoStr, "Pontuacao: %d", (*placar).pontuacaoRodadas[rodada]);
        outtextxy(getmaxx() - 10, 10, pontuacaoStr);
    }
}

void projetil() {
    int centroX = 739;
    int centroY = 549;
    int i = 0;

    setcolor(BLACK);
    setfillstyle(SOLID_FILL, BLACK);

    //movimentação do projetil
    for (i = 0; i <= 400; i = i + 10) {

        setfillstyle(SOLID_FILL, BLACK);
        fillellipse(centroX, centroY, 10, 10);

        centroY = centroY - 10;

        setfillstyle(SOLID_FILL, RED);
        fillellipse(centroX, centroY, 10, 10);

        delay(50);
    }

    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(centroX, centroY, 10, 10);
}

void moverAlvo(int *xAlvo, int *direcao, int *x2Alvo) {
    int larguraJan = 1530;
    int velocidade = 35;

    *xAlvo = (*xAlvo) + (*direcao) * velocidade;
    *x2Alvo = *xAlvo + 150;

    if (*xAlvo <= 0 || *x2Alvo >= larguraJan) {
        *direcao = -(*direcao);
    }
}

int colisao(int xElipse, int yElipse, int raioElipse, int xAlvo, int yAlvo) {
    if (xElipse >= xAlvo && xElipse <= xAlvo + 150 && yElipse <= yAlvo+150) {
        return 1;
    }
}

int calcularPontos(int xElipse, int xAlvo) {
    int distanciaX = abs(xElipse - (xAlvo + 75));

    if (distanciaX <= 10) {
        return 20;
    }
    if (distanciaX < 20) {
        return 10;
    }
    if (distanciaX < 50) {
        return 5;
    }
    else {
        return 0;
    }
}

void exibirPontuacaoRodadas(Placar *placar) {
    char pontuacaoStr[50];
    int baseX = (getmaxx() / 2);
    int baseY = (getmaxy() / 2);
    int i = 0;
    
    setcolor(WHITE);
    settextjustify(CENTER_TEXT, BOTTOM_TEXT);

    //armazena a pontuação de cada rodada na string e depois mostra na tela
    for (i = 0; i < 3; i++) {
        sprintf(pontuacaoStr, "Rodada %d: %d pontos", i + 1, (*placar).pontuacaoRodadas[i]);
        outtextxy(baseX, baseY - (3 - i) * 20, pontuacaoStr);
    }

    //armazena a pontuação final e mostra depois
    sprintf(pontuacaoStr, "Total: %d pontos", (*placar).pontuacaoTotal);
    outtextxy(baseX, baseY, pontuacaoStr);
}
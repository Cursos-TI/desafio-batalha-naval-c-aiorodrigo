#include <stdio.h>
#include <stdbool.h> // Para usar o tipo booleano
#include <stdlib.h> // Para usar a função abs()

#define TABULEIRO_TAM 10
#define NAVIO_TAM 3
#define HABILIDADE_TAM 7 // Tamanho padrão para as matrizes de habilidade (ex: 7x7)

// Função para exibir o tabuleiro
void exibirTabuleiro(int tabuleiro[TABULEIRO_TAM][TABULEIRO_TAM]) {
    for (int i = 0; i < TABULEIRO_TAM; i++) {
        for (int j = 0; j < TABULEIRO_TAM; j++) {
            printf("%d ", tabuleiro[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Função para inicializar o tabuleiro com água (0)
void inicializarTabuleiro(int tabuleiro[TABULEIRO_TAM][TABULEIRO_TAM]) {
    for (int i = 0; i < TABULEIRO_TAM; i++) {
        for (int j = 0; j < TABULEIRO_TAM; j++) {
            tabuleiro[i][j] = 0; // 0 representa água
        }
    }
}

// Função para verificar se a posição é válida dentro do tabuleiro
bool isPosicaoValida(int linha, int coluna) {
    return linha >= 0 && linha < TABULEIRO_TAM && coluna >= 0 && coluna < TABULEIRO_TAM;
}

// Função para posicionar um navio (horizontal ou vertical)
void posicionarNavio(int tabuleiro[TABULEIRO_TAM][TABULEIRO_TAM], int inicioLinha, int inicioColuna, int tamanho, bool isHorizontal) {
    for (int k = 0; k < tamanho; k++) {
        int l = inicioLinha + (isHorizontal ? 0 : k);
        int c = inicioColuna + (isHorizontal ? k : 0);

        if (isPosicaoValida(l, c)) {
            // Simplificação de validação de sobreposição: apenas marca a posição
            // Em um jogo real, haveria uma verificação antes de marcar
            tabuleiro[l][c] = 3; // 3 representa o navio
        }
    }
}

// Função para posicionar um navio diagonal
void posicionarNavioDiagonal(int tabuleiro[TABULEIRO_TAM][TABULEIRO_TAM], int inicioLinha, int inicioColuna, int tamanho, bool crescente) {
    for (int k = 0; k < tamanho; k++) {
        int l = inicioLinha + k;
        int c = crescente ? (inicioColuna + k) : (inicioColuna - k);

        if (isPosicaoValida(l, c)) {
            tabuleiro[l][c] = 3; // 3 representa o navio
        }
    }
}

// Funções para construir as matrizes de habilidade (valor 1 para área afetada, 0 para não afetada)

// Habilidade Cone (aponta para baixo)
void construirHabilidadeCone(int habilidade[HABILIDADE_TAM][HABILIDADE_TAM]) {
    for (int i = 0; i < HABILIDADE_TAM; i++) {
        for (int j = 0; j < HABILIDADE_TAM; j++) {
            habilidade[i][j] = 0;
            // Centro do cone está na linha 0, coluna do meio
            int centro_col = HABILIDADE_TAM / 2;
            int dist_horizontal = j - centro_col;
            // Um cone se expande com a linha
            if (i >= 0 && i < HABILIDADE_TAM) { // Garante que estamos dentro dos limites da "altura" do cone
                if (dist_horizontal >= -i && dist_horizontal <= i) { // A largura do cone aumenta com 'i'
                    habilidade[i][j] = 1;
                }
            }
        }
    }
}

// Habilidade Cruz
void construirHabilidadeCruz(int habilidade[HABILIDADE_TAM][HABILIDADE_TAM]) {
    int centro = HABILIDADE_TAM / 2;
    for (int i = 0; i < HABILIDADE_TAM; i++) {
        for (int j = 0; j < HABILIDADE_TAM; j++) {
            habilidade[i][j] = 0;
            if (i == centro || j == centro) {
                habilidade[i][j] = 1;
            }
        }
    }
}

// Habilidade Octaedro (Losango na vista frontal)
void construirHabilidadeOctaedro(int habilidade[HABILIDADE_TAM][HABILIDADE_TAM]) {
    int centro = HABILIDADE_TAM / 2;
    for (int i = 0; i < HABILIDADE_TAM; i++) {
        for (int j = 0; j < HABILIDADE_TAM; j++) {
            habilidade[i][j] = 0;
            int dist = abs(i - centro) + abs(j - centro); // Distância Manhattan do centro
            if (dist <= centro) { // O "raio" do losango é o centro
                habilidade[i][j] = 1;
            }
        }
    }
}

// Função para sobrepor uma habilidade ao tabuleiro
void sobreporHabilidade(int tabuleiro[TABULEIRO_TAM][TABULEIRO_TAM], int habilidade[HABILIDADE_TAM][HABILIDADE_TAM], int origemLinha, int origemColuna) {
    int offset = HABILIDADE_TAM / 2; // Offset para centralizar a habilidade
    for (int hl = 0; hl < HABILIDADE_TAM; hl++) { // hl = habilidade_linha
        for (int hc = 0; hc < HABILIDADE_TAM; hc++) { // hc = habilidade_coluna
            if (habilidade[hl][hc] == 1) {
                int tl = origemLinha + (hl - offset); // tl = tabuleiro_linha
                int tc = origemColuna + (hc - offset); // tc = tabuleiro_coluna

                if (isPosicaoValida(tl, tc)) {
                    // Marca com 5 se for uma área afetada pela habilidade e não for navio (3)
                    // Se já for navio, a habilidade não "apaga" o navio visualmente no tabuleiro neste desafio
                    if (tabuleiro[tl][tc] != 3) {
                        tabuleiro[tl][tc] = 5; // 5 representa área afetada pela habilidade
                    }
                }
            }
        }
    }
}


int main() {
    int tabuleiro[TABULEIRO_TAM][TABULEIRO_TAM];
    inicializarTabuleiro(tabuleiro);

    // --- Nível Novato: Posicionando Navios Simples ---
    // Navio horizontal (3 posições)
    posicionarNavio(tabuleiro, 2, 1, NAVIO_TAM, true); // Linha 2, Coluna 1, horizontal
    // Navio vertical (3 posições)
    posicionarNavio(tabuleiro, 5, 5, NAVIO_TAM, false); // Linha 5, Coluna 5, vertical
    
    printf("--- Tabuleiro com Navios Simples (Nível Novato) ---\n");
    exibirTabuleiro(tabuleiro);

    // --- Nível Aventureiro: Posicionando Navios Diagonais ---
    // Navio diagonal crescente
    posicionarNavioDiagonal(tabuleiro, 1, 7, NAVIO_TAM, false); // Linha 1, Coluna 7, diagonal decrescente
    // Navio diagonal decrescente
    posicionarNavioDiagonal(tabuleiro, 7, 1, NAVIO_TAM, true); // Linha 7, Coluna 1, diagonal crescente

    printf("--- Tabuleiro com Navios Diagonais (Nível Aventureiro) ---\n");
    exibirTabuleiro(tabuleiro);

    // --- Nível Mestre: Habilidades Especiais ---

    // Criar matrizes de habilidade
    int habilidadeCone[HABILIDADE_TAM][HABILIDADE_TAM];
    int habilidadeCruz[HABILIDADE_TAM][HABILIDADE_TAM];
    int habilidadeOctaedro[HABILIDADE_TAM][HABILIDADE_TAM];

    construirHabilidadeCone(habilidadeCone);
    construirHabilidadeCruz(habilidadeCruz);
    construirHabilidadeOctaedro(habilidadeOctaedro);

    // Definir pontos de origem para as habilidades no tabuleiro
    int origemConeL = 2; // Centro do cone no tabuleiro
    int origemConeC = 2;

    int origemCruzL = 6; // Centro da cruz no tabuleiro
    int origemCruzC = 3;

    int origemOctaedroL = 4; // Centro do octaedro no tabuleiro
    int origemOctaedroC = 7;

    // Aplicar habilidades ao tabuleiro
    sobreporHabilidade(tabuleiro, habilidadeCone, origemConeL, origemConeC);
    sobreporHabilidade(tabuleiro, habilidadeCruz, origemCruzL, origemCruzC);
    sobreporHabilidade(tabuleiro, habilidadeOctaedro, origemOctaedroL, origemOctaedroC);

    printf("--- Tabuleiro com Habilidades Especiais (Nível Mestre) ---\n");
    exibirTabuleiro(tabuleiro);

    return 0;
}
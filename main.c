// Includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <dirent.h>

// Cores em ANSI utilizadas
#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_RESET "\x1b[0m"

// Structs de matriz e jogador
struct matrizes
{
    int tam;
    char cor;
};

struct jogadores
{
    char nome[18];
};

// Pega o centro do terminal
int termCenter()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int colunas, colunacentro;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    colunas = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    colunacentro = colunas / 2;
    return colunacentro;
}

// Printa no centro do terminal
void printCenter(char conteudo[], int linha)
{
    int colunacentro = termCenter();
    gotoxy(colunacentro - (strlen(conteudo) / 2), linha);
    printf("%s", conteudo);
}

// Funçao para limitar o tamanho da entrada de input
void getInput(char *buff, int size)
{
    char keyboard[BUFSIZ];
    setbuf(stdin, keyboard);
    fgets(buff, size, stdin);
}

// Funçao para imprimir em coordenadas especificas do terminal
void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Faz o replace de char especificos em uma string
char replace(char *str, char find, char replace)
{
    char *pos = strchr(str, find);
    while (pos)
    {
        *pos = replace;
        pos = strchr(pos, find);
    }
    return str;
}

// Gera as peças com cores aleatorias
char gerarPeca()
{
    int random = rand() % 4;
    char peca, possibilidades[4] = {'R', 'G', 'B', 'Y'};
    peca = possibilidades[random];
    return peca;
}

// Printa as setas coloridas, conforme a cor do jogador
void exibeSeta(int seta)
{
    switch (seta)
    {
    case 0:
        printf(COLOR_RED "-> " COLOR_RESET);
        break;

    case 1:
        printf(COLOR_GREEN "-> " COLOR_RESET);
        break;

    case 2:
        printf(COLOR_BLUE "-> " COLOR_RESET);
        break;

    case 3:
        printf(COLOR_YELLOW "-> " COLOR_RESET);
        break;
    }
}

// Conta as peças do jogador para exibir no placar
int placar(int pcr, struct matrizes matriz[5][5])
{
    int pcrf = 0;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (matriz[i][j].cor == 'R' && pcr == 0)
                pcrf += matriz[i][j].tam;
            if (matriz[i][j].cor == 'G' && pcr == 1)
                pcrf += matriz[i][j].tam;
            if (matriz[i][j].cor == 'B' && pcr == 2)
                pcrf += matriz[i][j].tam;
            if (matriz[i][j].cor == 'Y' && pcr == 3)
                pcrf += matriz[i][j].tam;
        }
    }
    return pcrf;
}

// Preenche o tabuleiro com os valores iniciais
void resetMatriz(struct matrizes matriz[5][5])
{
    int r = 0, g = 0, b = 0, y = 0;
    char peca;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if ((i == 0 && j == 0) || (i == 4 && j == 4) || (i == 0 && j == 4) || (i == 2 && j == 2) || (i == 4 && j == 0))
            {
                matriz[i][j].tam = 0;
                matriz[i][j].cor = 'n';
            }
            else
            {
                matriz[i][j].tam = 1;

                do
                {
                    peca = gerarPeca();

                    if (peca == 'R' && r < 5)
                    {
                        matriz[i][j].cor = peca;
                        r++;
                    }

                    if (peca == 'G' && g < 5)
                    {
                        matriz[i][j].cor = peca;
                        g++;
                    }
                    if (peca == 'Y' && y < 5)
                    {
                        matriz[i][j].cor = peca;
                        y++;
                    }
                    if (peca == 'B' && b < 5)
                    {
                        matriz[i][j].cor = peca;
                        b++;
                    }
                } while (matriz[i][j].cor != peca);
            }
        }
    }
}

// Renderiza o tabuleiro
void exibeTabuleiro(char nomePartida[10], struct matrizes matriz[5][5], struct jogadores jogador[4])
{
    system("cls");
    int pcrf;
    gotoxy(0, 0);
    printf("%s", nomePartida);
    printCenter("[COMANDOS: H0 = Home | F0 = Fechar]", 0);

    printCenter("[TABULEIRO]", 2);
    printCenter("1   2   3   4   5", 4);
    printCenter(" -|---|---|---|---|---|-", 5);
    printCenter("A |   |   |   |   |   | ", 6);
    printCenter(" -|---|---|---|---|---|-", 7);
    printCenter("B |   |   |   |   |   | ", 8);
    printCenter(" -|---|---|---|---|---|-", 9);
    printCenter("C |   |   |   |   |   | ", 10);
    printCenter(" -|---|---|---|---|---|-", 11);
    printCenter("D |   |   |   |   |   | ", 12);
    printCenter(" -|---|---|---|---|---|-", 13);
    printCenter("E |   |   |   |   |   | ", 14);
    printCenter(" -|---|---|---|---|---|-", 15);

    int pos = 9, pos2 = 6;
    for (int l = 0; l < 5; l++)
    {
        for (int c = 0; c < 5; c++)
        {
            switch (matriz[l][c].cor)
            {
            case 'n':
                gotoxy(termCenter() - pos, l + pos2);
                printf(COLOR_RED "   " COLOR_RESET);
                pos -= 4;
                break;

            case 'R':
                gotoxy(termCenter() - pos, l + pos2);
                printf(COLOR_RED " %d " COLOR_RESET, matriz[l][c].tam);
                pos -= 4;
                break;

            case 'G':
                gotoxy(termCenter() - pos, l + pos2);
                printf(COLOR_GREEN " %d " COLOR_RESET, matriz[l][c].tam);
                pos -= 4;
                break;

            case 'B':
                gotoxy(termCenter() - pos, l + pos2);
                printf(COLOR_BLUE " %d " COLOR_RESET, matriz[l][c].tam);
                pos -= 4;
                break;

            case 'Y':
                gotoxy(termCenter() - pos, l + pos2);
                printf(COLOR_YELLOW " %d " COLOR_RESET, matriz[l][c].tam);
                pos -= 4;
                break;
            }
            if (pos == -11)
                pos = 9;
            if (c == 4)
                pos2 += 1;
        }
    }
    gotoxy(termCenter() + 16, 8);
    printf("[PLACAR]");
    for (int j = 0; j < 4; j++)
    {
        pcrf = placar(j, matriz);
        gotoxy(termCenter() + 16, 9 + j);
        printf("%d ", pcrf);
        exibeSeta(j);
        printf("%s", jogador[j].nome);
    }
}

// Converte a entrada do jogador para as coordenadas da matriz
void posicaoMatriz(char x, int y, int coord[2])
{
    x = tolower(x);

    if (x == 'h' && y == 0)
        jogar();

    if (x == 'f' && y == 0)
        exit(0);

    switch (x)
    {
    case 'a':
        coord[0] = 0;
        break;

    case 'b':
        coord[0] = 1;
        break;

    case 'c':
        coord[0] = 2;
        break;

    case 'd':
        coord[0] = 3;
        break;

    case 'e':
        coord[0] = 4;
        break;

    default:
        coord[0] = 9;
        break;
    }
    coord[1] = y - 1;
}

// Converte o sentido que o jogador deseja movimentar em coordenada
void posicaoMatriz2(int x2, int coord[2], int coord2[2])
{
    switch (x2)
    {
    case 7:
        coord2[0] = coord[0] - 1;
        coord2[1] = coord[1] - 1;
        break;

    case 8:
        coord2[0] = coord[0] - 1;
        coord2[1] = coord[1];
        break;

    case 9:
        coord2[0] = coord[0] - 1;
        coord2[1] = coord[1] + 1;
        break;

    case 4:
        coord2[0] = coord[0];
        coord2[1] = coord[1] - 1;
        break;

    case 6:
        coord2[0] = coord[0];
        coord2[1] = coord[1] + 1;
        break;

    case 1:
        coord2[0] = coord[0] + 1;
        coord2[1] = coord[1] - 1;
        break;

    case 2:
        coord2[0] = coord[0] + 1;
        coord2[1] = coord[1];
        break;

    case 3:
        coord2[0] = coord[0] + 1;
        coord2[1] = coord[1] + 1;
        break;
    }
}

// Verifica se a peça pode ser movida pelo jogador
int validaPeca(int turno, int coord[2], struct matrizes matriz[5][5])
{
    if ((coord[0] >= 0) && (coord[0] <= 4) && (coord[1] >= 0) && (coord[1] <= 4))
    {
        if (movPos(5, coord, matriz) == 0)
            return 0;
        else if (turno == 0 && matriz[coord[0]][coord[1]].cor == 'R')
            return 1;
        else if (turno == 1 && matriz[coord[0]][coord[1]].cor == 'G')
            return 1;
        else if (turno == 2 && matriz[coord[0]][coord[1]].cor == 'B')
            return 1;
        else if (turno == 3 && matriz[coord[0]][coord[1]].cor == 'Y')
            return 1;
        else
            return 0;
    }
    else
        return 0;
}

// Verifica se o movimento pretendido pelo jogador é possivel
int validaMovimento(int x2, int coord[2], int coord2[2], struct matrizes matriz[5][5])
{
    int contador = 0;
    // Validaçoes para não movimentar para fora do tabuleiro
    if (x2 >= 1 && x2 <= 9 && x2 != 5)
        contador += 1;
    if ((x2 == 4 || x2 == 1 || x2 == 7) && coord[1] == 0)
        return 0;
    if ((x2 == 9 || x2 == 6 || x2 == 3) && coord[1] == 4)
        return 0;
    if ((x2 == 7 || x2 == 8 || x2 == 9) && coord[0] == 0)
        return 0;
    if ((x2 == 1 || x2 == 2 || x2 == 3) && coord[0] == 4)
        return 0;

    // Validação para não mover para espaço vazio
    if (matriz[coord2[0]][coord2[1]].cor == 'n')
        return 0;

    // Validaçãoo para não mover para uma peça maior
    if (matriz[coord2[0]][coord2[1]].tam > matriz[coord[0]][coord[1]].tam)
        return 0;

    if (contador > 0)
        return 1;
}

// Conta a quantidade de movimentos possíveis para cada jogador, ou para uma peça específica
int movPos(int turno, int coord[2], struct matrizes matriz[5][5])
{
    int contador = 0;
    char cor[5] = {'R', 'G', 'B', 'Y', 'n', 'x'};
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (matriz[i][j].cor == cor[turno] || (coord[0] == i && coord[1] == j))
            {
                // Se petence a primeira linha
                if (i == 0)
                {
                    // Se pertence as colunas 2, 3, ou 4
                    if ((j == 1) || (j == 2) || (j == 3))
                    {
                        if ((matriz[i][j].tam >= matriz[i][j - 1].tam && matriz[i][j - 1].tam != 0) || (matriz[i][j].tam >= matriz[i][j + 1].tam && matriz[i][j + 1].tam != 0) || (matriz[i][j].tam >= matriz[i + 1][j - 1].tam && matriz[i + 1][j - 1].tam != 0) || (matriz[i][j].tam >= matriz[i + 1][j].tam && matriz[i + 1][j].tam != 0) || (matriz[i][j].tam >= matriz[i + 1][j + 1].tam && matriz[i + 1][j + 1].tam != 0))
                            contador += 1;
                    }
                }

                // Se petence as linhas 2, 3, ou 4
                if ((i == 1) || (i == 2) || (i == 3))
                {
                    // Se pertence a primeira coluna
                    if (j == 0)
                    {
                        if ((matriz[i][j].tam >= matriz[i - 1][j].tam && matriz[i - 1][j].tam != 0) || (matriz[i][j].tam >= matriz[i - 1][j + 1].tam && matriz[i - 1][j + 1].tam != 0) || (matriz[i][j].tam >= matriz[i][j + 1].tam && matriz[i][j + 1].tam != 0) || (matriz[i][j].tam >= matriz[i + 1][j].tam && matriz[i + 1][j].tam != 0) || (matriz[i][j].tam >= matriz[i + 1][j + 1].tam && matriz[i + 1][j + 1].tam != 0))
                            contador += 1;
                    }
                    // Se pertence a ultima coluna
                    if (j == 4)
                    {
                        if ((matriz[i][j].tam >= matriz[i - 1][j].tam && matriz[i - 1][j].tam != 0) || (matriz[i][j].tam >= matriz[i - 1][j - 1].tam && matriz[i - 1][j - 1].tam != 0) || (matriz[i][j].tam >= matriz[i][j - 1].tam && matriz[i][j - 1].tam != 0) || (matriz[i][j].tam >= matriz[i + 1][j].tam && matriz[i + 1][j].tam != 0) || (matriz[i][j].tam >= matriz[i + 1][j - 1].tam && matriz[i + 1][j - 1].tam != 0))
                            contador += 1;
                    }
                    // Se pertence as colunas 2, 3, ou 4
                    if ((j == 1) || (j == 2) || (j == 3))
                    {
                        if ((matriz[i][j].tam >= matriz[i - 1][j].tam && matriz[i - 1][j].tam != 0) || (matriz[i][j].tam >= matriz[i - 1][j - 1].tam && matriz[i - 1][j - 1].tam != 0) || (matriz[i][j].tam >= matriz[i][j - 1].tam && matriz[i][j - 1].tam != 0) || (matriz[i][j].tam >= matriz[i + 1][j].tam && matriz[i + 1][j].tam != 0) || (matriz[i][j].tam >= matriz[i + 1][j - 1].tam && matriz[i + 1][j - 1].tam != 0) || (matriz[i][j].tam >= matriz[i - 1][j + 1].tam && matriz[i - 1][j + 1].tam != 0) || (matriz[i][j].tam >= matriz[i][j + 1].tam && matriz[i][j + 1].tam != 0) || (matriz[i][j].tam >= matriz[i + 1][j + 1].tam && matriz[i + 1][j + 1].tam != 0))
                            contador += 1;
                    }
                }

                // Se pertence a ultima linha
                if (i == 4)
                {
                    // Se pertence as colunas 2, 3, ou 4
                    if ((j == 1) || (j == 2) || (j == 3))
                    {
                        if ((matriz[i][j].tam >= matriz[i - 1][j - 1].tam && matriz[i - 1][j - 1].tam != 0) || (matriz[i][j].tam >= matriz[i - 1][j].tam && matriz[i - 1][j].tam != 0) || (matriz[i][j].tam >= matriz[i - 1][j + 1].tam && matriz[i - 1][j + 1].tam != 0) || (matriz[i][j].tam >= matriz[i][j - 1].tam && matriz[i][j - 1].tam != 0) || (matriz[i][j].tam >= matriz[i][j + 1].tam && matriz[i][j + 1].tam != 0))
                            contador += 1;
                    }
                }
            }
        }
    }
    return contador;
}

// Troca os turnos
int trocaTurno(int turno)
{
    switch (turno)
    {
    case 0:
        turno += 1;
        return turno;
        break;

    case 1:
        turno += 1;
        return turno;
        break;

    case 2:
        turno += 1;
        return turno;
        break;

    case 3:
        turno = 0;
        return turno;
        break;
    }
}

// Verifica se a partida ja terminou
int fimPartida(struct matrizes matriz[5][5])
{
    int coord[2] = {5, 5};
    int contador = 0;

    for (int i = 0; i < 5; i++)
        contador += movPos(i, coord, matriz);

    if (contador == 0)
        return 0;
    else
        return 1;
}

// Salva o estdado do game
void salvaPartida(FILE *saves, int turno, char nomePartida[10], struct jogadores jogador[4], struct matrizes matriz[5][5])
{
    char diretorio[] = ".\\saves\\";

    // Cria a pasta de saves, caso nao exista
    CreateDirectory("saves", NULL);

    saves = fopen(strcat(diretorio, nomePartida), "w");
    fprintf(saves, "%d\n", turno);

    for (int p = 0; p < 4; p++)
    {
        replace(jogador[p].nome, ' ', '_');
        fprintf(saves, "%s\n", jogador[p].nome);
        replace(jogador[p].nome, '_', ' ');
    }

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            fprintf(saves, "%d", matriz[i][j].tam);
            fprintf(saves, "%c", matriz[i][j].cor);
            fprintf(saves, "%s", "\n");
        }
    }
    fclose(saves);
}

void apagaSave(char nomePartida[10])
{
    char diretorio[] = ".\\saves\\";
    remove(strcat(diretorio, nomePartida));
}

// Logica principal do jogo
void jogar()
{
    FILE *saves;
    DIR *d;
    struct dirent *dir;
    static char nomePartida[10];
    char x, partidaNC, diretorio[] = ".\\saves\\", keyboard[BUFSIZ];
    static int y, x2, vencedor = 0;
    int turno;
    static int coord[2], coord2[2];

    // define a struct do tabuleiro
    struct matrizes matriz[5][5];

    // define a struct dos jogadores
    struct jogadores jogador[4];

    // Pergunta se o usuario quer iniciar nova partida, ou carregar
    do
    {
        system("cls");
        printCenter("Novo Jogo [N]", 15);
        printCenter("Carregar  [C]", 16);
        gotoxy(termCenter(), 17);
        partidaNC = getch();
    } while (tolower(partidaNC) != 'n' && tolower(partidaNC) != 'c');

    // cria uma nova partida ou carrega uma existente
    if (tolower(partidaNC) == 'n')
    {
        // Preenche o tabuleiro com os valores iniciais
        resetMatriz(matriz);
        turno = 0;
        // Pega o nome da partida
        system("cls");
        printCenter("[Insira o nome da partida]", 15);
        gotoxy(termCenter() - 5, 16);
        setbuf(stdin, keyboard);
        gets(nomePartida);

        // pega os nomes dos jogadores
        for (int j = 0; j < 4; j++)
        {
            system("cls");
            gotoxy(termCenter() - 10, 15);
            printf("[Nome do Jogador %d]", j + 1);
            gotoxy(termCenter() - 10, 16);
            exibeSeta(j);
            getInput(jogador[j].nome, 18);
            jogador[j].nome[strcspn(jogador[j].nome, "\n")] = 0;
        }
        salvaPartida(saves, turno, nomePartida, jogador, matriz);
    }
    else if (tolower(partidaNC) == 'c')
    {
        system("cls");
        d = opendir(diretorio);
        if (d)
        {
            while ((dir = readdir(d)) != NULL)
                printf("%s\n", dir->d_name);
            closedir(d);
        }
        gotoxy(0, 0);
        printf("[Todos os saves]\n[^A vZ]");
        printCenter("[Nome da partida que deseja carregar]", 15);
        gotoxy(termCenter(), 16);
        setbuf(stdin, keyboard);
        gets(nomePartida);

        saves = fopen(strcat(diretorio, nomePartida), "r");
        if (saves == NULL)
            jogar();

        fscanf(saves, "%d", &turno);

        for (int r = 0; r < 4; r++)
        {
            fscanf(saves, "%s", &jogador[r].nome);
            replace(jogador[r].nome, '_', ' ');
        }

        for (int mi = 0; mi < 5; mi++)
        {
            for (int mj = 0; mj < 5; mj++)
                fscanf(saves, "%d%c", &matriz[mi][mj].tam, &matriz[mi][mj].cor);
        }
        fclose(saves);
    }

    // Inicio do loop do jogo
    do
    {
        coord[0] = 5;
        coord[1] = 5;
        if (movPos(turno, coord, matriz) > 0)
        {
            // Entrada inicial do movimento
            exibeTabuleiro(nomePartida, matriz, jogador);
            gotoxy(termCenter() - 5 - (strlen(jogador[turno].nome) / 2), 17);
            printf("Vez de ");
            exibeSeta(turno);
            printf("%s", jogador[turno].nome);
            printCenter("[Qual peca deseja mover? Ex. a2] -> ", 18);
            setbuf(stdin, keyboard);
            scanf("%c%d", &x, &y);
            posicaoMatriz(x, y, coord);

            if (validaPeca(turno, coord, matriz))
            {
                do
                {
                    // Entrada final do movimento
                    exibeTabuleiro(nomePartida, matriz, jogador);
                    gotoxy(termCenter() - 16, 17);
                    printf("[Para qual sentido mover (%c%d)?]", x, y);
                    printCenter(" 7  8  9", 18);
                    printCenter(" \\ ^ /", 19);
                    printCenter(" 4 < o > 6", 20);
                    printCenter(" / v \\", 21);
                    printCenter(" 1  2  3", 22);
                    gotoxy(termCenter() - 2, 23);
                    exibeSeta(turno);

                    setbuf(stdin, keyboard);
                    scanf("%d", &x2);

                    posicaoMatriz2(x2, coord, coord2);
                } while (validaMovimento(x2, coord, coord2, matriz) != 1);
            }

            if (validaMovimento(x2, coord, coord2, matriz) && validaPeca(turno, coord, matriz))
            {
                // Se todas as condiçoes forem cumpridas, realiza o movimento
                matriz[coord2[0]][coord2[1]].tam = (matriz[coord2[0]][coord2[1]].tam + matriz[coord[0]][coord[1]].tam);
                matriz[coord2[0]][coord2[1]].cor = matriz[coord[0]][coord[1]].cor;
                matriz[coord[0]][coord[1]].cor = 'n';
                matriz[coord[0]][coord[1]].tam = 0;
                turno = trocaTurno(turno);
                salvaPartida(saves, turno, nomePartida, jogador, matriz);
            }
        }
        else
            turno = trocaTurno(turno);
    } while (fimPartida(matriz) != 0);

    exibeTabuleiro(nomePartida, matriz, jogador);
    printCenter("A PARTIDA ACABOU!", 17);
    for (int v = 0; v < 4; v++)
    {
        if (placar(v, matriz) >= placar(vencedor, matriz))
            vencedor = v;
    }
    printCenter("[Vencedor]", 19);
    gotoxy(termCenter() - (1 + strlen(jogador[vencedor].nome) / 2), 20);
    exibeSeta(vencedor);
    printf("%s", jogador[vencedor].nome);
    printCenter("[Obs. apos o termino da partida, o save sera deletado automaticamente]", 22);
    apagaSave(nomePartida);
    turno = 0;
    for (int w = 0; w < 5; w++)
    {
        for (int z = 0; z < 5; z++)
        {
            matriz[w][z].cor = 'n';
            matriz[w][z].tam = 0;
        }
    }
    printCenter("[Pressione qualquer tecla para voltar para a Home]", 23);
    gotoxy(termCenter(), 24);
    getch();
    jogar();
}

int main()
{
    system("cls");

    SetConsoleTitle("Cappuccino");
    system("MODE con cols=76 lines=32 ");
    // Gera uma nova seed aleatoria
    srand(time(NULL));

    jogar();

    return 0;
}

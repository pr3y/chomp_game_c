/*
 * Autor: prey37
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

char ** matrix;    //define a matrix principal

struct Game_Info     //cria a struct com as informacoes dos nomes e tamanho do tabuleiro
{
    int player;

    char * player_name_1;
    char * player_name_2;

    int altura;
    int largura;
}gi;

struct Game_Control    //cria struct que declara as variaveis de controle do programa
{
    int reiniciar_jogo;
    int sair_sem_salvar;
    int continuar_jogo;
    int carregar_jogo_salvo;
}gc;

void setar_valores_padroes()  //inicia as variaveis da estrutura game_control com os valores zerados
{
    gc.reiniciar_jogo       = 0;
    gc.sair_sem_salvar      = 0;
    gc.continuar_jogo       = 0;
    gc.carregar_jogo_salvo  = 0;
}

void inicializa()
{
    int row, col,i;

    printf("\nDigite o numero de linhas : ");
    scanf("%d", &gi.altura);
    printf("Digite o numero de colunas : ");
    scanf("%d", &gi.largura);

    matrix = malloc (gi.altura * sizeof(char *));// aloca na memoria a quantidade de linhas digitado pelo jogador

    for (i = 0; i < gi.altura; i++)
        matrix[i] = malloc (gi.largura * sizeof (char));// aloca a quantidade de colunas

    for(row = 0; row < gi.altura; row++)  //define a matriz padrao
        for(col = 0; col < gi.largura; col++)
            matrix[row][col] = 'O';
}


void imprime_tabuleiro() //funcao q cuida da impressao do tabuleiro
{
    int row, col;

    for(row = 0; row < gi.altura; row++) //cria laco para a altura da matriz
    {
        for(col = 0; col < gi.largura; col++)//laco para a largura da matriz
        {
            printf("%c", matrix[row][col]);
        }
        printf("\n");
    }
    printf("\n");
}

void move(int input[]) // funcao q cuida da movimentacao 
{
    printf("Jogador %d (%s), faca um movimento: ", gi.player, (gi.player-1) == 0 ? gi.player_name_1 : gi.player_name_2);
    scanf("%d %d", &input[0], &input[1]);
}

int check_move(int position[]) //funcao q checa se o movimento eh valido ou nao
{

    int row = position[0] - 1;
    int col = position[1] - 1;

   if(matrix[row][col] == ' ')
   {
        //printf("Movimento Invalido \n");
        return 0;
   } else return 1;
}

void update_board(int x, int y)  
{
    for(int xi = x; xi <= gi.altura; ++xi)
    {
        for(int yi = y; yi <= gi.largura; ++yi)
            matrix[xi-1][yi-1] = ' ';
    }
}

bool vencedor()
{
    if(matrix[0][0] == 'O')
        return false;
    else return true;
}

void menu_opcoes_dentro_jogo() //funcao q faz perguntas a cada jogada feita pelo jogador
{
    int op;

    do
    {
        system("clear");

        printf("Deseja:\n\n1 - Continuar jogando.\n2 - Exibir menu de opcoes.\n\n");
        scanf("%d", &op);
    } while(op != 1 && op != 2);

    if(op == 1) return; // continua com o jogo
    if(op == 2) //exibe o menu para outras opcoes
    {
        int op;

        do
        {
            system("clear");

            printf("\n\nDeseja:\n\n1 - Salvar jogo.\n2 - Reiniciar jogo.\n3 - Sair sem salvar.\n4 - Voltar ao jogo atual.\n\n");
            scanf("%d", &op);
        } while(op != 1 && op != 2 && op != 3 && op != 4);

        if(op == 1)//salve o jogo no .txt
        {
            printf("\n\nJogo que sera salvo :\n\n");

            imprime_tabuleiro(); //mostra o tabuleiro q sera salvo

			system("pause"); 

            FILE * arquivo = fopen("game_save.txt", "w+"); //cria arquivo .txt para ser salvo (independente se existir ou nao)

            int row, col, i;

            fprintf(arquivo, "%d\n", gi.altura);//imprime altura do tabuleiro no .txt
            fprintf(arquivo, "%d\n", gi.largura);//imprime largura do tabuleiro no .txt

            fprintf(arquivo, "%d\n", strlen(gi.player_name_1));//imprime o tamanho do nome do jogador 1 no .txt
            fprintf(arquivo, "%s\n", gi.player_name_1);//imprime o nome do jogador 1 no .txt
            fprintf(arquivo, "%d\n", strlen(gi.player_name_2));//imprime o tamanho do nome do jogador 2 no .txt
            fprintf(arquivo, "%s\n", gi.player_name_2);//imprime o nome do jogador 2 no .txt

            fprintf(arquivo, "%d\n", gi.player); //imprime vez do jogador

            for(row = 0; row < gi.altura; row++) //salva matriz
            {
                for(col = 0; col < gi.largura; col++)
                {
                    if(row == 0 && col == 0)
                        fprintf(arquivo, "2");
                    else {
                        if(matrix[row][col] == 'O')
                        {
                            fprintf(arquivo, "1");
                        }

                        if(matrix[row][col] == ' ')
                        {
                            fprintf(arquivo, "0");
                        }
                    }
                }
            }

            fclose(arquivo);

            printf("\n\nJogo salvo com sucesso!\n\n");

            system("pause");

            system("clear");
        } else if(op == 2)//reinicia o jogo
        {
            gc.reiniciar_jogo = 1;
            return;
        } else if(op == 3)//sai do jogo sem salvar
        {
            gc.sair_sem_salvar = 1;
            return;
        } else if(op == 4) return;//retorna ao jogo
    }
}

void menu()
{
    int op; //cria variavel de escolha

    printf("Deseja:\n\n1 - Jogar.\n2 - Carregar jogo.\n3 - Sair.\n\n");
    scanf("%d", &op);

    if(op == 1)
    {
        gc.continuar_jogo = 1;  //caso usuario queira iniciar novo jogo , retorna ao main
        return;
    }
    else if(op == 2)// carrega jogo
    {
        FILE * arquivo = fopen("game_save.txt", "r");

        if(arquivo == NULL)
        {
            printf("\n\nImpossivel carregar um jogo salvo !\n\n");
            return;
        }

        int row = 0, col = 0, i = 0, vez = 0;
        char armazena_matriz[300], armazena_vez_jogador[2], armazena_nome[2][80];
        int tamanho_1, tamanho_2;

        while(!feof(arquivo)) //enquanto nao chegar ao final do arquivo
        {
            if(vez == 0)
                fscanf(arquivo, "%d", &gi.altura);
            if(vez == 1)
                fscanf(arquivo, "%d", &gi.largura);
            if(vez == 2)
                fscanf(arquivo, "%d", &tamanho_1);
            if(vez == 3)
                fscanf(arquivo, "%s", &armazena_nome[0]);
            if(vez == 4)
                fscanf(arquivo, "%d", &tamanho_2);
            if(vez == 5)
                fscanf(arquivo, "%s", &armazena_nome[1]);
            if(vez == 6)
                fscanf(arquivo, "%d", &gi.player);
            if(vez == 7)
                fscanf(arquivo, "%s", &armazena_matriz);
            if(vez > 7)
                break;
            vez++;
        }

        gi.player_name_1 = malloc(tamanho_1 * sizeof(char*)); // aloca na memoria a variavel com o tamanho da string lido no .txt
        gi.player_name_2 = malloc(tamanho_2 * sizeof(char*));

        strcpy(gi.player_name_1, armazena_nome[0]); //copia string nome do jogador para outra variavel
        strcpy(gi.player_name_2, armazena_nome[1]);

        matrix = malloc (gi.altura * sizeof(char *));// aloca na memoria a altura da matriz

        for (int i = 0; i < gi.altura; ++i)  //aloca cada coluna na quantidade de linhas na matriz
            matrix[i] = malloc (gi.largura * sizeof (char));

        armazena_matriz[strlen(armazena_matriz)] = '\0';  //seta o ultimo caracter como nulo

        for(row = 0; row < gi.altura; row++)  //armazena na variavel matriz cada peca do jogo
        {
            for(col = 0; col < gi.largura; col++)
            {
                if(armazena_matriz[i] == '2')  //caso seja '2' sera guardado no primeiro espaco da matriz como a peca infectada
                {
                    matrix[row][col] = 'O';
                    i++;
                    continue;
                }

                if(armazena_matriz[i] == '0')  // caso seja peca invisivel , salva como ' '
                {
                    matrix[row][col] = ' ';
                    i++;
                    continue;
                }

                if(armazena_matriz[i] == '1')  //caso seja peca normal define como 'O'
                {
                    matrix[row][col] = 'O';
                    i++;
                    continue;
                }
            }
        }

        gc.carregar_jogo_salvo = 1;  //define a variavel do controle do jogo salvo como '1' , para iniciar o jogo pelos dados carregados

        fclose(arquivo);  //fecha o arquivo dos dados no .txt

        return;
    } else if(op == 3) //sai do jogo
    {
        gc.continuar_jogo = 0;
        return;
    }
}

int main(void)
{
    setar_valores_padroes();  //chama funcao para setar valores padroes 

    for(;;)  //cria loop infinito
    {
        menu();

        if(gc.carregar_jogo_salvo == 0) //se usuario estiver criando novo jogo
        {
            if(gc.continuar_jogo == 0)  //caso usuario queira sair do jogo
                return 1;

            inicializa(); //chama funcao para inicializar o tabuleiro

            char nome1[10] = ""; //cria strings vazias
            char nome2[10] = "";

            int aleatorio ;   //criando variavel para usar o rand
            srand(time(NULL));

            gi.player_name_1 = malloc(80 * sizeof(char*)); //aloca o tamanho da variavel da estrutura com tamanho 80
            gi.player_name_2 = malloc(80 * sizeof(char*));

            printf("\nInsira o nome do primeiro jogador : ");
            scanf(" %s", &nome1);//le o nome dos jogadores
            printf("Insira o nome do segundo jogador : ");
            scanf(" %s", &nome2);

            strcpy(gi.player_name_1, nome1);//copia string do nome para outra string
            strcpy(gi.player_name_2, nome2);

            aleatorio = rand() % 100; //seta o aleatorio

            system("clear");

            if(aleatorio <= 50){  //seleciona o jogador aleatoriamente para comecar
                gi.player = 1;
            }else{
                gi.player = 2;
            }

            printf("Jogador sorteado foi numero : %d \n\n", gi.player);
        }

        setar_valores_padroes();  //roda a funcao de valores padroes para o tabuleiro

        do
        {
            menu_opcoes_dentro_jogo(); // chama funcao que faz perguntas para o jogador se quer continuar , salvar ,etc

            system("clear");

            if(gc.sair_sem_salvar == 1)  //se o usuario quiser sair sem salvar , fecha o programa
                return 1;
            if(gc.reiniciar_jogo == 1) //se quiser reiniciar o jogo , forca a saida do loop infinito e comeca denovo
                break;

            int position[2];   //declara variavel q recebera o valor que o usuario digita da linha/coluna do tabuleiro

            imprime_tabuleiro();

            move(position); //recebe os valores da jogada

            while(check_move(position) != 1)   //enquanto a jogada nao for valida
            {
                printf("\nTenta denovo!\n\n");
                imprime_tabuleiro();
                move(position);
            }

            update_board(position[0], position[1]);  //retira as pecas do tabuleiro

            //imprime_tabuleiro();

            if(gi.player == 2) //troca de turnos entre jogadores
                gi.player = 1;
            else gi.player = 2;
        } while (vencedor() == false);

         if(gc.reiniciar_jogo == 1)  //caso jogador queira reiniciar, volta ao inicio do loop
            continue;

        system("clear");

        printf("\n\nJogador %d (%s) foi o vencedor !\n\n", gi.player, (gi.player-1) == 0 ? gi.player_name_1 : gi.player_name_2); //declara o vencedor da partida

        for (int i = 0; i < gi.altura; ++i)//libera o espaco alocado na memoria da variavel matriz
            free(matrix[i]);

        free(matrix);

        free(gi.player_name_1);//libera o espaco alocado na memoria da variavel nome do jogador
        free(gi.player_name_2);

    }

    return 1;
}

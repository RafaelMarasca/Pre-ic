/******************************************************
 * Pré-IC - Gerador de Melodias Baseado em Ruído Rosa 
 * 
 * Autor: Rafael Marasca Martins
 * Data: 06/10/2021
 ******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <math.h>

/******************************************************
 * Estrutura node_t
 * 
 * Representa uma nota e suas propriedades
 *******************************************************/
typedef struct 
{
    int midi;      //Representa o numero midi da nota.
    int frequency; //Representa a frequência da nota.
    int figure;    //Representa a figura rítmica da nota.
    int duration;  //Representa a duração em milissegundos.
}note_t;

/************************************************************
 * Função: generate_song
 * 
 * Gera uma melodia aleatória com base nas regras definidas.
 * 
 * Parâmetros:
 * - song: vetor onde a melodia será armazenada.
 * - notes_num: número de notas da melodia.
 * - seminima: número de semínimas por segundo na melodia.
 ************************************************************/
void generate_song(note_t* song, unsigned int notes_num, unsigned int seminima, int octave);


/************************************************************
 * Função: get_frequency
 * 
 * Retorna a frequência em Hz da nota passada por parâmetro.
 * 
 * Parâmetros:
 * - note: valor midi da nota cuja frequência é desejada.
 ************************************************************/
double get_frequency(int note);


/****************************************************************
 * Função: print_song
 * 
 * Imprime uma tabela contendo a sequência das notas utilizadas
 * na melodia, as respectivas frequências, figuras rítmicas e 
 * duração em milissegundos.
 * 
 * Parâmetros:
 * - song: vetor de notas que compõem a melodia.
 * - notes_num: número de notas da melodia.
 ***************************************************************/
void print_song(note_t* song, unsigned int notes_num);


/****************************************************************
 * Função: play_song
 * 
 * Toca a melodia composta utilizando a função beep.
 * 
 * Parâmetros:
 * - song: vetor de notas que compõem a melodia.
 * - notes_num: número de notas da melodia.
 ***************************************************************/
void play_song(note_t* song, unsigned int notes_num);


/****************************************************************
 * Função: roll_dices
 * 
 * Gera as notas com base na jogada dos dados.
 * 
 * Parâmetros:
 * - sum: vetor que armazena as somas dos dados(deve estar 
 *        inicialmente preenchido com 0s).
 * - size: número máximo de notas.
 ***************************************************************/
void roll_dices(int* sum, int size);


int main()
{
    //Armazena a quantidade de seminimas por segundo.
    unsigned seminima = 0;

    //Armazena a quantidade de notas que serão usadas na melodia.
    unsigned int notes_num = 0;

    //Armazena a oitava que será utilizada na composição.
    int octave = 0;

    //Vetor que armazenam as notas utilizadas na melodia.
    note_t* song = NULL;

    printf("Seleciona a oitava:");
    scanf("%d", &octave);

    printf("Numero de notas da melodia:");
    scanf("%u", &notes_num);

    printf("Seminimas por minuto:");
    scanf("%u", &seminima);

    //Aloca memória para as notas da melodia
    song = (note_t*)malloc(sizeof(note_t)*notes_num);

    //Verifica se houve falha na alocação de memória.
    if(song == NULL)
    {
        printf("Falha de alocação de memória.");
        return -1;
    }

    generate_song(song,notes_num,seminima,octave);

    printf("Melodia Gerada:\n");

    //Imprime a tabela de notas.
    print_song(song, notes_num);

    //Toca a melodia
    play_song(song, notes_num);

    if(song != NULL)
        free(song);

    return 0;
}


//Definição da função generate_song
void generate_song(note_t* song, unsigned int notes_num, unsigned int seminima, int octave)
{
    //Vetor que armazena a duração das figuras rítmicas.
    int duration[7] = {0};

    //Variável auxiliar que representa a última nota utilizada na melodia.
    int last_note_index = 0;

    //Variável auxiliar.
    int temp = 0;

    //Vetor que armazena as somas dos valores dos dados.
    int* sum = NULL;

    sum = calloc(notes_num,sizeof(int));

    //Semente para geração de números aleatórios
    srand((unsigned)time(NULL));

    //Computa a duração de uma semínima.
    duration[4] = (60000.0)/seminima;

    //Calcula as durações das demais figuras rítmicas.
    for(int i = 0; i< 7; i++)
    {
        if(i != 4)
          duration[i] = duration[4]*pow(2,i-4);
    }

    //Gera a sequência de notas através do algoritmo de composição baseado em dados.
    roll_dices(sum,notes_num);
    
    //Completa o vetor de notas da melodia com os valores adequados.
    for(int i = 0; i<notes_num; i++)
    {
        song[i].midi = (sum[i]+(12*(octave+1)))%127;
        song[i].frequency = get_frequency(song[i].midi);
        song[i].figure = rand()%7;
        song[i].duration = duration[song[i].figure];
    }

    if(sum != NULL)
        free(sum);
}

//Definicação da função get_frequency.
double get_frequency(int note)
{
    //Calcula a frequência com base na frequência do Lá Central (69).
    double power = (note - 69)/12.0;
    return 440*pow(2,power);
}


//Definição da função roll_dices.
void roll_dices(int* sum, int size)
{
    //Armazena o número da nota em binário.
    int current_bits = 0;

    //Variável auxiliar.
    int aux = 2;

    //Armazena o número de dados.
    int dice_num = 1;

    //Vetor que armazena os valores dos dados.
    int* dice = NULL;

    //Verifica quantos dados são necessários para compor a melodia.
    while(aux<size)
    {
        aux = (aux<<1);
        dice_num++;
    }
        
    dice = (int*)calloc(dice_num,sizeof(int));

    //Imprime o cabeçalho da tabela contendo os resultados do algoritmo 
    printf("+");
    
    for(int k = 0; k<3*dice_num+4; k++)
        printf("-");

    printf("+");
    printf("\n");

    //Laço para a execução do algoritmo de lançamento de dados.
    for(int i = 0; i<size; i++)
    {
        if(i == 0)
        {
            for(int i = 0; i<dice_num; i++)
            dice[i] = (rand()%6)+1;

        }else{
            
            //Seta em 1 os bits que mudaram em relação à nota anterior.
            aux = ((current_bits-1)^(current_bits));

            //Preenche os dados referentes aos bits que mudaram com um novo valor aleatório
            //entre 1 e 6.
            for(int j = 1; aux>0 && j<=dice_num; j++)
            {
                if(aux & 1)
                    dice[dice_num-j] = (rand()%6)+1;
                
                aux>>=1;
            } 
        }

        //Soma o valor dos dados.
        for(int j = 0; j< dice_num; j++)
            sum[i]+= dice[j];

        //Imprime os dados tabela contendo os resultados do algoritmo 
        printf("|");

        for(int k = 0; k<dice_num; k++)
            printf("%d", (((1<<dice_num-1)&(i<<k))>0)?1:0);

        printf("|");

        for(int j = 0; j<dice_num; j++)
            printf("%d ", dice[j]);

        printf("|");

        printf("%2d", sum[i]);
         printf("|\n");

         current_bits++;
    }
    //Impressão do fim da tabela do algoritmo.
    printf("+");
    
    for(int k = 0; k<3*dice_num+4; k++)
        printf("-");

    printf("+");

    printf("\n");

    if(dice != NULL)
        free(dice);

}


//Definicação da função print_song.
void print_song(note_t* song, unsigned int notes_num)
{
    printf("+----+----------+------+-----------+\n");
    printf("|MIDI|FREQUENCIA|FIGURA|DURACAO(ms)|\n");
    printf("+----+----------+------+-----------+\n");
    for(int i = 0; i<notes_num; i++)
    {
        printf("| %d |   %4d   |   %d  |   %5d   |\n", song[i].midi, song[i].frequency, 
        song[i].figure, song[i].duration);
    }
    printf("+----------------------------------+\n");
}

//Definicação da função play_song.
void play_song(note_t* song, unsigned int notes_num)
{
    //Toca a frequência de cada nota por um tempo igual a sua duração.
    for(int i = 0; i < notes_num; i++)
    {
        Beep(song[i].frequency, song[i].duration);
    }
}







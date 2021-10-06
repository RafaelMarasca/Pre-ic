/**************************************************
 * Pré-IC - Gerador de Melodias dodecafônicas 
 * 
 * Autor: Rafael Marasca Martins
 * Data: 05/10/2021
 **************************************************/

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
 * - series_num: número de notas da melodia.
 * - seminima: número de semínimas por segundo na melodia.
 * - octave: oitava na qual as séries serão geradas.    
 ************************************************************/
void generate_song(note_t* song, unsigned int series_num, unsigned int seminima, int octave);


/************************************************************
 * Função: get_frequency
 * 
 * Retorna a frequência em Hz da nota passada por parâmetro.
 * 
 * Parâmetros:
 * - note: valor midi da nota cuja frequência é desejada.
 ************************************************************/
double get_frequency(int note);


/******************************************************************
 * Função: shuffle
 * 
 * Embaralha aleatóriamente os valores de um vetor.
 * 
 * Parâmetros:
 * - array: vetor de inteiros representando a série dodecafônica.
 * - size: tamanho do vetor.
 ******************************************************************/
void shuffle(int* array, unsigned int size);


/****************************************************************** 
 * Função: inverse
 * 
 * Realiza a operação de inversão na matriz dodecafônica.
 * 
 * Parâmetros:
 * - matrix_array: vetor da matriz dodecafônica de 12 inteiros.
 * - array: vetor de inteiros representando a série dodecafônica.
 * - size: tamanho dos vetores.
 ******************************************************************/
void inverse(int* matrix_array, int* array, unsigned int size);


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


int main()
{
    //Armazena a quantidade de seminimas por segundo.
    unsigned seminima = 0;

    //Armazena a quantidade de notas que serão usadas na melodia.
    unsigned int series_num = 0;

    //Armazena a oitava que será utilizada na composição.
    int octave = 0;

    //Vetor que armazenam as notas utilizadas na melodia.
    note_t* song = NULL;

    printf("Seleciona a oitava:");
    scanf("%d", &octave);

    printf("Numero de series da melodia:");
    scanf("%u", &series_num);

    printf("Seminimas por minuto:");
    scanf("%u", &seminima);

    //Aloca memória para as notas da melodia
    song = (note_t*)malloc(sizeof(note_t)*series_num*12);

    //Verifica se houve falha na alocação de memória.
    if(song == NULL)
    {
        printf("Falha de alocação de memória.");
        return -1;
    }

    generate_song(song,series_num,seminima,octave);

    printf("\nMelodia Gerada:\n");

    //Imprime a tabela de notas.
    print_song(song, series_num*12);

    //Toca a melodia
    play_song(song, series_num*12);

    if(song != NULL)
        free(song);

    return 0;
}


//Definição da função generate_song
void generate_song(note_t* song, unsigned int series_num, unsigned int seminima, int octave)
{
    int series[12] = {0,1,2,3,4,5,6,7,8,9,10,11};

    //Vetor que armazena a duração das figuras rítmicas.
    int duration[7] = {0};

    //Matriz 12x12 dodecafônica.
    int matrix [12][12] = {0};

    //Vetor auxiliar.
    int temp[12] = {0};

    //Variável auxiliar.
    int value = 0;

    //Semente para geração de números aleatórios.
    srand((unsigned)time(NULL));

    //Computa a duração de uma semínima.
    duration[4] = (60000.0)/seminima;

    //Calcula as durações das demais figuras rítmicas.
    for(int i = 0; i< 7; i++)
    {
        if(i != 4)
          duration[i] = duration[4]*pow(2,i-4);
    }

    //Embaralha o vetor series para gerar uma série dodecafônica aleatória. 
    shuffle(series,12);

    //Preenche a primeira linha da matriz com a série dodecafônica original.
    for(int i = 0; i < 12; i++)
    {
        matrix[0][i] = series[i];
    }

    //Obtém a série inversa da série original.
    inverse(temp, series, 12);

    //Completa a primeira coluna da matriz com a inversa da série original.
    for(int i = 1; i<12; i++)
        matrix[i][0] = temp[i];

    //Preenche a matriz com o restante dos valores através da transposição.
    for(int i = 1; i<12; i++)
    {
        value = matrix[i][0] - series[0];

        for(int j = 1; j<12; j++)
        {
            matrix[i][j] = (series[j] + value + 12)%12;
        }
    }
    
    printf("Matriz dodecafonica:\n");
    
    //Imprime a matriz dodecafônica
    for(int i = 0; i<12; i++)
    {
        for(int j = 0; j<12; j++)
            printf("%2d ", matrix[i][j]);

        printf("\n");
    }

    //Seleciona séries aleatórias da matriz dodecafônica gerada
    for(int i = 0; i<series_num; i++)
    {
        int aux = rand()%4;
        int aux_index = rand()%12;

        switch(aux)
        {
            case 0:
                for(int j = 0; j<12; j++)
                {
                    song[(12*i)+j].midi = matrix[aux_index][j]+(12*(octave+1));
                    song[(12*i)+j].frequency = get_frequency(song[(12*i)+j].midi);
                    song[(12*i)+j].figure = rand()%7;
                    song[(12*i)+j].duration = duration[song[(12*i)+j].figure];
                }
            break;

            case 1:
                for(int j = 11; j>=0; j--)
                {
                    song[(12*i)+(11-j)].midi = matrix[aux_index][j]+(12*(octave+1));
                    song[(12*i)+(11-j)].frequency = get_frequency(song[(12*i)+(11-j)].midi);
                    song[(12*i)+(11-j)].figure = rand()%7;
                    song[(12*i)+(11-j)].duration = duration[song[(12*i)+(11-j)].figure];
                }
            break;

            case 2:
                for(int j = 0; j<12; j++)
                {
                    song[(12*i)+j].midi = matrix[j][aux_index]+(12*(octave+1));
                    song[(12*i)+j].frequency = get_frequency(song[(12*i)+j].midi);
                    song[(12*i)+j].figure = rand()%7;
                    song[(12*i)+j].duration = duration[song[(12*i)+j].figure];
                }
            break;

            case 3:
                for(int j = 11; j>=0; j--)
                {
                    song[(12*i)+(11-j)].midi = matrix[j][aux_index]+(12*(octave+1));
                    song[(12*i)+(11-j)].frequency = get_frequency(song[(12*i)+(11-j)].midi);
                    song[(12*i)+(11-j)].figure = rand()%7;
                    song[(12*i)+(11-j)].duration = duration[song[(12*i)+(11-j)].figure];
                    
                }
            break;
        }
    }
}

//Definicação da função get_frequency.
double get_frequency(int note)
{
    //Calcula a frequência com base na frequência do Lá Central (69).
    double power = (note - 69)/12.0;
    return 440*pow(2,power);
}


//Definição da função shuffle
void shuffle(int* array, unsigned int size)
{
    int temp_value = 0;
    int temp_index = 0;

    for(unsigned int i = size-1; i>0; i--)
    {
        temp_value = array[i];
        temp_index = rand()%(i+1);
        array[i] = array[temp_index];
        array[temp_index] = temp_value; 
    }
    
    return;
}

//Definicação da função inverse
void inverse(int* inverse_array, int* array, unsigned int size)
{
    int temp = 0;

    inverse_array[0] = array[0];

    for(unsigned int i = 1; i < size; i++)
    {
        temp = array[0]-array[i];

        if(temp < 0)
        {
            temp = 12 + temp;
        }

        inverse_array[i] = (array[0]+temp)%12;
    }
}


//Definicação da função print_song.
void print_song(note_t* song, unsigned int notes_num)
{
    printf("+----+----------+------+-----------+\n");
    printf("|MIDI|FREQUENCIA|FIGURA|DURACAO(ms)|\n");
    printf("+----+----------+------+-----------+\n");
    for(int i = 0; i<notes_num; i++)
    {
        printf("| %2d |   %4d   |   %d  |   %5d   |\n", song[i].midi, song[i].frequency, 
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







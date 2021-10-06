/**************************************************
 * Pré-IC - Gerador de Melodias Baseado em Regras 
 * 
 * Autor: Rafael Marasca Martins
 * Data: 04/10/2021
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
 * - notes_num: número de notas da melodia.
 * - seminima: número de semínimas por segundo na melodia.
 ************************************************************/
void generate_song(note_t* song, unsigned int notes_num, unsigned int seminima);


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

int main()
{
    //Armazena a quantidade de seminimas por segundo.
    unsigned seminima = 0;

    //Armazena a quantidade de notas que serão usadas na melodia.
    unsigned int notes_num = 0;

    //Vetor que armazenam as notas utilizadas na melodia.
    note_t* song = NULL;

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

    generate_song(song,notes_num,seminima);

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
void generate_song(note_t* song, unsigned int notes_num, unsigned int seminima)
{
    //Vetor que armazena as notas que serão utilizadas para compor as melodias.
    const int notes[15] = {48, 50, 52, 53, 55, 57, 59, 60, 62, 64, 65, 67, 69, 71, 72};

    //Vetor que armazena os índices das possíveis notas iniciais (Dó e Sol).
    const int first_note[5] = {0,4,7,11,14};

    //Vetor que armazena a duração das figuras rítmicas.
    int duration[7] = {0};

    //Variável auxiliar que representa a última nota utilizada na melodia.
    int last_note_index = 0;

    //Variável auxiliar.
    int temp = 0;

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

    //Gera a melodia conforme as regras estabelecidas
    for(int i = 0; i<notes_num; i++)
    {
        if(i == 0 && notes_num > 1)//Inicia a melodia com um Dó ou com um Sol
        {
           last_note_index = first_note[rand()%5];

           song[i].frequency = get_frequency(notes[last_note_index]);
           song[i].midi = notes[last_note_index];
            
        }else if(notes_num == 1 || i == notes_num-1)//Termina a melodia com um Dó central 
        {
            last_note_index = 7; 
            song[i].frequency = get_frequency(notes[last_note_index]);
            song[i].midi = notes[last_note_index];

        }else if(notes[last_note_index] == 64 || notes[last_note_index] == 52 
                 || notes[last_note_index] == 59 || notes[last_note_index] == 71)
        {
            /* Verifica se a última nota foi um Mi ou um Si,caso seja, 
            define a próxima nota como Fá ou Dó.*/
            song[i].frequency = get_frequency(notes[++last_note_index]);
            song[i].midi = notes[last_note_index];

        }else if(notes[last_note_index] == 65 || notes[last_note_index] == 53)
        {
            //Verifica se a última nota foi um Fá, se foi, impede que a próxima nota seja um Si.
            do
            {
                //Soma um valor aleatório entre 4 e -4 à nota anterior
                temp = last_note_index + ((rand()%5)*pow(-1,rand()%2));
                
                //Impede que o índice ultrapasse os limites do vetor (borda não reflectante)
                while(temp <0)
                    temp++;
                while(temp>14)
                    temp--;

            }while(notes[temp] == 59 || notes[temp] == 71);//Repete enquanto a nota não for um si.

            song[i].frequency = get_frequency(notes[temp]);
            song[i].midi = notes[temp];

            last_note_index = temp;

        }else if(notes[last_note_index] == 48) 
        {
            /*Verifica se a nota anterior se encontra na borda esquerda do vetor, caso sim, 
            restringe o movimento para a direita*/
            
            //Soma um valor aleatório entre 0 e 4 à nota anterior
            last_note_index += rand()%5;

            song[i].frequency = get_frequency(notes[last_note_index]);
            song[i].midi = notes[last_note_index];
        }else if(notes[last_note_index] == 72)
        {
            /*Verifica se a nota anterior se encontra na borda direita do vetor, caso sim, 
            restringe o movimento para a esquerda*/
            
            //Soma um valor aleatório entre -4 e 0 à nota anterior
            last_note_index -= rand()%5;

            song[i].frequency = get_frequency(notes[last_note_index]);
            song[i].midi = notes[last_note_index];
        }else
        {
            //Soma um valor aleatório entre -4 e 4 à nota anterior
            last_note_index += ((rand()%5)*pow(-1,rand()%2));

            //Impede que o índice ultrapasse os limites do vetor (borda não reflectante)
            while(last_note_index<0)
                last_note_index++;
            while(last_note_index>14)
                last_note_index--;

            song[i].frequency = get_frequency(notes[last_note_index]);
            song[i].midi = notes[last_note_index];
        }
        
        //Atribui uma figura rítmica aleatória à nota.
        song[i].figure = rand()%7;

        //Atribui a duração da nota de acordo com sua figura rítmica.
        song[i].duration = duration[song[i].figure];
    }

}

//Definicação da função get_frequency.
double get_frequency(int note)
{
    //Calcula a frequência com base na frequência do Lá Central (69).
    double power = (note - 69)/12.0;
    return 440*pow(2,power);
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







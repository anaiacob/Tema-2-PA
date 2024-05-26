#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NR_ECHIPE 32
#define Q 0.15
#define INALTIME 6
struct team
{
    char* name_team;
    float points;
    int pozitie;
    int nr_victorii;
    struct team* next;
    struct team* prev;
};
typedef struct team team;

struct Node
{
    team* team1;
    //team* team2;
    struct Node* urm;
};
typedef struct Node Node;
struct Queue
{
    Node* front,*rear;
};
typedef struct Queue Queue;

struct node
{
    team* t;
    struct node* next;
};
typedef struct node node;
struct Node_aux
{
    team* t;
};
typedef struct Node_aux node_aux;
void deschidere_fisiere(char* ar1, FILE** f1, char* ar2, FILE** f2, char* ar3, FILE** f3);
char* citire_linie(FILE* f);
void omitere_spatiu(FILE* f, long poz);
void adauga_la_inceput_lista(team** head, char* nume, float scor,int i);
team* creaza_lista_echipa(FILE* intput);
Queue* createQueue();
int isEmpty(Queue* q);
void deleteQueue(Queue* q);
void enQueue(Queue* q, team* echipa1);
void deQueue(Queue* q, team** t1);
int isEmptystack(node* top);
void deleteStack(node** top);
node_aux* pop(node** top);
void push(node** top, team* echip);
node_aux* top(node* top);
double ridicare_la_putere(double n, int m);
double prestigiu_echipa(int r);
void creare_coada_runde(team* lista, int** matrice_graf, node** stiva_pierzatori,FILE* output2);
team* eliminare_pierzatori(node** stiva_pierzatori, team** lista,FILE* output2);
void cerinta1(FILE* intput, int** matrice_graf,FILE* output1,FILE* output2);
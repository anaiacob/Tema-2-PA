#include "functii.h"

//pt fiecare echipa am scorul si numele in fisierul de input
//dar pt urmatoarele operatii ne trebuie si pozitia din fisier(pe ce linie se gaseste) si cate victorii are pt prestigiu
//adaug la inceput in lista pt complexitate de O(1)
void adauga_la_inceput_lista(team** head, char* nume, float scor, int i) {
    team* newNod = (team*)malloc(sizeof(team));
    if (newNod == NULL) {
        printf("Nu s-a putut aloca memoria pentru echipa.\n");
        exit(1);
    }

    newNod->name_team = (char*)malloc((strlen(nume) + 1) * sizeof(char));
    if (newNod->name_team == NULL) {
        free(newNod);
        printf("Nu s-a putut aloca memoria pentru numele echipei.\n");
        exit(1);
    }
    strcpy(newNod->name_team, nume);
    newNod->points = scor;
    newNod->pozitie = i;
    newNod->nr_victorii=0;
    newNod->next = *head;
    newNod->prev = NULL;
    if (*head != NULL) {
        (*head)->prev = newNod;
    }

    *head = newNod;
}
//aici am creat lista propriu-zisa prin citirea din fisier a scorului si a numelui
//am setat pozitia ca fiind a cata echipa este citita si victoriile setate default la 0

team* creaza_lista_echipa(FILE* intput) {
    team* lista = NULL;
    for (int i = 0; i < NR_ECHIPE; i++) {
        float scor;
        if (fscanf(intput, "%f ", &scor) != 1) {
            printf("Eroare la citirea scorului de echipe.\n");
            exit(1);
        }
        char* nume_echipa = citire_linie(intput);
        if (!nume_echipa) {
            printf("Eroare la citirea numelui echipei %d.\n", i + 1);
            exit(1);
        }
        adauga_la_inceput_lista(&lista, nume_echipa, scor, i);
    }
    return lista;
}
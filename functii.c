#include "functii.h"
//deschide cele 3 fisiere date ca argumente in linia de comanda
void deschidere_fisiere(char* ar1, FILE** f1, char* ar2, FILE** f2, char* ar3, FILE** f3)
{
    *f1=fopen(ar1,"rt");
    if(f1==NULL)
    {
        printf("Nu s-a putut deschide.\n");
        exit(1);
    }
    *f2=fopen(ar2,"wt");
    if(f2==NULL)
    {
        printf("Nu s-a putut deschide.\n");
        exit(1);
    }
    *f3=fopen(ar3,"wt");
    if(f3==NULL)
    {
        printf("Nu s-a putut deschide.\n");
        exit(1);
    }
}
//functia va citi de pe o linie din fisier toate caracterele existente
//de la o pozitie in care a ramas pana la finalul liniei sau documentului
char* citire_linie(FILE* f) {
    long long dimensiune_linie = 100, lungime = 0;
    char* linie = (char*)malloc(dimensiune_linie * sizeof(char));
    if (!linie) {
        printf("Eroare la alocare memorie pentru linie.\n");
        exit(1);
    }
    //fiindca dimensiunea numelui poate sa difere de la o echipa la alta
    // am abstractizat putin dimensiunea si o recalculam prin dublare pana cand ajungem la sfarsit de linie sau document
    while (fgets(linie + lungime, dimensiune_linie - lungime, f)) {
        lungime = strlen(linie);
        if (linie[lungime - 1] == '\n') {
            linie[lungime - 1] = '\0';
            break;
        }
        dimensiune_linie *= 2;
        char* new_line = (char*)realloc(linie, dimensiune_linie);
        if (new_line == NULL) {
            free(linie);
            printf("Nu s-a putut realoca.\n");
            exit(1);
        }
        linie = new_line;
    }
    if (lungime == 0 && feof(f)) {
        free(linie);
        return NULL;
    }
    for (int i = lungime - 2; i >= 0; i--) {
        if (linie[i] == ' ') {
            linie[i] = '\0';
        } else break;
    }
    return linie;
}

//nu merge pow cum trebuie
double ridicare_la_putere(double n, int m)
{
    double p=1;
    while(m>0)
    {
        p=p*n;
        m--;
    }
    return p;
}
//daca puneam de tip float imi punea in fisier 0.0000
double prestigiu_echipa(int r)
{
    double a=2.00-0.15;
    double numarator=0.15*ridicare_la_putere(a,r);
    double numitor=ridicare_la_putere(2,INALTIME)+ridicare_la_putere(a,INALTIME)*(0.15-1.00);
    return numarator/numitor;
}
//aceasta functie imi returneaza o lista care nu mai contine pierzatorii
team* eliminare_pierzatori(node** stiva_pierzatori, team** lista,FILE* output2) {
    team* lista_ramas = NULL;
    team* last_ramas = NULL;
    while (!isEmptystack(*stiva_pierzatori)) {
        node_aux* pierzator_aux = pop(stiva_pierzatori);
        if (pierzator_aux == NULL) {
            printf("Eroare la obtinerea pierzatorului\n");
            return NULL;
        }
        team* pierzator = pierzator_aux->t;
        team* cap_lista = *lista;
        team* prev_lista = NULL;
        int found = 0;
        while (cap_lista != NULL) {
            if (strcmp(cap_lista->name_team, pierzator->name_team) == 0) {
                float prestigiu=prestigiu_echipa(pierzator->nr_victorii);//echipa pierzator a pierdut deci e eliminata si ii calculam prestigiul
                fprintf(output2,"%.4f ",prestigiu);
                fprintf(output2,"%s\n",pierzator->name_team);
                if (prev_lista == NULL) {//daca e la inceput echipa pierzatoare doar se muta pozitia de inceput de unde pointeaza
                    *lista = cap_lista->next;
                } else {
                    prev_lista->next = cap_lista->next;
                }
                team* de_eliminat = cap_lista;
                cap_lista = cap_lista->next;
                free(de_eliminat->name_team);
                free(de_eliminat);
                found = 1;
                break;
            } else {
                prev_lista = cap_lista;
                cap_lista = cap_lista->next;
            }
        }
        if (!found) {
            printf("Echipa pierzatoare nu a fost gasita in lista: %s\n", pierzator->name_team);
        }
        free(pierzator_aux);
    }
    //construim lista din care se sterg echipele pierzatoare
    team* cap_lista = *lista;
    while (cap_lista != NULL) {
        if (lista_ramas == NULL) {
            lista_ramas = cap_lista;
            last_ramas = lista_ramas;
        } else {
            last_ramas->next = cap_lista;
            last_ramas = last_ramas->next;
        }
        cap_lista = cap_lista->next;
    }
    if (last_ramas != NULL) {
        last_ramas->next = NULL;
    }
    return lista_ramas;
}

void cerinta1(FILE* intput, int** matrice_graf,FILE* output1,FILE* output2) {
    team* lista_echipa = creaza_lista_echipa(intput);
    //facem toate valorile din matrice 0
    for (int i = 0; i < NR_ECHIPE; i++) {
        for (int j = 0; j < NR_ECHIPE; j++) {
            matrice_graf[i][j] = 0;
        }
    }
    //practic de fiecare data se elimina jumatate din echipele existente
    for (int i = NR_ECHIPE; i > 1; i /= 2) {
        node* stiva_pierzatori = NULL;
        creare_coada_runde(lista_echipa, matrice_graf, &stiva_pierzatori,output2);
        team* lista_castigatori = eliminare_pierzatori(&stiva_pierzatori, &lista_echipa,output2);
        lista_echipa = lista_castigatori;
    }
    //adaugarea echipei castigatoare in fisier
    float prestigiu=prestigiu_echipa(lista_echipa->nr_victorii);
    fprintf(output2,"%.4f ",prestigiu);
    fprintf(output2,"%s\n",lista_echipa->name_team);
    for (int k = 0; k < NR_ECHIPE; k++) {
        for (int j = 0; j < NR_ECHIPE; j++) {
            fprintf(output1,"%d ",matrice_graf[k][j]);
        }
        fprintf(output1,"\n");
    }
    while (lista_echipa != NULL) {
        team* temp = lista_echipa;
        lista_echipa = lista_echipa->next;
        free(temp->name_team);
        free(temp);
    }
}

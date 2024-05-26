#include "functii.h"

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

char* citire_linie(FILE* f) {
    long long dimensiune_linie = 100, lungime = 0;
    char* linie = (char*)malloc(dimensiune_linie * sizeof(char));
    if (!linie) {
        printf("Eroare la alocare memorie pentru linie.\n");
        exit(1);
    }
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

Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    if (q == NULL) return NULL;
    q->front = q->rear = NULL;
    return q;
}

int isEmpty(Queue* q) {
    return (q->front == NULL);
}

void deleteQueue(Queue* q) {
    Node* aux;
    while (!isEmpty(q)) {
        aux = q->front;
        q->front = q->front->urm;
        free(aux);
    }
    free(q);
}

void enQueue(Queue* q, team* echipa1) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Eroare la alocarea memoriei pentru newNode\n");
        exit(EXIT_FAILURE);
    }
    newNode->team1 = echipa1;
    newNode->urm = NULL;
    if (q->rear == NULL) {
        q->rear = newNode;
        q->front = newNode;
    } else {
        q->rear->urm = newNode;
        q->rear = newNode;
    }
}

void deQueue(Queue* q, team** t1) {
    if (isEmpty(q)) {
        *t1 = NULL;
        return;
    }

    Node* aux = q->front;
    *t1 = aux->team1;
    q->front = q->front->urm;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(aux);
}

int isEmptystack(node* top) {
    return top == NULL;
}

void deleteStack(node** top) {
    node* temp;
    while (*top != NULL) {
        temp = *top;
        *top = (*top)->next;
        free(temp);
    }
}

node_aux* pop(node** top) {
    if (isEmptystack(*top)) {
        return NULL;
    }
    node* temp = *top;
    node_aux* aux = (node_aux*)malloc(sizeof(node_aux));
    if (aux == NULL) {
        fprintf(stderr, "Eroare la alocarea memoriei pentru node_aux\n");
        exit(EXIT_FAILURE);
    }
    aux->t = temp->t;
    *top = (*top)->next;
    free(temp);
    return aux;
}

void push(node** top, team* echip) {
    node* newNode = (node*)malloc(sizeof(node));
    if (newNode == NULL) {
        fprintf(stderr, "Eroare la alocarea memoriei pentru node\n");
        exit(EXIT_FAILURE);
    }
    newNode->t = echip;
    newNode->next = *top;
    *top = newNode;
}
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
double prestigiu_echipa(int r)
{
    double a=2.00-0.15;
    double numarator=0.15*ridicare_la_putere(a,r);
    double numitor=ridicare_la_putere(2,INALTIME)+ridicare_la_putere(a,INALTIME)*(0.15-1.00);
    return numarator/numitor;
}
void creare_coada_runde(team* lista, int** matrice_graf, node** stiva_pierzatori,FILE* output2) {
    team* cap_lista = lista;
    Queue* coada = createQueue();
    if (coada == NULL) {
        fprintf(stderr, "Eroare la crearea cozii\n");
        exit(EXIT_FAILURE);
    }
    while (cap_lista != NULL) {
        enQueue(coada, cap_lista);
        cap_lista = cap_lista->next;
    }

    while (!isEmpty(coada)) {
        team* t1, *t2;
        deQueue(coada, &t1);
        deQueue(coada, &t2);
        if (t1 != NULL && t2 != NULL) {
            if (t1->points > t2->points) {
                matrice_graf[t1->pozitie][t2->pozitie] = 0;
                t1->nr_victorii++;
                //printf("Pozitie castigator %d\n",t1->pozitie+1);
                matrice_graf[t2->pozitie][t1->pozitie] = 1;
                float prestigiu=prestigiu_echipa(t2->nr_victorii);
                /*fprintf(output2,"%.4f ",prestigiu);
                printf("ECHIPA:%s are PRESTIGIU: %.4f\n",t2->name_team,prestigiu);
                fprintf(output2,"%s\n",t2->name_team);*/
                //printf("Pozitie pierzator %d\n",t2->pozitie+1);
                push(stiva_pierzatori, t2);
            } else if(t1->points < t2->points){
                matrice_graf[t1->pozitie][t2->pozitie] = 1;
                t2->nr_victorii++;
                //printf("Pozitie castigator %d\n",t2->pozitie+1);
                matrice_graf[t2->pozitie][t1->pozitie] = 0;
                float prestigiu=prestigiu_echipa(t1->nr_victorii);
                /*fprintf(output2,"%.4f ",prestigiu);
                printf("ECHIPA:%s are PRESTIGIU: %.4f\n",t1->name_team,prestigiu);
                fprintf(output2,"%s\n",t1->name_team);*/
                //printf("Pozitie pierzator %d\n",t1->pozitie+1);
                push(stiva_pierzatori, t1);
            }
            else{
                if(strcmp(t1->name_team,t2->name_team)<0)
                {
                    matrice_graf[t1->pozitie][t2->pozitie] = 1;
                t2->nr_victorii++;
                //printf("Pozitie castigator %d\n",t2->pozitie+1);
                matrice_graf[t2->pozitie][t1->pozitie] = 0;
                float prestigiu=prestigiu_echipa(t1->nr_victorii);
                /*fprintf(output2,"%.4f ",prestigiu);
                printf("ECHIPA:%s are PRESTIGIU: %.4f\n",t1->name_team,prestigiu);
                fprintf(output2,"%s\n",t1->name_team);*/
                //printf("Pozitie pierzator %d\n",t1->pozitie+1);
                push(stiva_pierzatori, t1);
                }
                else{
                    matrice_graf[t1->pozitie][t2->pozitie] = 0;
                t1->nr_victorii++;
                //printf("Pozitie castigator %d\n",t1->pozitie+1);
                matrice_graf[t2->pozitie][t1->pozitie] = 1;
                float prestigiu=prestigiu_echipa(t2->nr_victorii);
                /*fprintf(output2,"%.4f ",prestigiu);
                printf("ECHIPA:%s are PRESTIGIU: %.4f\n",t2->name_team,prestigiu);
                fprintf(output2,"%s\n",t2->name_team);*/
                //printf("Pozitie pierzator %d\n",t2->pozitie+1);
                push(stiva_pierzatori, t2);
                }
            }
        }
    }
    deleteQueue(coada);
}

team* eliminare_pierzatori(node** stiva_pierzatori, team** lista,FILE* output2) {
    team* lista_ramas = NULL;
    team* last_ramas = NULL;
    while (!isEmptystack(*stiva_pierzatori)) {
        node_aux* pierzator_aux = pop(stiva_pierzatori);
        if (pierzator_aux == NULL) {
            printf("Eroare la obținerea pierzatorului\n");
            return NULL;
        }
        team* pierzator = pierzator_aux->t;
        team* cap_lista = *lista;
        team* prev_lista = NULL;
        int found = 0;
        while (cap_lista != NULL) {
            if (strcmp(cap_lista->name_team, pierzator->name_team) == 0) {
                float prestigiu=prestigiu_echipa(pierzator->nr_victorii);
                fprintf(output2,"%.4f ",prestigiu);
                //printf("ECHIPA:%s are PRESTIGIU: %.4f\n",pierzator->name_team,prestigiu);
                fprintf(output2,"%s\n",pierzator->name_team);
                if (prev_lista == NULL) {
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
    for (int i = 0; i < NR_ECHIPE; i++) {
        for (int j = 0; j < NR_ECHIPE; j++) {
            matrice_graf[i][j] = 0;
        }
    }
    for (int i = NR_ECHIPE; i > 1; i /= 2) {
        node* stiva_pierzatori = NULL;
        creare_coada_runde(lista_echipa, matrice_graf, &stiva_pierzatori,output2);
        team* lista_castigatori = eliminare_pierzatori(&stiva_pierzatori, &lista_echipa,output2);
        lista_echipa = lista_castigatori;
    }
    float prestigiu=prestigiu_echipa(lista_echipa->nr_victorii);
    fprintf(output2,"%.4f ",prestigiu);
    //printf("ECHIPA:%s are PRESTIGIU: %.4f\n",lista_echipa->name_team,prestigiu);
    fprintf(output2,"%s\n",lista_echipa->name_team);
    for (int k = 0; k < NR_ECHIPE; k++) {
        for (int j = 0; j < NR_ECHIPE; j++) {
            //printf("%d ", matrice_graf[k][j]);
            fprintf(output1,"%d ",matrice_graf[k][j]);

        }
        //printf("\n");
        fprintf(output1,"\n");
    }

    // Eliberare memorie lista echipe
    while (lista_echipa != NULL) {
        team* temp = lista_echipa;
        lista_echipa = lista_echipa->next;
        free(temp->name_team);
        free(temp);
    }
}

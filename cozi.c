#include "functii.h"
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

//din neatentie nu am citit ca trebuie retinute dupa in 2 cozi si am facut 2 stive
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
                matrice_graf[t1->pozitie][t2->pozitie] = 0;//echipa t1 a castigat in fata echipei t2
                t1->nr_victorii++;
                matrice_graf[t2->pozitie][t1->pozitie] = 1;
                float prestigiu=prestigiu_echipa(t2->nr_victorii);
                push(stiva_pierzatori, t2);//folosim stiva de pierzatori pentru a elimina din lista pierzatorii
            } else if(t1->points < t2->points){
                matrice_graf[t1->pozitie][t2->pozitie] = 1;
                t2->nr_victorii++;
                matrice_graf[t2->pozitie][t1->pozitie] = 0;
                float prestigiu=prestigiu_echipa(t1->nr_victorii);
                push(stiva_pierzatori, t1);
            }
            else{//se poate intampla sa aiba acelasi punctaj, asa ca le distingem prin criteriul lexicografic
                if(strcmp(t1->name_team,t2->name_team)<0)
                {
                    matrice_graf[t1->pozitie][t2->pozitie] = 1;
                t2->nr_victorii++;
                matrice_graf[t2->pozitie][t1->pozitie] = 0;
                //float prestigiu=prestigiu_echipa(t1->nr_victorii);
                push(stiva_pierzatori, t1);
                }
                else{
                    matrice_graf[t1->pozitie][t2->pozitie] = 0;
                t1->nr_victorii++;
                matrice_graf[t2->pozitie][t1->pozitie] = 1;
                //float prestigiu=prestigiu_echipa(t2->nr_victorii);
                push(stiva_pierzatori, t2);
                }
            }
        }
    }
    deleteQueue(coada);
}
#include "functii.h"

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
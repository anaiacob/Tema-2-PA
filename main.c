#include "functii.h"

int main(int argc, char* argv[])
{
    FILE* date,*output1,*output2;
    if (argc < 4) {
        fprintf(stderr, "Usage: %s file1 file2 file3\n", argv[0]);
        return 1;
    }

    deschidere_fisiere(argv[1],&date,argv[2],&output1,argv[3],&output2);
    int** matrice_graf = (int**)malloc(NR_ECHIPE * sizeof(int*));
    for (int i = 0; i < NR_ECHIPE; i++) {
        matrice_graf[i] = (int*)malloc(NR_ECHIPE * sizeof(int));
    }

    cerinta1(date, matrice_graf,output1,output2);

    for (int i = 0; i < NR_ECHIPE; i++) {
        free(matrice_graf[i]);
    }
    free(matrice_graf);

    fclose(date);
    fclose(output2);
    fclose(output1);
    return 0;
}

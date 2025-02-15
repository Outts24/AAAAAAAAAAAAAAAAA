#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_ITEMS 569
#define NUM_FEATURES 30
#define MAX_LINE_LENGTH 1024  // Tamanho máximo de uma linha do CSV
#define TRAIN_SIZE ((int)(NUM_ITEMS * 0.8))
#define TEST_SIZE (NUM_ITEMS - TRAIN_SIZE)

void load_csv(const char *filename, double dataset[NUM_ITEMS][NUM_FEATURES], int true_labels[NUM_ITEMS]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    char line[1024];
    fgets(line, sizeof(line), file);  // Ignora o cabeçalho (assumindo que existe)

    for (int i = 0; i < NUM_ITEMS; i++) {
        fgets(line, sizeof(line), file);  // Lê uma linha
        char *token = strtok(line, ",");  // Ignora o ID (primeira coluna)

        // Pega a label (segunda coluna, onde a label verdadeira está)
        token = strtok(NULL, ",");
        
        // Converte a label de 'M' ou 'B' para 0 ou 1, respectivamente
        if (token != NULL) {
            if (token[0] == 'M') {
                true_labels[i] = 0;  // 0 para Maligno
            } else if (token[0] == 'B') {
                true_labels[i] = 1;  // 1 para Benigno
            } else {
                true_labels[i] = -1;  // Caso a label seja desconhecida ou inválida
            }
        }

        // Lê as características (do ponto de vista do K-Means)
        for (int j = 0; j < NUM_FEATURES; j++) {
            token = strtok(NULL, ",");
            dataset[i][j] = atof(token);  // Converte para double e armazena nas características
        }
    }
    fclose(file);
}
void split_dataset(double dataset[][NUM_FEATURES], int labels[], 
    double train[][NUM_FEATURES], int train_labels[], 
    double test[][NUM_FEATURES], int test_labels[]) {
    for (int i = 0; i < TRAIN_SIZE; i++) {
            for (int j = 0; j < NUM_FEATURES; j++) {
                train[i][j] = dataset[i][j];
            }
        train_labels[i] = labels[i];
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        for (int j = 0; j < NUM_FEATURES; j++) {
            test[i][j] = dataset[i + (int)TRAIN_SIZE][j];
        }
        test_labels[i] = labels[i + TRAIN_SIZE];
    }
    //Print pra ver se os dados estão corretos
    /*printf("Train:\n");
    for(int i=0; i<0.2*TRAIN_SIZE; i++){
        for(int j=0; j<NUM_FEATURES; j++){
            printf("%f \t", train[i][j]); 
        }
    }
    /*printf("Train Labels:\n");
    for(int i=0; i<0.2*TRAIN_SIZE; i++){
        printf("%d \n", train_labels[i]);
    }
    printf("Test:\n");
    for(int i =0; i<TEST_SIZE; i++){
        for(int j=0; j<NUM_FEATURES; j++){
            printf("%f \t", test[i][j]);
        }
    }
    printf("Test Labels:\n");
    for(int i=0; i<TEST_SIZE; i++){
        printf("%d ", test_labels[i]);
    }*/
}
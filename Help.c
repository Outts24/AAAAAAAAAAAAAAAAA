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
}
void Precisao_compara(int *test_labels, int *predicted_clusters, int num_test) {
    int TP = 0, TN = 0, FP = 0, FN = 0;

    for(int i = 0; i < num_test; i++) {
        if(test_labels[i] == 0 && predicted_clusters[i] == 0) {
            TP++;
        } else if(test_labels[i] == 1 && predicted_clusters[i] == 1) {
            TN++;
        } else if(test_labels[i] == 0 && predicted_clusters[i] == 1) {
            FP++;
        } else if(test_labels[i] == 1 && predicted_clusters[i] == 0) {
            FN++;
        }
    }
    printf("Índice\tTrue Label\tPredicted Cluster\n");
    for (int i = 0; i < num_test; i++) {
        printf("%d\t%d\t%d\n", i, test_labels[i], predicted_clusters[i]);
    }
    printf("TP: %d\n", TP);
    printf("TN: %d\n", TN);
    printf("FP: %d\n", FP);
    printf("FN: %d\n", FN);

    float erro_previsao = (float)(FP + FN) / (TP + TN + FP + FN);
    float acurracia = (float)(TP + TN) / (TP + TN + FP + FN);
    float precisao = (float)TP / (TP + FP);
    float recall = (float)TP / (TP + FN);
    float f1 = 2 * (precisao * recall) / (precisao + recall);

    printf("Erro de previsao: %.2f %\n", erro_previsao*100);
    printf("Acuracia: %.2f %\n", acurracia*100);
    printf("Precisao: %.2f %\n", precisao*100);
    printf("Recall: %.2f %\n", recall*100);
    printf("F1: %.2f %\n", f1*100);

    return;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "kmeans.h"
#include "help.h"

#define NUM_ITEMS 569
#define NUM_FEATURES 30
#define K 2  // Número de clusters
#define TRAIN_SIZE ((int)(NUM_ITEMS * 0.8))
#define TEST_SIZE (NUM_ITEMS - TRAIN_SIZE)

void print_comparison(int *true_labels, int *predicted_clusters, int num_items) {
    printf("Índice\tTrue Label\tPredicted Cluster\n");
    for (int i = 0; i < num_items; i++) {
        printf("%d\t%d\t%d\n", i, true_labels[i], predicted_clusters[i]);
    }
}
int main() {
    double dataset[NUM_ITEMS][NUM_FEATURES];
    int true_labels[NUM_ITEMS];

    // Carregar os dados do CSV
    load_csv("breast-cancer.csv", dataset, true_labels);

    // Dividir os dados em treino e teste
    double train[TRAIN_SIZE][NUM_FEATURES], test[TEST_SIZE][NUM_FEATURES];
    int train_labels[TRAIN_SIZE], test_labels[TEST_SIZE];
    split_dataset(dataset, true_labels, train, train_labels, test, test_labels);

   // Criar ponteiros para os objetos
   Pointer objs[TRAIN_SIZE];
   for (int i = 0; i < TRAIN_SIZE; i++) {
       objs[i] = train[i];
   }
   
   Pointer test_objs[TEST_SIZE];
   for (int i = 0; i < TEST_SIZE; i++) {
       test_objs[i] = test[i];
   }
   
    // Criar array para armazenar os clusters
    int clusters[TRAIN_SIZE];
    int test_clusters[TEST_SIZE];

    // Inicializar centros dos clusters aleatoriamente
    Pointer centers[K];
    initialize_clusters(centers, objs, K, TRAIN_SIZE);

    // Configuração do K-Means
    kmeans_config config = {
        .distance_method = item_distance,
        .centroid_method = item_centroid,
        .objs = objs,
        .num_objs = TRAIN_SIZE,
        .centers = centers,
        .k = K,
        .max_iterations = KMEANS_MAX_ITERATIONS,
        .total_iterations = 0,
        .clusters = clusters,
        .test_objs = test_objs,
        .num_test = TEST_SIZE,
        .test_clusters = test_clusters,
    };

    // Executar o algoritmo K-Means
    kmeans_result result = kmeans(&config);

    // Verificar o resultado
    if (result == KMEANS_OK) {
        printf("K-Means finalizado em %d iterações.\n", config.total_iterations);
    } else {
        printf("K-Means não convergiu após %d iterações.\n", config.total_iterations);
    }

    // Verificar a precisão comparando clusters com os rótulos verdadeiros
    int correct = 0;
    for (int i = 0; i < TRAIN_SIZE; i++) {
        if (clusters[i] == train_labels[i]) correct++;
    }
    double accuracy = (double)correct / TRAIN_SIZE * 100;
    printf("Precisão: %.2f%%\n", accuracy);
    print_comparison(train_labels, clusters, TRAIN_SIZE);

    return 0;
}

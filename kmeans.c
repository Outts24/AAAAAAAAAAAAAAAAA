#include "kmeans.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>  // Para usar INFINITY
#define NUM_FEATURES 30

// Função para calcular a distância entre dois pontos (padrão Euclidiana)
double item_distance(const Pointer a, const Pointer b)
{
    double *p1 = (double *)a;
    double *p2 = (double *)b;
    double sum = 0.0;

    for (int i = 0; i < NUM_FEATURES; i++) {
        sum += (p1[i] - p2[i]) * (p1[i] - p2[i]);
    }

    return sqrt(sum);
}

// Função para calcular o centróide de um cluster
void item_centroid(const Pointer *objs, const int *clusters, size_t num_objs, int cluster, Pointer centroid) {
    double *cent = (double *)centroid;
    int count = 0;

    // Inicializar o centróide como zero
    for (int i = 0; i < NUM_FEATURES; i++) {
        cent[i] = 0.0;
    }

    // Somar os pontos que pertencem ao cluster
    for (size_t i = 0; i < num_objs; i++) {
        if (clusters[i] == cluster) {
            double *obj = (double *)objs[i];
            for (int j = 0; j < NUM_FEATURES; j++) {
                cent[j] += obj[j];
            }
            count++;
        }
    }

    // Calcular a média
    if (count > 0) {
        for (int j = 0; j < NUM_FEATURES; j++) {
            cent[j] /= count;
        }
    }
}

// Função para inicializar os centros dos clusters com pontos aleatórios
void initialize_clusters(Pointer *centers, Pointer *objs, int k, int num_objs) {
    for (int i = 0; i < k; i++) {
        int random_index = rand() % num_objs;
        centers[i] = malloc(sizeof(double) * NUM_FEATURES);  // Alocar memória para os centróides
        for (int j = 0; j < NUM_FEATURES; j++) {
            ((double *)centers[i])[j] = ((double *)objs[random_index])[j];  // Copiar os valores
        }
    }
}

// Função para atribuir os pontos aos clusters mais próximos
int assign_clusters(Pointer *objs, Pointer *centers, int *clusters, kmeans_distance_method distance_method, int num_objs, int k) {
    int changes = 0;  // Contador de mudanças nos clusters

    for (int i = 0; i < num_objs; i++) {
        double min_distance = INFINITY;
        int best_cluster = -1;

        // Verificar a qual centroide o ponto está mais próximo
        for (int j = 0; j < k; j++) {
            double dist = distance_method(objs[i], centers[j]);  // Calcula distância ao centroide j

            if (dist < min_distance) {
                min_distance = dist;
                best_cluster = j;
            }
        }

        // Verificar se o ponto mudou de cluster
        if (clusters[i] != best_cluster) {
            clusters[i] = best_cluster;
            changes++;
        }
    }

    return changes;  // Retorna quantos pontos mudaram de cluster
}

// Função para atualizar os centróides
void update_centroids(Pointer *objs, Pointer *centers, int *clusters, kmeans_centroid_method centroid_method, int num_objs, int k) {
    for (int i = 0; i < k; i++) {
        centroid_method(objs, clusters, num_objs, i, centers[i]);  // Atualiza o centróide do cluster i
    }
}

// Função principal do K-Means
kmeans_result kmeans(kmeans_config *config) {
    // Inicializar clusters
    initialize_clusters(config->centers, config->objs, config->k, config->num_objs);

    // Iterar até convergir ou atingir o número máximo de iterações
    for (config->total_iterations = 0; config->total_iterations < config->max_iterations; config->total_iterations++) {
        int changes = assign_clusters(config->objs, config->centers, config->clusters, config->distance_method, config->num_objs, config->k);
        update_centroids(config->objs, config->centers, config->clusters, config->centroid_method, config->num_objs, config->k);

        // Se não houver mudanças nos clusters, o algoritmo convergiu
        if (changes == 0) {
            return KMEANS_OK;
        }
    }

    // Se o número máximo de iterações for atingido
    return KMEANS_EXCEEDED_MAX_ITERATIONS;
}
/*void correct_clusters(int *clusters, int *train_labels, int num_objs, int k) {
    for (int i = 0; i < num_objs; i++) {
        if (clusters[i] != train_labels[i]) {
            clusters[i] = train_labels[i];
        }
    }
}

void save_centroids(Pointer *objs, Pointer *centers, int *clusters, kmeans_centroid_method centroid_method, int num_objs, int k,double centroids_array[][NUM_FEATURES]) {
    for (int i = 0; i < k; i++) {
        centroid_method(objs, clusters, num_objs, i, centers[i]);
        for (int j = 0; j < NUM_FEATURES; j++) {
            centroids_array[i][j] = ((double *)centers[i])[j];  // Salvar centróides no array
        }
    }
}

void classify_test_data(Pointer *test_objs, Pointer *centers, int *test_clusters, kmeans_distance_method distance_method, int num_test, int k) {
    for (int i = 0; i < num_test; i++) {
        double min_distance = INFINITY;
        int best_cluster = -1;

        for (int j = 0; j < k; j++) {
            double dist = distance_method(test_objs[i], centers[j]);
            if (dist < min_distance) {
                min_distance = dist;
                best_cluster = j;
            }
        }

        test_clusters[i] = best_cluster;
    }
}*/



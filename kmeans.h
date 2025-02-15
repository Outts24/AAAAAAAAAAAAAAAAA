#ifndef KMEANS_H
#define KMEANS_H

#include <stdlib.h>
#include <math.h>

#define KMEANS_NULL_CLUSTER -1
#define KMEANS_MAX_ITERATIONS 10000
#define NUM_FEATURES 30

typedef void * Pointer;

typedef enum {
    KMEANS_OK,
    KMEANS_EXCEEDED_MAX_ITERATIONS,
    KMEANS_ERROR
} kmeans_result;

typedef double (*kmeans_distance_method) (const Pointer a, const Pointer b);
typedef void (*kmeans_centroid_method) (const Pointer * objs, const int * clusters, size_t num_objs, int cluster, Pointer centroid);

typedef struct kmeans_config {
    kmeans_distance_method distance_method;
    kmeans_centroid_method centroid_method;
    Pointer * objs;
    size_t num_objs;
    Pointer * centers;
    unsigned int k;
    unsigned int max_iterations;
    unsigned int total_iterations;
    int * clusters;
    Pointer * test_objs;
    size_t num_test;
    unsigned int * test_clusters;
} kmeans_config;

kmeans_result kmeans(kmeans_config *config);

// 🔹 Adicione essas declarações para que a `main.c` as reconheça
double item_distance(const Pointer a, const Pointer b);
void item_centroid(const Pointer *objs, const int *clusters, size_t num_objs, int cluster, Pointer centroid);
void initialize_clusters(Pointer *centers, Pointer *objs, int k, int num_objs);
int assign_clusters(Pointer *objs, Pointer *centers, int *clusters, kmeans_distance_method distance_method, int num_objs, int k);
void update_centroids(Pointer *objs, Pointer *centers, int *clusters, kmeans_centroid_method centroid_method, int num_objs, int k);
/*void correct_clusters(int *clusters, int *train_labels, int num_objs, int k);
void classify_test_data(Pointer *test_objs, Pointer *centers, int *test_clusters, kmeans_distance_method distance_method, int num_test, int k);
void save_centroids(Pointer *objs, Pointer *centers, int *clusters, kmeans_centroid_method centroid_method, int num_objs, int k, double centroids_array[][NUM_FEATURES]);
*/
#endif

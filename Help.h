#ifndef DATASHEET_H
#define DATASHEET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_ITEMS 569
#define NUM_FEATURES 30

void load_csv(const char *filename, double dataset[NUM_ITEMS][NUM_FEATURES], int true_labels[NUM_ITEMS]);
void split_dataset(double dataset[][NUM_FEATURES], int labels[],double train[][NUM_FEATURES], int train_labels[],double test[][NUM_FEATURES], int test_labels[]);
void Precisao_compara(int *test_labels, int *predicted_clusters, int num_test);

#endif // DATASHEET_H
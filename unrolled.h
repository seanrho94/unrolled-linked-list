#ifndef UNROLLED_H
#define UNROLLED_H

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

struct unrolled_ll {
    int max_capacity; // Max size of Array
    int num_element;  // Current number of elements
    void **list; // Array of void pointers (The acutal array)
    pthread_mutex_t lock; // Mutex Lock
    struct unrolled_ll *next; // Pointer to next.
};

struct unrolled_ll* unrolled_ll_new(size_t n);
void unrolled_ll_append(struct unrolled_ll* list, void* element);
void* unrolled_ll_remove(struct unrolled_ll* list, size_t i);
void* unrolled_ll_get(struct unrolled_ll* list, size_t i);
void unrolled_ll_destroy(struct unrolled_ll* list);

#endif

#include "unrolled.h"

struct unrolled_ll* unrolled_ll_new(size_t n) {
    struct unrolled_ll *node = (struct unrolled_ll*)malloc(sizeof(struct unrolled_ll));
    // Number of elements (max capacity) that each node can hold.
    node->max_capacity = n;
    // Current number of elements in a node.
    node->num_element = 0;
    node->list = (void **)malloc(sizeof(void *) * node->max_capacity);
    pthread_mutex_init(&node->lock, NULL);
    node->next = NULL;
    return node;
}


void unrolled_ll_append(struct unrolled_ll* list, void* element) {    
    // Do nothing if list OR element is NULL.
    if (list == NULL || element == NULL) {
        return;
    }

    struct unrolled_ll *current_node = list;

    // Move to the end of list.
    while(current_node->next != NULL) {
        current_node = current_node->next;
    }

    // Lock the node.
    pthread_mutex_lock(&current_node->lock);

    int list_idx = 0;
    // If the array in the node is not full, add the element.
    if(current_node->num_element < current_node->max_capacity) {
        current_node->num_element++;
        list_idx = current_node->num_element - 1;
        current_node->list[list_idx] = element;
    // If the array is full (at its capacity), create a new node and add the element in there.
    }else {
        struct unrolled_ll *new_node = unrolled_ll_new(list->max_capacity);
        current_node->next = new_node;
        new_node->num_element++;
        list_idx = new_node->num_element - 1;
        new_node->list[list_idx] = element;
    }

    // Unlock the node.
    pthread_mutex_unlock(&current_node->lock); 
}

// Find the total index of given list.
int find_total_index (struct unrolled_ll* list) {
    int total_index = 0;
    struct unrolled_ll *current_node = list;

    while(current_node->next != NULL) {
        total_index += current_node->num_element;
        current_node = current_node->next;
    }

    return total_index;
}

void* unrolled_ll_remove(struct unrolled_ll* list, size_t i) {
    // If list is NULL, return NULL.
    if (list == NULL) {
        return NULL;
    }
    
    // Calculate total number of index (elements) in the list.
    int total_index = find_total_index(list);
    
    // If the value i is outside of the range of contents in the list, this function should return NULL.
    if (total_index < i) {
        return NULL;
    }
    
    struct unrolled_ll *current_node = list;
    struct unrolled_ll *prev_node;
    int list_idx = i;
    // Find the index that we are going to delete based on the given i value.
    while(1) {
        // If number of elements in the node <= list_idx, move to the next node.
        if(current_node->num_element <= list_idx) {
            list_idx -= current_node->num_element;
            prev_node = current_node;
            current_node = current_node->next;
        // Otherwise, we have found the desired index within the node.
        }else {
            //printf("index: %d\n", list_idx);
            break;
        }
    }

    // Lock the node.
    pthread_mutex_lock(&current_node->lock);

    // Keep the retrival value and delete the value in the i index.
    void *retrieval = current_node->list[list_idx]; // This value will be retunred
    current_node->list[list_idx] = NULL;
    current_node->num_element--;

    // If the node no longer contains any elements, remove it and ensure its previous node points to its next.
    if (current_node->num_element == 0) {
        prev_node->next = current_node->next;
        free(current_node->list);
        free(current_node);
    // If the node still contains any elements, re-arrange the elements to the correct indexes after removing the value in the i index.
    }else {
        void **temp_list = (void **)malloc(sizeof(void *) * current_node->max_capacity);
        int temp_list_idx = 0;
        // Copy every elements in current node to the temp list array(list of void pointers).
        // The copied elements will be placed in the order from 0 ... n (current_node->max_capacity).
        for (int i = 0; i < current_node->max_capacity; i++) {
            // Skip it as the value in this index has been deleted before.
            if (i == list_idx) {
                continue;
            }
            temp_list[temp_list_idx] = current_node->list[i];
            temp_list_idx++;
        }
        // After completion of copy, set last index of temp list to NULL.
        temp_list[current_node->max_capacity - 1] = NULL;

        // Copy temp list into current node's list.
        for (int i = 0; i < current_node->max_capacity; i++) {
            memcpy(&current_node->list[i], &temp_list[i], sizeof(void*));
        }
        // Free temp list
        free(temp_list);
    }

    // Unlock the node.
    pthread_mutex_unlock(&current_node->lock); 

    return retrieval;
}

void* unrolled_ll_get(struct unrolled_ll* list, size_t i) {
    // If list is NULL, return NULL.
    if (list == NULL) {
        return NULL;
    }
    
    // Calculate total number of index (elements) in the list.
    int total_index = find_total_index(list);
    
    // If the value i is outside of the range of contents in the list, this function should return NULL.
    if (total_index < i) {
        return NULL;
    }

    struct unrolled_ll *current_node = list;
    int list_idx = i;

    // Find the index that we are going to delete based on the given i value.
    while(1) {
        // If number of elements in the node <= list_idx, move to the next node.
        if(current_node->num_element <= list_idx) {
            list_idx -= current_node->num_element;
            current_node = current_node->next;
        // Otherwise, we have found the desired index within the node.
        }else {
            break;
        }
    }

    // Lock the node.
    pthread_mutex_lock(&current_node->lock);

    void *retrieval = current_node->list[list_idx];

    // Unlock the node.
    pthread_mutex_unlock(&current_node->lock);

    return retrieval;
}

void unrolled_ll_destroy(struct unrolled_ll* list) {
    // Do nothing if list is NULL
    if (list == NULL) {
        return;
    }

    // Lock the node.
    pthread_mutex_lock(&list->lock);

    struct unrolled_ll *current_node = list;
    struct unrolled_ll *temp_node;

    while (current_node != NULL) {
        temp_node = current_node->next;
        free(current_node->list);
        free(current_node);
        current_node = temp_node;
    }

    // Unlock the node.
    pthread_mutex_unlock(&list->lock);
}

void dump(struct unrolled_ll* list) {
    struct unrolled_ll *current_node = list;

    while(current_node != NULL) {
        for(int i = 0; i < current_node->num_element; i++) {
            printf("|%d|", current_node->list[i]);
        }
        current_node = current_node->next;
        printf("->");
    }
    printf("\n");
}

int main () {
    struct unrolled_ll* head = unrolled_ll_new(4);

    head->list[0] = (int*)1;
    head->num_element++;

    head->list[1] = (int*)2;
    head->num_element++;


    head->list[2] = (int*)3;
    head->num_element++;

    head->list[3] = (int*)4;
    head->num_element++;

    struct unrolled_ll* node = unrolled_ll_new(4);
    head->next = node;

    node->list[0] = (int*)5;
    node->num_element++;

    struct unrolled_ll* node1 = unrolled_ll_new(4);
    head->next->next = node1;

    node1->list[0] = (int*)10;
    node1->num_element++;

    node1->list[1] = (int*)11;
    node1->num_element++;

    node1->list[2] = (int*)12;
    node1->num_element++;

    unrolled_ll_append(head, (int *)13);
    unrolled_ll_append(head, (int *)14);

    void * ptr = unrolled_ll_remove(head, 4);
    //void * ptr1 = unrolled_ll_get(head, 6);

    dump(head);

    printf("%d\n", ptr);

    unrolled_ll_destroy(head);

    return 0;
}

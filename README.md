# Unrolled Linked List
Like a linked list, this data structure has nodes, however it differs from a regular linked list as each node contains an array of size N.
## Functions
```
struct unrolled_ll* unrolled_ll_new(size_t n);
```
Construct an unrolled linked list where each node can contain n elements. This should return a heap allocated unrolled linked list.

```
void unrolled_ll_append(struct unrolled_ll* list, void* element);
```
This will add an element to the end of the list, if the last node is at capacity, you will need to create a new node and add the element to the list.

```
void* unrolled_ll_remove(struct unrolled_ll* list, size_t i);
```
This must remove an element from the unrolled link list at a given i value. This i value represents the position is in your list. If the node no longer contains any elements, the node should be removed and the previous node should link to the next node.
Error case: If the value i is outside of the range of contents in the list, this function should return NULL

```
void* unrolled_ll_get(struct unrolled_ll* list, size_t i);
```
This must retrieve an element from the unrolled linked list using a given i value. This i value represents the position it is in your list. Error case: If the value i is outside of the range of contents in the list, this function should return NULL.

```
void unrolled_ll_destroy(struct unrolled_ll* list);
```
This must deallocate all elements within the data structure as well as the list itself

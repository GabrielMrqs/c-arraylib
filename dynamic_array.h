#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stddef.h>

typedef struct
{
    void *items;
    size_t count;
    size_t capacity;
    size_t element_size;
} vector;

int v_init(vector *vec, size_t element_size);
int v_add(vector *vec, void *item);
int v_set(vector *vec, void *item, size_t index);
int v_remove(vector *vec, size_t index);
int v_insert(vector *vec, void *item, size_t index);
int v_get(vector *vec, void *item, size_t index);
int v_destroy(vector *vec);

int resize_vector(vector *vec);


#endif

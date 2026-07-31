#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "dynamic_array.h"

#define INITIAL_CAPACITY 4

int v_init(vector *vec, size_t element_size)
{
    if (vec == NULL || element_size == 0)
    {
        fprintf(stderr, "Error: failed to initialize vector.\n");
        return 1;
    }

    void *items = calloc(INITIAL_CAPACITY, element_size);
    if (items == NULL)
    {
        fprintf(stderr, "Error: failed to allocate memory for vector.\n");
        return 1;
    }

    vec->items = items;
    vec->count = 0;
    vec->capacity = INITIAL_CAPACITY;
    vec->element_size = element_size;

    return 0;
}

int v_add(vector *vec, void *item)
{
    if (vec == NULL || item == NULL || resize_vector(vec) == 1)
    {
        fprintf(stderr, "Error: failed to add item to vector.\n");
        return 1;
    }

    memcpy(
        (char *)vec->items + (vec->count * vec->element_size),
        item,
        vec->element_size);

    vec->count++;

    return 0;
}

int v_set(vector *vec, void *item, size_t index)
{
    if (vec == NULL || item == NULL || index >= vec->count)
    {
        fprintf(stderr, "Error: failed to set item at index %zu.\n", index);
        return 1;
    }

    unsigned char *items = vec->items;
    size_t offset = index * vec->element_size;

    memcpy(
        items + offset,
        item,
        vec->element_size);

    return 0;
}

int v_remove(vector *vec, size_t index)
{
    if (vec == NULL || index >= vec->count)
    {
        fprintf(stderr, "Error: failed to remove item at index %zu.\n", index);
        return 1;
    }

    size_t elements_after = vec->count - index - 1;
    unsigned char *items = vec->items;

    if (elements_after > 0)
    {
        memmove(
            items + index * vec->element_size,
            items + (index + 1) * vec->element_size,
            elements_after * vec->element_size);
    }

    vec->count--;

    return 0;
}

int v_insert(vector *vec, void *item, size_t index)
{
    if (vec == NULL || item == NULL || index > vec->count)
    {
        fprintf(stderr, "Error: failed to insert item into vector.\n");
        return 1;
    }

    if (resize_vector(vec) == 1)
    {
        fprintf(stderr, "Error: failed to add item to vector.\n");
        return 1;
    }

    size_t elements_after = vec->count - index;
    unsigned char *items = vec->items;

    if (elements_after > 0)
    {
        memmove(
            items + (index + 1) * vec->element_size,
            items + index * vec->element_size,
            elements_after * vec->element_size);
    }

    memcpy(
        items + index * vec->element_size,
        item,
        vec->element_size);

    vec->count++;

    return 0;
}

int v_get(vector *vec, void *item, size_t index)
{
    if (vec == NULL || item == NULL || index >= vec->count)
    {
        fprintf(stderr, "Error: failed to get item at index %zu.\n", index);
        return 1;
    }

    unsigned char *items = vec->items;

    size_t offset = index * vec->element_size;

    memcpy(
        item,
        items + offset,
        vec->element_size);

    return 0;
}

int v_destroy(vector *vec)
{
    if (vec == NULL)
    {
        return 1;
    }

    free(vec->items);

    vec->items = NULL;
    vec->count = 0;
    vec->capacity = 0;
    vec->element_size = 0;

    return 0;
}

int resize_vector(vector *vec)
{
    if (vec == NULL || vec->capacity == 0 || vec->element_size == 0 || vec->count > vec->capacity)
    {
        fprintf(stderr, "Error: failed to resize vector.\n");
        return 1;
    }

    if (vec->count >= vec->capacity)
    {
        if (vec->capacity > SIZE_MAX / 2)
        {
            fprintf(stderr, "Error: failed to resize vector due to integer overflow.\n");
            return 1;
        }
        size_t new_capacity = vec->capacity * 2;

        if (new_capacity > SIZE_MAX / vec->element_size)
        {
            fprintf(stderr, "Error: failed to resize vector due to integer overflow.\n");
            return 1;
        }
        size_t new_size = new_capacity * vec->element_size;

        void *new_items = realloc(
            vec->items,
            new_size);

        if (new_items == NULL)
        {
            fprintf(stderr, "Error: failed to resize vector.\n");
            return 1;
        }

        vec->capacity = new_capacity;
        vec->items = new_items;
    }

    return 0;
}

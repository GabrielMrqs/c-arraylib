#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include "dynamic_array.h"

typedef struct
{
    int id;
    double score;
} record;

static void assert_int_contents(vector *vec, const int *expected, size_t count)
{
    assert(vec->count == count);

    for (size_t i = 0; i < count; i++)
    {
        int value = 0;
        assert(v_get(vec, &value, i) == 0);
        assert(value == expected[i]);
    }
}

static void test_add_realloc_and_ints(void)
{
    vector vec;
    assert(v_init(&vec, sizeof(int)) == 0);
    assert(vec.capacity == 4);

    for (int value = 1; value <= 5; value++)
        assert(v_add(&vec, &value) == 0);

    assert(vec.capacity == 8);
    {
        const int expected[] = {1, 2, 3, 4, 5};
        assert_int_contents(&vec, expected, 5);
    }
    assert(v_destroy(&vec) == 0);
}

static void test_insert_positions(void)
{
    vector vec;
    assert(v_init(&vec, sizeof(int)) == 0);

    int first = 2;
    int last = 4;
    int middle = 3;
    int at_count = 5;
    assert(v_add(&vec, &first) == 0);
    assert(v_add(&vec, &last) == 0);
    assert(v_insert(&vec, &middle, 1) == 0);
    assert(v_insert(&vec, &at_count, vec.count) == 0);

    int beginning = 1;
    assert(v_insert(&vec, &beginning, 0) == 0);

    {
        const int expected[] = {1, 2, 3, 4, 5};
        assert_int_contents(&vec, expected, 5);
    }

    int invalid = 99;
    assert(v_insert(&vec, &invalid, vec.count + 1) == 1);
    assert(vec.count == 5);
    assert(v_destroy(&vec) == 0);
}

static void test_remove_positions(void)
{
    vector vec;
    assert(v_init(&vec, sizeof(int)) == 0);

    for (int value = 1; value <= 5; value++)
        assert(v_add(&vec, &value) == 0);

    assert(v_remove(&vec, 0) == 0);
    assert(v_remove(&vec, 1) == 0);
    assert(v_remove(&vec, vec.count - 1) == 0);

    {
        const int expected[] = {2, 4};
        assert_int_contents(&vec, expected, 2);
    }
    assert(v_destroy(&vec) == 0);
}

static void test_invalid_get_set(void)
{
    vector vec;
    int value = 10;
    int output = 0;

    assert(v_init(&vec, sizeof(int)) == 0);
    assert(v_add(&vec, &value) == 0);
    assert(v_get(&vec, &output, vec.count) == 1);
    assert(v_get(&vec, &output, vec.count + 10) == 1);
    assert(v_set(&vec, &value, vec.count) == 1);
    assert(v_set(&vec, &value, vec.count + 10) == 1);
    assert(v_destroy(&vec) == 0);
}

static void test_structs(void)
{
    vector vec;
    record expected = {7, 9.5};
    record actual = {0, 0};

    assert(v_init(&vec, sizeof(record)) == 0);
    assert(v_add(&vec, &expected) == 0);
    assert(v_get(&vec, &actual, 0) == 0);
    assert(actual.id == expected.id);
    assert(actual.score == expected.score);

    actual.score = 12.25;
    assert(v_set(&vec, &actual, 0) == 0);
    actual.score = 0;
    assert(v_get(&vec, &actual, 0) == 0);
    assert(actual.score == 12.25);
    assert(v_destroy(&vec) == 0);
}

static void test_destroy_twice_and_use_after_destroy(void)
{
    vector vec;
    int value = 42;
    int output = 0;

    assert(v_init(&vec, sizeof(int)) == 0);
    assert(v_add(&vec, &value) == 0);
    assert(v_destroy(&vec) == 0);
    assert(v_destroy(&vec) == 0);

    assert(v_add(&vec, &value) == 1);
    assert(v_insert(&vec, &value, 0) == 1);
    assert(v_remove(&vec, 0) == 1);
    assert(v_get(&vec, &output, 0) == 1);
    assert(v_set(&vec, &value, 0) == 1);
}

int main(void)
{
    test_add_realloc_and_ints();
    test_insert_positions();
    test_remove_positions();
    test_invalid_get_set();
    test_structs();
    test_destroy_twice_and_use_after_destroy();

    puts("All tests passed.");
    return 0;
}

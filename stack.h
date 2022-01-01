/* date = December 16th 2021 5:23 pm */

#ifndef STACK_H
#define STACK_H
#include "memory_arena.h"

struct stack_header
{
    u64 count, capacity;
    size_t elem_size;
    struct mem_arena *allocator;
    void *data;
};

#define stack_header(st) ((struct stack_header*)(st) - 1)

void* stack_alloc(struct mem_arena *allocator, 
                  u64 capacity, 
                  size_t elem_size)
{
    struct stack_header *header;
    
    header = mem_arena_get(allocator, (capacity * elem_size) + sizeof(*header));
    header->count = 0;
    header->elem_size = elem_size;
    header->capacity = capacity;
    header->allocator = allocator;
    header->data = (char*)header + sizeof(struct stack_header);
    
    return header->data;
}

void stack_free(void *stack)
{
    struct stack_header *header = stack_header(stack);
    if (header->allocator == NULL)
    {
        free(header);
    }
}

void stack_clear(void *stack)
{
    struct stack_header *header = stack_header(stack);
    header->count = 0;
}

u64 stack_count(void *stack)
{
    struct stack_header *header = stack_header(stack);
    if (header == NULL) exit(-1);
    return header->count;
}

void* stack_maybe_grow(void *stack)
{
    struct stack_header *header = stack_header(stack);
    if (header->count == header->capacity)
    {
        size_t old_size = (header->capacity * header->elem_size) + sizeof(struct stack_header);
        header->capacity *= 2;
        size_t new_size = (header->capacity * header->elem_size) + sizeof(struct stack_header);
        if (header->allocator == NULL)
        {
            println("stack prev: %p", header);
            header = realloc(header, new_size);
            println("stack grow: %p", header);
        }
        else
        {
            struct stack_header *old_header = header;
            if (header->allocator->data_last == header)
            {
                mem_arena_putback(header->allocator, header);
                header = mem_arena_get(old_header->allocator, new_size);
            }
            else
            {
                header = mem_arena_get(header->allocator, new_size);
                memcpy(header, old_header, old_size);
            }
        }
    }
    header->data = (char*)header + sizeof(*header);
    return header->data;
}

#define stack_push(st, v) ((st) = stack_maybe_grow(st),(st)[stack_header(st)->count++] = v, (st))

#define stack_last(st) ((st)[stack_header(st)->count - 1])

#define stack_pop(st) (\
stack_header(st)->count--,\
(st)[stack_header(st)->count]\
)

#define stack_pop_index(st, i, tmp) (\
(tmp) = (st)[i],\
(st)[i] = stack_last(st),\
stack_last(st) = (tmp),\
stack_pop(st)\
)

void* stack_insert(void *stack, void *val, u64 idx)
{
    stack = stack_maybe_grow(stack);
    struct stack_header *header = stack_header(stack);
    header->count += 1;
    memmove(header->data + (idx * header->elem_size),
            header->data + ((idx - 1) * header->elem_size),
            (header->count - idx) * header->elem_size);
    memcpy(header->data + (idx * header->elem_size), 
           val, 
           header->elem_size);
    return stack;
}

void stack_zero(void *stack)
{
    struct stack_header *header = stack_header(stack);
    memset(stack, 0, header->capacity * header->elem_size);
}

void stack_test()
{
    i32 *values = NULL;
    values = stack_alloc(&global_arena, 1, sizeof(i32));
    stack_push(values, 12);
    i32 *other_values = stack_alloc(&global_arena, 3, sizeof(i32));
    values = stack_push(values, 32);
    printf("stack i32; %d\n", stack_pop(values));
    
    char *characters = NULL;
    characters = stack_alloc(NULL, 1, sizeof(char));
    stack_push(characters, 'a');
    printf("stack char; %c\n", stack_pop(characters));
    
    stack_free(&values);
    stack_free(&characters);
}
#endif //STACK_H

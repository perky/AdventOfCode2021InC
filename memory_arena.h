/* date = December 23rd 2021 0:38 am */

#ifndef MEMORY_ARENA_H
#define MEMORY_ARENA_H

#if defined(WIN32)
#define ARENA_MALLOC(capacity) VirtualAlloc(0, capacity, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE)
#define ARENA_FREE(ptr) VirtualFree(ptr, 0, MEM_RELEASE)
#else
#define ARENA_MALLOC(capacity) malloc(capacity)
#define ARENA_FREE(ptr) free(ptr)
#endif

struct mem_arena
{
    size_t capacity, used;
    void *data_start;
    void *data_last;
    void *data;
};

struct mem_arena mem_arena_alloc(size_t capacity)
{
    struct mem_arena arena;
    arena.capacity = capacity;
    arena.used = 0;
    arena.data_start = ARENA_MALLOC(capacity);
    arena.data = arena.data_start;
    return arena;
}

void mem_arena_free(struct mem_arena *arena)
{
    ARENA_FREE(arena->data_start);
}

void* mem_arena_get(struct mem_arena *arena, size_t amount)
{
    if (arena == NULL)
    {
        return malloc(amount);
    }
    if (arena->used + amount >= arena->capacity)
    {
        printf("ERROR: memory arena over capacity.\n");
        exit(0);
    }
    arena->data_last = arena->data;
    arena->data = ((char*)arena->data + amount);
    arena->used += amount;
    return arena->data_last;
}

void* mem_arena_get_zeroed(struct mem_arena *arena, size_t amount)
{
    void *data = mem_arena_get(arena, amount);
    memset(data, 0, amount);
    return data;
}

void mem_arena_putback(struct mem_arena *arena, void *start)
{
    arena->used -= (char*)arena->data - (char*)start;
    arena->data = start;
    arena->data_last = start;
}

void mem_arena_reset(struct mem_arena *arena)
{
    arena->data = arena->data_start;
    arena->used = 0;
}

#endif //MEMORY_ARENA_H

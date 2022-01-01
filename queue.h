/* date = December 22nd 2021 11:40 pm */

#ifndef QUEUE_H
#define QUEUE_H

struct queue_header
{
    u64 front, back;
    u32 capacity;
    size_t elem_size;
    struct mem_arena *allocator;
    void *data;
};

#define queue_header(q) ((struct queue_header*)(q) - 1)

void* queue_alloc(struct mem_arena *allocator, u64 capacity, size_t elem_size)
{
    struct queue_header *header = NULL;
    header = mem_arena_get(allocator, (capacity * elem_size) + sizeof(*header));
    header->capacity = capacity;
    header->front = 0;
    header->back = 0;
    header->elem_size = elem_size;
    header->allocator = allocator;
    header->data = (char*)header + sizeof(*header);
    return header->data;
}

void queue_free(void *q)
{
    struct queue_header *header = queue_header(q);
    if (header->allocator == NULL)
    {
        free(header);
    }
}

extern inline u64 queue_count(void *q);
extern inline u64 queue_increment_back(void *q);
extern inline u64 queue_increment_front(void *q);

inline u64 queue_count(void *q)
{
    struct queue_header *header = queue_header(q);
    if (header->back >= header->front)
    {
        return (header->back - header->front);
    }
    else
    {
        return (header->capacity - header->front) + header->back;
    }
}

inline u64 queue_increment_back(void *q)
{
    struct queue_header *header = queue_header(q);
    u64 old_back = header->back;
    header->back = (header->back + 1) % header->capacity;
    if (header->back == header->front)
    {
        println("ERROR: queue front == back.");
        println(U64FMT, header->front);
        exit(0);
    }
    return old_back;
}

inline u64 queue_increment_front(void *q)
{
    struct queue_header *header = queue_header(q);
    u64 old_front = header->front;
    header->front = (header->front + 1) % header->capacity;
    return old_front;
}

#define queue_push(q, v) ((q)[queue_increment_back(q)] = v, (q))
#define queue_pop(q) ((q)[queue_increment_front(q)])

#endif //QUEUE_H

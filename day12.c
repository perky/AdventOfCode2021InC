// Day 12
#include "input_helper.h"
#include "common.h"
#include "stack.h"
#include "queue.h"
#define DAY12_QUEUE_CAPACITY 150000

typedef struct cave_node* cave_node_ptr;
struct cave_node
{
    char *name;
    u64 *connected;
    bool is_small;
};

typedef struct path_node* path_node_ptr;
struct path_node
{
    cave_node_ptr cave;
    path_node_ptr prev;
    bool visited_small_cave_twice;
};

struct cave_node cave_node_alloc(char *name)
{
    struct cave_node node;
    node.name = mem_arena_get(&global_arena, 32 * sizeof(char));
    strcpy(node.name, name);
    node.connected = stack_alloc(&global_arena, 10, sizeof(u64));
    node.is_small = is_lowercase(name[0]);
    return node;
}

path_node_ptr path_node_alloc(cave_node_ptr cave, 
                              path_node_ptr prev, 
                              bool visited_small_cave_twice)
{
    path_node_ptr node = mem_arena_get(&global_arena, sizeof(struct path_node));
    node->cave = cave;
    node->prev = prev;
    node->visited_small_cave_twice = visited_small_cave_twice;
    return node;
}

// Stack helpers
static cave_node_ptr get_cave_node_ptr(struct cave_node *nodes, u64 idx)
{
    return &nodes[idx];
}

bool find_cave_node(struct cave_node *nodes, char *name, u64 *node_i)
{
    for (u64 i = 0; i < stack_count(nodes); i++)
    {
        if (strcmp(name, nodes[i].name) == 0)
        {
            *node_i = i;
            return true;
        }
    }
    return false;
}

bool read_day12_input(FILE *f, u64 *count, void *userdata)
{
    struct cave_node *cave_nodes = userdata;
    char line[32];
    i32 scan_count = fscanf(f, "%s", line);
    if (scan_count == 1)
    {
        char *left_name = strtok(line, "-");
        char *right_name = strtok(NULL, "-");
        u64 left_node_i = 0;
        u64 right_node_i = 0;
        if (!find_cave_node(cave_nodes, left_name, &left_node_i))
        {
            struct cave_node new_node = cave_node_alloc(left_name);
            cave_nodes = stack_push(cave_nodes, new_node);
            left_node_i = stack_count(cave_nodes) - 1;
        }
        if (!find_cave_node(cave_nodes, right_name, &right_node_i))
        {
            struct cave_node new_node = cave_node_alloc(right_name);
            cave_nodes = stack_push(cave_nodes, new_node);
            right_node_i = stack_count(cave_nodes) - 1;
        }
        
        cave_node_ptr left_node = get_cave_node_ptr(cave_nodes, left_node_i);
        left_node->connected = stack_push(left_node->connected, right_node_i);
        cave_node_ptr right_node = get_cave_node_ptr(cave_nodes, right_node_i);
        right_node->connected = stack_push(right_node->connected, left_node_i);
        
        return true;
    }
    return false;
}

void day12()
{
    u64 input_count = 0;
    struct cave_node *cave_nodes = stack_alloc(&global_arena, 20, sizeof(struct cave_node));
    cave_nodes = stack_push(cave_nodes, cave_node_alloc("start"));
    cave_nodes = stack_push(cave_nodes, cave_node_alloc("end"));
    
    bool did_read = read_puzzle_input("input_day12.txt",
                                      &input_count,
                                      cave_nodes,
                                      read_day12_input);
    
    if (did_read)
    {
        println("Succesfully built cave graph.");
        cave_node_ptr start_node_ptr = get_cave_node_ptr(cave_nodes, 0);
        cave_node_ptr end_node_ptr = get_cave_node_ptr(cave_nodes, 1);
        path_node_ptr start_path = path_node_alloc(start_node_ptr, NULL, false);
        path_node_ptr *frontier = queue_alloc(&global_arena, DAY12_QUEUE_CAPACITY, sizeof(path_node_ptr));
        queue_push(frontier, start_path);
        u32 paths_count = 0;
        
        while(queue_count(frontier) > 0)
        {
            // Dequeue the next cave from our frontier.
            path_node_ptr path = queue_pop(frontier);
            
            if (path->cave == end_node_ptr) 
            {
                paths_count += 1;
                continue;
            }
            
            // For each connected cave.
            u64 *connected_caves = path->cave->connected;
            for (u64 i = 0; i < stack_count(connected_caves); i++)
            {
                // Get connected cave.
                u64 connected_cave_i = connected_caves[i];
                cave_node_ptr connected_cave = get_cave_node_ptr(cave_nodes, connected_cave_i);
                
                // Ignore start cave.
                if (connected_cave == start_node_ptr) continue;
                
                // Check if cave already visited in along current path tree branch.
                u8 visit_count = 0;
                if (connected_cave->is_small)
                {
                    path_node_ptr prev = path->prev;
                    while (prev != NULL)
                    {
                        if (prev->cave->is_small && prev->cave == connected_cave)
                        {
                            visit_count += 1;
                            break;
                        }
                        prev = prev->prev;
                    }
                }
                
                if (   (!connected_cave->is_small)
                    || (path->visited_small_cave_twice && visit_count == 0)
                    || (!path->visited_small_cave_twice && visit_count < 2))
                {
                    // Add cave to frontier.
                    bool is_twice = path->visited_small_cave_twice || (visit_count > 0);
                    path_node_ptr next = path_node_alloc(connected_cave, path, is_twice);
                    queue_push(frontier, next);
                }
            }
        }
        
        println("There are %u paths.", paths_count);
        assert_equal_i64(paths_count, 93686, "Day 12");
    }
}
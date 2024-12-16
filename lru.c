#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "stack.h"

typedef struct Page {
    int data;
    int pos;
    short refbit;
} Page;

typedef struct Cache {
    Page *cache;
    int available_blocks;
    int size;
} Cache;


void push(Stack *stack, void *data) {
    Node *n = malloc(sizeof(Node));
    n->data = data;
    n->prev = stack->top;
    stack->top = n;
}

int find_in_cache(int page, Page *cache, int cache_size)
{
    for (int i = 0; i < cache_size; i++) {
        if (page == cache[i].data) {
            return i;
        }
    }
    return -1;
}

bool is_cache_full(Cache *cache)
{
    return cache->available_blocks == 0;
}

void reset_cache_refs(Cache *cache)
{
    for (int i = 0; i < cache->size; i++) {
        cache->cache[i].refbit = 0;
    }
}

void display_cache_contents(Cache *cache, int current)
{
    printf("%d: ", current);
    for (int i = 0; i < cache->size; i++) {
        printf("%d ", cache->cache[i].data);
    }
    printf("\n");
}

int main(int argc, char const *argv[])
{
    int pages[] = {1, 2, 1, 9, 3, 9, 4, 2, 4};
    int num_pages = sizeof(pages) / sizeof(int);

    int n = 3;

    Cache cache = {
        .cache = malloc(n * sizeof(Page)),
        .available_blocks = n,
        .size = n
    };

    Stack pages_stack = {
        .top = NULL
    };

    for (int i = 0; i < num_pages; i++) {
        Page *p = malloc(sizeof(Page));
        p->data = pages[i];

        int found_idx = find_in_cache(p->data, cache.cache, n);
        if (found_idx < 0) {
            // cache initially isn't full, so just linearly insert
            if (!is_cache_full(&cache)) {
                int evicted = cache.size - cache.available_blocks;
                p->pos = evicted;
                cache.cache[evicted] = *p;
                cache.available_blocks -= 1;
            } else {
                Node *currentNode = pages_stack.top;
                Page *last_used_page = (Page *) currentNode->data;
                int unique = 0;
                while (unique < cache.size) {
                    if (cache.cache[last_used_page->pos].refbit == 0) {
                        cache.cache[last_used_page->pos].refbit = 1;
                        unique++;

                        if (unique >= cache.size)
                            break;
                    }
                    currentNode = currentNode->prev;
                    last_used_page = (Page *) currentNode->data;
                }
                // We now have the victim page
                p->pos = last_used_page->pos;
                cache.cache[last_used_page->pos] = *p;

                // Reset the refbits
                reset_cache_refs(&cache);
            }
        } else {
            p->pos = found_idx;
        }    
        push(&pages_stack, p);
        display_cache_contents(&cache, pages[i]);
    }

    // TODO free all mallocs
    free(cache.cache);

    return 0;
}

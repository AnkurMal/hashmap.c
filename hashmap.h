// Copyright 2020 Joshua J Baker. All rights reserved.
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file.

#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif  // __cplusplus

#define pair(map_type, key, val) &(map_type){key, val}
#define key(map_type, key) &(map_type){key}
#define HashMap(map_type, hash_func, compare_func) hashmap_new(sizeof(map_type), 0, 0, 0, hash_func, compare_func, NULL, NULL);

#define map_generate(name, key_type, value_type)                                        \
    typedef struct name { key_type key; value_type value; } name

#define map_generate_compare(func_name, map_type, map_name1, map_name2, how_to)         \
    int func_name(const void *a, const void *b, void *udata) {                          \
        (void)udata;                                                                    \
        const map_type *map_name1 = a;                                                  \
        const map_type *map_name2 = b;                                                  \
        return how_to; } 

#define map_generate_hash(func_name, map_type, map_name, key_ptr, size_of_key)          \
    uint64_t func_name(const void *item, uint64_t seed0, uint64_t seed1) {              \
        const map_type *map_name = item;                                                \
        return hashmap_sip(key_ptr, size_of_key, seed0, seed1); }            

#define hashmap_print(map_type, fmt_key, fmt_value) do {                                \
    size_t iter = 0;                                                                    \
    void *item;                                                                         \
    printf("{ ");                                                               \
    while (hashmap_iter(map, &iter, &item)) {                                           \
        map_type *us = item;                                                            \
        printf(fmt_key": "fmt_value", ", us->key, us->value); }                         \
    printf("}\n"); } while(0);                 
    
typedef struct HashMap HashMap;

struct HashMap *hashmap_new(size_t elsize, size_t cap, uint64_t seed0, 
    uint64_t seed1, 
    uint64_t (*hash)(const void *item, uint64_t seed0, uint64_t seed1),
    int (*compare)(const void *a, const void *b, void *udata),
    void (*elfree)(void *item),
    void *udata);

struct HashMap *hashmap_new_with_allocator(void *(*malloc)(size_t), 
    void *(*realloc)(void *, size_t), void (*free)(void*), size_t elsize, 
    size_t cap, uint64_t seed0, uint64_t seed1,
    uint64_t (*hash)(const void *item, uint64_t seed0, uint64_t seed1),
    int (*compare)(const void *a, const void *b, void *udata),
    void (*elfree)(void *item),
    void *udata);

void hashmap_free(struct HashMap *map);
void hashmap_clear(struct HashMap *map, bool update_cap);
size_t hashmap_count(struct HashMap *map);
bool hashmap_oom(struct HashMap *map);
const void *hashmap_get(struct HashMap *map, const void *item);
const void *hashmap_set(struct HashMap *map, const void *item);
const void *hashmap_delete(struct HashMap *map, const void *item);
const void *hashmap_probe(struct HashMap *map, uint64_t position);
bool hashmap_scan(struct HashMap *map, bool (*iter)(const void *item, void *udata), void *udata);
bool hashmap_iter(struct HashMap *map, size_t *i, void **item);

uint64_t hashmap_sip(const void *data, size_t len, uint64_t seed0, uint64_t seed1);
uint64_t hashmap_murmur(const void *data, size_t len, uint64_t seed0, uint64_t seed1);
uint64_t hashmap_xxhash3(const void *data, size_t len, uint64_t seed0, uint64_t seed1);

const void *hashmap_get_with_hash(struct HashMap *map, const void *key, uint64_t hash);
const void *hashmap_delete_with_hash(struct HashMap *map, const void *key, uint64_t hash);
const void *hashmap_set_with_hash(struct HashMap *map, const void *item, uint64_t hash);
void hashmap_set_grow_by_power(struct HashMap *map, size_t power);
void hashmap_set_load_factor(struct HashMap *map, double load_factor);


// DEPRECATED: use `hashmap_new_with_allocator`
void hashmap_set_allocator(void *(*malloc)(size_t), void (*free)(void*));

#if defined(__cplusplus)
}
#endif  // __cplusplus

#endif  // HASHMAP_H

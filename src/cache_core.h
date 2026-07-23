#ifndef CACHE_CORE_H
#define CACHE_CORE_H

typedef struct {
    int valid;
    int lru_counter;
    unsigned long long tag;
} CacheLine;

// 全局配置
extern int set_bits;
extern int ways;
extern int offset_bits;
extern int hit_count;
extern int miss_count;
extern int evict_count;
extern int global_timer;

// 函数声明
void init_cache(void);
void free_cache(void);
void access_cache(unsigned long long address);
void process_trace_file(const char *filename);
void print_stats(void);
void print_help(void);

#endif
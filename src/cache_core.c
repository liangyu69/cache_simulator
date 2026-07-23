#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cache_core.h"

// 全局变量定义
int set_bits = 0;
int ways = 0;
int offset_bits = 0;
int hit_count = 0;
int miss_count = 0;
int evict_count = 0;
int global_timer = 0;

CacheLine **cache = NULL;
int group_count = 0;
int block_size = 0;

void init_cache() {
    group_count = 1 << set_bits;
    block_size = 1 << offset_bits;
    
    cache = (CacheLine **)malloc(group_count * sizeof(CacheLine *));
    for (int i = 0; i < group_count; i++) {
        cache[i] = (CacheLine *)malloc(ways * sizeof(CacheLine));
        for (int j = 0; j < ways; j++) {
            cache[i][j].valid = 0;
            cache[i][j].tag = 0;
            cache[i][j].lru_counter = 0;
        }
    }
}

void free_cache() {
    if (cache) {
        for (int i = 0; i < group_count; i++) {
            free(cache[i]);
        }
        free(cache);
        cache = NULL;
    }
}

void access_cache(unsigned long long address) {
    int group = (address >> offset_bits) & (group_count - 1);
    unsigned long long tag = address >> (set_bits + offset_bits);
    
    // 查找命中
    for (int i = 0; i < ways; i++) {
        if (cache[group][i].valid && cache[group][i].tag == tag) {
            hit_count++;
            cache[group][i].lru_counter = ++global_timer;
            return;
        }
    }
    
    // 缺失，找空位
    miss_count++;
    for (int i = 0; i < ways; i++) {
        if (!cache[group][i].valid) {
            cache[group][i].valid = 1;
            cache[group][i].tag = tag;
            cache[group][i].lru_counter = ++global_timer;
            return;
        }
    }
    
    // 替换
    evict_count++;
    int lru_index = 0;
    for (int i = 1; i < ways; i++) {
        if (cache[group][i].lru_counter < cache[group][lru_index].lru_counter) {
            lru_index = i;
        }
    }
    cache[group][lru_index].tag = tag;
    cache[group][lru_index].lru_counter = ++global_timer;
}

void process_trace_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("错误: 无法打开文件 '%s'\n", filename);
        return;
    }
    
    char operation;
    unsigned long long address;
    int size;
    int line_count = 0;
    
    printf("\n正在处理trace文件...\n");
    
    while (fscanf(fp, " %c %llx,%d", &operation, &address, &size) == 3) {
        line_count++;
        if (operation == 'I') continue;
        
        access_cache(address);
        if (operation == 'M') {
            access_cache(address);
        }
    }
    
    fclose(fp);
    printf("处理完成！共读取 %d 行记录。\n\n", line_count);
}

void print_stats() {
    int total = hit_count + miss_count;
    double hit_rate = total > 0 ? (double)hit_count / total * 100 : 0;
    double miss_rate = total > 0 ? (double)miss_count / total * 100 : 0;
    
    printf("========================================\n");
    printf("           缓存统计结果\n");
    printf("========================================\n");
    printf("  总访问次数: %d\n", total);
    printf("  命中次数:   %d (%.2f%%)\n", hit_count, hit_rate);
    printf("  缺失次数:   %d (%.2f%%)\n", miss_count, miss_rate);
    printf("  替换次数:   %d\n", evict_count);
    printf("========================================\n\n");
}

void print_help() {
    printf("\n缓存模拟器说明：\n");
    printf("----------------------------------------\n");
    printf("1. 缓存参数说明：\n");
    printf("   - s: 组索引位数，组数 = 2^s\n");
    printf("   - E: 每组合路数（关联度）\n");
    printf("   - b: 块偏移位数，块大小 = 2^b 字节\n\n");
    printf("2. Trace文件格式：\n");
    printf("   - 每行: 操作 地址,大小\n");
    printf("   - 操作类型: L(读), S(写), M(修改), I(忽略)\n");
    printf("   - 例如: L 0x1000,4\n\n");
    printf("3. 替换策略：\n");
    printf("   - 使用 LRU (最近最少使用) 算法\n");
    printf("----------------------------------------\n\n");
}
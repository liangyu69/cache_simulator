#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cache_core.h"

void show_banner() {
    printf("\n========================================\n");
    printf("   缓存模拟器 - Cache Simulator v1.0\n");
    printf("========================================\n\n");
}

void interactive_config() {
    printf("请输入缓存配置参数：\n");
    printf("----------------------------------------\n");
    
    printf("组索引位数 s (组数 = 2^s): ");
    scanf("%d", &set_bits);
    
    printf("每组合路数 E: ");
    scanf("%d", &ways);
    
    printf("块偏移位数 b (块大小 = 2^b): ");
    scanf("%d", &offset_bits);
    
    printf("\n配置结果：\n");
    printf("  组数: %d\n", 1 << set_bits);
    printf("  每组行数: %d\n", ways);
    printf("  块大小: %d 字节\n", 1 << offset_bits);
    printf("----------------------------------------\n\n");
}

void interactive_trace() {
    char filename[256];
    printf("请输入trace文件路径 (或输入 'quit' 退出): ");
    scanf("%s", filename);
    
    if (strcmp(filename, "quit") == 0) {
        printf("退出程序。\n");
        exit(0);
    }
    
    process_trace_file(filename);
}

int main() {
    show_banner();
    
    while (1) {
        printf("\n请选择操作：\n");
        printf("  1. 运行缓存模拟\n");
        printf("  2. 查看配置说明\n");
        printf("  3. 退出\n");
        printf("请输入选项 (1-3): ");
        
        int choice;
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                interactive_config();
                init_cache();
                interactive_trace();
                print_stats();
                free_cache();
                break;
            case 2:
                print_help();
                break;
            case 3:
                printf("再见！\n");
                return 0;
            default:
                printf("无效选项，请重新选择。\n");
        }
    }
    
    return 0;
}
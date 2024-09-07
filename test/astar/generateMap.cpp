#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 10000
#define MAX_PAIRS 20000

int map[MAX_SIZE][MAX_SIZE];

// 设置起始点和目的地的函数
void setStartAndEnd(int map[MAX_SIZE][MAX_SIZE], int *x1, int *y1, int *x2, int *y2, int m, int n) {
    // 确保起始点和目的地不在地图边界外
    *x1 = (m - 1) / 2; *y1 = (n - 1) / 2; // 起始点设置为中心点
    *x2 = *x1 + 1; *y2 = *y1 + 1; // 目的地设置为起始点的下一个点
    map[*x1][*y1] = 2;
    map[*x2][*y2] = 3;
}

void printMapToFile(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // 假定 MAX_SIZE 是地图的最大尺寸，这里直接使用
    for (int i = 0; i < MAX_SIZE; i++) {
        for (int j = 0; j < MAX_SIZE; j++) {
            // 写入地图的每个单元格的值，如果需要特定尺寸的地图，可以添加参数控制循环边界
            fprintf(file, "%d", map[i][j]);
            if (j < MAX_SIZE - 1) {
                fprintf(file, ",");  // 在同一行的值之间添加逗号分隔符
            }
        }
        fprintf(file, "\n");  // 每行结束后添加换行符
    }

    fclose(file);
}

void generateMap(int m, int n) {
    // 初始化地图
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            map[i][j] = 0;
        }
    }

    // 随机生成起始点和目的地
    int x1, y1, x2, y2;
    setStartAndEnd(map, &x1, &y1, &x2, &y2, m, n);

    // 随机生成障碍物
    for (int i = 0; i < m * n / 20; i++) { // 障碍物数量为地图大小的1/20
        int x = rand() % m;
        int y = rand() % n;
        if (map[x][y] != 2 && map[x][y] != 3) {
            map[x][y] = 1;
        }
    }
}

void generateNodes(int M, int N) {
    int pairs = 5; // 直接赋值，假设有5对起止点

    FILE *file = fopen("testNodes.txt", "w");
    if (file == NULL) {
        printf("无法打开文件以进行写入。\n");
        return;
    }

    fprintf(file, "起止点对数%d\n", pairs);

    for (int i = 0; i < pairs; i++) {
        int startX, startY, endX, endY;
        setStartAndEnd(map, &startX, &startY, &endX, &endY, M, N);

        fprintf(file, "起止点 %d: (%d, %d) -> (%d, %d)\n", i + 1, startX, startY, endX, endY);
    }

    fclose(file);
    printf("所有起止点坐标已成功写入到文件 testNodes.txt 中。\n");
}

int main() {
    int m = 1000; // 直接赋值，地图大小m
    int n = 1000; // 直接赋值，地图大小n
    int x1 = 0, y1 = 0, x2 = 200, y2 = 300; // 直接赋值，起始点和目的地坐标

    generateMap(m, n);
    printMapToFile(m, n, x1, y1, x2, y2, "input.txt");
    generateNodes(m, n);

    return 0;
}
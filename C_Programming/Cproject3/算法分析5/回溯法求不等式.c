#include <stdio.h>
#include <stdlib.h>

#define MAX_SOLUTIONS 1000  // 最多存储1000个解
#define N 3                // 变量个数

// 解结构体：存储单个解（x1,x2,x3）
typedef struct {
    int x[3];
} Solution;

int coefficients[N] = {3, 4, 2};  // 系数
Solution solutions[MAX_SOLUTIONS];  // 存储所有解
int sol_count = 0;  // 解计数器

// 回溯函数（k:当前变量索引，curr_sum:当前和）
void backtrack(int k, int curr_sum) {
    if (k == N) {
        if (curr_sum <= 12) {
            solutions[sol_count] = *(Solution*)malloc(sizeof(Solution));
            solutions[sol_count].x[0] = solutions[sol_count - 1].x[0];
            solutions[sol_count].x[1] = solutions[sol_count - 1].x[1];
            solutions[sol_count].x[2] = solutions[sol_count - 1].x[2];
            sol_count++;
        }
        return;
    }

    // 计算当前变量可取的最大值
    int max_val = (12 - curr_sum) / coefficients[k];
    for (int v = 0; v <= max_val; v++) {
        // 更新当前变量的值
        solutions[sol_count - 1].x[k] = v;
        int new_sum = curr_sum + coefficients[k] * v;
        backtrack(k + 1, new_sum);
    }
}

int main() {
    sol_count = 0;
    backtrack(0, 0);  // 从x1开始回溯

    // 输出所有解
    printf("所有可行解（x1, x2, x3）:\n");
    for (int i = 0; i < sol_count; i++) {
        printf("(%d, %d, %d)\n",
               solutions[i].x[0],
               solutions[i].x[1],
               solutions[i].x[2]);
    }

    return 0;
}
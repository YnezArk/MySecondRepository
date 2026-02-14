#include <stdio.h>

#define N 4  // 4皇后问题

int col[N];          // col[row] = column of queen in that row
int solution_count = 0;

// 检查在 (row, col) 放置皇后是否安全
bool is_safe(int row, int column) {
    for (int i = 0; i < row; i++) {
        // 检查列冲突 或 对角线冲突
        if (col[i] == column ||
            col[i] - i == column - row ||
            col[i] + i == column + row) {
            return false;
        }
    }
    return true;
}

// 打印解
void print_solution() {
    printf("情况 %d:\n", ++solution_count);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (col[i] == j)
                printf("Q ");
            else
                printf(". ");
        }
        printf("\n");
    }
    printf("\n");
}

// 回溯函数：尝试在第 row 行放置皇后
void backtrack(int row) {
    if (row == N) {
        print_solution();
        return;
    }

    for (int j = 0; j < N; j++) {  // 尝试每一列
        if (is_safe(row, j)) {
            col[row] = j;
            backtrack(row + 1);
        }
    }
}

int main() {
    solution_count = 0;
    backtrack(0);

    if (solution_count == 0) {
        printf("情况不存在\n");
    } else {
        printf("总情况数量: %d\n", solution_count);
    }

    return 0;
}
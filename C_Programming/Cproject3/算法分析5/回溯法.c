#include <stdio.h>

#define MAXN 30

int n, W;
int weight[MAXN], value[MAXN];

int best_value = 0;
int current_weight = 0;
int current_value = 0;

int current_choice[MAXN];   // 当前选择路径（0/1）
int best_choice[MAXN];      // 最优解向量

void backtrack(int i) {
    if (i == n) {
        if (current_value > best_value && current_weight <= W) {
            best_value = current_value;
            for (int j = 0; j < n; j++) {
                best_choice[j] = current_choice[j];
            }
        }
        return;
    }

    // 剪枝：如果已经超重，不再继续
    if (current_weight > W) {
        return;
    }

    // 尝试放入第 i 个物品
    current_choice[i] = 1;
    current_weight += weight[i];
    current_value += value[i];
    backtrack(i + 1);
    current_weight -= weight[i];
    current_value -= value[i];

    // 不放入第 i 个物品
    current_choice[i] = 0;
    backtrack(i + 1);
}

int main() {
    printf("请输入物品数量 背包容量\n");
    scanf("%d %d", &n, &W);
    printf("请输入物品重量\n");
    // 读入重量
    for (int i = 0; i < n; i++) {
        printf("第%d个:",i+1);
        scanf("%d", &weight[i]);
    }
    // 读入价值
    printf("请输入物品价值\n");
    for (int i = 0; i < n; i++) {
        printf("第%d个:", i + 1);
        scanf("%d", &value[i]);
    }
    best_value = 0;
    current_weight = 0;
    current_value = 0;

    backtrack(0);

    // 输出最大价值
    printf("最大价值为：%d\n", best_value);
    printf("解向量为：");
    // 输出解向量
    for (int i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", best_choice[i]);
    }
    printf("\n");

    return 0;
}
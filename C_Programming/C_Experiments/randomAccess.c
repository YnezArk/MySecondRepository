#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
// 创建文件并写入数据
FILE *fp = fopen("numbers.txt", "w");
if (fp == NULL) {
printf("无法打开文件\n");
return 1;
}
for (int i = 1; i <= 10; i++) {
fprintf(fp, "%d\n", i * 10); // 写入10, 20, ..., 100
}
fclose(fp);

// 读取特定行
int line_number;
printf("请输入要读取的行号（1-10）：");
scanf("%d", &line_number);

fp = fopen("numbers.txt", "r");
if (fp == NULL) {
printf("无法打开文件\n");
return 1;
}

char buffer[256];
int current_line = 0;
int found = 0;

while (fgets(buffer, sizeof(buffer), fp) != NULL) {
current_line++;
if (current_line == line_number) {
printf("第%d行的内容是：%s", line_number, buffer);
found = 1;
break;
}
}

if (!found) {
printf("未找到第%d行的内容\n", line_number);
}

fclose(fp);
return 0;
}

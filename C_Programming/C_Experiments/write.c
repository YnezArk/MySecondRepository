#include <stdio.h>

int main() {
FILE *fp = fopen("example.txt", "w");
if (fp == NULL) {
printf("无法打开文件\n");
return 1;
}
fprintf(fp, "第一行内容\n");
fprintf(fp, "第二行内容\n");
fclose(fp);
return 0;
}

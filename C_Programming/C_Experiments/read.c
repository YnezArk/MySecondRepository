#include <stdio.h>

int main() {
FILE *fp = fopen("example.txt", "r");
if (fp == NULL) {
printf("无法打开文件\n");
return 1;
}
char buffer[256];
while (fgets(buffer, sizeof(buffer), fp) != NULL) {
printf("%s", buffer);
}
fclose(fp);
return 0;
}

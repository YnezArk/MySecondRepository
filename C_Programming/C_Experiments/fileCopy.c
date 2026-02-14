#include <stdio.h>

int main() {
FILE *src, *dest;
char ch;

src = fopen("source.txt", "r");
if (src == NULL) {
printf("无法打开源文件\n");
return 1;
}

dest = fopen("destination.txt", "w");
if (dest == NULL) {
printf("无法打开目标文件\n");
fclose(src);
return 1;
}

while ((ch = fgetc(src)) != EOF) {
fputc(ch, dest);
}

fclose(src);
fclose(dest);
printf("文件复制完成\n");
return 0;
}

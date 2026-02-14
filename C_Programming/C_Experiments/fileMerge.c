// 文件合并
#include <stdio.h>
#include <string.h>

int main() {
FILE *files[3], *merged;
char filenames[3][50] = {"file1.txt", "file2.txt", "file3.txt"};
char merged_filename[] = "merged.txt";
char ch;

merged = fopen(merged_filename, "w");
if (merged == NULL) {
printf("无法创建合并文件\n");
return 1;
}

for (int i = 0; i < 3; i++) {
files[i] = fopen(filenames[i], "r");
if (files[i] == NULL) {
printf("无法打开文件 %s\n", filenames[i]);
fclose(merged);
return 1;
}

while ((ch = fgetc(files[i])) != EOF) {
fputc(ch, merged);
}

fclose(files[i]);
}

fclose(merged);
printf("文件合并完成\n");
return 0;
}

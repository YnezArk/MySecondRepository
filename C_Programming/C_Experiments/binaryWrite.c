#include <stdio.h>
#include <string.h>

typedef struct {
int id;
char name[50];
float score;
} Student;

// 写入二进制文件
int main() {
Student s1 = {1, "张三", 85.5};
Student s2 = {2, "李四", 90.0};
FILE *fp = fopen("students.dat", "wb");
if (fp == NULL) {
printf("无法打开文件\n");
return 1;
}
fwrite(&s1, sizeof(Student), 1, fp);
fwrite(&s2, sizeof(Student), 1, fp);
fclose(fp);
return 0;
}

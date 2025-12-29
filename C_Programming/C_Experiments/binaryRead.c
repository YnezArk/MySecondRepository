#include <stdio.h>

typedef struct {
int id;
char name[50];
float score;
} Student;

int main() {
FILE *fp = fopen("students.dat", "rb");
if (fp == NULL) {
printf("无法打开文件\n");
return 1;
}
Student s;
while (fread(&s, sizeof(Student), 1, fp) == 1) {
printf("学号: %d, 姓名: %s, 成绩: %.2f\n", s.id, s.name, s.score);
}
fclose(fp);
return 0;
}

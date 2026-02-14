package PTAweek14;

import java.util.Arrays;

/**
 * 实现Comparable接口的Person类，按name和age排序
 * Author: Ynez Ark
 * Date: 2025/12/21 16:20
 */
class PersonSortable implements Comparable<PersonSortable> {
    private String name;
    private int age;

    // 有参构造函数
    public PersonSortable(String name, int age) {
        this.name = name;
        this.age = age;
    }

    // toString方法
    @Override
    public String toString() {
        return name + "-" + age;
    }

    // 实现Comparable接口的compareTo方法
    @Override
    public int compareTo(PersonSortable other) {
        // 先按name升序排序
        int nameComparison = this.name.compareTo(other.name);
        if (nameComparison != 0) {
            return nameComparison;
        }
        // 如果name相同，则按age升序排序
        return Integer.compare(this.age, other.age);
    }
}

public class NewClass {
    public static void main(String[] args) {
        java.util.Scanner scanner = new java.util.Scanner(System.in);

        // 输入n
        int n = scanner.nextInt();
        scanner.nextLine(); // 消费换行符

        // 创建PersonSortable数组
        PersonSortable[] people = new PersonSortable[n];

        // 输入n行name age，并创建n个对象
        for (int i = 0; i < n; i++) {
            String[] parts = scanner.nextLine().split(" ");
            String name = parts[0];
            int age = Integer.parseInt(parts[1]);
            people[i] = new PersonSortable(name, age);
        }

        // 对数组进行排序
        Arrays.sort(people);

        // 输出排序后的结果
        for (PersonSortable person : people) {
            System.out.println(person);
        }

        // 输出PersonSortable所实现的所有接口
        System.out.println(Arrays.toString(PersonSortable.class.getInterfaces()));

        scanner.close();
    }
}

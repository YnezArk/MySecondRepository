package ArrayListDemo;

import java.util.ArrayList;
/**
 * Author: Ynez Ark
 * Date: 2026/1/31 15:33
 */
class Student {
    private int id;
    private String name;
    private int age;
    private int score;

    public Student() {
    }

    public Student(int id, String name, int age, int score) {
        this.id = id;
        this.name = name;
        this.age = age;
        this.score = score;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }
}
public class Test {
    public static void main(String[] args) {
        ArrayList<String> list = new ArrayList<>();
        list.add("hello");
        list.add("world");
        list.add("java");
        list.add("world");
        list.add("world");
        System.out.println(list);
    }
}

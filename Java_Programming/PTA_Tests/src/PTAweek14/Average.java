package PTAweek14;

import java.util.HashSet;
import java.util.Scanner;
import java.util.Set;

/**
 * Author: Ynez Ark
 * Date: 2025/12/21 16:13
 */
public class Average {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        String input = scanner.nextLine();
        String[] heightsStr = input.split(" ");

        // 使用Set来存储不重复的高度值
        Set<Integer> uniqueHeights = new HashSet<>();

        // 将输入的高度转换为整数并添加到Set中
        for (String heightStr : heightsStr) {
            uniqueHeights.add(Integer.parseInt(heightStr));
        }

        // 计算总和
        int sum = 0;
        for (int height : uniqueHeights) {
            sum += height;
        }

        // 计算平均值并保留3位小数
        double average = (double) sum / uniqueHeights.size();
        System.out.printf("%.3f%n", average);

        scanner.close();
    }
}

package PTAweek14;

import java.util.*;

/**
 * Author: Ynez Ark
 * Date: 2025/12/21 16:18
 */
public class Dictionary {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        // 读取输入
        String line1 = scanner.nextLine().trim();
        String line2 = scanner.nextLine().trim();

        // 解析第一个集合（整数）
        String[] numsStr = line1.substring(1, line1.length() - 1).split(",");
        List<Integer> numbers = new ArrayList<>();
        for (String num : numsStr) {
            numbers.add(Integer.parseInt(num.trim()));
        }

        // 解析第二个列表（字符串）
        String[] strsStr = line2.substring(1, line2.length() - 1).split(",");
        List<String> strings = new ArrayList<>();
        for (String str : strsStr) {
            strings.add(str.trim().replace("'", ""));
        }

        // 对两个列表分别排序
        Collections.sort(numbers);
        Collections.sort(strings);

        // 组合成字典
        Map<Integer, String> dictionary = new LinkedHashMap<>();
        for (int i = 0; i < numbers.size(); i++) {
            dictionary.put(numbers.get(i), strings.get(i));
        }

        // 输出结果
        System.out.print("{");
        boolean first = true;
        for (Map.Entry<Integer, String> entry : dictionary.entrySet()) {
            if (!first) {
                System.out.print(", ");
            }
            System.out.print(entry.getKey() + ": '" + entry.getValue() + "'");
            first = false;
        }
        System.out.println("}");

        scanner.close();
    }
}

package StringDemo;

/**
 * Author: Ynez Ark
 * Date: 2026/1/29 18:02
 */
public class Test {
    public static void main(String[] args) {
        String s1 = "你好啊,TMD,NMB,MLGB";
        String[] badWords = {"TMD", "NMB", "MLGB"};
        for (String badWord : badWords) {
            if (s1.contains(badWord)) {
                s1 = s1.replace(badWord, "***");
            }
        }
        System.out.println(s1);
    }
}

import java.io.*;
import java.util.Scanner;

public class APlusB {
    public static void main(String[] args) {
        try {
            // 读取父目录下的a.txt
            File aFile = new File("../a.txt");
            Scanner aScanner = new Scanner(aFile);
            int a = aScanner.nextInt();
            aScanner.close();

            // 读取父目录下的b.txt
            File bFile = new File("../b.txt");
            Scanner bScanner = new Scanner(bFile);
            int b = bScanner.nextInt();
            bScanner.close();

            // 计算结果
            int result = a + b;

            // 在父目录创建c.txt并写入结果
            File cFile = new File("../c.txt");
            PrintWriter writer = new PrintWriter(cFile);
            writer.println(result);
            writer.close();

        } catch (FileNotFoundException e) {
            System.err.println("文件未找到: " + e.getMessage());
            e.printStackTrace();
        } catch (Exception e) {
            System.err.println("发生错误: " + e.getMessage());
            e.printStackTrace();
        }
    }
}
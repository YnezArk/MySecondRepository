package Experiment4;

import java.io.*;

/**
 * Author: Ynez Ark
 * Date: 2025/12/10 17:09
 */
public class ReadWrite {
    public static void main(String[] args) {
        String file1 = "./test1.txt";
        String file2 = "./test2.txt";
        int lineNum = 10;
        try {
            BufferedWriter bw = new BufferedWriter(new FileWriter(file1));
            for (int i = 0; i < lineNum; i++) {
                bw.write("hello world" + i);
                if (i< lineNum-1) {
                    bw.newLine();
                }
            }
            bw.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        try {
            BufferedReader br = new BufferedReader(new FileReader(file1));
            BufferedWriter bw = new BufferedWriter(new FileWriter(file2));
            String line;
            boolean firstLine = true;
            while ((line = br.readLine()) != null) {
                if (firstLine!= true) {//除了第一行外，每行之前都添加一个换行符，以保证写入的最后一行没有换行符
                    bw.newLine();
                }
                bw.write(line);
                firstLine = false;
            }
            bw.close();
            br.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

package StringDemo2;

import java.util.Scanner;

/**
 * Author: Ynez Ark
 * Date: 2026/1/29 18:07
 */
public class NumberTest {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        String str;
       while ( true) {
           str = sc.nextLine();
           if (StrCheck(str)) {
               System.out.println("输入的数字字符串是合法字符串");
               break;
           } else {
               System.out.println("输入的数字字符串不合法,请重新输入");
           }
       }
        for (int i = 0; i < str.length(); i++) {
            System.out.print(TransRoman(str.charAt(i)-'0')+" ");
        }
    }
    public static String TransRoman(int  num) {
        String[] romans = {"0","I","II","III","IV","V","VI","VII","VIII","IX","X"};
        return romans[num];
    }

    public static boolean StrCheck(String str) {
        if (str.length() > 9) {
            return false;
        }
        for (int i = 0; i < str.length(); i++) {
            char c = str.charAt(i);
            if (c < '0' || c > '9') {
                return false;
            }
        }
        return true;
    }
}


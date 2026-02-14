package ArrayListDemo2;

import java.util.ArrayList;

/**
 * Author: Ynez Ark
 * Date: 2026/2/9 10:22
 */
public class Test {
    public static ArrayList<Phone> getPhoneList(ArrayList< Phone> list) {
        ArrayList<Phone> phoneList = new ArrayList<>();
        for (Phone p : list) {
            if (p.getPrice() < 3000) {
                phoneList.add(p);
            }
        }
        return phoneList;
    }
    public static void main(String[] args) {
        Phone p1 = new Phone("IPhone", 8000);
        Phone p2 = new Phone("小米",2999);
        Phone p3 = new Phone("HUAWEI",4999);
        ArrayList<Phone> list = new ArrayList<>();
        list.add(p1);
        list.add(p2);
        list.add(p3);

        ArrayList<Phone> phoneList = getPhoneList(list);
        for (int i = 0; i < phoneList.size(); i++) {
            System.out.println(phoneList.get(i).getName()+" "+phoneList.get(i).getPrice());
        }
    }
}

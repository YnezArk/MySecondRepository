/**
 * Author: Ynez Ark
 * Date: 2026/1/20 16:12
 */

public class GameTest {
    public static void main(String[] args) {
        Role role1 = new Role("张三", 100, '男');
        Role role2 = new Role("李四", 100,'女');
        role1.showRoleInfo();
        System.out.println();
        role2.showRoleInfo();
        int count = 1;
        while (role1.getHp()>0 && role2.getHp()>0) {
            System.out.println("\n---------"+count+"---------\n");
            System.out.println(role1.getName() + ":" + role1.getHp() + "点血  "+ role2.getName() + ":" + role2.getHp() + "点血\n");
            role1.attack(role2);
            if (role2.getHp()<=0) {
                System.out.println(role1.getName()+"获胜");
                break;
            }
            role2.attack(role1);
            if (role1.getHp()<=0) {
                System.out.println(role2.getName()+"获胜");
                break;
            }
            count++;
            }
        }
}


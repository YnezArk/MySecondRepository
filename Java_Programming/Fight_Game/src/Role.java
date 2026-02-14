import java.util.Random;

/**
 * Author: Ynez Ark
 * Date: 2026/1/20 16:13
 */
public class Role {

    private String name;
    private int hp;
    private char gender;
    private final String looks;
    String[] maleLooks = {"气宇轩昂","英俊潇洒","相貌平平","五官端正","面目狰狞"};
    String[] femaleLooks = {"沉鱼落雁","清秀绝俗","闭月羞花","五官端正","亭亭玉立"};
    String[] attackDesc = {
            "%s使出普通攻击，绕到对方身后，一掌拍去",
            "%s使出铁砂掌，飞身跃起向对方攻击",
            "%s使出幻雷术,雷球向前冲出",
            "%s使出翻江倒海，一股大力向前一推",
            "%s使出火云掌,以迅雷不及掩耳之势攻击对方下盘"};
    String[] hurtDesc = {
            "结果%s后退半步，毫发无损",
            "给%s造成一处皮外伤",
            "一击命中，%s疼得弯下腰",
            "%s痛苦地闷哼一声，造成内伤",
            "%s鲜血喷出,退后好几步"};

    public Role(String name, int hp, char gender) {
        this.name = name;
        this.hp = hp;
        this.gender = gender;
        if (gender=='男'){
            this.looks = this.maleLooks[new Random().nextInt(maleLooks.length)];
        }
        else if (gender=='女'){
            this.looks = this.femaleLooks[new Random().nextInt(femaleLooks.length)];
        }
        else {
            this.looks = "未知";
        }
    }

    public String getName() {
        return this.name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getHp() {
        return hp;
    }

    public void setHp(int hp) {
        this.hp = hp;
    }
    public char getGender() {
        return gender;
    }
    public void setGender(char gender) {
        this.gender = gender;
    }
    public String getLooks(){
        return this.looks;
    }
    public void showRoleInfo() {
        System.out.println("角色名称：" + this.getName());
        System.out.println("角色血量：" + this.getHp());
        System.out.println("角色性别："+ this.gender);
        System.out.println("角色面貌：" + this.looks);

    }
    public void hurt(Role role, int damage) {
        this.hp = damage>=this.hp ?0:this.hp -damage;
        if (damage>15){
            System.out.printf(this.hurtDesc[4]+"\n", this.getName());
        }
        else if (damage>10){
            System.out.printf(this.hurtDesc[3]+"\n", this.getName());
        }
        else if (damage>5){
            System.out.printf(this.hurtDesc[2]+"\n", this.getName());
        }
        else if (damage>0){
            System.out.printf(this.hurtDesc[1]+"\n", this.getName());
        }
        else {
            System.out.printf(this.hurtDesc[0]+"\n", this.getName());
        }

    }
    public void attack(Role role) {
        Random random = new Random();
        int damage = random.nextInt(20)+1;
        System.out.printf(this.attackDesc[random.nextInt(5)]+" "+"造成"+damage+"点伤害\n", this.name);
        role.hurt(this, damage);
    }
}

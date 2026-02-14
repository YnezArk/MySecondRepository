package ArrayListDemo2;

import java.util.ArrayList;

/**
 * Author: Ynez Ark
 * Date: 2026/2/9 10:20
 */
public class Phone {
    private String name;
    private int price;

    public Phone() {
    }

    public Phone(String name, int price) {
        this.name = name;
        this.price = price;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getPrice() {
        return price;
    }

    public void setPrice(int price) {
        this.price = price;
    }

}

package PTAtest5;

/**
 * Author: Ynez Ark
 * Date: 2025/11/28 17:28
 */
import java.util.Scanner;
interface Container{
    public static final double pi=3.1415926;
    public abstract double area();
    public abstract double volume();
    static double sumofArea(Container c[]) {
        double sum = 0;
        for (Container container : c) {
            sum += container.area();
        }
        return sum;
    }

    static double sumofVolume(Container c[]) {
        double sum = 0;
        for (Container container : c) {
            sum += container.volume();
        }
        return sum;
    }
}
class Cube implements Container {
    private double side;

    public Cube(double side) {
        this.side = side;
    }

    @Override
    public double area() {
        return 6 * side * side;
    }

    @Override
    public double volume() {
        return side * side * side;
    }
}
class Cylinder implements Container {
    private double radius;
    private double height;

    public Cylinder(double radius, double height) {
        this.radius = radius;
        this.height = height;
    }

    @Override
    public double area() {
        return 2 * pi * radius * radius + 2 * pi * radius * height;
    }

    @Override
    public double volume() {
        return pi * radius * radius * height;
    }
}


public class PTAtset5 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int n = scanner.nextInt();
        Container[] containers = new Container[n];

        for (int i = 0; i < n; i++) {
            String type = scanner.next();
            if ("cube".equals(type)) {
                double side = scanner.nextDouble();
                containers[i] = new Cube(side);
            } else if ("cylinder".equals(type)) {
                double radius = scanner.nextDouble();
                double height = scanner.nextDouble();
                containers[i] = new Cylinder(radius, height);
            }
        }

        double totalArea = Container.sumofArea(containers);
        double totalVolume = Container.sumofVolume(containers);

        System.out.printf("%.2f\n", totalArea);
        System.out.printf("%.2f", totalVolume);

        scanner.close();
    }
}


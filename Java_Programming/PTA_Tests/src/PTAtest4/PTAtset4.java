package PTAtest4;
import java.util.*;

/**
 * Author: Ynez Ark
 * Date: 2025/11/28 17:16
 */
abstract class Shape {
    static final double PI = 3.14;
    public abstract double getPerimeter();
    public abstract double getArea();
}
class Rectangle extends Shape {
    private int width;
    private int length;
    public Rectangle(int width, int length) {
        this.width = width;
        this.length = length;
    }
    @Override
    public double getPerimeter() {
        return 2 * (width + length);
    }

    @Override
    public double getArea() {
        return width * length;
    }

    @Override
    public String toString() {
        return "Rectangle [width=" + width + ", length=" + length + "]";
    }
}
class Circle extends Shape {
    private int radius;

    public Circle(int radius) {
        this.radius = radius;
    }

    @Override
    public double getPerimeter() {
        return 2 * PI * radius;
    }

    @Override
    public double getArea() {
        return PI * radius * radius;
    }

    @Override
    public String toString() {
        return "Circle [radius=" + radius + "]";
    }
}

public class PTAtset4 {

    public static double sumAllArea(Shape[] shapes) {
        double sum = 0;
        for (Shape shape : shapes) {
            sum += shape.getArea();
        }
        return sum;
    }

    public static double sumAllPerimeter(Shape[] shapes) {
        double sum = 0;
        for (Shape shape : shapes) {
            sum += shape.getPerimeter();
        }
        return sum;
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int n = scanner.nextInt();
        Shape[] shapes = new Shape[n];

        for (int i = 0; i < n; i++) {
            String type = scanner.next();
            if ("rect".equals(type)) {
                int width = scanner.nextInt();
                int length = scanner.nextInt();
                shapes[i] = new Rectangle(width, length);
            } else if ("cir".equals(type)) {
                int radius = scanner.nextInt();
                shapes[i] = new Circle(radius);
            }
        }

        System.out.println(sumAllPerimeter(shapes));
        System.out.println(sumAllArea(shapes));
        System.out.println(Arrays.toString(shapes));

        for (Shape shape : shapes) {
            System.out.println(shape.getClass() + "," + shape.getClass().getSuperclass());
        }

        scanner.close();
    }
}


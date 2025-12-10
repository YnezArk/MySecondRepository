package PTAweek11;

/**
 * Author: Ynez Ark
 * Date: 2025/11/28 17:39
 */
import java.util.ArrayList;
import java.util.Scanner;
class Date {
    protected int year;
    protected int month;
    protected int day;

    public Date(int year, int month, int day) {
        this.year = year;
        this.month = month;
        this.day = day;
    }

    public int getYear() {
        return year;
    }

    public int getMonth() {
        return month;
    }

    public int getDay() {
        return day;
    }
}
class Time {
    protected int hour;
    protected int minute;
    protected int second;

    public Time(int hour, int minute, int second) {
        this.hour = hour;
        this.minute = minute;
        this.second = second;
    }

    public int getHour() {
        return hour;
    }

    public int getMinute() {
        return minute;
    }

    public int getSecond() {
        return second;
    }
}
class Schedule extends Date {
    private int ID;
    private Time time;

    public Schedule(int ID, int year, int month, int day, int hour, int minute, int second) {
        super(year, month, day);
        this.ID = ID;
        this.time = new Time(hour, minute, second);
    }

    public int getID() {
        return ID;
    }

    public boolean isEarlierThan(Schedule s2) {
        // 比较年份
        if (this.year != s2.year) {
            return this.year < s2.year;
        }
        // 比较月份
        if (this.month != s2.month) {
            return this.month < s2.month;
        }
        // 比较日期
        if (this.day != s2.day) {
            return this.day < s2.day;
        }
        // 比较小时
        if (this.time.getHour() != s2.time.getHour()) {
            return this.time.getHour() < s2.time.getHour();
        }
        // 比较分钟
        if (this.time.getMinute() != s2.time.getMinute()) {
            return this.time.getMinute() < s2.time.getMinute();
        }
        // 比较秒
        return this.time.getSecond() < s2.time.getSecond();
    }

    @Override
    public String toString() {
        return "No." + ID + ": " + year + "/" + month + "/" + day + " " +
               time.getHour() + ":" + time.getMinute() + ":" + time.getSecond();
    }
}


public class PTAweek11 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        ArrayList<Schedule> schedules = new ArrayList<>();

        while (true) {
            String line = scanner.nextLine().trim();
            if (line.equals("0")) {
                break;
            }

            String[] parts = line.split(" ");
            int ID = Integer.parseInt(parts[0]);

            // 解析日期部分
            String[] dateParts = parts[1].split("/");
            int year = Integer.parseInt(dateParts[0]);
            int month = Integer.parseInt(dateParts[1]);
            int day = Integer.parseInt(dateParts[2]);

            // 解析时间部分
            String[] timeParts = parts[2].split(":");
            int hour = Integer.parseInt(timeParts[0]);
            int minute = Integer.parseInt(timeParts[1]);
            int second = Integer.parseInt(timeParts[2]);

            Schedule schedule = new Schedule(ID, year, month, day, hour, minute, second);
            schedules.add(schedule);
        }

        if (!schedules.isEmpty()) {
            Schedule earliest = schedules.get(0);
            for (int i = 1; i < schedules.size(); i++) {
                if (schedules.get(i).isEarlierThan(earliest)) {
                    earliest = schedules.get(i);
                }
            }
            System.out.println("The urgent schedule is " + earliest);
        }

        scanner.close();
    }
}


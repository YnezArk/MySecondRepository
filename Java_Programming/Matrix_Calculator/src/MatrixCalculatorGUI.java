import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.util.StringTokenizer;

public class MatrixCalculatorGUI extends JFrame {
    private JTextArea inputAreaA;
    private JTextArea inputAreaB;
    private JTextArea outputArea;

    public MatrixCalculatorGUI() {
        setTitle("矩阵计算器（支持齐次/非齐次方程组）");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(750, 650);
        setLocationRelativeTo(null);
        setLayout(new BorderLayout());

        // === A 矩阵输入区 ===
        JPanel panelA = new JPanel(new BorderLayout());
        panelA.setBorder(BorderFactory.createTitledBorder("系数矩阵 A"));
        inputAreaA = new JTextArea(8, 40);
        inputAreaA.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 13));
        panelA.add(new JScrollPane(inputAreaA), BorderLayout.CENTER);

        // === B 输入区（用于 b 向量 或 第二个矩阵）===
        JPanel panelB = new JPanel(new BorderLayout());
        panelB.setBorder(BorderFactory.createTitledBorder("矩阵 B 或 向量 b"));
        inputAreaB = new JTextArea(8, 40);
        inputAreaB.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 13));
        panelB.add(new JScrollPane(inputAreaB), BorderLayout.CENTER);

        // === 按钮区 ===
        JPanel buttonPanel = new JPanel(new GridLayout(3, 5, 5, 5));
        String[] buttons = {
                "A + B", "A - B", "A × B", "k·A",
                "转置(A)", "行列式(A)", "逆(A)", "迹(A)", "秩(A)",
                "解 Ax=b", "齐次解 Ax=0", "示例1(唯一解)", "示例2(无穷多解)", "示例3(无解)",
                "清空"
        };
        for (String label : buttons) {
            JButton btn = new JButton(label);
            btn.addActionListener(this::handleButtonClick);
            buttonPanel.add(btn);
        }


        // === 输出区 ===
        JPanel outputPanel = new JPanel(new BorderLayout());
        outputPanel.setBorder(BorderFactory.createTitledBorder("结果"));
        outputArea = new JTextArea(12, 50);
        outputArea.setEditable(false);
        outputArea.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 13));
        outputPanel.add(new JScrollPane(outputArea), BorderLayout.CENTER);

        // === 布局 ===
        JSplitPane splitInput = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, panelA, panelB);
        splitInput.setDividerLocation(360);

        add(splitInput, BorderLayout.NORTH);
        add(buttonPanel, BorderLayout.CENTER);
        add(outputPanel, BorderLayout.SOUTH);
    }

    private Matrix parseMatrix(JTextArea area, String name) {
        String text = area.getText().trim();
        if (text.isEmpty()) {
            throw new IllegalArgumentException("请输入 " + name + " 的数据。");
        }
        String[] lines = text.split("\\n");
        int rows = lines.length;
        if (rows == 0) throw new IllegalArgumentException(name + " 数据无效。");

        StringTokenizer st0 = new StringTokenizer(lines[0].trim());
        int cols = st0.countTokens();
        if (cols == 0) throw new IllegalArgumentException(name + " 第一行为空。");

        double[][] data = new double[rows][cols];
        for (int i = 0; i < rows; i++) {
            StringTokenizer st = new StringTokenizer(lines[i].trim());
            for (int j = 0; j < cols; j++) {
                if (st.hasMoreTokens()) {
                    try {
                        data[i][j] = Double.parseDouble(st.nextToken());
                    } catch (NumberFormatException e) {
                        data[i][j] = 0.0;
                    }
                } else {
                    data[i][j] = 0.0;
                }
            }
        }
        return new Matrix(data);
    }

    private void handleButtonClick(ActionEvent e) {
        String cmd = e.getActionCommand();
        try {
            outputArea.setText("");

            switch (cmd) {
                case "A + B":
                    outputArea.setText(parseMatrix(inputAreaA, "A").add(parseMatrix(inputAreaB, "B")).toString());
                    break;
                case "A - B":
                    outputArea.setText(parseMatrix(inputAreaA, "A").subtract(parseMatrix(inputAreaB, "B")).toString());
                    break;
                case "A × B":
                    outputArea.setText(parseMatrix(inputAreaA, "A").multiply(parseMatrix(inputAreaB, "B")).toString());
                    break;
                case "k·A":
                    Matrix A_k = parseMatrix(inputAreaA, "A");
                    String kStr = JOptionPane.showInputDialog(this, "请输入标量 k:", "2.0");
                    if (kStr == null) return;
                    double k = Double.parseDouble(kStr.trim());
                    outputArea.setText(A_k.scalarMultiply(k).toString());
                    break;
                case "转置(A)":
                    outputArea.setText(parseMatrix(inputAreaA, "A").transpose().toString());
                    break;
                case "行列式(A)":
                    double det = parseMatrix(inputAreaA, "A").determinant();
                    outputArea.setText("行列式 = " + String.format("%.6f", det));
                    break;
                case "逆(A)":
                    outputArea.setText(parseMatrix(inputAreaA, "A").inverse().toString());
                    break;
                case "迹(A)":
                    double tr = parseMatrix(inputAreaA, "A").trace();
                    outputArea.setText("迹 = " + String.format("%.6f", tr));
                    break;
                case "秩(A)":
                    int r = parseMatrix(inputAreaA, "A").rank();
                    outputArea.setText("秩 = " + r);
                    break;
                case "解 Ax=b":
                    Matrix A_solve = parseMatrix(inputAreaA, "A");
                    Matrix b_solve = parseMatrix(inputAreaB, "b");
                    Matrix x = A_solve.solve(b_solve);
                    outputArea.setText("特解（或唯一解）：\n" + x.toString());
                    if (A_solve.rank() < A_solve.cols) {
                        outputArea.append("\n>>> 提示：存在自由变量，通解 = 特解 + 齐次通解。");
                    }
                    break;
                case "齐次解 Ax=0":
                    Matrix A_homo = parseMatrix(inputAreaA, "A");
                    Matrix basis = A_homo.nullSpace();
                    if (basis == null) {
                        outputArea.setText("齐次方程组只有零解。");
                    } else {
                        outputArea.setText("基础解系（每列为一个解向量）：\n" + basis.toString());
                    }
                    break;
                case "清空":
                    inputAreaA.setText("");
                    inputAreaB.setText("");
                    outputArea.setText("");
                    break;
                case "示例1(唯一解)": // 唯一解
                    inputAreaA.setText("1 2\n3 4");
                    inputAreaB.setText("5\n11");
                    outputArea.setText("已加载：唯一解示例");
                    break;
                case "示例2(无穷多解)": // 无穷多解
                    inputAreaA.setText("1 2 3\n2 4 6");
                    inputAreaB.setText("6\n12");
                    outputArea.setText("已加载：无穷多解示例");
                    break;
                case "示例3(无解)": // 无解
                    inputAreaA.setText("1 1\n1 1");
                    inputAreaB.setText("1\n2");
                    outputArea.setText("已加载：无解示例");
                    break;
                default:
                    outputArea.setText("未知操作");
            }

        } catch (Exception ex) {
            JOptionPane.showMessageDialog(this,
                    "错误: " + ex.getMessage(),
                    "计算失败",
                    JOptionPane.ERROR_MESSAGE);
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            try {
                UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
            } catch (Exception ignored) {
            }
            new MatrixCalculatorGUI().setVisible(true);
        });
    }
}
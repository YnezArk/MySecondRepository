import javax.swing.*;
import javax.swing.border.Border;
import javax.swing.border.TitledBorder;
import javax.swing.plaf.basic.BasicSplitPaneDivider;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.util.HashMap;
import java.util.Map;
import java.util.StringTokenizer;

/**
 * 矩阵计算器GUI
 * 特点：配置集中管理，按钮样式优化，代码结构清晰，流畅的按钮动画
 */
public class MatrixCalculatorGUI extends JFrame {
    // ======================== 集中式UI配置区 ========================
    // 颜色配置
    private static final Color PRIMARY_COLOR = new Color(99, 124, 253);      // 主色(大标题)
    private static final Color SECONDARY_COLOR = new Color(210, 226, 251);  // 次要色
    private static final Color SPLIT_PANE_DIVIDER_COLOR = PRIMARY_COLOR;    // 分割线颜色
    private static final Color TEXT_COLOR = new Color(30, 30, 30);           // 文本色
    private static final Color LIGHT_GRAY = new Color(170, 170, 170);        // 浅灰色
    private static final Color INPUT_PANEL_BG = new Color(210, 226, 251);    // 输入区背景
    private static final Color FOCUS_BORDER = new Color(99, 124, 253);      // 输入框焦点边框色
    private static final Color BUTTON_PANEL_BG = new Color(210, 226, 251);   // 按钮区背景
    private static final Color OUTPUT_PANEL_BG = new Color(210, 226, 251);   // 输出区背景
    private static final Color TITLE_COLOR = new Color(112, 139, 252);       // 小标题色
    private static final Color HINT_COLOR = new Color(99, 124, 253);        // 提示色
    private static final Color CLEAR_BTN_COLOR = new Color(114, 141, 253, 255); // 清空按钮
    private static final Color CLEAR_BTN_HOVER = new Color(88, 86, 214);     // 清空按钮悬停色
    private static final Color ACCENT_COLOR = new Color(99, 124, 253);      // 强调色(清空按钮的边框)
    private static final Color BUTTON_NORMAL = new Color(150, 167, 255, 255); // 按钮常规色
    private static final Color BUTTON_HOVER = new Color(99, 124, 253);      // 按钮悬停色
    private static final Color BUTTON_BORDER = new Color(99, 124, 253, 255); // 按钮边框色

    // 字体配置
    private static final Font TITLE_FONT = new Font("幼圆", Font.BOLD, 18);              // 窗口标题字体
    private static final Font PANEL_TITLE_FONT = new Font("幼圆", Font.BOLD, 13);    // 面板标题字体
    private static final Font HINT_FONT = new Font("幼圆", Font.PLAIN, 11);          // 提示文字字体
    private static final Font TEXT_AREA_FONT = new Font(Font.MONOSPACED, Font.PLAIN, 13);// 输入输出字体
    private static final Font BUTTON_FONT = new Font("微软雅黑", Font.PLAIN, 12);        // 普通按钮字体
    private static final Font CLEAR_BUTTON_FONT = new Font("微软雅黑", Font.BOLD, 13);   // 清空按钮字体

    // 尺寸配置
    private static final int FRAME_WIDTH = 950;          // 窗口宽度
    private static final int FRAME_HEIGHT = 800;         // 窗口高度
    private static final int SPLIT_PANE_DIVIDER = 450;   // 分割面板位置
    private static final int SPLIT_PANE_DIVIDER_SIZE = 3;// 分割线宽度
    private static final int TEXT_AREA_ROWS = 10;         // 输入框行数
    private static final int OUTPUT_AREA_ROWS = 10;      // 输出框行数
    private static final int BUTTON_GRID_HGAP = 6;       // 按钮网格水平间距
    private static final int BUTTON_GRID_VGAP = 6;       // 按钮网格垂直间距
    private static final int BORDER_EMPTY_PADDING = 5;   // 通用内边距
    private static final int TEXT_AREA_PADDING = 6;      // 文本框内边距
    private static final int OUTPUT_AREA_PADDING = 8;    // 输出框内边距

    // 动画配置（新增）
    private static final int ANIMATION_DURATION = 100;   // 动画总时长(ms)
    private static final int ANIMATION_INTERVAL = 10;    // 动画刷新间隔(ms)
    private static final int ANIMATION_STEPS = ANIMATION_DURATION / ANIMATION_INTERVAL; // 动画步数

    // ======================== 组件声明区 ========================
    private JTextArea inputAreaA;
    private JTextArea inputAreaB;
    private JTextArea outputArea;

    // 动画相关（优化版）
    private Map<JButton, AnimationData> buttonAnimationMap = new HashMap<>();
    private Timer globalAnimationTimer; // 全局动画定时器

    // 动画数据类（内部类）
    private static class AnimationData {
        boolean isHovering;          // 是否悬停
        boolean isAnimating;         // 是否正在动画
        float progress;              // 动画进度(0-1)
        Color startColor;            // 起始颜色
        Color targetColor;           // 目标颜色

        AnimationData() {
            this.isHovering = false;
            this.isAnimating = false;
            this.progress = 0f;
        }
    }

    // ======================== 构造方法 ========================
    public MatrixCalculatorGUI() {
        initializeFrame();
        initializeAnimationTimer(); // 初始化全局动画定时器
        addComponents();
    }

    // ======================== 初始化方法 ========================
    private void initializeFrame() {
        // 窗口基本设置
        setTitle("宇宙超级无敌矩阵计算器");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(FRAME_WIDTH, FRAME_HEIGHT);
        setLocationRelativeTo(null);
        getContentPane().setBackground(SECONDARY_COLOR);

        // 图标设置
        try {
            ImageIcon icon = new ImageIcon("matrix_icon.png");
            setIconImage(icon.getImage());
        } catch (Exception e) {
            System.out.println("图标加载失败: " + e.getMessage());
        }
    }

    /**
     * 初始化全局动画定时器（核心优化）
     */
    private void initializeAnimationTimer() {
        globalAnimationTimer = new Timer(ANIMATION_INTERVAL, e -> {
            boolean hasActiveAnimations = false;

            // 遍历所有按钮的动画数据
            synchronized (buttonAnimationMap) {
                for (Map.Entry<JButton, AnimationData> entry : buttonAnimationMap.entrySet()) {
                    JButton btn = entry.getKey();
                    AnimationData data = entry.getValue();

                    if (data.isAnimating) {
                        hasActiveAnimations = true;

                        // 更新动画进度
                        data.progress += 1f / ANIMATION_STEPS;
                        if (data.progress >= 1f) {
                            data.progress = 1f;
                            data.isAnimating = false;
                        }

                        // 计算当前颜色并更新按钮
                        Color currentColor = interpolateColor(data.startColor, data.targetColor, data.progress);
                        btn.setBackground(currentColor);
                    }
                }
            }

            // 如果没有活跃的动画，暂停定时器（节省资源）
            if (!hasActiveAnimations) {
                globalAnimationTimer.stop();
            }
        });
    }

    private void addComponents() {
        add(createTitleLabel(), BorderLayout.NORTH);
        add(createMainContentPanel(), BorderLayout.CENTER);
        add(createOutputPanel(), BorderLayout.SOUTH);
    }

    // ======================== UI组件创建方法 ========================
    private JLabel createTitleLabel() {
        JLabel titleLabel = new JLabel("矩阵计算器", SwingConstants.CENTER);
        titleLabel.setFont(TITLE_FONT);
        titleLabel.setForeground(PRIMARY_COLOR);
        titleLabel.setBorder(BorderFactory.createEmptyBorder(10, 0, 10, 0));
        return titleLabel;
    }

    private JPanel createMainContentPanel() {
        JPanel mainPanel = new JPanel(new BorderLayout(0, 10));
        mainPanel.setBackground(SECONDARY_COLOR);
        mainPanel.setBorder(BorderFactory.createEmptyBorder(0, 10, 10, 10));

        mainPanel.add(createInputPanels(), BorderLayout.NORTH);
        mainPanel.add(createButtonPanel(), BorderLayout.CENTER);

        return mainPanel;
    }

    private Component createInputPanels() {
        // 1. 创建自定义样式的JSplitPane
        JSplitPane splitInput = new JSplitPane(
                JSplitPane.HORIZONTAL_SPLIT,
                createMatrixInputPanel("系数矩阵 A", true),
                createMatrixInputPanel("矩阵 B 或 向量 b", false)
        );

        // 关键：移除JSplitPane的默认边框
        splitInput.setBorder(BorderFactory.createEmptyBorder());
        // 关键：设置分割线颜色和样式
        splitInput.setDividerLocation(SPLIT_PANE_DIVIDER);
        splitInput.setDividerSize(SPLIT_PANE_DIVIDER_SIZE);
        splitInput.setBackground(SECONDARY_COLOR);

        // 自定义分割线UI
        splitInput.setUI(new javax.swing.plaf.basic.BasicSplitPaneUI() {
            @Override
            public BasicSplitPaneDivider createDefaultDivider() {
                return new BasicSplitPaneDivider(this) {
                    @Override
                    public void paint(Graphics g) {
                        super.paint(g);
                        // 绘制自定义颜色的分割线
                        g.setColor(SPLIT_PANE_DIVIDER_COLOR);
                        g.fillRect(0, 0, getWidth(), getHeight());
                    }
                };
            }
        });

        // 优化：让分割面板的背景与整体风格统一
        splitInput.setOpaque(false);
        ((JPanel) splitInput.getLeftComponent()).setOpaque(true);
        ((JPanel) splitInput.getRightComponent()).setOpaque(true);

        return splitInput;
    }

    private JPanel createMatrixInputPanel(String title, boolean isA) {
        JPanel panel = new JPanel(new BorderLayout());
        panel.setBackground(INPUT_PANEL_BG);

        // 创建带标题的边框
        TitledBorder titledBorder = BorderFactory.createTitledBorder(
                BorderFactory.createLineBorder(PRIMARY_COLOR, 2, true),
                title,
                TitledBorder.LEFT,
                TitledBorder.TOP,
                PANEL_TITLE_FONT,
                TITLE_COLOR
        );
        panel.setBorder(BorderFactory.createCompoundBorder(
                titledBorder,
                BorderFactory.createEmptyBorder(BORDER_EMPTY_PADDING, BORDER_EMPTY_PADDING,
                        BORDER_EMPTY_PADDING, BORDER_EMPTY_PADDING)
        ));

        // 提示标签
        JLabel hintLabel = new JLabel("提示：每行元素用空格分隔，每行代表矩阵的一行");
        hintLabel.setFont(HINT_FONT);
        hintLabel.setForeground(HINT_COLOR);
        hintLabel.setBorder(BorderFactory.createEmptyBorder(0, 5, 5, 0));

        // 文本输入区域
        JTextArea textArea = new JTextArea(TEXT_AREA_ROWS, 40);
        textArea.setFont(TEXT_AREA_FONT);
        textArea.setBackground(INPUT_PANEL_BG);
        textArea.setBorder(createTextAreaBorder(false));
        textArea.setLineWrap(false);
        textArea.setWrapStyleWord(false);

        // 焦点变化效果
        textArea.addFocusListener(new FocusAdapter() {
            @Override
            public void focusGained(FocusEvent evt) {
                textArea.setBorder(createTextAreaBorder(true));
            }

            @Override
            public void focusLost(FocusEvent evt) {
                textArea.setBorder(createTextAreaBorder(false));
            }
        });

        // 滚动面板
        JScrollPane scroll = new JScrollPane(textArea);
        scroll.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
        scroll.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        scroll.setBorder(BorderFactory.createEmptyBorder());

        // 组装面板
        panel.add(hintLabel, BorderLayout.NORTH);
        panel.add(scroll, BorderLayout.CENTER);

        // 保存引用
        if (isA) {
            inputAreaA = textArea;
        } else {
            inputAreaB = textArea;
        }

        return panel;
    }

    private JPanel createButtonPanel() {
        JPanel buttonPanel = new JPanel(new GridLayout(3, 5, BUTTON_GRID_HGAP, BUTTON_GRID_VGAP));
        buttonPanel.setBackground(BUTTON_PANEL_BG);
        buttonPanel.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createTitledBorder(
                        BorderFactory.createLineBorder(PRIMARY_COLOR, 2, true),
                        "操作",
                        TitledBorder.CENTER,
                        TitledBorder.TOP,
                        PANEL_TITLE_FONT,
                        PRIMARY_COLOR
                ),
                BorderFactory.createEmptyBorder(10, 10, 10, 10)
        ));

        // 按钮文本数组
        String[] buttons = {
                "A + B", "A - B", "A × B", "k·A", "转置(A)",
                "行列式(A)", "逆(A)", "迹(A)", "秩(A)", "解 Ax=b",
                "齐次解 Ax=0", "示例1(唯一解)", "示例2(无穷多解)", "示例3(无解)", "清空"
        };

        // 创建按钮
        for (String label : buttons) {
            if (label.isEmpty()) {
                buttonPanel.add(new JLabel()); // 占位符
            } else {
                JButton btn = new JButton(label);
                // 根据按钮类型设置样式
                if ("清空".equals(label)) {
                    styleClearButton(btn);
                } else {
                    styleNormalButton(btn);
                }
                btn.addActionListener(this::handleButtonClick);
                // 初始化按钮动画数据
                setupSmoothButtonEffect(btn);
                buttonPanel.add(btn);
            }
        }

        return buttonPanel;
    }

    private JPanel createOutputPanel() {
        JPanel panel = new JPanel(new BorderLayout());
        panel.setBackground(OUTPUT_PANEL_BG);

        // 边框设置
        panel.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createTitledBorder(
                        BorderFactory.createLineBorder(PRIMARY_COLOR, 2, true),
                        "计算结果",
                        TitledBorder.CENTER,
                        TitledBorder.TOP,
                        PANEL_TITLE_FONT,
                        PRIMARY_COLOR
                ),
                BorderFactory.createEmptyBorder(BORDER_EMPTY_PADDING, 10, 10, 10)
        ));

        // 输出文本区域
        outputArea = new JTextArea(OUTPUT_AREA_ROWS, 50);
        outputArea.setEditable(false);
        outputArea.setFont(TEXT_AREA_FONT);
        outputArea.setBackground(OUTPUT_PANEL_BG);
        outputArea.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createLineBorder(LIGHT_GRAY),
                BorderFactory.createEmptyBorder(OUTPUT_AREA_PADDING, OUTPUT_AREA_PADDING,
                        OUTPUT_AREA_PADDING, OUTPUT_AREA_PADDING)
        ));

        // 滚动面板
        JScrollPane outputScroll = new JScrollPane(outputArea);
        outputScroll.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
        outputScroll.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        outputScroll.setBorder(BorderFactory.createEmptyBorder());

        panel.add(outputScroll, BorderLayout.CENTER);
        return panel;
    }

    // ======================== 样式工具方法 ========================

    /**
     * 创建文本框边框（带焦点/无焦点）
     */
    private Border createTextAreaBorder(boolean hasFocus) {
        return BorderFactory.createCompoundBorder(
                BorderFactory.createLineBorder(hasFocus ? FOCUS_BORDER : LIGHT_GRAY),
                BorderFactory.createEmptyBorder(TEXT_AREA_PADDING, TEXT_AREA_PADDING,
                        TEXT_AREA_PADDING, TEXT_AREA_PADDING)
        );
    }

    /**
     * 普通按钮样式
     */
    private void styleNormalButton(JButton btn) {
        // 基础样式
        btn.setFocusPainted(false);
        btn.setFont(BUTTON_FONT);
        btn.setForeground(TEXT_COLOR);
        btn.setBackground(BUTTON_NORMAL);
        btn.setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));

        // 边框设置 - 优雅的按钮边框
        btn.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createLineBorder(BUTTON_BORDER, 2),
                BorderFactory.createEmptyBorder(6, 12, 6, 12)
        ));

        // 关键设置：确保背景色显示
        btn.setContentAreaFilled(false);
        btn.setOpaque(true);

        // 按下效果优化
        btn.setPressedIcon(new ImageIcon(new BufferedImage(1, 1, BufferedImage.TYPE_INT_ARGB)));
    }

    /**
     * 清空按钮专属样式
     */
    private void styleClearButton(JButton btn) {
        // 基础样式
        btn.setFocusPainted(false);
        btn.setFont(CLEAR_BUTTON_FONT);
        btn.setForeground(TEXT_COLOR);
        btn.setBackground(CLEAR_BTN_COLOR);
        btn.setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));

        // 边框设置 - 更醒目的边框
        btn.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createLineBorder(ACCENT_COLOR, 3),
                BorderFactory.createEmptyBorder(6, 12, 6, 12)
        ));

        // 关键设置
        btn.setContentAreaFilled(false);
        btn.setOpaque(true);

        // 按下效果优化
        btn.setPressedIcon(new ImageIcon(new BufferedImage(1, 1, BufferedImage.TYPE_INT_ARGB)));
    }

    // ======================== 动画优化方法 ========================

    /**
     * 设置平滑按钮效果（优化版）
     */
    private void setupSmoothButtonEffect(JButton btn) {
        // 初始化动画数据
        AnimationData data = new AnimationData();
        synchronized (buttonAnimationMap) {
            buttonAnimationMap.put(btn, data);
        }

        btn.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseEntered(MouseEvent e) {
                startButtonAnimation(btn, true);
            }

            @Override
            public void mouseExited(MouseEvent e) {
                startButtonAnimation(btn, false);
            }
        });
    }

    /**
     * 启动按钮动画（核心优化）
     */
    private void startButtonAnimation(JButton btn, boolean isHovering) {
        synchronized (buttonAnimationMap) {
            AnimationData data = buttonAnimationMap.get(btn);
            if (data == null) return;

            // 获取按钮的基础颜色
            Color normalColor = "清空".equals(btn.getText()) ? CLEAR_BTN_COLOR : BUTTON_NORMAL;
            Color hoverColor = "清空".equals(btn.getText()) ? CLEAR_BTN_HOVER : BUTTON_HOVER;

            // 更新悬停状态
            data.isHovering = isHovering;

            // 设置动画参数
            data.isAnimating = true;
            data.progress = 0f;
            data.startColor = btn.getBackground(); // 从当前颜色开始
            data.targetColor = isHovering ? hoverColor : normalColor;

            // 启动全局定时器
            if (!globalAnimationTimer.isRunning()) {
                globalAnimationTimer.start();
            }
        }
    }

    /**
     * 颜色插值方法（支持Alpha通道）
     */
    private Color interpolateColor(Color start, Color end, float ratio) {
        // 限制ratio在0-1之间
        ratio = Math.max(0f, Math.min(1f, ratio));

        // 计算RGB和Alpha通道的插值
        int red = (int) (start.getRed() + (end.getRed() - start.getRed()) * ratio);
        int green = (int) (start.getGreen() + (end.getGreen() - start.getGreen()) * ratio);
        int blue = (int) (start.getBlue() + (end.getBlue() - start.getBlue()) * ratio);
        int alpha = (int) (start.getAlpha() + (end.getAlpha() - start.getAlpha()) * ratio);

        return new Color(red, green, blue, alpha);
    }

    // ======================== 业务逻辑方法 ========================
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
                try {
                    data[i][j] = st.hasMoreTokens() ? Double.parseDouble(st.nextToken()) : 0.0;
                } catch (NumberFormatException e) {
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
                case "示例1(唯一解)":
                    inputAreaA.setText("1 2\n3 4");
                    inputAreaB.setText("5\n11");
                    outputArea.setText("已加载：唯一解示例");
                    break;
                case "示例2(无穷多解)":
                    inputAreaA.setText("1 2 3\n2 4 6");
                    inputAreaB.setText("6\n12");
                    outputArea.setText("已加载：无穷多解示例");
                    break;
                case "示例3(无解)":
                    inputAreaA.setText("1 1\n1 1");
                    inputAreaB.setText("1\n2");
                    outputArea.setText("已加载：无解示例");
                    break;
                default:
                    outputArea.setText("未知操作");
            }
        } catch (Exception ex) {
            JOptionPane.showMessageDialog(this,
                    "<html><body style='width: 250px;'>" +
                            "错误: " + ex.getMessage().replace("\n", "<br>") +
                            "</body></html>",
                    "计算失败",
                    JOptionPane.ERROR_MESSAGE);
        }
    }

    // ======================== 主方法 ========================
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
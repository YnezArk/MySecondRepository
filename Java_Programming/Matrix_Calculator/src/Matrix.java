/**
 * 矩阵类
 * 支持：加减乘、数乘、转置、行列式、逆、迹、秩、
 *      齐次方程组（nullSpace）、非齐次方程组（solve）
 */
public class Matrix {

    private double[][] data;
    private int rows;
    protected int cols;

    public Matrix(double[][] data) {
        if (data == null || data.length == 0 || data[0].length == 0) {
            throw new IllegalArgumentException("数据不能为空");
        }
        this.data = data;
        this.rows = data.length;
        this.cols = data[0].length;
    }

    // ========== 基础运算 ==========

    // 矩阵相加
    public Matrix add(Matrix other) {
        // 检查两个矩阵的维度是否相同
        if (this.rows != other.rows || this.cols != other.cols) {
            throw new IllegalArgumentException("矩阵加法要求两个矩阵具有相同的维度。");
        }

        // 执行矩阵加法运算
        double[][] result = new double[rows][cols];
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result[i][j] = this.data[i][j] + other.data[i][j];
            }
        }
        return new Matrix(result);
    }

// 矩阵相减
    public Matrix subtract(Matrix other) {
        if (this.rows != other.rows || this.cols != other.cols) {
            throw new IllegalArgumentException("矩阵减法要求两个矩阵具有相同的维度。");
        }
        double[][] result = new double[rows][cols];
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result[i][j] = this.data[i][j] - other.data[i][j];
            }
        }
        return new Matrix(result);
    }
// 矩阵乘法
    public Matrix multiply(Matrix other) {
        if (this.cols != other.rows) {
            throw new IllegalArgumentException("矩阵乘法要求第一个矩阵的列数等于第二个矩阵的行数。");
        }
        int newRows = this.rows;
        int newCols = other.cols;
        double[][] result = new double[newRows][newCols];
        for (int i = 0; i < newRows; i++) {
            for (int j = 0; j < newCols; j++) {
                for (int k = 0; k < this.cols; k++) {
                    result[i][j] += this.data[i][k] * other.data[k][j];
                }
            }
        }
        return new Matrix(result);
    }
// 数乘
    public Matrix scalarMultiply(double scalar) {
        double[][] result = new double[rows][cols];
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result[i][j] = this.data[i][j] * scalar;
            }
        }
        return new Matrix(result);
    }
// 转置
    public Matrix transpose() {
        double[][] transposeData = new double[cols][rows];
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                transposeData[j][i] = data[i][j];
            }
        }
        return new Matrix(transposeData);
    }
// 行列式前置检查
    public double determinant() {
        if (rows != cols) {
            throw new IllegalStateException("只能计算方阵的行列式.");
        }
        return calculateDeterminant(this.data);
    }
// 行列式计算
    private double calculateDeterminant(double[][] matrix) {
        int n = matrix.length;
        if (n == 1) return matrix[0][0];
        if (n == 2) return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
        double det = 0;
        for (int j = 0; j < n; j++) {
            det += Math.pow(-1, j) * matrix[0][j] * calculateDeterminant(getSubmatrix(matrix, 0, j));
        }
        return det;
    }
// 逆矩阵计算
    public Matrix inverse() {
        double det = determinant();
        if (Math.abs(det) < 1e-10) {
            throw new ArithmeticException("矩阵是奇异矩阵（行列式为零），无逆矩阵.");
        }
        int n = rows;
        double[][] cofactorMatrix = new double[n][n];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                double[][] submatrix = getSubmatrix(this.data, i, j);
                cofactorMatrix[i][j] = Math.pow(-1, i + j) * calculateDeterminant(submatrix);
            }
        }
        Matrix adjugateMatrix = new Matrix(cofactorMatrix).transpose();
        double[][] inverseData = new double[n][n];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                inverseData[i][j] = adjugateMatrix.data[i][j] / det;
            }
        }
        return new Matrix(inverseData);
    }
// 获取子矩阵
    private double[][] getSubmatrix(double[][] matrix, int rowToRemove, int colToRemove) {
        int n = matrix.length;
        double[][] submatrix = new double[n - 1][n - 1];
        int r = -1;
        for (int i = 0; i < n; i++) {
            if (i == rowToRemove) continue;
            r++;
            int c = -1;
            for (int j = 0; j < n; j++) {
                if (j == colToRemove) continue;
                c++;
                submatrix[r][c] = matrix[i][j];
            }
        }
        return submatrix;
    }
// 矩阵的迹
    public double trace() {
        if (rows != cols) {
            throw new IllegalStateException("迹仅对方阵定义。");
        }
        double sum = 0.0;
        for (int i = 0; i < rows; i++) {
            sum += data[i][i];
        }
        return sum;
    }
// 矩阵的秩
    public int rank() {
        double[][] mat = new double[rows][cols];
        for (int i = 0; i < rows; i++) {
            System.arraycopy(this.data[i], 0, mat[i], 0, cols);
        }

        int r = 0;
        final double EPS = 1e-10;

        for (int col = 0; col < cols && r < rows; col++) {
            int pivot = r;
            for (int i = r + 1; i < rows; i++) {
                if (Math.abs(mat[i][col]) > Math.abs(mat[pivot][col])) {
                    pivot = i;
                }
            }

            if (Math.abs(mat[pivot][col]) < EPS) continue;

            double[] temp = mat[r];
            mat[r] = mat[pivot];
            mat[pivot] = temp;

            for (int i = r + 1; i < rows; i++) {
                double factor = mat[i][col] / mat[r][col];
                for (int j = col; j < cols; j++) {
                    mat[i][j] -= factor * mat[r][j];
                }
            }
            r++;
        }
        return r;
    }

    // ========== 齐次方程组 Ax = 0 ==========

    /**
     * 求齐次线性方程组 Ax = 0 的基础解系（零空间的一组基）
     * 返回一个 Matrix，其每一列是一个基础解向量
     * 若只有零解（即 rank == cols），返回 null
     */
    public Matrix nullSpace() {
        final double EPS = 1e-10;
        int n = this.cols;
        int m = this.rows;

        double[][] rref = new double[m][n];
        for (int i = 0; i < m; i++) {
            System.arraycopy(this.data[i], 0, rref[i], 0, n);
        }

        int[] pivotCols = new int[Math.min(m, n)];
        int pivotCount = 0;
        int r = 0;

        for (int col = 0; col < n && r < m; col++) {
            int pivotRow = r;
            for (int i = r + 1; i < m; i++) {
                if (Math.abs(rref[i][col]) > Math.abs(rref[pivotRow][col])) {
                    pivotRow = i;
                }
            }

            if (Math.abs(rref[pivotRow][col]) < EPS) continue;

            double[] temp = rref[r];
            rref[r] = rref[pivotRow];
            rref[pivotRow] = temp;

            double pivotVal = rref[r][col];
            for (int j = col; j < n; j++) {
                rref[r][j] /= pivotVal;
            }

            for (int i = 0; i < m; i++) {
                if (i != r && Math.abs(rref[i][col]) > EPS) {
                    double factor = rref[i][col];
                    for (int j = col; j < n; j++) {
                        rref[i][j] -= factor * rref[r][j];
                    }
                }
            }

            pivotCols[pivotCount++] = col;
            r++;
        }

        if (pivotCount == n) {
            return null;
        }

        java.util.List<Integer> freeVars = new java.util.ArrayList<>();
        boolean[] isPivotCol = new boolean[n];
        for (int i = 0; i < pivotCount; i++) {
            isPivotCol[pivotCols[i]] = true;
        }
        for (int j = 0; j < n; j++) {
            if (!isPivotCol[j]) {
                freeVars.add(j);
            }
        }

        int freeCount = freeVars.size();
        double[][] basis = new double[n][freeCount];

        for (int fIdx = 0; fIdx < freeCount; fIdx++) {
            int freeCol = freeVars.get(fIdx);
            for (int i = 0; i < n; i++) {
                basis[i][fIdx] = 0.0;
            }
            basis[freeCol][fIdx] = 1.0;

            for (int pIdx = 0; pIdx < pivotCount; pIdx++) {
                int pivotRow = pIdx;
                int pivotCol = pivotCols[pIdx];
                double sum = 0.0;
                for (int f = 0; f < freeCount; f++) {
                    int fc = freeVars.get(f);
                    sum += rref[pivotRow][fc] * basis[fc][fIdx];
                }
                basis[pivotCol][fIdx] = -sum;
            }
        }

        return new Matrix(basis);
    }

    // ========== 非齐次方程组 Ax = b（通用）==========

    /**
     * 求解非齐次线性方程组 Ax = b（通用情况）
     * - 无解：抛出异常
     * - 唯一解或无穷多解：返回一个特解
     */
    public Matrix solve(Matrix b) {
        if (b.rows != this.rows || b.cols != 1) {
            throw new IllegalArgumentException("b 必须是 " + this.rows + "×1 的列向量。");
        }

        final double EPS = 1e-10;
        int m = this.rows;
        int n = this.cols;

        double[][] aug = new double[m][n + 1];
        for (int i = 0; i < m; i++) {
            System.arraycopy(this.data[i], 0, aug[i], 0, n);
            aug[i][n] = b.data[i][0];
        }

        int r = 0;
        int[] pivotCols = new int[Math.min(m, n)];
        int pivotCount = 0;

        for (int col = 0; col < n && r < m; col++) {
            int pivotRow = r;
            for (int i = r + 1; i < m; i++) {
                if (Math.abs(aug[i][col]) > Math.abs(aug[pivotRow][col])) {
                    pivotRow = i;
                }
            }

            if (Math.abs(aug[pivotRow][col]) < EPS) continue;

            double[] temp = aug[r];
            aug[r] = aug[pivotRow];
            aug[pivotRow] = temp;

            double pivotVal = aug[r][col];
            for (int j = col; j <= n; j++) {
                aug[r][j] /= pivotVal;
            }

            for (int i = 0; i < m; i++) {
                if (i != r && Math.abs(aug[i][col]) > EPS) {
                    double factor = aug[i][col];
                    for (int j = col; j <= n; j++) {
                        aug[i][j] -= factor * aug[r][j];
                    }
                }
            }

            pivotCols[pivotCount++] = col;
            r++;
        }

        // 检查无解
        for (int i = r; i < m; i++) {
            boolean allZero = true;
            for (int j = 0; j < n; j++) {
                if (Math.abs(aug[i][j]) > EPS) {
                    allZero = false;
                    break;
                }
            }
            if (allZero && Math.abs(aug[i][n]) > EPS) {
                throw new IllegalArgumentException("方程组无解（矛盾方程）。");
            }
        }

        double[] solution = new double[n];
        boolean[] isPivotCol = new boolean[n];
        for (int i = 0; i < pivotCount; i++) {
            isPivotCol[pivotCols[i]] = true;
        }

        for (int i = 0; i < pivotCount; i++) {
            int col = pivotCols[i];
            solution[col] = aug[i][n];
        }

        for (int j = 0; j < n; j++) {
            if (!isPivotCol[j]) {
                solution[j] = 0.0; // 自由变量设为0（特解）
            }
        }

        double[][] solMatrix = new double[n][1];
        for (int i = 0; i < n; i++) {
            solMatrix[i][0] = solution[i];
        }

        return new Matrix(solMatrix);
    }
// 结果输出，矩阵转字符串
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                sb.append(String.format("%.4f\t", data[i][j]));
            }
            sb.append("\n");
        }
        return sb.toString();
    }
}
def print_matrix(M, elements, title):
    print(f"\n{title}:")
    n = len(elements)
    # 打印表头
    print("   ", " ".join(elements))
    for i in range(n):
        row_str = " ".join(str(int(M[i][j])) for j in range(n))
        print(f"{elements[i]} | {row_str}")


def pairs_to_matrix(pairs, elements):
    """将序偶列表转换为关系矩阵"""
    n = len(elements)
    index = {elem: i for i, elem in enumerate(elements)}
    M = [[False] * n for _ in range(n)]

    for pair in pairs:
        if len(pair) != 2:
            continue  # 忽略无效序偶
        a, b = pair[0], pair[1]
        if a in index and b in index:
            i, j = index[a], index[b]
            M[i][j] = True
    return M


def matrix_to_pairs(M, elements):
    """将矩阵转换回序偶列表（用于可读输出）"""
    pairs = []
    n = len(elements)
    for i in range(n):
        for j in range(n):
            if M[i][j]:
                pairs.append(elements[i] + elements[j])
    return pairs


def main():
    # 输入集合
    A_str = input("请输入有限集合的元素（连续字母，如 abc）: ").strip()
    elements = list(A_str)
    if not elements:
        print("集合不能为空！")
        return

    # 输入关系（空格分隔的序偶）
    R_str = input(f"请输入关系 R 的序偶（如 ab bc ca，表示 (a,b),(b,c),(c,a)）: ").strip()
    if R_str:
        pairs = R_str.split()
    else:
        pairs = []

    # 构建原始关系矩阵
    M = pairs_to_matrix(pairs, elements)
    n = len(elements)

    # 自反闭包: 主对角线设为 True
    M_r = [row[:] for row in M]
    for i in range(n):
        M_r[i][i] = True

    # 对称闭包: M ∨ M^T
    M_s = [[M[i][j] or M[j][i] for j in range(n)] for i in range(n)]

    # 传递闭包: Warshall 算法
    M_t = [row[:] for row in M]
    for k in range(n):
        for i in range(n):
            for j in range(n):
                M_t[i][j] = M_t[i][j] or (M_t[i][k] and M_t[k][j])

    # 输出
    print("\n=== 关系闭包结果 ===")
    print(f"集合 A = {{{', '.join(elements)}}}")
    print(f"原始关系 R = {{{', '.join(matrix_to_pairs(M, elements)) or '∅'}}}")

    print_matrix(M, elements, "原始关系 R 的矩阵")
    print_matrix(M_r, elements, "自反闭包 r(R)")
    print_matrix(M_s, elements, "对称闭包 s(R)")
    print_matrix(M_t, elements, "传递闭包 t(R)（Warshall 算法）")

    # 可选：输出序偶形式
    print("\n--- 序偶形式 ---")
    print("r(R) =", "{" + ", ".join(matrix_to_pairs(M_r, elements)) + "}" if matrix_to_pairs(M_r, elements) else "∅")
    print("s(R) =", "{" + ", ".join(matrix_to_pairs(M_s, elements)) + "}" if matrix_to_pairs(M_s, elements) else "∅")
    print("t(R) =", "{" + ", ".join(matrix_to_pairs(M_t, elements)) + "}" if matrix_to_pairs(M_t, elements) else "∅")


if __name__ == "__main__":
    main()
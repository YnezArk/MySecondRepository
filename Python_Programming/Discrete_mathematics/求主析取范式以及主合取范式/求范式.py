import itertools
import re


def get_vars(expr):
    return sorted(set(re.findall(r'\b[pqr]\b', expr)))


raw_expr = input("请输入命题公式（可用 ->, <->，变量 p/q/r）: ")

# 替换逻辑符号
expr = re.sub(r'\s*<->\s*', ' == ', raw_expr)  # p <-> q  → p == q
expr = re.sub(r'\s*->\s*', ' <= ', expr)  # p -> q   → p <= q

# 提取变量
vars = get_vars(expr)
if not vars:
    print("未找到变量 p/q/r")
    exit()

n = len(vars)
print("\n真值表:")
print(" ".join(vars) + " | 结果")
print("-" * (len(vars) * 2 + 8))

true_indices = []
false_indices = []

for idx, values in enumerate(itertools.product([False, True], repeat=n)):
    env = dict(zip(vars, values))
    try:
        result = eval(expr, {"__builtins__": {}}, env)
    except Exception as e:
        print(f"表达式错误: {e}")
        print("请确保使用合法的逻辑表达式，如: p -> (q or r)")
        exit()

    print(" ".join(str(int(v)) for v in values) + f" | {int(result)}")

    if result:
        true_indices.append(idx)
    else:
        false_indices.append(idx)


pdnf_parts = []
for i in true_indices:
    bits = format(i, f'0{n}b')
    terms = [var if b == '1' else f"(not {var})" for var, b in zip(vars, bits)]
    pdnf_parts.append("(" + " and ".join(terms) + ")")
pdnf = " or ".join(pdnf_parts) if pdnf_parts else "False"
minterms = " ∨ ".join(f"m{i}" for i in true_indices) if true_indices else "无"

pcnf_parts = []
for i in false_indices:
    bits = format(i, f'0{n}b')
    terms = [f"(not {var})" if b == '1' else var for var, b in zip(vars, bits)]
    pcnf_parts.append("(" + " or ".join(terms) + ")")
pcnf = " and ".join(pcnf_parts) if pcnf_parts else "True"
maxterms = " ∧ ".join(f"M{i}" for i in false_indices) if false_indices else "无"

# --- 输出 ---
print("\n主析取范式 (PDNF):", pdnf)
print("极小项编号（PDNF）:", minterms if true_indices else "矛盾式")

print("\n主合取范式 (PCNF):", pcnf)
print("极大项编号（PCNF）:", maxterms if false_indices else "重言式")
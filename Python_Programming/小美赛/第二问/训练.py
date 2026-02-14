# -*- coding: utf-8 -*-
import pandas as pd
import numpy as np
from scipy.optimize import minimize
import matplotlib.pyplot as plt
import warnings

warnings.filterwarnings("ignore")

# -----------------------------
# 新增：PyTorch 依赖（仅用于 LSTM）
# -----------------------------
import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader, TensorDataset

# -----------------------------
# Step 0: 配置路径（不变）
# -----------------------------
PATH_RAW = "HealthGymV2_CbdrhDatathon_ART4HIV.csv"
PATH_COMBO = "drug_combination_analysis_results.csv"

# -----------------------------
# Step 1: 加载药物组合 & 构建映射（不变）
# -----------------------------
df_combo = pd.read_csv(PATH_COMBO)
expected_columns = [
    'Base_Drug_Combo', 'Comp_INI', 'Comp_NNRTI', 'Extra_PI', 'Extra_pk_En',
    'count', 'patient_count',
    'avg_S_persistence', 'avg_S_slope', 'avg_S_suppression',
    'avg_S_immunity', 'avg_S_burden', 'avg_S_stability', 'avg_S_smoothness',
    'comprehensive_score'
]

if len(df_combo.columns) == len(expected_columns):
    df_combo.columns = expected_columns
else:
    raise ValueError(f"列数不匹配！期望15列，实际{len(df_combo.columns)}列")

df_combo = df_combo[df_combo['patient_count'] >= 0].reset_index(drop=True)

drug_key_cols = ['Base_Drug_Combo', 'Comp_INI', 'Comp_NNRTI', 'Extra_PI', 'Extra_pk_En']
df_combo['drug_key'] = df_combo[drug_key_cols].apply(lambda x: tuple(x.astype(int)), axis=1)

combo_to_id = {key: idx for idx, key in enumerate(df_combo['drug_key'].unique())}
id_to_combo = {idx: key for key, idx in combo_to_id.items()}


def describe_combo(combo_tuple):
    base, ini, nnrti, pi, pk = combo_tuple
    return f"Base:{base}, INI:{ini}, NNRTI:{nnrti}, PI:{pi}, pk-En:{pk}"


combo_descriptions = {cid: describe_combo(id_to_combo[cid]) for cid in id_to_combo}
M = len(combo_to_id)

print(f"✅ 共 {M} 种有效药物组合")
print("\n📋 药物组合详情（ID → 配方）:")
for cid in range(M):
    print(f"  ID {cid:2d}: {combo_descriptions[cid]}")
print("")

combo_burden = {cid: df_combo.iloc[cid]['avg_S_burden'] for cid in range(M)}

# -----------------------------
# Step 2: 加载原始数据 & 提取转移样本 (Timestep <= 47)（不变）
# -----------------------------
df_raw = pd.read_csv(PATH_RAW)
df_raw = df_raw.rename(columns={
    'Base Drug Combo': 'Base_Drug_Combo',
    'Comp. INI': 'Comp_INI',
    'Comp. NNRTI': 'Comp_NNRTI',
    'Extra PI': 'Extra_PI',
    'Extra pk-En': 'Extra_pk_En',
    'Rel CD4': 'Rel_CD4'
})
df_raw = df_raw.sort_values(['PatientID', 'Timestep']).reset_index(drop=True)

transitions = []
for pid, group in df_raw.groupby('PatientID'):
    group = group.reset_index(drop=True)
    for i in range(len(group) - 1):
        t_row = group.iloc[i]
        t1_row = group.iloc[i + 1]
        if t1_row['Timestep'] != t_row['Timestep'] + 1:
            continue
        if t_row['Timestep'] > 47:
            continue

        drug_key = (
            int(t_row['Base_Drug_Combo']),
            int(t_row['Comp_INI']),
            int(t_row['Comp_NNRTI']),
            int(t_row['Extra_PI']),
            int(t_row['Extra_pk_En'])
        )
        if drug_key not in combo_to_id:
            continue

        a_t = combo_to_id[drug_key]
        s_t = [np.log10(t_row['VL'] + 1e-3), t_row['CD4'], t_row['Rel_CD4']]
        s_t1 = [np.log10(t1_row['VL'] + 1e-3), t1_row['CD4'], t1_row['Rel_CD4']]
        transitions.append((s_t, a_t, s_t1))

print(f"✅ 构建了 {len(transitions)} 个转移样本（仅使用 Timestep ≤ 47）")

# -----------------------------
# Step 3: 移除 ODE 模型（不再需要）
# -----------------------------
# 删除 _predict_next_state_ode 和 ode_loss_function

# -----------------------------
# Step 4: 不再拟合 ODE 参数（跳过）
# -----------------------------

# -----------------------------
# Step 5: 纯数据驱动状态转移模型（思路 B）
# -----------------------------
print("正在训练纯数据驱动状态转移模型（MLP）...")

# 准备训练数据：输入 = [s_t + a_t], 输出 = s_{t+1}
X_list, Y_list = [], []
for s_t, a_t, s_t1 in transitions:
    s_t_np = np.array(s_t, dtype=np.float32)
    a_t_onehot = np.zeros(M, dtype=np.float32)  # one-hot 编码药物组合（更稳定）
    a_t_onehot[a_t] = 1.0
    x_input = np.concatenate([s_t_np, a_t_onehot])  # shape: (3 + M,)
    y_output = np.array(s_t1, dtype=np.float32)
    X_list.append(x_input)
    Y_list.append(y_output)

X_train = torch.tensor(np.stack(X_list), dtype=torch.float32)
Y_train = torch.tensor(np.stack(Y_list), dtype=torch.float32)

train_dataset = TensorDataset(X_train, Y_train)
train_loader = DataLoader(train_dataset, batch_size=128, shuffle=True)


# 定义纯 MLP 模型
class StateTransitionMLP(nn.Module):
    def __init__(self, input_dim=3 + M, hidden_dim=128, output_dim=3):
        super().__init__()
        self.net = nn.Sequential(
            nn.Linear(input_dim, hidden_dim),
            nn.ReLU(),
            nn.Dropout(0.2),
            nn.Linear(hidden_dim, hidden_dim),
            nn.ReLU(),
            nn.Dropout(0.1),
            nn.Linear(hidden_dim, output_dim)
        )

    def forward(self, x):
        return self.net(x)


# 初始化模型
torch.manual_seed(42)
state_model = StateTransitionMLP(input_dim=3 + M, hidden_dim=128, output_dim=3)
optimizer = optim.Adam(state_model.parameters(), lr=1e-3)
criterion = nn.MSELoss()

# 训练
state_model.train()
for epoch in range(80):  # 增加轮数确保收敛
    total_loss = 0
    for x_batch, y_batch in train_loader:
        optimizer.zero_grad()
        pred = state_model(x_batch)
        loss = criterion(pred, y_batch)
        loss.backward()
        optimizer.step()
        total_loss += loss.item()
    if (epoch + 1) % 20 == 0:
        print(f"  Epoch {epoch + 1}/80, Loss: {total_loss / len(train_loader):.6f}")

print("✅ 纯数据驱动状态转移模型训练完成！")


# -----------------------------
# Step 6: 公共预测接口（纯思路 B）
# -----------------------------
def predict_next_state(s_core, a_t):
    """
    纯数据驱动预测：MLP 直接映射 (s_t, a_t) -> s_{t+1}
    """
    s_core_tensor = torch.tensor(s_core, dtype=torch.float32)
    a_onehot = torch.zeros(M, dtype=torch.float32)
    a_onehot[a_t] = 1.0
    x_input = torch.cat([s_core_tensor, a_onehot], dim=0).unsqueeze(0)  # [1, 3+M]

    state_model.eval()
    with torch.no_grad():
        s_pred = state_model(x_input).squeeze(0).numpy()

    # 边界处理（防止异常值）
    s_pred[0] = np.clip(s_pred[0], np.log10(1e-3), np.log10(1e6))  # log10(VL)
    s_pred[1] = np.clip(s_pred[1], 1.0, 2000.0)  # CD4
    s_pred[2] = np.clip(s_pred[2], 0.0, 1.0)  # Rel_CD4

    return s_pred.tolist()

# -----------------------------
# Step 7: 模型验证（完全不变）
# -----------------------------
def validate_and_plot_patient(patient_id):
    patient_rows = df_raw[df_raw['PatientID'] == patient_id]
    if len(patient_rows) < 60:
        print(f"⚠️ Patient {patient_id} has less than 60 weeks, skip validation.")
        return None

    row_47 = patient_rows[patient_rows['Timestep'] == 47]
    if row_47.empty:
        print(f"⚠️ No data at week 47 for Patient {patient_id}")
        return None
    row_47 = row_47.iloc[0]

    s_pred = [np.log10(row_47['VL'] + 1e-3), row_47['CD4'], row_47['Rel_CD4']]

    pred_vls, pred_cd4s = [], []
    true_vls, true_cd4s = [], []
    weeks_list, drug_descriptions = [], []

    for t in range(48, 60):
        drug_row = patient_rows[patient_rows['Timestep'] == t - 1]
        if drug_row.empty:
            break
        drug_row = drug_row.iloc[0]

        drug_key = (
            int(drug_row['Base_Drug_Combo']),
            int(drug_row['Comp_INI']),
            int(drug_row['Comp_NNRTI']),
            int(drug_row['Extra_PI']),
            int(drug_row['Extra_pk_En'])
        )
        if drug_key not in combo_to_id:
            break

        a_t = combo_to_id[drug_key]
        drug_desc = combo_descriptions[a_t]
        s_pred = predict_next_state(s_pred, a_t)

        vl_pred = 10 ** s_pred[0]
        cd4_pred = s_pred[1]

        true_row = patient_rows[patient_rows['Timestep'] == t]
        if true_row.empty:
            break
        true_row = true_row.iloc[0]
        vl_true = true_row['VL']
        cd4_true = true_row['CD4']

        pred_vls.append(vl_pred)
        pred_cd4s.append(cd4_pred)
        true_vls.append(vl_true)
        true_cd4s.append(cd4_true)
        weeks_list.append(t)
        drug_descriptions.append(drug_desc)

    if not weeks_list:
        return None

    val_df = pd.DataFrame({
        'PatientID': patient_id,
        'Week': weeks_list,
        'Used_Drug_Combo_Description': drug_descriptions,
        'Predicted_VL': pred_vls,
        'True_VL': true_vls,
        'Predicted_CD4': pred_cd4s,
        'True_CD4': true_cd4s,
        'VL_AE': np.abs(np.array(pred_vls) - np.array(true_vls)),
        'CD4_AE': np.abs(np.array(pred_cd4s) - np.array(true_cd4s))
    })

    plt.figure(figsize=(12, 5))
    plt.subplot(1, 2, 1)
    plt.plot(weeks_list, true_vls, 'o-', label='True VL', color='blue')
    plt.plot(weeks_list, pred_vls, 's--', label='Predicted VL', color='red')
    plt.yscale('log')
    plt.title(f'Patient {patient_id}: Viral Load (48~59)')
    plt.xlabel('Week')
    plt.ylabel('Viral Load (log scale)')
    plt.legend()
    plt.grid(True)

    plt.subplot(1, 2, 2)
    plt.plot(weeks_list, true_cd4s, 'o-', label='True CD4', color='green')
    plt.plot(weeks_list, pred_cd4s, 's--', label='Predicted CD4', color='orange')
    plt.title(f'Patient {patient_id}: CD4 Count (48~59)')
    plt.xlabel('Week')
    plt.ylabel('CD4 Count')
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plot_file = f"patient_{patient_id}_vl_cd4_comparison.png"
    plt.savefig(plot_file, dpi=150)
    plt.close()
    print(f"✅ 验证图已保存: {plot_file}")

    return val_df


# -----------------------------
# Step 8: 执行验证（不变）
# -----------------------------
test_patients = [100, 1350]
all_val_dfs = []
for pid in test_patients:
    print(f"\n🔍 验证 Patient {pid} 的预测模型（48~59周）...")
    val_df = validate_and_plot_patient(pid)
    if val_df is not None:
        all_val_dfs.append(val_df)

if all_val_dfs:
    validation_results = pd.concat(all_val_dfs, ignore_index=True)
    validation_file = "model_validation_48_to_59_weeks_improved.csv"
    validation_results.to_csv(validation_file, index=False, float_format="%.4f")
    print(f"\n💾 模型验证结果已保存至: {validation_file}")
    print("📊 平均绝对误差 (MAE):")
    print(f"  VL MAE: {validation_results['VL_AE'].mean():.2f}")
    print(f"  CD4 MAE: {validation_results['CD4_AE'].mean():.2f}")
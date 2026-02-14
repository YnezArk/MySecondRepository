import pandas as pd
import numpy as np
from scipy import stats
import warnings

warnings.filterwarnings('ignore')


class HIVTreatmentEvaluator:
    def __init__(self, window_size=12):
        self.window_size = window_size
        self.drug_components = ['Base Drug Combo', 'Comp. INI', 'Comp. NNRTI', 'Extra PI', 'Extra pk-En']

    def load_data(self, file_path):
        """加载数据"""
        df = pd.read_csv(file_path)
        print(f"数据加载完成，共 {len(df)} 行记录")
        return df

    def preprocess_data(self, df):
        """数据预处理"""
        # 确保Timestep是数值类型并按患者分组排序
        df = df.sort_values(['PatientID', 'Timestep'])

        # 创建处理后的数据副本
        processed_df = df.copy()

        # 对VL进行log10转换（加1避免log(0)）
        processed_df['VL_log'] = np.log10(processed_df['VL'] + 1)

        return processed_df

    def calculate_persistence(self, patient_data):
        """计算病毒抑制持续性 S_persistence"""
        S_persistence = np.zeros(len(patient_data))

        for t in range(len(patient_data)):
            start_idx = max(0, t - self.window_size + 1)
            window_data = patient_data.iloc[start_idx:t + 1]
            N_t = len(window_data)

            if N_t > 0:
                # 计算VL ≤ 50的比例
                suppressed_count = np.sum(window_data['VL'] <= 50)
                S_persistence[t] = suppressed_count / N_t
            else:
                S_persistence[t] = 0

        return S_persistence

    def calculate_slope(self, patient_data):
        """计算CD4增长速率 S_slope"""
        S_slope = np.zeros(len(patient_data))

        for t in range(len(patient_data)):
            start_idx = max(0, t - self.window_size + 1)
            window_data = patient_data.iloc[start_idx:t + 1]
            valid_data = window_data.dropna(subset=['CD4'])

            if len(valid_data) < 2:
                S_slope[t] = 0.5
            else:
                try:
                    # 线性回归计算斜率
                    x = np.arange(len(valid_data))
                    y = valid_data['CD4'].values
                    slope, _, _, _, _ = stats.linregress(x, y)

                    # 使用数值稳定的sigmoid计算
                    exp_val = -0.1 * slope
                    # 防止指数溢出
                    if exp_val > 700:
                        S_slope[t] = 1.0
                    elif exp_val < -700:
                        S_slope[t] = 0.0
                    else:
                        S_slope[t] = 1 / (1 + np.exp(exp_val))
                except:
                    S_slope[t] = 0.5

        return S_slope

    def calculate_suppression(self, patient_data):
        """计算病毒抑制能力 S_suppression"""
        S_suppression = np.zeros(len(patient_data))

        for t in range(len(patient_data)):
            vl_log = patient_data['VL_log'].iloc[t]

            if vl_log <= np.log10(50):  # ≈1.699
                S_suppression[t] = 1.0
            else:
                # 线性衰减到0
                suppression = max(0, 1 - (vl_log - 1.699) / 2)
                S_suppression[t] = suppression

        return S_suppression

    def calculate_immunity(self, patient_data):
        """计算免疫恢复水平 S_immunity"""
        S_immunity = np.zeros(len(patient_data))

        for t in range(len(patient_data)):
            cd4 = patient_data['CD4'].iloc[t]
            if pd.notna(cd4):
                # 数值稳定的sigmoid计算
                exp_val = -0.01 * (cd4 - 500)
                if exp_val > 700:
                    S_immunity[t] = 0.0
                elif exp_val < -700:
                    S_immunity[t] = 1.0
                else:
                    S_immunity[t] = 1 / (1 + np.exp(exp_val))
            else:
                S_immunity[t] = 0.5  # 默认值

        return S_immunity

    def calculate_burden(self, patient_data):
        """计算治疗负担 S_burden"""
        S_burden = np.zeros(len(patient_data))

        for t in range(len(patient_data)):
            # 统计激活的药物组件数量
            n_active = 0
            if patient_data['Comp. INI'].iloc[t] > 0:
                n_active += 1
            if patient_data['Comp. NNRTI'].iloc[t] > 0:
                n_active += 1
            if patient_data['Extra PI'].iloc[t] > 0:
                n_active += 1
            if patient_data['Extra pk-En'].iloc[t] > 0:
                n_active += 1

            S_burden[t] = 1 - n_active / 4

        return S_burden

    def calculate_stability(self, patient_data):
        """计算用药方案稳定性 S_stability"""
        S_stability = np.zeros(len(patient_data))

        # 第一周稳定性为1.0
        if len(patient_data) > 0:
            S_stability[0] = 1.0

        for t in range(1, len(patient_data)):
            # 构建当前和前一周的用药方案标识
            current_regimen = tuple(patient_data[comp].iloc[t] for comp in self.drug_components)
            prev_regimen = tuple(patient_data[comp].iloc[t - 1] for comp in self.drug_components)

            if current_regimen == prev_regimen:
                S_stability[t] = 1.0
            else:
                S_stability[t] = 0.8

        return S_stability

    def calculate_smoothness(self, patient_data):
        """计算状态轨迹平稳性 S_smoothness"""
        S_smoothness = np.zeros(len(patient_data))

        for t in range(len(patient_data)):
            start_idx = max(0, t - self.window_size + 1)
            window_data = patient_data.iloc[start_idx:t + 1]
            valid_data = window_data.dropna(subset=['CD4'])

            if len(valid_data) < 2:
                S_smoothness[t] = 1.0
            else:
                # 计算CD4的标准差
                cd4_std = np.std(valid_data['CD4'].values)
                # 数值稳定的指数计算
                exp_val = -0.01 * cd4_std
                if exp_val < -700:
                    S_smoothness[t] = 0.0
                else:
                    S_smoothness[t] = np.exp(exp_val)

        return S_smoothness

    def entropy_weight_method(self, indicators_matrix):
        """熵权法确定权重"""
        # 标准化指标矩阵
        X = indicators_matrix.copy()
        X = (X - X.min(axis=0)) / (X.max(axis=0) - X.min(axis=0) + 1e-10)

        # 计算比重
        P = X / (X.sum(axis=0) + 1e-10)

        # 计算熵值
        E = -np.sum(P * np.log(P + 1e-10), axis=0) / np.log(len(X))

        # 计算权重
        weights = (1 - E) / (np.sum(1 - E))

        return weights

    def calculate_all_indicators(self, df):
        """计算所有指标"""
        print("开始计算各项指标...")

        # 预处理数据
        processed_df = self.preprocess_data(df)

        # 初始化结果列
        for indicator in ['S_persistence', 'S_slope', 'S_suppression',
                          'S_immunity', 'S_burden', 'S_stability', 'S_smoothness']:
            processed_df[indicator] = np.nan

        # 按患者分组计算指标
        patient_groups = processed_df.groupby('PatientID')

        for patient_id, patient_data in patient_groups:
            if len(patient_data) == 0:
                continue

            # 计算各项指标
            indices = patient_data.index

            processed_df.loc[indices, 'S_persistence'] = self.calculate_persistence(patient_data)
            processed_df.loc[indices, 'S_slope'] = self.calculate_slope(patient_data)
            processed_df.loc[indices, 'S_suppression'] = self.calculate_suppression(patient_data)
            processed_df.loc[indices, 'S_immunity'] = self.calculate_immunity(patient_data)
            processed_df.loc[indices, 'S_burden'] = self.calculate_burden(patient_data)
            processed_df.loc[indices, 'S_stability'] = self.calculate_stability(patient_data)
            processed_df.loc[indices, 'S_smoothness'] = self.calculate_smoothness(patient_data)

        print("指标计算完成")
        return processed_df

    def analyze_drug_combinations(self, df):
        """分析不同药物组合的指标表现"""
        print("开始分析药物组合...")

        # 计算所有指标
        result_df = self.calculate_all_indicators(df)

        # 按药物组合分组统计
        drug_combinations = result_df.groupby(self.drug_components)

        # 计算每种组合的平均指标
        combination_results = []

        for combo, group in drug_combinations:
            if len(group) > 0:  # 确保有数据
                # 统计使用该组合的患者数量
                patient_count = group['PatientID'].nunique()

                combo_dict = {
                    'Base_Drug_Combo': combo[0],
                    'Comp_INI': combo[1],
                    'Comp_NNRTI': combo[2],
                    'Extra_PI': combo[3],
                    'Extra_pk_En': combo[4],
                    'count': len(group),  # 数据行数
                    'patient_count': patient_count  # 患者数量
                }

                # 计算各项指标的平均值
                for indicator in ['S_persistence', 'S_slope', 'S_suppression',
                                  'S_immunity', 'S_burden', 'S_stability', 'S_smoothness']:
                    combo_dict[f'avg_{indicator}'] = group[indicator].mean()

                combination_results.append(combo_dict)

        combination_df = pd.DataFrame(combination_results)

        # 使用熵权法计算综合评分
        indicator_cols = [f'avg_S_{name}' for name in ['persistence', 'slope', 'suppression',
                                                       'immunity', 'burden', 'stability', 'smoothness']]

        indicators_matrix = combination_df[indicator_cols].values
        weights = self.entropy_weight_method(indicators_matrix)

        print("熵权法计算权重:", weights)

        # 计算综合评分
        combination_df['comprehensive_score'] = np.sum(indicators_matrix * weights, axis=1)

        # 按综合评分排序
        combination_df = combination_df.sort_values('comprehensive_score', ascending=False)

        return combination_df, weights


def main():
    # 初始化评估器
    evaluator = HIVTreatmentEvaluator(window_size=12)

    # 加载数据
    try:
        df = evaluator.load_data('HealthGymV2_CbdrhDatathon_ART4HIV.csv')
    except FileNotFoundError:
        print("文件未找到，请检查文件路径")
        return

    # 分析药物组合
    results, weights = evaluator.analyze_drug_combinations(df)

    # 保存结果
    results.to_csv('drug_combination_analysis_results.csv', index=False)
    print("结果已保存到 drug_combination_analysis_results.csv")

    # 显示前10名组合
    print("\n=== 前10名药物组合 ===")
    top_10 = results.head(10)
    display_columns = ['Base_Drug_Combo', 'Comp_INI', 'Comp_NNRTI', 'Extra_PI',
                       'Extra_pk_En', 'comprehensive_score', 'count', 'patient_count']

    print(top_10[display_columns].to_string(index=False))

    # 显示权重信息
    print(f"\n=== 指标权重 ===")
    indicator_names = ['持续性', '增长速率', '抑制能力', '免疫恢复', '治疗负担', '稳定性', '平稳性']
    for name, weight in zip(indicator_names, weights):
        print(f"{name}: {weight:.4f}")

    # 显示统计信息
    print(f"\n=== 统计信息 ===")
    print(f"总药物组合数量: {len(results)}")
    print(f"总数据行数: {results['count'].sum()}")
    print(f"总患者数量: {results['patient_count'].sum()}")
    print(f"平均每个组合的患者数量: {results['patient_count'].mean():.2f}")
    print(f"数据行数最多的组合: {results['count'].max()}")
    print(f"患者数量最多的组合: {results['patient_count'].max()}")


if __name__ == "__main__":
    main()
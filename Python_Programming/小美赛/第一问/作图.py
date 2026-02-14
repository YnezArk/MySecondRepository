import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from matplotlib import font_manager
import warnings

warnings.filterwarnings('ignore')

# 设置中文字体（用于代码注释，图表使用英文）
plt.rcParams['font.sans-serif'] = ['SimHei', 'Arial']
plt.rcParams['axes.unicode_minus'] = False


class DrugCombinationVisualizer:
    def __init__(self, results_file):
        self.df = pd.read_csv(results_file)
        # 筛选patient_count > 50的数据
        self.df_filtered = self.df[self.df['patient_count'] > 50].copy()
        self.setup_style()

    def setup_style(self):
        """设置图表样式"""
        plt.style.use('seaborn-v0_8')
        sns.set_palette("husl")

    def plot_top_combinations_score(self, top_n=15, use_filtered=False):
        """绘制综合评分前N的药物组合"""
        if use_filtered:
            data = self.df_filtered
            title_suffix = ' (Patient Count > 50)'
            filename_suffix = '_filtered'
        else:
            data = self.df
            title_suffix = ' (All Combinations)'
            filename_suffix = '_all'

        plt.figure(figsize=(14, 8))
        top_df = data.nlargest(top_n, 'comprehensive_score')

        # 创建组合名称
        combinations = []
        for _, row in top_df.iterrows():
            combo_name = f"Base{row['Base_Drug_Combo']}-Comp_INI{row['Comp_INI']}-Comp_NNRTI{row['Comp_NNRTI']}-Extra_PI{row['Extra_PI']}-Extra_pk_En{row['Extra_pk_En']}"
            combinations.append(combo_name)

        # 绘制条形图
        colors = plt.cm.viridis(np.linspace(0, 1, len(combinations)))
        if use_filtered:
            colors = plt.cm.plasma(np.linspace(0, 1, len(combinations)))

        bars = plt.barh(range(len(combinations)), top_df['comprehensive_score'],
                        color=colors)

        # 添加数据标签
        for i, (bar, score, count, patient_count) in enumerate(zip(bars, top_df['comprehensive_score'],
                                                                   top_df['count'], top_df['patient_count'])):
            plt.text(score + 0.01, i, f'{score:.3f}\n({patient_count} patients, {count} records)',
                     va='center', fontsize=8)

        plt.yticks(range(len(combinations)), combinations, fontsize=10)
        plt.xlabel('Comprehensive Score', fontsize=12)
        plt.title(f'Top {top_n} Drug Combinations by Comprehensive Score{title_suffix}',
                  fontsize=14, fontweight='bold')
        plt.gca().invert_yaxis()
        plt.tight_layout()
        plt.savefig(f'top_combinations_score{filename_suffix}.png', dpi=300, bbox_inches='tight')
        plt.show()

        # 打印筛选信息
        if use_filtered:
            print(f"Filtered dataset: {len(self.df_filtered)} combinations (patient_count > 50)")
            print(f"Original dataset: {len(self.df)} combinations")
        else:
            print(f"Original dataset: {len(self.df)} combinations")

    def plot_indicators_heatmap(self, top_n=20, use_filtered=True):
        """绘制各指标的热力图"""
        data = self.df_filtered if use_filtered else self.df
        top_df = data.nlargest(top_n, 'comprehensive_score')

        # 提取指标数据
        indicator_cols = ['avg_S_persistence', 'avg_S_slope', 'avg_S_suppression',
                          'avg_S_immunity', 'avg_S_burden', 'avg_S_stability', 'avg_S_smoothness']
        indicator_names = ['Persistence', 'Slope', 'Suppression', 'Immunity',
                           'Burden', 'Stability', 'Smoothness']

        indicator_data = top_df[indicator_cols]
        indicator_data.columns = indicator_names

        # 创建组合标签
        combo_labels = []
        for _, row in top_df.iterrows():
            label = f"Base{row['Base_Drug_Combo']}-Comp_INI{row['Comp_INI']}-Comp_NNRTI{row['Comp_NNRTI']}-Extra_PI{row['Extra_PI']}-Extra_pk_En{row['Extra_pk_En']}-P{row['patient_count']}"
            combo_labels.append(label)

        plt.figure(figsize=(14, 10))
        sns.heatmap(indicator_data.T, annot=True, cmap='YlOrRd',
                    xticklabels=combo_labels, yticklabels=indicator_names,
                    cbar_kws={'label': 'Score Value'}, fmt='.3f')

        title_suffix = ' (Patient Count > 50)' if use_filtered else ' (All Combinations)'
        plt.title(f'Performance Indicators Heatmap for Top {top_n} Drug Combinations{title_suffix}',
                  fontsize=14, fontweight='bold')
        plt.xticks(rotation=45, ha='right')
        plt.tight_layout()

        filename_suffix = '_filtered' if use_filtered else '_all'
        plt.savefig(f'indicators_heatmap{filename_suffix}.png', dpi=300, bbox_inches='tight')
        plt.show()

    def plot_patient_count_vs_score(self, use_filtered=True):
        """绘制患者数量与综合评分的关系图"""
        data = self.df_filtered if use_filtered else self.df

        plt.figure(figsize=(12, 8))

        # 使用气泡图显示关系
        scatter = plt.scatter(data['patient_count'], data['comprehensive_score'],
                              c=data['comprehensive_score'], cmap='viridis',
                              s=data['count'] / 100, alpha=0.7)  # 气泡大小表示数据量

        plt.colorbar(scatter, label='Comprehensive Score')
        plt.xlabel('Number of Patients', fontsize=12)
        plt.ylabel('Comprehensive Score', fontsize=12)

        title_suffix = ' (Patient Count > 50)' if use_filtered else ' (All Combinations)'
        plt.title(f'Relationship between Patient Count and Comprehensive Score{title_suffix}',
                  fontsize=14, fontweight='bold')

        # 添加趋势线
        if len(data) > 1:
            z = np.polyfit(data['patient_count'], data['comprehensive_score'], 1)
            p = np.poly1d(z)
            plt.plot(data['patient_count'], p(data['patient_count']), "r--", alpha=0.8,
                     label=f'Trend: y = {z[0]:.4f}x + {z[1]:.4f}')
            plt.legend()

        plt.grid(True, alpha=0.3)
        plt.tight_layout()

        filename_suffix = '_filtered' if use_filtered else '_all'
        plt.savefig(f'patient_count_vs_score{filename_suffix}.png', dpi=300, bbox_inches='tight')
        plt.show()

    def plot_indicators_distribution(self, use_filtered=True):
        """绘制各指标的分布图"""
        data = self.df_filtered if use_filtered else self.df

        indicator_cols = ['avg_S_persistence', 'avg_S_slope', 'avg_S_suppression',
                          'avg_S_immunity', 'avg_S_burden', 'avg_S_stability', 'avg_S_smoothness']
        indicator_names = ['Persistence', 'Slope', 'Suppression', 'Immunity',
                           'Burden', 'Stability', 'Smoothness']

        fig, axes = plt.subplots(2, 4, figsize=(16, 10))
        axes = axes.flatten()

        for i, (col, name) in enumerate(zip(indicator_cols, indicator_names)):
            axes[i].hist(data[col], bins=20, alpha=0.7, color=plt.cm.Set3(i / 7))
            axes[i].set_title(f'{name} Distribution', fontweight='bold')
            axes[i].set_xlabel('Score')
            axes[i].set_ylabel('Frequency')
            axes[i].grid(True, alpha=0.3)

            # 添加统计信息
            mean_val = data[col].mean()
            axes[i].axvline(mean_val, color='red', linestyle='--',
                            label=f'Mean: {mean_val:.3f}')
            axes[i].legend()

        # 移除多余的子图
        for i in range(len(indicator_cols), len(axes)):
            fig.delaxes(axes[i])

        title_suffix = ' (Patient Count > 50)' if use_filtered else ' (All Combinations)'
        plt.suptitle(f'Distribution of Performance Indicators{title_suffix}',
                     fontsize=16, fontweight='bold')
        plt.tight_layout()

        filename_suffix = '_filtered' if use_filtered else '_all'
        plt.savefig(f'indicators_distribution{filename_suffix}.png', dpi=300, bbox_inches='tight')
        plt.show()

    def plot_base_combo_comparison(self, use_filtered=True):
        """比较不同基础药物组合的表现"""
        data = self.df_filtered if use_filtered else self.df

        plt.figure(figsize=(14, 8))

        # 按基础药物组合分组计算平均评分
        base_combo_stats = data.groupby('Base_Drug_Combo').agg({
            'comprehensive_score': ['mean', 'std', 'count'],
            'patient_count': 'sum',
            'count': 'sum'
        }).round(4)

        base_combo_stats.columns = ['avg_score', 'std_score', 'combo_count', 'total_patients', 'total_records']
        base_combo_stats = base_combo_stats.sort_values('avg_score', ascending=False)

        # 绘制条形图
        bars = plt.bar(range(len(base_combo_stats)), base_combo_stats['avg_score'],
                       yerr=base_combo_stats['std_score'], capsize=5, alpha=0.7,
                       color=plt.cm.Paired(np.arange(len(base_combo_stats)) / len(base_combo_stats)))

        plt.xlabel('Base Drug Combination', fontsize=12)
        plt.ylabel('Average Comprehensive Score', fontsize=12)

        title_suffix = ' (Patient Count > 50)' if use_filtered else ' (All Combinations)'
        plt.title(f'Performance Comparison by Base Drug Combination{title_suffix}',
                  fontsize=14, fontweight='bold')

        # 设置x轴标签
        x_labels = []
        for idx, row in base_combo_stats.iterrows():
            label = f'Base {idx}\n({row.total_patients} patients)'
            x_labels.append(label)

        plt.xticks(range(len(base_combo_stats)), x_labels, rotation=45, ha='right')

        # 添加数据标签
        for i, (idx, row) in enumerate(base_combo_stats.iterrows()):
            plt.text(i, row.avg_score + 0.01, f'{row.avg_score:.3f}',
                     ha='center', va='bottom', fontweight='bold')

        plt.tight_layout()

        filename_suffix = '_filtered' if use_filtered else '_all'
        plt.savefig(f'base_combo_comparison{filename_suffix}.png', dpi=300, bbox_inches='tight')
        plt.show()

    def plot_correlation_matrix(self, use_filtered=True):
        """绘制指标间的相关性矩阵"""
        data = self.df_filtered if use_filtered else self.df

        indicator_cols = ['avg_S_persistence', 'avg_S_slope', 'avg_S_suppression',
                          'avg_S_immunity', 'avg_S_burden', 'avg_S_stability', 'avg_S_smoothness',
                          'comprehensive_score', 'patient_count', 'count']
        indicator_names = ['Persistence', 'Slope', 'Suppression', 'Immunity', 'Burden',
                           'Stability', 'Smoothness', 'Overall Score', 'Patients', 'Records']

        corr_data = data[indicator_cols]
        corr_data.columns = indicator_names
        correlation_matrix = corr_data.corr()

        plt.figure(figsize=(12, 10))
        mask = np.triu(np.ones_like(correlation_matrix, dtype=bool))
        sns.heatmap(correlation_matrix, mask=mask, annot=True, cmap='coolwarm',
                    center=0, square=True, fmt='.2f')

        title_suffix = ' (Patient Count > 50)' if use_filtered else ' (All Combinations)'
        plt.title(f'Correlation Matrix of Indicators and Metrics{title_suffix}',
                  fontsize=14, fontweight='bold')
        plt.tight_layout()

        filename_suffix = '_filtered' if use_filtered else '_all'
        plt.savefig(f'correlation_matrix{filename_suffix}.png', dpi=300, bbox_inches='tight')
        plt.show()

    def plot_component_analysis(self, use_filtered=True):
        """分析各药物组件的效果"""
        data = self.df_filtered if use_filtered else self.df

        fig, axes = plt.subplots(2, 2, figsize=(15, 10))
        components = ['Comp_INI', 'Comp_NNRTI', 'Extra_PI', 'Extra_pk_En']
        component_names = ['INI Component', 'NNRTI Component', 'Extra PI', 'Extra pk-En']

        for i, (comp, name) in enumerate(zip(components, component_names)):
            ax = axes[i // 2, i % 2]

            # 统计每个组件值对应的平均评分
            comp_stats = data.groupby(comp).agg({
                'comprehensive_score': ['mean', 'std', 'count'],
                'patient_count': 'sum'
            }).round(4)
            comp_stats.columns = ['avg_score', 'std_score', 'combo_count', 'total_patients']

            # 只保留有足够数据的组
            comp_stats = comp_stats[comp_stats['total_patients'] > 10]

            if len(comp_stats) > 0:
                # 绘制条形图
                bars = ax.bar(comp_stats.index.astype(str), comp_stats['avg_score'],
                              yerr=comp_stats['std_score'], capsize=5, alpha=0.7,
                              color=plt.cm.Set2(i / 4))

                ax.set_title(f'{name} Impact on Score', fontweight='bold')
                ax.set_xlabel(f'{name} Value')
                ax.set_ylabel('Average Comprehensive Score')
                ax.grid(True, alpha=0.3)

                # 添加数据标签
                for j, (idx, row) in enumerate(comp_stats.iterrows()):
                    ax.text(j, row.avg_score + 0.01, f'{row.avg_score:.3f}\n({row.total_patients}P)',
                            ha='center', va='bottom', fontsize=8)

        title_suffix = ' (Patient Count > 50)' if use_filtered else ' (All Combinations)'
        plt.suptitle(f'Impact of Drug Components on Treatment Effectiveness{title_suffix}',
                     fontsize=16, fontweight='bold')
        plt.tight_layout()

        filename_suffix = '_filtered' if use_filtered else '_all'
        plt.savefig(f'component_analysis{filename_suffix}.png', dpi=300, bbox_inches='tight')
        plt.show()

    def plot_top_combinations_radar(self, top_n=8, use_filtered=True):
        """绘制前N名组合的雷达图"""
        from math import pi

        data = self.df_filtered if use_filtered else self.df
        top_df = data.nlargest(top_n, 'comprehensive_score')

        if len(top_df) == 0:
            print("No data available for radar chart")
            return

        indicator_cols = ['avg_S_persistence', 'avg_S_slope', 'avg_S_suppression',
                          'avg_S_immunity', 'avg_S_burden', 'avg_S_stability', 'avg_S_smoothness']
        indicator_names = ['Persistence', 'Slope', 'Suppression', 'Immunity',
                           'Burden', 'Stability', 'Smoothness']

        # 设置雷达图角度
        angles = [n / float(len(indicator_names)) * 2 * pi for n in range(len(indicator_names))]
        angles += angles[:1]  # 闭合图形

        fig, ax = plt.subplots(figsize=(12, 12), subplot_kw=dict(projection='polar'))

        colors = plt.cm.tab10(np.linspace(0, 1, len(top_df)))

        for i, (idx, row) in enumerate(top_df.iterrows()):
            values = row[indicator_cols].tolist()
            values += values[:1]  # 闭合图形

            # 创建组合标签
            combo_label = f"Base{row['Base_Drug_Combo']}-Comp_INI{row['Comp_INI']}-Comp_NNRTI{row['Comp_NNRTI']}-Extra_PI{row['Extra_PI']}-Extra_pk_En{row['Extra_pk_En']}({row['patient_count']}P)"

            ax.plot(angles, values, 'o-', linewidth=2, label=combo_label, color=colors[i])
            ax.fill(angles, values, alpha=0.1, color=colors[i])

        # 设置坐标轴
        ax.set_theta_offset(pi / 2)
        ax.set_theta_direction(-1)
        ax.set_xticks(angles[:-1])
        ax.set_xticklabels(indicator_names)

        # 设置y轴标签
        ax.set_ylim(0, 1)
        ax.set_yticks([0.2, 0.4, 0.6, 0.8, 1.0])
        ax.grid(True)

        title_suffix = ' (Patient Count > 50)' if use_filtered else ' (All Combinations)'
        plt.title(f'Radar Chart of Top {len(top_df)} Drug Combinations{title_suffix}',
                  fontsize=16, fontweight='bold', pad=20)
        plt.legend(bbox_to_anchor=(1.2, 1.0), loc='upper left')
        plt.tight_layout()

        filename_suffix = '_filtered' if use_filtered else '_all'
        plt.savefig(f'radar_chart{filename_suffix}.png', dpi=300, bbox_inches='tight')
        plt.show()

    def generate_comprehensive_report(self):
        """生成综合报告"""
        print("=== Drug Combination Analysis Report ===")
        print(f"Total number of drug combinations: {len(self.df)}")
        print(f"Number of combinations with patient_count > 50: {len(self.df_filtered)}")
        print(f"Percentage of combinations retained: {len(self.df_filtered) / len(self.df) * 100:.1f}%")
        print(f"Total number of patient records (all): {self.df['count'].sum():,}")
        print(f"Total number of unique patients (all): {self.df['patient_count'].sum():,}")
        print(f"Total number of patient records (filtered): {self.df_filtered['count'].sum():,}")
        print(f"Total number of unique patients (filtered): {self.df_filtered['patient_count'].sum():,}")
        print(f"Average comprehensive score (all): {self.df['comprehensive_score'].mean():.4f}")
        print(f"Average comprehensive score (filtered): {self.df_filtered['comprehensive_score'].mean():.4f}")

        # 显示前5名组合（全部数据）
        print("\n=== Top 5 Drug Combinations (All Data) ===")
        top_5_all = self.df.nlargest(5, 'comprehensive_score')[
            ['Base_Drug_Combo', 'Comp_INI', 'Comp_NNRTI', 'Extra_PI', 'Extra_pk_En',
             'comprehensive_score', 'patient_count', 'count']
        ]
        print(top_5_all.to_string(index=False))

        # 显示前5名组合（筛选后数据）
        print("\n=== Top 5 Drug Combinations (Patient Count > 50) ===")
        if len(self.df_filtered) >= 5:
            top_5_filtered = self.df_filtered.nlargest(5, 'comprehensive_score')[
                ['Base_Drug_Combo', 'Comp_INI', 'Comp_NNRTI', 'Extra_PI', 'Extra_pk_En',
                 'comprehensive_score', 'patient_count', 'count']
            ]
            print(top_5_filtered.to_string(index=False))
        else:
            print("Not enough combinations with patient_count > 50")


def main():
    # 初始化可视化器
    visualizer = DrugCombinationVisualizer('drug_combination_analysis_results.csv')

    # 生成综合报告
    visualizer.generate_comprehensive_report()

    # 第一阶段：展示未筛选的所有组合图表
    print("\n" + "=" * 60)
    print("PHASE 1: Visualizing All Drug Combinations (No Filter)")
    print("=" * 60)

    # 1. 未筛选的综合评分前N的药物组合
    visualizer.plot_top_combinations_score(top_n=15, use_filtered=False)

    # 第二阶段：展示筛选后的图表（patient_count > 50）
    print("\n" + "=" * 60)
    print("PHASE 2: Visualizing Filtered Drug Combinations (Patient Count > 50)")
    print("=" * 60)

    # 2. 筛选后的综合评分前N的药物组合
    visualizer.plot_top_combinations_score(top_n=15, use_filtered=True)

    # 3. 指标热力图（筛选后）
    visualizer.plot_indicators_heatmap(top_n=15, use_filtered=True)

    # 4. 患者数量与评分关系图（筛选后）
    visualizer.plot_patient_count_vs_score(use_filtered=True)

    # 5. 指标分布图（筛选后）
    visualizer.plot_indicators_distribution(use_filtered=True)

    # 6. 基础药物组合比较（筛选后）
    visualizer.plot_base_combo_comparison(use_filtered=True)

    # 7. 相关性矩阵（筛选后）
    visualizer.plot_correlation_matrix(use_filtered=True)

    # 8. 药物组件分析（筛选后）
    visualizer.plot_component_analysis(use_filtered=True)

    # 9. 雷达图（筛选后）
    visualizer.plot_top_combinations_radar(top_n=8, use_filtered=True)

    print("\nAll visualizations completed!")
    print("Files generated:")
    print("- top_combinations_score_all.png (all combinations)")
    print("- top_combinations_score_filtered.png (patient_count > 50)")
    print("- indicators_heatmap_filtered.png")
    print("- patient_count_vs_score_filtered.png")
    print("- indicators_distribution_filtered.png")
    print("- base_combo_comparison_filtered.png")
    print("- correlation_matrix_filtered.png")
    print("- component_analysis_filtered.png")
    print("- radar_chart_filtered.png")


if __name__ == "__main__":
    main()
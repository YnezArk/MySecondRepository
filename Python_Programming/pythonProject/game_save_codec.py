#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import json
import urllib.parse
import re
import sys
import os


# ======================
# 解码：原始编码 → 中文 JSON
# ======================
def decode_game_save(content: str) -> str:
    """将 %uXXXX + URL 编码的字符串解码为可读的中文 JSON"""
    # 1. 将 %uXXXX 替换为 \\uXXXX（用于 JSON 解析）
    fixed = re.sub(r'%u([0-9A-Fa-f]{4})', r'\\u\1', content)
    # 2. 标准 URL 解码（%7B → {, %22 → " 等）
    url_decoded = urllib.parse.unquote(fixed)
    # 3. 用 JSON 解析自动还原 Unicode
    obj = json.loads(url_decoded)
    return json.dumps(obj, ensure_ascii=False, indent=2)


# ======================
# 编码：中文 JSON → 原始编码
# ======================
def encode_game_save(content: str) -> str:
    """将中文 JSON 编码为游戏原始的 %uXXXX + URL 编码格式"""
    # 确保输入是合法 JSON
    obj = json.loads(content)
    # 转为 \uXXXX 形式的紧凑 JSON 字符串
    json_str = json.dumps(obj, ensure_ascii=True, separators=(',', ':'))

    # 临时替换 \uXXXX 为标记，防止被 quote 二次编码
    temp_str = re.sub(r'\\u([0-9A-Fa-f]{4})', r'__U\1__', json_str)
    # URL 编码（safe='' 表示连 / : 都编码，符合原始存档风格）
    quoted = urllib.parse.quote(temp_str, safe='')
    # 恢复为 %uXXXX
    final = re.sub(r'__U([0-9A-Fa-f]{4})__', r'%u\1', quoted)
    return final


# ======================
# 主程序
# ======================
def main():
    if len(sys.argv) != 3:
        print("用法:")
        print("  解码: python game_save_codec.py decode <输入文件>")
        print("  编码: python game_save_codec.py encode <输入文件>")
        print("\n输出文件将自动命名为: <原文件名>_{decoded|encoded}.txt")
        sys.exit(1)

    mode = sys.argv[1].lower()
    input_path = sys.argv[2]

    if mode not in ('decode', 'encode'):
        print("错误: 模式必须是 'decode' 或 'encode'")
        sys.exit(1)

    if not os.path.isfile(input_path):
        print(f"错误: 文件不存在 '{input_path}'")
        sys.exit(1)

    # 读取输入
    with open(input_path, 'r', encoding='utf-8') as f:
        content = f.read()

    # 处理
    try:
        if mode == 'decode':
            result = decode_game_save(content)
            suffix = 'decoded'
        else:  # encode
            result = encode_game_save(content)
            suffix = 'encoded'
    except Exception as e:
        print(f"❌ 处理失败: {e}", file=sys.stderr)
        sys.exit(1)

    # 写入输出
    base, _ = os.path.splitext(input_path)
    output_path = f"{base}_{suffix}.txt"
    with open(output_path, 'w', encoding='utf-8') as f:
        f.write(result)

    print(f"✅ 成功! 输出文件: {output_path}")


if __name__ == "__main__":
    main()
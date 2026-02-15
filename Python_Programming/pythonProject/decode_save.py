#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import urllib.parse
import json
import re
import sys
import os

def fix_percent_u(content: str) -> str:
    """将 %uXXXX 替换为 \\uXXXX（用于后续 JSON 解析）"""
    return re.sub(r'%u([0-9A-Fa-f]{4})', r'\\u\1', content)

def decode_full(content: str) -> str:
    # 1. 修复 %uXXXX
    fixed = fix_percent_u(content)
    # 2. 标准 URL 解码
    url_decoded = urllib.parse.unquote(fixed)
    # 3. 尝试作为 JSON 解析
    try:
        obj = json.loads(url_decoded)
        return json.dumps(obj, ensure_ascii=False, indent=2)
    except json.JSONDecodeError:
        # 若失败，尝试作为纯字符串解析 Unicode
        try:
            s = json.loads('"' + url_decoded.replace('"', r'\"').replace('\\', r'\\') + '"')
            return s
        except:
            return url_decoded

def main():
    if len(sys.argv) != 2:
        print("用法: python decode_save.py <输入文件>")
        sys.exit(1)

    input_path = sys.argv[1]
    if not os.path.isfile(input_path):
        print(f"错误: 文件不存在 {input_path}")
        sys.exit(1)

    with open(input_path, 'r', encoding='utf-8') as f:
        raw = f.read()

    result = decode_full(raw)

    output_path = os.path.splitext(input_path)[0] + "_decoded.txt"
    with open(output_path, 'w', encoding='utf-8') as f:
        f.write(result)

    print(f"✅ 已保存到: {output_path}")

if __name__ == "__main__":
    main()
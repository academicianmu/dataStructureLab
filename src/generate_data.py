#!/usr/bin/env python3
import random

# 生成50,000个随机数字
def generate_data():
    numbers = []
    for i in range(50000):
        # 生成1到100,000之间的随机数
        num = random.randint(1, 100000)
        numbers.append(str(num))
    
    # 写入文件，用逗号分隔
    with open('50000_numbers_data.txt', 'w') as f:
        f.write(','.join(numbers))
    
    print(f"已生成50,000个随机数据到 50000_numbers_data.txt")

if __name__ == "__main__":
    generate_data()

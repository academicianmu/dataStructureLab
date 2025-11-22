# 数据结构实验平台

[![Qt](https://img.shields.io/badge/Qt-5.x-green.svg)](https://www.qt.io/)
[![C++](https://img.shields.io/badge/C++-11-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.5+-blue.svg)](https://cmake.org/)

一个基于Qt框架开发的数据结构算法可视化实验平台，包含多个经典数据结构算法的图形化实现。

## 📋 项目概述

本项目是东北大学数据结构课程实验的综合实现，采用模块化设计思想，不使用STL库，所有基础数据结构均由团队成员自主实现。平台提供直观的图形界面，支持算法可视化演示和性能分析。

### 🎯 实验内容

平台包含以下四个核心实验模块：

1. **约瑟夫环问题** - 双向循环链表实现
2. **四则表达式求值** - 中缀转后缀表达式并求值
3. **校园光纤管网铺设** - 最小生成树算法（Prim/Kruskal）
4. **快速排序与归并排序比较** - 排序算法性能分析

## 🏗️ 系统架构

### 核心数据结构库

- **顺序表** (`SqList`) - 动态数组实现
- **双向链表** (`DoubleList`) - 双向循环链表
- **双端队列** (`LinkDeque`) - 队列的扩展实现
- **栈结构** (`SqStack`, `DStack`) - 顺序栈和双精度栈
- **图结构** (`MGraph`) - 邻接矩阵表示

### 技术栈

- **编程语言**: C++11
- **GUI框架**: Qt5 (Widgets, Core)
- **构建系统**: CMake 3.5+
- **开发平台**: Windows (支持跨平台)

## 🚀 快速开始

### 环境要求

- Qt 5.x 或更高版本
- CMake 3.5 或更高版本
- C++11 兼容的编译器 (GCC/Clang/MSVC)

### 编译安装

1. **克隆仓库**
   ```bash
   git clone https://github.com/your-username/datastructure.git
   cd datastructure
   ```

2. **创建构建目录**
   ```bash
   mkdir build
   cd build
   ```

3. **配置项目**
   ```bash
   cmake ..
   ```

4. **编译**
   ```bash
   cmake --build . --config Release
   ```

5. **运行程序**
   ```bash
   # Windows
   ./DataStructureLab.exe
   
   # Linux/macOS
   ./DataStructureLab
   ```

## 📖 功能模块详解

### 1. 约瑟夫环问题

**算法特点**:
- 使用双向循环链表实现
- 支持顺时针/逆时针交替轮转
- 实时可视化出列过程

**核心功能**:
- 自定义人数N和步长M
- 动态演示出列过程
- 显示最终优胜者和完整出列序列

### 2. 表达式求值

**算法特点**:
- 中缀表达式转后缀表达式
- 后缀表达式求值
- 支持四则运算和括号

**核心功能**:
- 实时表达式解析
- 步骤可视化
- 错误检测和提示

### 3. 最小生成树

**算法特点**:
- Prim算法实现
- Kruskal算法实现
- 图形化展示生成过程

**核心功能**:
- 校园地图建模
- 边权重可视化
- 最优路径高亮显示

### 4. 排序算法比较

**算法特点**:
- 快速排序实现
- 归并排序实现
- 性能指标对比

**核心功能**:
- 大数据集测试 (50000条记录)
- 时间复杂度分析
- 可视化排序过程

## 📁 项目结构

```
datastructure/
├── CMakeLists.txt          # CMake构建配置
├── README.md              # 项目说明文档
├── src/                   # 源代码目录
│   ├── main.cpp           # 程序入口
│   ├── main_window.cpp    # 主窗口实现
│   ├── *.cpp              # 各模块实现文件
│   └── *.h                # 头文件
├── include/               # 数据结构库头文件
│   ├── linkedlist.h       # 链表实现
│   ├── stack.h            # 栈实现
│   ├── sequencelist.h     # 顺序表实现
│   └── ...                # 其他数据结构
├── ui/                    # UI文件目录
└── build/                 # 构建输出目录
```

## 🔧 开发指南

### 添加新算法模块

1. **创建数据结构** - 在`include/`目录下添加头文件
2. **实现算法逻辑** - 在`src/`目录下添加实现文件
3. **创建GUI界面** - 继承Qt窗口类
4. **注册到主程序** - 在`main_window.cpp`中添加按钮和事件处理

### 代码规范

- 遵循Qt编码规范
- 使用驼峰命名法
- 添加必要的注释
- 模块化设计，低耦合高内聚

## 📊 性能分析

### 时间复杂度对比

| 算法 | 平均时间复杂度 | 最坏时间复杂度 | 空间复杂度 |
|------|----------------|----------------|------------|
| 快速排序 | O(n log n) | O(n²) | O(log n) |
| 归并排序 | O(n log n) | O(n log n) | O(n) |
| Prim算法 | O(V²) | O(V²) | O(V) |
| Kruskal算法 | O(E log E) | O(E log E) | O(E) |

### 测试结果

基于50000条随机数据的性能测试结果详见实验报告。

## 🎨 界面截图

### 主界面
![主界面](src/主页面.png)

### 约瑟夫环演示
![约瑟夫环](src/约瑟夫环.png)

### 表达式求值
![表达式求值](src/表达式求值.png)

### 快速排序
![快速排序](src/快速排序.png)

### 最小生成树
![最小生成树](src/光纤.png)

## 📚 实验报告

详细的实验报告包括：
- 问题定义及需求分析
- 概要设计和详细设计
- 算法复杂度分析
- 测试结果和性能评估
- 使用说明和源代码

报告文档：[报告.pdf](src/报告.pdf)

## 👥 开发团队

- **孙嘉苧**
- **马妍**
- **牟彦泽**

## 🤝 贡献指南

欢迎提交 Issue 和 Pull Request！

1. Fork 本仓库
2. 创建特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启 Pull Request

## 📞 联系方式

如有问题或建议，请通过以下方式联系：

- 提交 Issue: [GitHub Issues](https://github.com/your-username/datastructure/issues)
- 邮箱: mouyz@mails.neu.edu.cn

## 🙏 致谢

感谢东北大学计算机科学与技术学院提供的数据结构课程实验平台，以及Qt社区提供的优秀GUI框架。

---

⭐ 如果这个项目对你有帮助，请给它一个星标！

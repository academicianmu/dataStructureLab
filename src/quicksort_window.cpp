#include "quicksort_window.h"
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QRandomGenerator>
#include <chrono>
#include <algorithm>
#include <iostream>

QuicksortWindow::QuicksortWindow(QWidget *parent) : QDialog(parent) {
    setWindowTitle("快速排序算法性能比较");
    setMinimumSize(800, 600);
    
    setupUI();
    resetPerformanceCounters();
}

void QuicksortWindow::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 数据加载区域
    QGroupBox *dataGroup = new QGroupBox("数据管理", this);
    QVBoxLayout *dataLayout = new QVBoxLayout(dataGroup);
    
    QLabel *infoLabel = new QLabel("点击'加载测试数据'从文件读取50,000个数据，然后选择测试大小进行算法性能比较", this);
    loadDataButton = new QPushButton("加载测试数据", this);
    
    // 数据大小选择
    QLabel *sizeLabel = new QLabel("选择测试数据大小:", this);
    dataSizeComboBox = new QComboBox(this);
    dataSizeComboBox->addItems({"100", "1000", "10000", "50000"});
    dataSizeComboBox->setCurrentIndex(1); // 默认选择1000
    dataSizeComboBox->setEnabled(false); // 初始禁用，加载后启用
    
    dataLayout->addWidget(infoLabel);
    dataLayout->addWidget(loadDataButton);
    dataLayout->addWidget(sizeLabel);
    dataLayout->addWidget(dataSizeComboBox);

    // 算法选择区域
    QGroupBox *algorithmGroup = new QGroupBox("算法选择", this);
    QVBoxLayout *algorithmLayout = new QVBoxLayout(algorithmGroup);
    
    quicksortRadio = new QRadioButton("仅测试快速排序", this);
    mergeSortRadio = new QRadioButton("仅测试归并排序", this);
    bothCompareRadio = new QRadioButton("比较两种算法", this);
    bothCompareRadio->setChecked(true); // 默认选择比较两种算法
    
    algorithmLayout->addWidget(quicksortRadio);
    algorithmLayout->addWidget(mergeSortRadio);
    algorithmLayout->addWidget(bothCompareRadio);
    
    compareButton = new QPushButton("开始测试", this);
    compareButton->setEnabled(false); // 初始禁用
    
    algorithmLayout->addWidget(compareButton);

    // 结果显示区域
    QGroupBox *resultGroup = new QGroupBox("测试结果", this);
    QVBoxLayout *resultLayout = new QVBoxLayout(resultGroup);
    
    resultLabel = new QLabel("结果: 请先加载数据", this);
    resultLabel->setWordWrap(true);
    performanceEdit = new QTextEdit(this);
    performanceEdit->setReadOnly(true);
    performanceEdit->setMaximumHeight(300);
    
    clearButton = new QPushButton("清空结果", this);
    
    resultLayout->addWidget(resultLabel);
    resultLayout->addWidget(performanceEdit);
    resultLayout->addWidget(clearButton);

    mainLayout->addWidget(dataGroup);
    mainLayout->addWidget(algorithmGroup);
    mainLayout->addWidget(resultGroup);

    // 连接信号和槽
    connect(loadDataButton, &QPushButton::clicked, this, &QuicksortWindow::loadDataFromFile);
    connect(compareButton, &QPushButton::clicked, this, &QuicksortWindow::compareAlgorithms);
    connect(clearButton, &QPushButton::clicked, this, &QuicksortWindow::clearResults);
    connect(dataSizeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &QuicksortWindow::onDataSizeChanged);
}

void QuicksortWindow::clearResults() {
    resultLabel->setText("结果: 请先加载数据");
    performanceEdit->clear();
    resetPerformanceCounters();
}

void QuicksortWindow::onDataSizeChanged() {
    if (ListLength(originalData) > 0) {
        int selectedSize = dataSizeComboBox->currentText().toInt();
        prepareTestData(selectedSize);
        resultLabel->setText(QString("已选择 %1 个数据进行测试").arg(selectedSize));
    }
}

void QuicksortWindow::prepareTestData(int size) {
    // 清空测试数据
    ClearList(&testData);
    InitList(&testData);
    
    // 从原始数据中复制指定数量的数据
    int actualSize = qMin(size, ListLength(originalData));
    for (int i = 1; i <= actualSize; i++) {
        ElemType value;
        GetElem(originalData, i, &value);
        ListInsert(&testData, i, value);
    }
}

void QuicksortWindow::resetPerformanceCounters() {
    comparisonCount = 0;
    swapCount = 0;
    elapsedTime = 0;
}

bool QuicksortWindow::readDataFromFile(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();
    
    // 初始化SqList
    InitList(&originalData);
    
    // 尝试用逗号分隔
    QStringList commaList = content.split(",", Qt::SkipEmptyParts);
    for (const QString &str : commaList) {
        bool ok;
        int num = str.trimmed().toInt(&ok);
        if (ok) {
            ListInsert(&originalData, ListLength(originalData) + 1, num);
        }
    }
    
    // 如果用逗号分隔没有成功，尝试用空格分隔
    if (ListLength(originalData) == 0) {
        ClearList(&originalData);
        QStringList spaceList = content.split(" ", Qt::SkipEmptyParts);
        for (const QString &str : spaceList) {
            bool ok;
            int num = str.trimmed().toInt(&ok);
            if (ok) {
                ListInsert(&originalData, ListLength(originalData) + 1, num);
            }
        }
    }
    
    if (ListLength(originalData) == 0) {
        QMessageBox::warning(this, "数据错误", "文件中没有有效数据！");
        return false;
    }
    
    return true;
}

void QuicksortWindow::loadDataFromFile() {
    // 尝试多个可能的路径
    QStringList possiblePaths = {
        "../src/50000_numbers_data.txt"
    };
    
    bool loaded = false;
    for (const QString &path : possiblePaths) {
        if (readDataFromFile(path)) {
            resultLabel->setText(QString("成功加载 %1 个数据，请选择测试大小").arg(ListLength(originalData)));
            compareButton->setEnabled(true);
            dataSizeComboBox->setEnabled(true);
            loaded = true;
            break;
        }
    }
    
    if (!loaded) {
        QMessageBox::warning(this, "文件错误", "无法找到数据文件！请确保 50000_numbers_data.txt 文件在正确位置。");
    }
}

void QuicksortWindow::compareAlgorithms() {
    if (ListLength(originalData) == 0) {
        QMessageBox::warning(this, "数据错误", "请先加载数据！");
        return;
    }
    
    // 确保测试数据已准备
    int selectedSize = dataSizeComboBox->currentText().toInt();
    if (ListLength(testData) != selectedSize) {
        prepareTestData(selectedSize);
    }
    
    QString reportText;
    
    if (quicksortRadio->isChecked()) {
        // 仅测试快速排序
        reportText = runQuicksortTest();
    } else if (mergeSortRadio->isChecked()) {
        // 仅测试归并排序
        reportText = runMergeSortTest();
    } else {
        // 比较两种算法
        reportText = runComparisonTest();
    }
    
    performanceEdit->setText(reportText);
    resultLabel->setText(QString("测试完成！数据量: %1 个").arg(selectedSize));
}

QString QuicksortWindow::runQuicksortTest() {
    SqList testQuicksort;
    InitList(&testQuicksort);
    // 复制测试数据
    for (int i = 1; i <= ListLength(testData); i++) {
        ElemType value;
        GetElem(testData, i, &value);
        ListInsert(&testQuicksort, ListLength(testQuicksort) + 1, value);
    }
    
    resetPerformanceCounters();
    
    auto start = std::chrono::high_resolution_clock::now();
    basicQuickSort(testQuicksort, 0, ListLength(testQuicksort) - 1);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    double timeMs = duration.count() / 1000.0;
    
    int comparisons = comparisonCount;
    int swaps = swapCount;
    
    return QString("快速排序测试结果:\n\n"
                   "数据量: %1 个\n"
                   "执行时间: %2 毫秒\n"
                   "比较次数: %3\n"
                   "交换次数: %4\n"
                   "平均时间复杂度: O(n log n)")
           .arg(ListLength(testData))
           .arg(timeMs, 0, 'f', 3)
           .arg(comparisons)
           .arg(swaps);
}

QString QuicksortWindow::runMergeSortTest() {
    SqList testMerge;
    InitList(&testMerge);
    // 复制测试数据
    for (int i = 1; i <= ListLength(testData); i++) {
        ElemType value;
        GetElem(testData, i, &value);
        ListInsert(&testMerge, ListLength(testMerge) + 1, value);
    }
    
    resetPerformanceCounters();
    
    auto start = std::chrono::high_resolution_clock::now();
    mergeSort(testMerge, 0, ListLength(testMerge) - 1);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    double timeMs = duration.count() / 1000.0;
    
    int comparisons = comparisonCount;
    int swaps = swapCount;
    
    return QString("归并排序测试结果:\n\n"
                   "数据量: %1 个\n"
                   "执行时间: %2 毫秒\n"
                   "比较次数: %3\n"
                   "交换次数: %4\n"
                   "平均时间复杂度: O(n log n)")
           .arg(ListLength(testData))
           .arg(timeMs, 0, 'f', 3)
           .arg(comparisons)
           .arg(swaps);
}

QString QuicksortWindow::runComparisonTest() {
    // 测试快速排序
    SqList testQuicksort;
    InitList(&testQuicksort);
    for (int i = 1; i <= ListLength(testData); i++) {
        ElemType value;
        GetElem(testData, i, &value);
        ListInsert(&testQuicksort, ListLength(testQuicksort) + 1, value);
    }
    
    resetPerformanceCounters();
    auto quickStart = std::chrono::high_resolution_clock::now();
    basicQuickSort(testQuicksort, 0, ListLength(testQuicksort) - 1);
    auto quickEnd = std::chrono::high_resolution_clock::now();
    
    auto quickDuration = std::chrono::duration_cast<std::chrono::microseconds>(quickEnd - quickStart);
    double quickTimeMs = quickDuration.count() / 1000.0;
    int quickComparisons = comparisonCount;
    int quickSwaps = swapCount;
    
    // 测试归并排序
    SqList testMerge;
    InitList(&testMerge);
    for (int i = 1; i <= ListLength(testData); i++) {
        ElemType value;
        GetElem(testData, i, &value);
        ListInsert(&testMerge, ListLength(testMerge) + 1, value);
    }
    
    resetPerformanceCounters();
    auto mergeStart = std::chrono::high_resolution_clock::now();
    mergeSort(testMerge, 0, ListLength(testMerge) - 1);
    auto mergeEnd = std::chrono::high_resolution_clock::now();
    
    auto mergeDuration = std::chrono::duration_cast<std::chrono::microseconds>(mergeEnd - mergeStart);
    double mergeTimeMs = mergeDuration.count() / 1000.0;
    int mergeComparisons = comparisonCount;
    int mergeSwaps = swapCount;
    
    return QString("算法性能比较结果:\n\n"
                   "数据量: %1 个\n\n"
                   "=== 快速排序 ===\n"
                   "执行时间: %2 毫秒\n"
                   "比较次数: %3\n"
                   "交换次数: %4\n\n"
                   "=== 归并排序 ===\n"
                   "执行时间: %5 毫秒\n"
                   "比较次数: %6\n"
                   "交换次数: %7\n\n"
                   "=== 性能对比 ===\n"
                   "时间比(快速/归并): %8\n"
                   "比较次数比(快速/归并): %9\n"
                   "交换次数比(快速/归并): %10")
           .arg(ListLength(testData))
           .arg(quickTimeMs, 0, 'f', 3)
           .arg(quickComparisons)
           .arg(quickSwaps)
           .arg(mergeTimeMs, 0, 'f', 3)
           .arg(mergeComparisons)
           .arg(mergeSwaps)
           .arg(QString::number(quickTimeMs / mergeTimeMs, 'f', 2))
           .arg(QString::number(double(quickComparisons) / mergeComparisons, 'f', 2))
           .arg(QString::number(double(quickSwaps) / mergeSwaps, 'f', 2));
}

void QuicksortWindow::generateComparisonReport(double basicTime, double mergeTime, int basicComparisons, int mergeComparisons, int basicSwaps, int mergeSwaps) {
    QString report = "=== 排序算法性能比较报告 ===\n\n";
    report += QString("数据量: %1 个数字\n\n").arg(ListLength(originalData));
    // ... (其余代码保持不变)
    // 基础快速排序结果
    report += "【基础快速排序】\n";
    report += QString("执行时间: %1 毫秒\n").arg(basicTime, 0, 'f', 3);
    report += QString("比较次数: %1\n").arg(basicComparisons);
    report += QString("交换次数: %1\n").arg(basicSwaps);
    
    if (basicTime > 0) {
        double basicOpsPerMs = (double)(basicComparisons + basicSwaps) / basicTime;
        report += QString("每毫秒操作数: %1\n\n").arg(basicOpsPerMs, 0, 'f', 0);
    } else {
        report += "每毫秒操作数: N/A\n\n";
    }
    
    // 归并排序结果
    report += "【归并排序】\n";
    report += QString("执行时间: %1 毫秒\n").arg(mergeTime, 0, 'f', 3);
    report += QString("比较次数: %1\n").arg(mergeComparisons);
    report += QString("交换次数: %1\n").arg(mergeSwaps);
    
    if (mergeTime > 0) {
        double mergeOpsPerMs = (double)(mergeComparisons + mergeSwaps) / mergeTime;
        report += QString("每毫秒操作数: %1\n\n").arg(mergeOpsPerMs, 0, 'f', 0);
    } else {
        report += "每毫秒操作数: N/A\n\n";
    }
    
    // 性能对比
    report += "=== 性能对比 ===\n";
    if (basicTime > 0 && mergeTime > 0) {
        double timeRatio = basicTime / mergeTime;
        if (timeRatio > 1.1) {
            report += QString("归并排序比基础快速排序快 %1 倍\n").arg(timeRatio, 0, 'f', 2);
        } else if (timeRatio < 0.9) {
            report += QString("基础快速排序比归并排序快 %1 倍\n").arg(1.0/timeRatio, 0, 'f', 2);
        } else {
            report += "两种算法性能相近\n";
        }
    }
    
    double comparisonRatio = (double)basicComparisons / mergeComparisons;
    report += QString("比较次数比: %1\n").arg(comparisonRatio, 0, 'f', 2);
    
    double swapRatio = (double)basicSwaps / mergeSwaps;
    report += QString("交换次数比: %1\n\n").arg(swapRatio, 0, 'f', 2);
    
    // 算法分析
    report += "=== 算法分析 ===\n";
    report += "基础快速排序:\n";
    report += "- 优点: 原地排序，空间复杂度低，缓存友好\n";
    report += "- 缺点: 在特定数据下可能出现最坏情况 O(n²)\n";
    report += "- 空间复杂度: O(log n) (递归栈)\n";
    report += "- 适用场景: 一般随机数据，内存受限环境\n\n";
    
    report += "归并排序:\n";
    report += "- 优点: 时间复杂度稳定，性能可预测\n";
    report += "- 缺点: 需要额外空间 O(n)\n";
    report += "- 空间复杂度: O(n)\n";
    report += "- 适用场景: 对性能稳定性要求高的场景\n\n";
    
    // 结论
    report += "=== 结论 ===\n";
    if (mergeTime < basicTime) {
        report += "归并排序在此数据集上表现更好\n";
        report += "归并排序的稳定时间复杂度使其在大多数情况下都有良好表现\n";
    } else {
        report += "基础快速排序在此数据集上表现更好\n";
        report += "快速排序的原地排序特性使其在内存使用上更高效\n";
    }
    
    performanceEdit->setText(report);
}

// 基础快速排序算法
void QuicksortWindow::basicQuickSort(SqList &arr, int left, int right) {
    if (left < right) {
        int pivotIndex = basicPartition(arr, left, right);
        basicQuickSort(arr, left, pivotIndex - 1);
        basicQuickSort(arr, pivotIndex + 1, right);
    }
}

int QuicksortWindow::basicPartition(SqList &arr, int left, int right) {
    ElemType pivot = arr.data[right];  // 直接访问data数组
    int i = left;
    
    for (int j = left; j < right; j++) {
        comparisonCount++;
        if (arr.data[j] <= pivot) {  // 直接访问data数组
            // 交换arr.data[i]和arr.data[j]
            ElemType temp = arr.data[i];
            arr.data[i] = arr.data[j];
            arr.data[j] = temp;
            swapCount++;
            i++;
        }
    }
    
    // 交换arr.data[i]和arr.data[right]
    ElemType temp = arr.data[i];
    arr.data[i] = arr.data[right];
    arr.data[right] = temp;
    swapCount++;
    return i;
}

// 归并排序算法
void QuicksortWindow::mergeSort(SqList &arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void QuicksortWindow::merge(SqList &arr, int left, int mid, int right) {
    // 创建临时数组 - 直接使用静态数组
    static ElemType temp[20000];  // 使用静态数组避免动态分配
    
    int i = left;
    int j = mid + 1;
    int k = 0;
    
    // 合并两个有序数组
    while (i <= mid && j <= right) {
        comparisonCount++;
        if (arr.data[i] <= arr.data[j]) {  // 直接访问data数组
            temp[k++] = arr.data[i++];
        } else {
            temp[k++] = arr.data[j++];
        }
        swapCount++; // 统计数据移动
    }
    
    // 复制剩余元素
    while (i <= mid) {
        temp[k++] = arr.data[i++];
        swapCount++;
    }
    
    while (j <= right) {
        temp[k++] = arr.data[j++];
        swapCount++;
    }
    
    // 将临时数组复制回原数组
    for (int t = 0; t < k; t++) {
        arr.data[left + t] = temp[t];
        swapCount++;
    }
}


#ifndef QUICKSORT_WINDOW_H
#define QUICKSORT_WINDOW_H

#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QTextEdit>
#include <QElapsedTimer>
#include <QRandomGenerator>
#include <QFile>
#include <QTextStream>
#include <QComboBox>
#include <QRadioButton>
#include <QButtonGroup>

// 使用用户自定义数据结构
#include "../include/sequencelist.h"

class QuicksortWindow : public QDialog {
    Q_OBJECT

public:
    explicit QuicksortWindow(QWidget *parent = nullptr);
    virtual ~QuicksortWindow() override = default;

private slots:
    void compareAlgorithms();
    void loadDataFromFile();
    void clearResults();
    void onDataSizeChanged();

private:
    QLabel *resultLabel;
    QTextEdit *performanceEdit;
    QPushButton *loadDataButton;
    QPushButton *compareButton;
    QPushButton *clearButton;
    
    // 数据大小选择控件
    QComboBox *dataSizeComboBox;
    QButtonGroup *algorithmGroup;
    QRadioButton *quicksortRadio;
    QRadioButton *mergeSortRadio;
    QRadioButton *bothCompareRadio;
    
    // 性能统计
    int comparisonCount;
    int swapCount;
    qint64 elapsedTime;
    
    // 使用自定义SqList数据结构
    SqList originalData;
    SqList testData; // 用于测试的数据子集
    
    // 快速排序算法
    void basicQuickSort(SqList &arr, int left, int right);
    void mergeSort(SqList &arr, int left, int right);
    
    // 辅助函数
    int basicPartition(SqList &arr, int left, int right);
    void merge(SqList &arr, int left, int mid, int right);
    
    // 性能分析
    void resetPerformanceCounters();
    void generateComparisonReport(double basicTime, double mergeTime, int basicComparisons, int mergeComparisons, int basicSwaps, int mergeSwaps);
    
    // 界面设置
    void setupUI();
    bool readDataFromFile(const QString &filename);
    void prepareTestData(int size);
    
    // 测试函数
    QString runQuicksortTest();
    QString runMergeSortTest();
    QString runComparisonTest();
};

#endif // QUICKSORT_WINDOW_H
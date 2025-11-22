#ifndef JOSEPHUS_WINDOW_H
#define JOSEPHUS_WINDOW_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#include <QGroupBox>
#include <QTextEdit>
#include <QTimer>
#include <QPainter>
#include <QWidget>
#include <QList>
#include <QRadioButton>
#include <QButtonGroup>
#include <QVector>

// 使用用户自定义数据结构
#include "../include/sequencelist.h"
#include "../include/doublelinkedlist.h"
#include "../include/deque.h"

class JosephusWidget;

class JosephusWindow : public QDialog {
    Q_OBJECT

public:
    explicit JosephusWindow(QWidget *parent = nullptr);
    ~JosephusWindow();

private slots:
    void calculateJosephus();
    void loadPresetData();
    void startAnimation();
    void updateAnimation();
    void onAlgorithmChanged();
    void clearResults();

private:
    // UI 组件
    QLineEdit *nInput;
    QLineEdit *mInput;
    QLabel *resultLabel;
    QTextEdit *detailEdit;
    QComboBox *presetComboBox;
    QPushButton *presetButton;
    QPushButton *animateButton;
    QPushButton *clearButton;
    JosephusWidget *animationWidget;
    QTimer *animationTimer;
    QButtonGroup *algorithmGroup;
    QRadioButton *sequenceListRadio;
    QRadioButton *doublyLinkedListRadio;
    QRadioButton *staticLinkedListRadio;
    
    // 数据和状态
    QList<int> people;
    QList<int> eliminationOrder;
    int currentIndex;
    bool isAnimating;
    int stepCount; // 记录步数，用于判断奇偶
    
    // 使用用户自定义数据结构实现
    void josephusWithSequenceList(int n, int m);  // 使用自定义SqList
    void josephusWithDoublyLinkedList(int n, int m);  // 使用自定义DoubleList
    void josephusWithDeque(int n, int m);  // 使用自定义Deque
    
    // 界面设置
    void setupUI();
    void generateDetailedReport(const QString &algorithmName, int n, int m);
};

class JosephusWidget : public QWidget {
    Q_OBJECT

public:
    explicit JosephusWidget(QWidget *parent = nullptr);
    void setData(const QList<int> &people, int current, const QList<int> &eliminated, int direction);
    
protected:
    void paintEvent(QPaintEvent *event) override;
    
private:
    QList<int> people;
    int currentPerson;
    QList<int> eliminatedPeople;
    int rotationDirection; // 1: 顺时针, -1: 逆时针
};

#endif // JOSEPHUS_WINDOW_H
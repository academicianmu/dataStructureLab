#include "josephus_window.h"
#include <cmath>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QMessageBox>
#include <QGroupBox>
#include <QTextEdit>
#include <QVector>

JosephusWindow::JosephusWindow(QWidget *parent) : QDialog(parent), currentIndex(0), isAnimating(false), stepCount(0) {
    setWindowTitle("约瑟夫环问题 - 多数据结构实现");
    setMinimumSize(900, 700);
    
    setupUI();
}

JosephusWindow::~JosephusWindow() {
    // 清理双向循环链表
}

void JosephusWindow::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 输入区域
    QGroupBox *inputGroup = new QGroupBox("参数设置", this);
    QGridLayout *inputLayout = new QGridLayout(inputGroup);
    
    QLabel *nLabel = new QLabel("总人数 (n):", this);
    nInput = new QLineEdit(this);
    QLabel *mLabel = new QLabel("报数间隔 (m):", this);
    mInput = new QLineEdit(this);
    
    QLabel *presetLabel = new QLabel("预设数据:", this);
    presetComboBox = new QComboBox(this);
    presetComboBox->addItem("自定义");
    presetComboBox->addItem("测试数据: n=7, m=3");
    
    presetButton = new QPushButton("加载预设", this);
    
    inputLayout->addWidget(nLabel, 0, 0);
    inputLayout->addWidget(nInput, 0, 1);
    inputLayout->addWidget(mLabel, 1, 0);
    inputLayout->addWidget(mInput, 1, 1);
    inputLayout->addWidget(presetLabel, 2, 0);
    inputLayout->addWidget(presetComboBox, 2, 1);
    inputLayout->addWidget(presetButton, 2, 2);

    // 算法选择区域
    QGroupBox *algorithmGroup = new QGroupBox("数据结构选择", this);
    QVBoxLayout *algorithmLayout = new QVBoxLayout(algorithmGroup);
    
    sequenceListRadio = new QRadioButton("顺序表实现 (自定义SqList)", this);
    doublyLinkedListRadio = new QRadioButton("双向链表实现 (自定义DoubleList)", this);
    staticLinkedListRadio = new QRadioButton("双端队列实现 (自定义Deque)", this);
    doublyLinkedListRadio->setChecked(true);
    
    algorithmLayout->addWidget(sequenceListRadio);
    algorithmLayout->addWidget(doublyLinkedListRadio);
    algorithmLayout->addWidget(staticLinkedListRadio);

    // 按钮区域
    QWidget *buttonWidget = new QWidget(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonWidget);
    
    QPushButton *calculateButton = new QPushButton("计算", this);
    animateButton = new QPushButton("动画演示", this);
    clearButton = new QPushButton("清空结果", this);
    
    buttonLayout->addWidget(calculateButton);
    buttonLayout->addWidget(animateButton);
    buttonLayout->addWidget(clearButton);

    // 动画显示区域
    animationWidget = new JosephusWidget(this);
    animationWidget->setMinimumSize(400, 300);
    animationWidget->setStyleSheet("border: 1px solid gray;");

    // 结果显示区域
    QGroupBox *resultGroup = new QGroupBox("结果显示", this);
    QVBoxLayout *resultLayout = new QVBoxLayout(resultGroup);
    
    resultLabel = new QLabel("结果: 请设置参数并选择算法", this);
    resultLabel->setWordWrap(true);
    
    detailEdit = new QTextEdit(this);
    detailEdit->setReadOnly(true);
    detailEdit->setMaximumHeight(200);
    
    resultLayout->addWidget(resultLabel);
    resultLayout->addWidget(detailEdit);

    mainLayout->addWidget(inputGroup);
    mainLayout->addWidget(algorithmGroup);
    mainLayout->addWidget(buttonWidget);
    mainLayout->addWidget(animationWidget);
    mainLayout->addWidget(resultGroup);

    // 连接信号和槽
    connect(calculateButton, &QPushButton::clicked, this, &JosephusWindow::calculateJosephus);
    connect(presetButton, &QPushButton::clicked, this, &JosephusWindow::loadPresetData);
    connect(animateButton, &QPushButton::clicked, this, &JosephusWindow::startAnimation);
    connect(clearButton, &QPushButton::clicked, this, &JosephusWindow::clearResults);
    connect(sequenceListRadio, &QRadioButton::toggled, this, &JosephusWindow::onAlgorithmChanged);
    connect(doublyLinkedListRadio, &QRadioButton::toggled, this, &JosephusWindow::onAlgorithmChanged);
    connect(staticLinkedListRadio, &QRadioButton::toggled, this, &JosephusWindow::onAlgorithmChanged);
    
    // 初始化动画定时器
    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &JosephusWindow::updateAnimation);
}

void JosephusWindow::onAlgorithmChanged() {
    // 算法切换时的提示
    QString info;
    if (sequenceListRadio->isChecked()) {
        info = "顺序表实现: 使用自定义SqList，基于数组实现";
    } else if (doublyLinkedListRadio->isChecked()) {
        info = "双向链表实现: 使用自定义DoubleList，支持双向遍历";
    } else {
        info = "双端队列实现: 使用自定义Deque，支持头尾操作";
    }
    resultLabel->setText(info);
}

void JosephusWindow::clearResults() {
    resultLabel->setText("结果: 请设置参数并选择算法");
    detailEdit->clear();
    people.clear();
    eliminationOrder.clear();
    currentIndex = 0;
    stepCount = 0;
    animationWidget->setData(QList<int>(), -1, QList<int>(), 1);
}

void JosephusWindow::loadPresetData() {
    int presetIndex = presetComboBox->currentIndex();
    switch (presetIndex) {
        case 1: // 测试数据: n=7, m=3
            nInput->setText("7");
            mInput->setText("3");
            break;
        default:
            break;
    }
}

void JosephusWindow::calculateJosephus() {
    bool ok;
    int n = nInput->text().toInt(&ok);
    if (!ok || n <= 0) {
        QMessageBox::warning(this, "输入错误", "请输入有效的总人数 (n)！");
        return;
    }

    int m = mInput->text().toInt(&ok);
    if (!ok || m <= 0) {
        QMessageBox::warning(this, "输入错误", "请输入有效的报数间隔 (m)！");
        return;
    }

    // 清空之前的结果
    people.clear();
    eliminationOrder.clear();
    stepCount = 0;

    // 根据选择的数据结构实现算法
    if (sequenceListRadio->isChecked()) {
        josephusWithSequenceList(n, m);
        generateDetailedReport("顺序表", n, m);
    } else if (doublyLinkedListRadio->isChecked()) {
        josephusWithDoublyLinkedList(n, m);
        generateDetailedReport("双向链表", n, m);
    } else {
        josephusWithDeque(n, m);
        generateDetailedReport("双端队列", n, m);
    }

    // 显示结果
    QString result = QString("(%1, %2) 约瑟夫排列: ").arg(n).arg(m);
    for (int i = 0; i < eliminationOrder.size(); ++i) {
        result += QString::number(eliminationOrder[i]);
        if (i < eliminationOrder.size() - 1) {
            result += ", ";
        }
    }
    resultLabel->setText(result);
    
    // 更新动画显示
    animationWidget->setData(people, -1, eliminationOrder, 1);
}

// 顺序表实现 - 使用自定义SqList
void JosephusWindow::josephusWithSequenceList(int n, int m) {
    SqList sequence;
    InitList(&sequence);
    
    // 初始化顺序表
    for (int i = 1; i <= n; ++i) {
        ListInsert(&sequence, i, i);
    }
    
    int index = 0;
    stepCount = 0;
    
    while (ListLength(sequence) > 0) {
        stepCount++;
        index = (index + m - 1) % ListLength(sequence);
        
        ElemType eliminated;
        ListDelete(&sequence, index + 1, &eliminated); // ListInsert是1-based
        eliminationOrder.append(eliminated);
    }
    
    // 更新people用于动画显示
    people.clear();
}

// 双向链表实现 - 使用自定义DoubleList
void JosephusWindow::josephusWithDoublyLinkedList(int n, int m) {
    DoubleList::DoubleLinkList list;
    InitList(&list);
    
    // 初始化双向链表
    for (int i = 1; i <= n; ++i) {
        ListInsert(list, i, i);
    }
    
    stepCount = 0;
    int currentPos = 1; // 当前位置（1-based）
    
    while (ListLength(list) > 1) {
        stepCount++;
        
        // 计算要删除的位置
        int steps = (m - 1) % ListLength(list);
        if (stepCount % 2 == 0) {
            // 偶数次：逆时针（向前移动）
            currentPos = (currentPos - steps - 1 + ListLength(list)) % ListLength(list) + 1;
        } else {
            // 奇数次：顺时针（向后移动）
            currentPos = (currentPos + steps - 1) % ListLength(list) + 1;
        }
        
        // 删除节点
        ElemType eliminated;
        ListDelete(list, currentPos, &eliminated);
        eliminationOrder.append(eliminated);
        
        // 如果删除的不是最后一个元素，当前位置不变
        // 如果删除的是最后一个元素，当前位置调整为前一个
        if (currentPos > ListLength(list)) {
            currentPos = ListLength(list);
        }
    }
    
    // 添加最后一个元素
    ElemType last;
    GetElem(list, 1, &last);
    eliminationOrder.append(last);
    
    // 清理链表
    DestroyList(&list);
}

// 双端队列实现 - 使用自定义Deque
void JosephusWindow::josephusWithDeque(int n, int m) {
    LinkDeque deque;
    InitDeque(deque);
    
    // 初始化双端队列
    for (int i = 1; i <= n; ++i) {
        EnDeque(deque, i);
    }
    
    stepCount = 0;
    
    while (DequeLength(deque) > 1) {
        stepCount++;
        
        // 模拟约瑟夫环的循环移动
        for (int i = 0; i < m - 1; ++i) {
            ElemType temp;
            DeDeque(deque, temp);
            EnDeque(deque, temp);
        }
        
        // 淘汰第m个人
        ElemType eliminated;
        DeDeque(deque, eliminated);
        eliminationOrder.append(eliminated);
    }
    
    // 添加最后一个元素
    ElemType last;
    GetHead(deque, last);
    eliminationOrder.append(last);
    
    // 清理队列
    DestroyDeque(deque);
}

void JosephusWindow::generateDetailedReport(const QString &algorithmName, int n, int m) {
    QString report = QString("=== %1 约瑟夫环求解报告 ===\n\n").arg(algorithmName);
    report += QString("输入参数: n=%1, m=%2\n").arg(n).arg(m);
    report += QString("总步数: %1\n\n").arg(stepCount);
    
    report += "出列顺序:\n";
    for (int i = 0; i < eliminationOrder.size(); ++i) {
        report += QString("第%1步: %2\n").arg(i + 1).arg(eliminationOrder[i]);
    }
    
    report += QString("\n优胜者: %1\n\n").arg(eliminationOrder.last());
    
    // 算法特点分析
    report += "=== 算法特点 ===\n";
    if (algorithmName == "顺序表") {
        report += "- 时间复杂度: O(n²)\n";
        report += "- 空间复杂度: O(n)\n";
        report += "- 优点: 实现简单，易于理解\n";
        report += "- 缺点: 删除操作需要移动元素\n";
    } else if (algorithmName == "双向链表") {
        report += "- 时间复杂度: O(n)\n";
        report += "- 空间复杂度: O(n)\n";
        report += "- 优点: 删除操作高效，支持双向遍历\n";
        report += "- 缺点: 内存开销稍大，指针操作复杂\n";
        report += "- 特色: 奇数次顺时针，偶数次逆时针轮转\n";
    } else {
        report += "- 时间复杂度: O(n)\n";
        report += "- 空间复杂度: O(n)\n";
        report += "- 优点: 内存连续，缓存友好\n";
        report += "- 缺点: 需要额外的标记空间\n";
    }
    
    detailEdit->setText(report);
}

void JosephusWindow::startAnimation() {
    if (isAnimating) {
        animationTimer->stop();
        isAnimating = false;
        animateButton->setText("动画演示");
        return;
    }

    // 先计算一次以获取数据
    calculateJosephus();
    
    currentIndex = 0;
    people.clear();
    for (int i = 1; i <= nInput->text().toInt(); ++i) {
        people.append(i);
    }
    
    isAnimating = true;
    animateButton->setText("停止动画");
    animationTimer->start(1000); // 每秒更新一次
}

void JosephusWindow::updateAnimation() {
    if (currentIndex >= eliminationOrder.size()) {
        animationTimer->stop();
        isAnimating = false;
        animateButton->setText("动画演示");
        return;
    }
    
    // 计算当前位置
    int currentPerson = eliminationOrder[currentIndex];
    
    // 显示当前步骤：即将被淘汰的人用红色标识
    animationWidget->setData(people, currentPerson, QList<int>(), 1);
    
    QString result = QString("约瑟夫环演示 (第 %1/%2 步):\n即将淘汰: %3").arg(currentIndex + 1).arg(eliminationOrder.size()).arg(currentPerson);
    resultLabel->setText(result);
    
    currentIndex++;
    
    // 延迟一帧后再实际移除，让用户看到红色标识
    QTimer::singleShot(500, [this, currentPerson]() {
        people.removeOne(currentPerson);
        
        QList<int> eliminatedSoFar;
        for (int i = 0; i < currentIndex; ++i) {
            eliminatedSoFar.append(eliminationOrder[i]);
        }
        
        animationWidget->setData(people, -1, eliminatedSoFar, 1);
        
        QString result = "出列顺序: ";
        for (int i = 0; i < currentIndex; ++i) {
            result += QString::number(eliminationOrder[i]) + " ";
        }
        resultLabel->setText(result);
    });
}

// JosephusWidget 实现
JosephusWidget::JosephusWidget(QWidget *parent) : QWidget(parent), currentPerson(-1), rotationDirection(1) {
    setMinimumSize(400, 300);
}

void JosephusWidget::setData(const QList<int> &people, int current, const QList<int> &eliminated, int direction) {
    this->people = people;
    this->currentPerson = current;
    this->eliminatedPeople = eliminated;
    this->rotationDirection = direction;
    update();
}

void JosephusWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    QRect rect = this->rect();
    int centerX = rect.width() / 2;
    int centerY = rect.height() / 2;
    int radius = qMin(centerX, centerY) - 50;
    
    if (people.isEmpty()) {
        painter.drawText(centerX - 50, centerY, "动画结束");
        return;
    }
    
    // 只绘制剩余的人，不显示已淘汰的
    int totalPeople = people.size();
    
    for (int i = 0; i < totalPeople; ++i) {
        int person = people[i];
        double angle = 2 * M_PI * i / totalPeople - M_PI / 2;
        int x = centerX + radius * cos(angle);
        int y = centerY + radius * sin(angle);
        
        // 设置颜色
        if (person == currentPerson) {
            painter.setBrush(QBrush(Qt::red)); // 即将被淘汰的人用红色
            painter.setPen(Qt::darkRed);
        } else {
            painter.setBrush(QBrush(QColor(173, 216, 230))); // light blue
            painter.setPen(Qt::blue);
        }
        
        // 绘制圆形节点
        painter.drawEllipse(x - 20, y - 20, 40, 40);
        
        // 绘制数字
        painter.setPen(Qt::black);
        QFont font = painter.font();
        font.setBold(true);
        font.setPointSize(12);
        painter.setFont(font);
        painter.drawText(x - 8, y + 5, QString::number(person));
    }
    
    // 绘制连线形成圆环（只连接剩余的人）
    painter.setPen(Qt::gray);
    painter.setBrush(Qt::NoBrush);
    for (int i = 0; i < totalPeople; ++i) {
        double angle1 = 2 * M_PI * i / totalPeople - M_PI / 2;
        double angle2 = 2 * M_PI * ((i + 1) % totalPeople) / totalPeople - M_PI / 2;
        
        int x1 = centerX + radius * cos(angle1);
        int y1 = centerY + radius * sin(angle1);
        int x2 = centerX + radius * cos(angle2);
        int y2 = centerY + radius * sin(angle2);
        
        painter.drawLine(x1, y1, x2, y2);
    }
}
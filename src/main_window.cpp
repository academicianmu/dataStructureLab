#include "main_window.h"
#include "ui_main_window.h"
#include "linkedlist.h"
#include "sequencelist.h"
#include "stack.h"
#include "graph.h"
#include "expression_window.h"
#include "quicksort_window.h"
#include "mst_campus_window.h"
#include "josephus_window.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <vector>
#include <algorithm>
#include <QDebug>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    qDebug() << "MainWindow constructor called";
    
    ui->setupUi(this);

    // Connect buttons to their respective slots
    connect(ui->linkedListButton, &QPushButton::clicked, this, &MainWindow::onLinkedListButtonClicked);
    connect(ui->stackButton, &QPushButton::clicked, this, &MainWindow::onStackButtonClicked);
    connect(ui->queueButton, &QPushButton::clicked, this, &MainWindow::onQueueButtonClicked);

    // 设置窗口属性
    setWindowTitle("数据结构实验平台");
    setMinimumSize(800, 600);
    
    // 创建主窗口部件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // 设置应用程序样式
    QApplication::setStyle("Fusion");
    
    // 设置调色板
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(53, 53, 53));
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Base, QColor(25, 25, 25));
    palette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    palette.setColor(QPalette::ToolTipBase, Qt::white);
    palette.setColor(QPalette::ToolTipText, Qt::white);
    palette.setColor(QPalette::Text, Qt::white);
    palette.setColor(QPalette::Button, QColor(53, 53, 53));
    palette.setColor(QPalette::ButtonText, Qt::white);
    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Link, QColor(42, 130, 218));
    palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    palette.setColor(QPalette::HighlightedText, Qt::black);
    qApp->setPalette(palette);
    
    // 确保样式表被应用
    QString styleSheet = R"(
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, 
                stop:0 #2c3e50, stop:1 #34495e);
        }
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, 
                stop:0 #3498db, stop:1 #2980b9);
            color: white;
            border: none;
            padding: 18px 36px;
            text-align: center;
            text-decoration: none;
            font-size: 18px;
            font-weight: bold;
            margin: 8px 4px;
            border-radius: 12px;
            min-width: 280px;
            min-height: 60px;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.3);
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, 
                stop:0 #5dade2, stop:1 #3498db);
            transform: translateY(-2px);
            box-shadow: 0 6px 12px rgba(0, 0, 0, 0.4);
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, 
                stop:0 #2874a6, stop:1 #1f618d);
            transform: translateY(1px);
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.2);
        }
    )";
    setStyleSheet(styleSheet);
    
    // 创建布局
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);
    
    // 创建按钮
    button1 = new QPushButton("1. 退出", this);
    button2 = new QPushButton("2. 约瑟夫环问题", this);
    button3 = new QPushButton("3. 表达式求值", this);
    button4 = new QPushButton("4. 快速排序", this);
    button5 = new QPushButton("5. 校园导航", this);
    
    // 添加按钮到布局
    layout->addWidget(button1);
    layout->addWidget(button2);
    layout->addWidget(button3);
    layout->addWidget(button4);
    layout->addWidget(button5);
    
    // 连接信号和槽
    connect(button1, &QPushButton::clicked, this, &MainWindow::onButton1Clicked);
    connect(button2, &QPushButton::clicked, this, &MainWindow::onButton2Clicked);
    connect(button3, &QPushButton::clicked, this, &MainWindow::onButton3Clicked);
    connect(button4, &QPushButton::clicked, this, &MainWindow::onButton4Clicked);
    connect(button5, &QPushButton::clicked, this, &MainWindow::onButton5Clicked);
    
    // 居中窗口
    centerWindow();

    // 设置任务按钮
    setupTaskButtons();
}

void MainWindow::centerWindow()
{
    qDebug() << "Centering window...";
    
    // 获取主屏幕
    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen) {
        qWarning() << "Failed to get primary screen";
        return;
    }
    
    // 确保窗口大小有效
    if (width() <= 0 || height() <= 0) {
        resize(800, 600);  // 设置默认大小
    }
    
    // 获取屏幕几何
    QRect screenGeometry = screen->availableGeometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    
    // 确保窗口不会超出屏幕
    x = qMax(screenGeometry.left(), x);
    y = qMax(screenGeometry.top(), y);
    
    move(x, y);
    
    qDebug() << "Window geometry set to:" << geometry();
    
    // 确保窗口显示
    show();
    raise();
    activateWindow();
    
    // 打印窗口状态
    qDebug() << "Window visible:" << isVisible()
             << "Active:" << isActiveWindow()
             << "Minimized:" << isMinimized()
             << "Maximized:" << isMaximized()
             << "FullScreen:" << isFullScreen();
             
    // 如果窗口仍然不可见，尝试强制显示
    if (!isVisible()) {
        qWarning() << "Window is not visible, trying to force show";
        setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
        showNormal();
    }
}

void MainWindow::setupTaskButtons() {
    // 创建任务按钮
    QPushButton *expressionButton = new QPushButton("表达式求值", this);
    QPushButton *josephusButton = new QPushButton("约瑟夫环问题", this);
    QPushButton *quicksortButton = new QPushButton("快速排序", this);
    QPushButton *mstButton = new QPushButton("校园光纤网", this);

    // 添加到布局
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(expressionButton);
    layout->addWidget(josephusButton);
    layout->addWidget(quicksortButton);
    layout->addWidget(mstButton);
    centralWidget()->setLayout(layout);

    // 连接信号和槽
    connect(expressionButton, &QPushButton::clicked, this, &MainWindow::openExpressionWindow);
    connect(josephusButton, &QPushButton::clicked, this, &MainWindow::openJosephusWindow);
    connect(quicksortButton, &QPushButton::clicked, this, &MainWindow::openQuicksortWindow);
    connect(mstButton, &QPushButton::clicked, this, &MainWindow::openMstWindow);
}

void MainWindow::openExpressionWindow() {
    ExpressionWindow *window = new ExpressionWindow(this);
    window->exec();
}

void MainWindow::openJosephusWindow() {
    JosephusWindow *window = new JosephusWindow(this);
    window->exec();
}

void MainWindow::openQuicksortWindow() {
    QuicksortWindow *window = new QuicksortWindow(this);
    window->exec();
}

void MainWindow::openMstWindow() {
    MstCampusWindow *window = new MstCampusWindow(this);
    window->exec();
}

void MainWindow::onButton1Clicked()
{
    close();
}

void MainWindow::onButton2Clicked()
{
    openJosephusWindow();
}

void MainWindow::onButton3Clicked()
{
    openExpressionWindow();
}

void MainWindow::onButton4Clicked()
{
    openQuicksortWindow();
}

void MainWindow::onButton5Clicked()
{
    openMstWindow();
}

void MainWindow::onLinkedListButtonClicked() {
    LinkedList<int> linkedList;
    linkedList.insert(0, 10); // 示例操作
    linkedList.insert(1, 20);

    QString result;
    for (int i = 0; i < linkedList.size(); ++i) {
        result += QString::number(linkedList.get(i)) + " ";
    }

    QMessageBox::information(this, "链表操作", "链表内容: " + result);
}

void MainWindow::onStackButtonClicked() {
    QMessageBox::information(this, "栈操作", "栈操作功能待实现");
}

void MainWindow::onQueueButtonClicked() {
    QMessageBox::information(this, "队列操作", "队列操作功能待实现");
}

void MainWindow::onExpressionEvaluateButtonClicked() {
    QString inputExpression = QInputDialog::getText(this, "输入表达式", "请输入中缀表达式:");
    if (inputExpression.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "表达式不能为空！");
        return;
    }

    // 将中缀表达式转换为后缀表达式
    QString postfixExpression;
    if (!convertToPostfix(inputExpression, postfixExpression)) {
        QMessageBox::warning(this, "错误", "表达式转换失败！");
        return;
    }

    // 计算后缀表达式的值
    double result;
    if (!evaluatePostfixExpression(postfixExpression, result)) {
        QMessageBox::warning(this, "错误", "表达式求值失败！");
        return;
    }

    QMessageBox::information(this, "计算结果", "后缀表达式: " + postfixExpression + "\n结果: " + QString::number(result));
}

void MainWindow::onGraphOperationButtonClicked() {
    MGraph graph;
    CreateGraph(&graph, 5, 0); // 示例：创建一个包含 5 个顶点的图

    AddVertex(&graph, 0, 'A');
    AddVertex(&graph, 1, 'B');
    AddVertex(&graph, 2, 'C');
    AddVertex(&graph, 3, 'D');
    AddVertex(&graph, 4, 'E');

    AddEdge(&graph, 0, 1, 10); // 示例：添加边 A-B，权重为 10
    AddEdge(&graph, 1, 2, 20); // 示例：添加边 B-C，权重为 20

    QString result = "图的顶点: ";
    for (int i = 0; i < graph.vexnum; ++i) {
        result += graph.vexs[i];
        result += " ";
    }

    result += "\n图的边: ";
    for (int i = 0; i < graph.vexnum; ++i) {
        for (int j = 0; j < graph.vexnum; ++j) {
            if (graph.arcs[i][j] != INFINITY && graph.arcs[i][j] != 0) {
                result += QString("(%1-%2: %3) ").arg(graph.vexs[i]).arg(graph.vexs[j]).arg(graph.arcs[i][j]);
            }
        }
    }

    QMessageBox::information(this, "图操作", result);
}

void MainWindow::onJosephusButtonClicked() {
    bool ok;
    int n = QInputDialog::getInt(this, "输入人数", "请输入总人数:", 10, 1, 100, 1, &ok);
    if (!ok) return;

    int m = QInputDialog::getInt(this, "输入报数", "请输入报数间隔:", 3, 1, 100, 1, &ok);
    if (!ok) return;

    std::vector<int> list;
    for (int i = 1; i <= n; ++i) {
        list.push_back(i);
    }

    QString result = "出列顺序: ";
    int current = 0;
    while (!list.empty()) {
        current = (current + m - 1) % list.size();
        result += QString::number(list[current]) + " ";
        list.erase(list.begin() + current);
    }

    QMessageBox::information(this, "约瑟夫环", result);
}

void MainWindow::onQuickSortButtonClicked() {
    std::vector<int> list = {64, 34, 25, 12, 22, 11, 90};

    std::sort(list.begin(), list.end());

    QString result = "排序结果: ";
    for (int value : list) {
        result += QString::number(value) + " ";
    }

    QMessageBox::information(this, "快速排序", result);
}

void MainWindow::setupMainMenu() {
    QPushButton *josephusButton = new QPushButton("约瑟夫环问题", this);
    QPushButton *expressionButton = new QPushButton("表达式求值", this);
    QPushButton *quicksortButton = new QPushButton("快速排序 - 求第K小元素", this);
    QPushButton *mstButton = new QPushButton("最小生成树（校园光纤网）", this);
    QPushButton *exitButton = new QPushButton("退出", this);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(josephusButton);
    layout->addWidget(expressionButton);
    layout->addWidget(quicksortButton);
    layout->addWidget(mstButton);
    layout->addWidget(exitButton);
    centralWidget()->setLayout(layout);

    connect(josephusButton, &QPushButton::clicked, this, &MainWindow::openJosephusWindow);
    connect(expressionButton, &QPushButton::clicked, this, &MainWindow::openExpressionWindow);
    connect(quicksortButton, &QPushButton::clicked, this, &MainWindow::openQuicksortWindow);
    connect(mstButton, &QPushButton::clicked, this, &MainWindow::openMstWindow);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::close);
}

bool MainWindow::convertToPostfix(const QString &infix, QString &postfix) {
    // 示例实现：简单的中缀转后缀逻辑
    // TODO: 根据实际需求完善转换逻辑
    postfix = infix; // 暂时直接返回输入表达式
    return true;
}

bool MainWindow::evaluatePostfixExpression(const QString &postfix, double &result) {
    // 示例实现：简单的后缀表达式求值逻辑
    // TODO: 根据实际需求完善求值逻辑
    result = 0; // 暂时返回 0
    return true;
}

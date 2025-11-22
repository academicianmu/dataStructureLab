#include "mst_campus_window.h"
#include <QString>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QTextStream>
#include <QStringList>
#include <algorithm>
#include <QPainter>
#include <QPaintEvent>
#include <cmath>

// 使用用户自定义数据结构 - 必须先包含graph.h
#include "../include/graph.h"
#include "../include/sequencelist.h"
#include "../include/edgelist.h"
#include "../include/stack.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

MstCampusWindow::MstCampusWindow(QWidget *parent) : QDialog(parent) {
    setWindowTitle("光纤管道铺设施工方案 - 最小生成树");
    setMinimumSize(900, 700);
    
    setupUI();
}

void MstCampusWindow::loadPresetData() {
    int presetIndex = presetComboBox->currentIndex();
    switch (presetIndex) {
        case 1: // 测试校园 (12栋建筑)
            graphInput->setText("0,6,0,14,0,0,0,18,0,0,4,20 6,0,7,0,15,0,0,0,0,0,0,0 0,7,0,8,0,16,0,0,0,0,0,5 14,0,8,0,9,0,0,19,0,0,0,0 0,15,0,9,0,10,0,20,0,0,0,0 0,0,16,0,10,0,11,0,21,0,0,22 0,0,0,0,0,11,0,12,0,22,0,0 18,0,0,19,0,0,12,0,13,0,23,0 0,0,0,0,0,21,0,13,0,14,0,24 0,0,0,0,0,0,22,0,14,0,15,25 4,0,0,0,0,0,0,23,0,15,0,16 20,0,5,0,0,22,0,0,24,25,16,0");
            break;
        default:
            break;
    }
}

void MstCampusWindow::parseGraphInput() {
    QString input = graphInput->text().trimmed();
    QStringList rows = input.split(' ', Qt::SkipEmptyParts);
    
    int n = rows.size();
    if (n == 0) return;
    
    // 初始化图
    CreateGraph(&graph, n, 0);
    
    // 解析邻接矩阵
    for (int i = 0; i < n; ++i) {
        QStringList values = rows[i].split(',', Qt::SkipEmptyParts);
        AddVertex(&graph, i, i + 1); // 添加顶点，编号从1开始
        
        for (int j = 0; j < values.size() && j < n; ++j) {
            int weight = values[j].toInt();
            if (weight > 0) {
                AddEdge(&graph, i, j, weight);
            }
        }
    }
}

void MstCampusWindow::calculateMST() {
    parseGraphInput();
    
    if (graph.vexnum == 0) {
        QMessageBox::warning(this, "输入错误", "请输入有效的邻接矩阵！");
        return;
    }
    
    // 初始化边列表
    InitEdgeList(&mstEdges);
    InitEdgeList(&allEdges);
    
    // 根据选择的算法计算MST
    if (primRadio->isChecked()) {
        primMST();
    } else {
        kruskalMST();
    }
    
    // 更新图形显示
    graphWidget->setGraphData(graph, allEdges, mstEdges);
    
    // 生成结果文本
    QString result = "最小生成树的边:\n";
    int totalWeight = 0;
    for (int i = 1; i <= EdgeListLength(mstEdges); ++i) {
        Edge edge;
        GetEdgeElem(mstEdges, i, &edge);
        result += QString("教学楼%1-教学楼%2: 造价%3万元\n")
                .arg(edge.start + 1)
                .arg(edge.end + 1)
                .arg(edge.weight);
        totalWeight += edge.weight;
    }
    result += QString("总造价: %1万元").arg(totalWeight);
    resultLabel->setText(result);
    
    // 生成施工方案
    generateConstructionPlan();
}

void MstCampusWindow::kruskalMST() {
    // 清空边列表
    ClearEdgeList(&mstEdges);
    ClearEdgeList(&allEdges);
    
    int n = graph.vexnum;
    
    // 收集所有边到allEdges
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (graph.arcs[i][j] > 0 && graph.arcs[i][j] < INFINITY) {
                Edge edge;
                edge.start = i;
                edge.end = j;
                edge.weight = graph.arcs[i][j];
                EdgeListInsert(&allEdges, EdgeListLength(allEdges) + 1, edge);
            }
        }
    }
    
    // 使用自定义Kruskal算法
    int mstCount = 0;
    Kruskal(&graph, nullptr, &mstCount); // 这里需要适配您的Kruskal实现
    
    // 由于您的Kruskal可能有自己的边存储方式，我们需要手动实现
    // 创建边数组用于排序
    Edge edges[100]; // 假设最多100条边
    int edgeCount = 0;
    
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (graph.arcs[i][j] > 0 && graph.arcs[i][j] < INFINITY) {
                edges[edgeCount].start = i;
                edges[edgeCount].end = j;
                edges[edgeCount].weight = graph.arcs[i][j];
                edgeCount++;
            }
        }
    }
    
    // 排序边
    SortEdges(edges, edgeCount);
    
    // Kruskal算法
    UnionFind uf;
    InitUnionFind(&uf, n);
    
    for (int i = 0; i < edgeCount && EdgeListLength(mstEdges) < n - 1; ++i) {
        int u = edges[i].start;
        int v = edges[i].end;
        
        if (Find(&uf, u) != Find(&uf, v)) {
            EdgeListInsert(&mstEdges, EdgeListLength(mstEdges) + 1, edges[i]);
            Union(&uf, u, v);
        }
    }
}

// MstGraphWidget 实现
MstGraphWidget::MstGraphWidget(QWidget *parent) : QWidget(parent) {
    setMinimumSize(600, 500); // 增大默认尺寸
}

void MstGraphWidget::setGraphData(const MGraph &graph, 
                                 const EdgeList &allEdges,
                                 const EdgeList &mstEdges) {
    this->graph = graph;
    this->allEdges = allEdges;
    this->mstEdges = mstEdges;
    update();
}

void MstGraphWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    if (graph.vexnum == 0) {
        painter.drawText(rect().center() - QPoint(50, 0), "请输入数据开始");
        return;
    }
    
    drawGraph(painter);
}

void MstGraphWidget::drawGraph(QPainter &painter) {
    drawEdges(painter);
    drawMSTEdges(painter);
    drawNodes(painter);
}

void MstGraphWidget::drawNodes(QPainter &painter) {
    int n = graph.vexnum;
    for (int i = 0; i < n; ++i) {
        QPoint pos = getNodePosition(i, n);
        
        // 绘制节点 - 增大节点尺寸
        painter.setBrush(QBrush(QColor(173, 216, 230))); // light blue
        painter.setPen(Qt::blue);
        painter.drawEllipse(pos.x() - 25, pos.y() - 25, 50, 50); // 从30增大到50
        
        // 绘制节点编号 - 修复文字位置
        painter.setPen(Qt::black);
        QFont font = painter.font();
        font.setBold(true);
        font.setPointSize(16); // 从12增大到16
        painter.setFont(font);
        
        // 使用QFontMetrics计算文字位置，避免方块
        QString nodeText = QString::number(graph.vexs[i]); // 使用自定义图数据结构
        QFontMetrics fm(font);
        QRect textRect = fm.boundingRect(nodeText);
        textRect.moveCenter(pos);
        painter.drawText(textRect, nodeText);
    }
}

void MstGraphWidget::drawEdges(QPainter &painter) {
    painter.setPen(QPen(Qt::gray, 3, Qt::DashLine)); // 增粗线条
    
    for (int i = 1; i <= EdgeListLength(allEdges); ++i) {
        Edge edge;
        GetEdgeElem(allEdges, i, &edge);
        
        QPoint pos1 = getNodePosition(edge.start, graph.vexnum);
        QPoint pos2 = getNodePosition(edge.end, graph.vexnum);
        
        // 检查是否在MST中
        bool inMST = false;
        for (int j = 1; j <= EdgeListLength(mstEdges); ++j) {
            Edge mstEdge;
            GetEdgeElem(mstEdges, j, &mstEdge);
            if ((mstEdge.start == edge.start && mstEdge.end == edge.end) ||
                (mstEdge.start == edge.end && mstEdge.end == edge.start)) {
                inMST = true;
                break;
            }
        }
        
        if (!inMST) {
            painter.drawLine(pos1, pos2);
            
            // 绘制权重 - 增大字体
            QPoint mid = (pos1 + pos2) / 2;
            QFont font = painter.font();
            font.setPointSize(12); // 从8增大到12
            painter.setFont(font);
            painter.setPen(Qt::black);
            painter.drawText(mid, QString::number(edge.weight));
        }
    }
}

void MstGraphWidget::drawMSTEdges(QPainter &painter) {
    for (int i = 1; i <= EdgeListLength(mstEdges); ++i) {
        Edge edge;
        GetEdgeElem(mstEdges, i, &edge);
        
        QPoint pos1 = getNodePosition(edge.start, graph.vexnum);
        QPoint pos2 = getNodePosition(edge.end, graph.vexnum);
        
        painter.setPen(QPen(Qt::green, 5)); // 增粗线条
        painter.drawLine(pos1, pos2);
        
        // 绘制权重 - 增大字体
        QPoint mid = (pos1 + pos2) / 2;
        QFont font = painter.font();
        font.setBold(true);
        font.setPointSize(14); // 从10增大到14
        painter.setFont(font);
        painter.setPen(Qt::black);
        painter.drawText(mid, QString::number(edge.weight));
    }
}

void MstCampusWindow::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 输入区域
    QGroupBox *inputGroup = new QGroupBox("校园光纤网络配置", this);
    QGridLayout *inputLayout = new QGridLayout(inputGroup);
    
    QLabel *instructionLabel = new QLabel("请输入教学楼/办公楼的邻接矩阵 (用空格分隔行，逗号分隔列):", this);
    graphInput = new QLineEdit(this);
    
    QLabel *presetLabel = new QLabel("预设校园规模:", this);
    presetComboBox = new QComboBox(this);
    presetComboBox->addItem("自定义");
    presetComboBox->addItem("测试校园 (12栋建筑)");
    
    presetButton = new QPushButton("加载预设", this);
    
    inputLayout->addWidget(instructionLabel, 0, 0, 1, 3);
    inputLayout->addWidget(graphInput, 1, 0, 1, 3);
    inputLayout->addWidget(presetLabel, 2, 0);
    inputLayout->addWidget(presetComboBox, 2, 1);
    inputLayout->addWidget(presetButton, 2, 2);

    // 算法选择区域
    QGroupBox *algorithmGroup = new QGroupBox("算法选择", this);
    QVBoxLayout *algorithmLayout = new QVBoxLayout(algorithmGroup);
    
    primRadio = new QRadioButton("Prim算法 (适合稠密图)", this);
    kruskalRadio = new QRadioButton("Kruskal算法 (适合稀疏图)", this);
    kruskalRadio->setChecked(true);
    
    algorithmLayout->addWidget(primRadio);
    algorithmLayout->addWidget(kruskalRadio);
    
    // 按钮区域
    QWidget *buttonWidget = new QWidget(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonWidget);
    
    QPushButton *calculateButton = new QPushButton("计算最优施工方案", this);
    
    buttonLayout->addWidget(calculateButton);

    // 图形显示区域
    graphWidget = new MstGraphWidget(this);
    graphWidget->setMinimumSize(600, 400);
    graphWidget->setStyleSheet("border: 1px solid gray;");

    // 结果显示区域
    resultTabWidget = new QTabWidget(this);
    resultLabel = new QLabel("结果: ", this);
    resultLabel->setWordWrap(true);
    constructionPlanEdit = new QTextEdit(this);
    constructionPlanEdit->setReadOnly(true);
    constructionPlanEdit->setMaximumHeight(200);
    
    resultTabWidget->addTab(resultLabel, "计算结果");
    resultTabWidget->addTab(constructionPlanEdit, "施工方案");

    mainLayout->addWidget(inputGroup);
    mainLayout->addWidget(algorithmGroup);
    mainLayout->addWidget(buttonWidget);
    mainLayout->addWidget(graphWidget);
    mainLayout->addWidget(resultTabWidget);

    // 连接信号和槽
    connect(calculateButton, &QPushButton::clicked, this, &MstCampusWindow::calculateMST);
    connect(presetButton, &QPushButton::clicked, this, &MstCampusWindow::loadPresetData);
    connect(primRadio, &QRadioButton::toggled, this, &MstCampusWindow::onAlgorithmChanged);
    connect(kruskalRadio, &QRadioButton::toggled, this, &MstCampusWindow::onAlgorithmChanged);
}

void MstCampusWindow::onAlgorithmChanged() {
    // 可以在这里添加算法切换时的提示或逻辑
}

void MstCampusWindow::primMST() {
    // 清空边列表
    ClearEdgeList(&mstEdges);
    ClearEdgeList(&allEdges);
    
    int n = graph.vexnum;
    if (n == 0) return;
    
    // 收集所有边到allEdges
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (graph.arcs[i][j] > 0 && graph.arcs[i][j] < INFINITY) {
                Edge edge;
                edge.start = i;
                edge.end = j;
                edge.weight = graph.arcs[i][j];
                EdgeListInsert(&allEdges, EdgeListLength(allEdges) + 1, edge);
            }
        }
    }
    
    // 使用自定义Prim算法
    MSTEdge mst[100]; // 假设最多100条边
    int mstCount = 0;
    
    Prim(&graph, 0, mst, &mstCount);
    
    // 将结果添加到mstEdges
    for (int i = 0; i < mstCount; ++i) {
        Edge edge;
        edge.start = mst[i].start;
        edge.end = mst[i].end;
        edge.weight = mst[i].weight;
        EdgeListInsert(&mstEdges, EdgeListLength(mstEdges) + 1, edge);
    }
}
void MstCampusWindow::generateConstructionPlan() {
    QString plan = "=== 光纤管道铺设施工方案 ===\n\n";
    plan += QString("使用算法: %1\n\n")
            .arg(primRadio->isChecked() ? "Prim算法" : "Kruskal算法");

    plan += "施工步骤:\n";
    int step = 1;
    int totalCost = 0;

    // 按权值排序施工步骤 - 使用EdgeList专用函数
    Edge sortedEdges[100];
    int edgeCount = EdgeListLength(mstEdges);

    for (int i = 1; i <= edgeCount; ++i) {
        GetEdgeElem(mstEdges, i, &sortedEdges[i - 1]);
    }

    // 简单排序
    for (int i = 0; i < edgeCount - 1; ++i) {
        for (int j = i + 1; j < edgeCount; ++j) {
            if (sortedEdges[i].weight > sortedEdges[j].weight) {
                Edge temp = sortedEdges[i];
                sortedEdges[i] = sortedEdges[j];
                sortedEdges[j] = temp;
            }
        }
    }

    for (int i = 0; i < edgeCount; ++i) {
        int cost = sortedEdges[i].weight;
        totalCost += cost;
        plan += QString("第%1步: 连接教学楼%2至教学楼%3，铺设光纤管道，造价：%4万元\n")
                .arg(step)
                .arg(sortedEdges[i].start + 1)
                .arg(sortedEdges[i].end + 1)
                .arg(cost);
        step++;
    }

    plan += QString("\n=== 施工总结 ===\n");
    plan += QString("总造价: %1万元\n").arg(totalCost);
    plan += QString("铺设管道总数: %1条\n").arg(edgeCount);
    plan += QString("覆盖教学楼: %1栋\n").arg(graph.vexnum);

    constructionPlanEdit->setText(plan);
}

QPoint MstGraphWidget::getNodePosition(int node, int totalNodes) {
    QRect rect = this->rect();
    int centerX = rect.width() / 2;
    int centerY = rect.height() / 2;
    int radius = qMin(centerX, centerY) - 60;
    
    // 所有节点围成一个圆形
    double angle = 2 * M_PI * node / totalNodes - M_PI / 2;
    int x = centerX + radius * cos(angle);
    int y = centerY + radius * sin(angle);
    
    return QPoint(x, y);
}


#ifndef MST_CAMPUS_WINDOW_H
#define MST_CAMPUS_WINDOW_H

#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QComboBox>
#include <QPainter>
#include <QWidget>
#include <QTextEdit>
#include <QTabWidget>
#include <QGroupBox>
#include <QRadioButton>
#include <QButtonGroup>

// 使用用户自定义数据结构 - 必须先包含graph.h
#include "../include/graph.h"
#include "../include/sequencelist.h"
#include "../include/edgelist.h"
#include "../include/stack.h"

class MstGraphWidget;

class MstCampusWindow : public QDialog {
    Q_OBJECT

public:
    explicit MstCampusWindow(QWidget *parent = nullptr);

private slots:
    void calculateMST();
    void loadPresetData();
    void onAlgorithmChanged();

private:
    QLineEdit *graphInput;
    QLabel *resultLabel;
    QComboBox *presetComboBox;
    QPushButton *presetButton;
    MstGraphWidget *graphWidget;
    QButtonGroup *algorithmGroup;
    QRadioButton *primRadio;
    QRadioButton *kruskalRadio;
    QTextEdit *constructionPlanEdit;
    QTabWidget *resultTabWidget;
    
    MGraph graph;
    EdgeList mstEdges;
    EdgeList allEdges;
    
    void parseGraphInput();
    void kruskalMST();
    void primMST();
    void generateConstructionPlan();
    void setupUI();
};

class MstGraphWidget : public QWidget {
    Q_OBJECT

public:
    explicit MstGraphWidget(QWidget *parent = nullptr);
    void setGraphData(const MGraph &graph, 
                      const EdgeList &allEdges,
                      const EdgeList &mstEdges);
    
protected:
    void paintEvent(QPaintEvent *event) override;
    
private:
    MGraph graph;
    EdgeList allEdges;
    EdgeList mstEdges;
    
    void drawGraph(QPainter &painter);
    void drawNodes(QPainter &painter);
    void drawEdges(QPainter &painter);
    void drawMSTEdges(QPainter &painter);
    QPoint getNodePosition(int node, int totalNodes);
};

#endif // MST_CAMPUS_WINDOW_H
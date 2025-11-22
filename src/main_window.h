#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QInputDialog> // 添加 QInputDialog
#include "ui_main_window.h" // 确保包含生成的 UI 文件

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow() override = default; // 添加虚析构函数

private slots:
    void onButton1Clicked();
    void onButton2Clicked();
    void onButton3Clicked();
    void onButton4Clicked();
    void onButton5Clicked();
    void onLinkedListButtonClicked();
    void onStackButtonClicked();
    void onQueueButtonClicked();
    void onExpressionEvaluateButtonClicked();
    void onGraphOperationButtonClicked();
    void onJosephusButtonClicked();
    void onQuickSortButtonClicked();

private:
    Ui::MainWindow *ui; // 确保定义 ui 成员变量
    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;
    QPushButton *button4;
    QPushButton *button5;

    void centerWindow();
    void setupTaskButtons();
    void openExpressionWindow();
    void openJosephusWindow();
    void openQuicksortWindow();
    void openMstWindow();
    void setupMainMenu();
    bool convertToPostfix(const QString &infix, QString &postfix);
    bool evaluatePostfixExpression(const QString &postfix, double &result);
};

#endif // MAIN_WINDOW_H

#ifndef EXPRESSION_WINDOW_H
#define EXPRESSION_WINDOW_H

#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QString>

// 使用用户自定义数据结构
#include "../include/stack.h"
#include "../include/sequencelist.h"

class ExpressionWindow : public QDialog {
    Q_OBJECT

public:
    explicit ExpressionWindow(QWidget *parent = nullptr);
    virtual ~ExpressionWindow() override = default;

private slots:
    void evaluateExpression();

private:
    QLineEdit *expressionInput;
    QLabel *resultLabel;
    
    // 表达式求值相关函数
    double evaluate(const QString &expression);
    bool isOperator(const QChar &c);
    int precedence(const QChar &op);
    double applyOperation(double a, double b, const QChar &op);
    QString infixToPostfix(const QString &infix);
    double evaluatePostfix(const QString &postfix);
};

#endif // EXPRESSION_WINDOW_H
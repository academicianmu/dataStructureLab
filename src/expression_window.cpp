#include "expression_window.h"
#include <QRegularExpression>
#include <cmath>

ExpressionWindow::ExpressionWindow(QWidget *parent) : QDialog(parent) {
    setWindowTitle("表达式求值");
    setMinimumSize(600, 500);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *inputLabel = new QLabel("请输入中缀表达式 (支持 +, -, *, /, (, )):", this);
    expressionInput = new QLineEdit(this);
    expressionInput->setPlaceholderText("例如: 3+4*2/(1-5)");
    
    QPushButton *evaluateButton = new QPushButton("计算并显示后缀表达式", this);
    resultLabel = new QLabel("结果: ", this);
    resultLabel->setWordWrap(true);

    layout->addWidget(inputLabel);
    layout->addWidget(expressionInput);
    layout->addWidget(evaluateButton);
    layout->addWidget(resultLabel);

    connect(evaluateButton, &QPushButton::clicked, this, &ExpressionWindow::evaluateExpression);
}

void ExpressionWindow::evaluateExpression() {
    QString expression = expressionInput->text().trimmed();
    if (expression.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "表达式不能为空！");
        return;
    }

    try {
        // 移除空格
        QString expr = expression.simplified();
        expr.remove(' ');
        
        // 转换为后缀表达式并计算
        QString postfix = infixToPostfix(expr);
        double result = evaluatePostfix(postfix);
        
        // 显示原始表达式、后缀表达式和结果
        QString displayResult = QString("原始表达式: %1\n\n后缀表达式: %2\n\n计算结果: %3")
            .arg(expression)
            .arg(postfix)
            .arg(result);
        
        resultLabel->setText(displayResult);
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "计算错误", QString("表达式计算错误: %1").arg(e.what()));
        resultLabel->setText("结果: 计算错误");
    }
}

double ExpressionWindow::evaluate(const QString &expression) {
    // 移除空格
    QString expr = expression.simplified();
    expr.remove(' ');
    
    // 转换为后缀表达式并计算
    QString postfix = infixToPostfix(expr);
    return evaluatePostfix(postfix);
}

bool ExpressionWindow::isOperator(const QChar &c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int ExpressionWindow::precedence(const QChar &op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

double ExpressionWindow::applyOperation(double a, double b, const QChar &op) {
    switch (op.toLatin1()) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0) throw std::runtime_error("除零错误");
            return a / b;
        default:
            throw std::runtime_error("未知操作符");
    }
}

QString ExpressionWindow::infixToPostfix(const QString &infix) {
    QString postfix;
    SqStack operators;
    InitStack(operators);
    
    for (int i = 0; i < infix.length(); ++i) {
        QChar c = infix[i];
        
        if (c.isSpace()) continue;
        
        // 处理数字（包括多位数和小数）
        if (c.isDigit() || c == '.') {
            while (i < infix.length() && (infix[i].isDigit() || infix[i] == '.')) {
                postfix += infix[i];
                i++;
            }
            i--; // 回退一位，因为for循环会自增
            postfix += ' '; // 数字分隔符
        }
        // 处理左括号
        else if (c == '(') {
            Push(operators, (SElemType)c.toLatin1());
        }
        // 处理右括号
        else if (c == ')') {
            SElemType topChar;
            while (!StackEmpty(operators) && GetTop(operators, topChar) && (char)topChar != '(') {
                postfix += (char)topChar;
                postfix += ' ';
                Pop(operators, topChar);
            }
            if (StackEmpty(operators)) {
                DestroyStack(operators);
                throw std::runtime_error("括号不匹配");
            }
            Pop(operators, topChar); // 弹出左括号
        }
        // 处理操作符
        else if (isOperator(c)) {
            SElemType topChar;
            while (!StackEmpty(operators) && GetTop(operators, topChar) && (char)topChar != '(' &&
                   precedence((char)topChar) >= precedence(c)) {
                postfix += (char)topChar;
                postfix += ' ';
                Pop(operators, topChar);
            }
            Push(operators, (SElemType)c.toLatin1());
        }
        else {
            DestroyStack(operators);
            throw std::runtime_error(QString("无效字符: %1").arg(c).toStdString());
        }
    }
    
    // 弹出剩余操作符
    while (!StackEmpty(operators)) {
        SElemType topChar;
        GetTop(operators, topChar);
        if ((char)topChar == '(') {
            DestroyStack(operators);
            throw std::runtime_error("括号不匹配");
        }
        postfix += (char)topChar;
        postfix += ' ';
        Pop(operators, topChar);
    }
    
    // 清理栈
    DestroyStack(operators);
    
    return postfix.trimmed();
}

double ExpressionWindow::evaluatePostfix(const QString &postfix) {
    DStack operands;
    InitDStack(operands);
    
    QStringList tokens = postfix.split(' ', Qt::SkipEmptyParts);
    
    for (const QString &token : tokens) {
        if (token.isEmpty()) continue;
        
        // 检查是否为数字
        bool ok;
        double num = token.toDouble(&ok);
        if (ok) {
            PushDStack(operands, num);
        }
        // 检查是否为操作符
        else if (token.length() == 1 && isOperator(token[0])) {
            if (DStackLength(operands) < 2) {
                DestroyDStack(operands);
                throw std::runtime_error("操作数不足");
            }
            double b, a;
            PopDStack(operands, b);
            PopDStack(operands, a);
            double result = applyOperation(a, b, token[0]);
            PushDStack(operands, result);
        }
        else {
            DestroyDStack(operands);
            throw std::runtime_error(QString("无效标记: %1").arg(token).toStdString());
        }
    }
    
    if (DStackLength(operands) != 1) {
        DestroyDStack(operands);
        throw std::runtime_error("表达式格式错误");
    }
    
    double result;
    GetDTop(operands, result);
    DestroyDStack(operands);
    
    return result;
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stack>
#include <vector>
#include <string>
#include <cctype>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit->setAlignment(Qt::AlignRight);
    ui->lineEdit->setText("0");

    QList<QPushButton*> allButtons = this->findChildren<QPushButton*>();

    for (QPushButton* button: allButtons) {
        if (button->objectName() != "equals"
            && button->objectName() != "clear"
            && button->objectName() != "backspace") {

            connect(button, &QPushButton::clicked, this, &MainWindow::buttonPressed);
        }
    }

    connect(ui->clear, &QPushButton::clicked, this, &MainWindow::clearPressed);
    connect(ui->backspace, &QPushButton::clicked, this, &MainWindow::backspacePressed);
    connect(ui->equals, &QPushButton::clicked, this, &MainWindow::equalsPressed);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::buttonPressed() {
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton) return;

    QString buttonText = clickedButton->text();
    QString currentText = ui->lineEdit->text();

    if (buttonText == "( )") {
        buttonText = getCorrectBracket(currentText);
    }

    if (currentText == "0" && buttonText != ",") {
        ui->lineEdit->setText(buttonText);
    }
    else {
        ui->lineEdit->setText(currentText + buttonText);
    }
}

void MainWindow::clearPressed() {
    ui->lineEdit->setText("0");
}

void MainWindow::backspacePressed() {
    QString currentText = ui->lineEdit->text();

    if (currentText.length() <= 1) {
        ui->lineEdit->setText("0");
    }
    else {
        currentText.chop(1);
        ui->lineEdit->setText(currentText);
    }
}

void MainWindow::equalsPressed() {
    QString expression = ui->lineEdit->text();
    std::string exp = expression.toStdString();

    std::vector<std::string> tokens = tokenize(exp);

    std::vector<std::string> rpn = convertToRPN(tokens);

    double finalResult = calculateRPN(rpn);

    QString final_result = QString::number(finalResult);
    final_result.replace('.', ',');

    ui->lineEdit->setText(final_result);
}

QString MainWindow::getCorrectBracket(const QString &currentText) {
    int openCount = currentText.count('(');
    int closeCount = currentText.count(')');
    char lastChar = currentText.toStdString().back();

    if (lastChar == '0' || lastChar == '+' || lastChar == '-'
        || lastChar == '*' || lastChar == '/' || lastChar == '^' || lastChar == '(') {

        return "(";
    }

    else if (openCount > closeCount) {
        return ")";
    }

    return "(";
}

std::vector<std::string> MainWindow::tokenize(const std::string &exp) {
    std::vector<std::string> str;
    std::string currentNumber = "";

    for (int i = 0; i < exp.size(); i++) {
        char c = exp[i];

        if (isdigit(c) || c == ',' || c == '.') {
            currentNumber += c;
        }

        else if (c == '+' || c == '*' || c == '/'
                 || c == '^' || c == '(' || c == ')') {
            if (!currentNumber.empty()) {
                str.push_back(currentNumber);
                currentNumber = "";
            }
            std::string op(1, c);
            str.push_back(op);
        }

        else if (c == '-') {
            if (i == 0 || exp[i - 1] == '(') {
                currentNumber += c;
            }
            else {
                if (!currentNumber.empty()) {
                    str.push_back(currentNumber);
                    currentNumber = "";
                }
                std::string op(1, c);
                str.push_back(op);
            }
        }
    }
    if (!currentNumber.empty()) {
        str.push_back(currentNumber);
    }
    return str;
}

int MainWindow::getPriority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

std::vector<std::string> MainWindow::convertToRPN(const std::vector<std::string> &tokens) {
    std::vector<std::string> rpn;
    std::stack<char> operators;

    for (int i = 0; i < tokens.size(); i++) {
        std::string token = tokens[i];

        if ((isdigit(token[0]) || (token[0] == '-' && token.size() > 1)))
            rpn.push_back(token);

        else if (token == "(") {
            operators.push('(');
        }

        else if (token == ")") {
            while (!operators.empty() && operators.top() != '(') {
                std::string op(1, operators.top());
                rpn.push_back(op);
                operators.pop();
            }
            operators.pop();
        }

        else if (token == "+" || token == "-" || token == "*" || token == "/" || token == "^") {
            char currentOp = token[0];
            while (!operators.empty() && (getPriority(operators.top()) >= getPriority(currentOp))) {
                std::string op(1, operators.top());
                rpn.push_back(op);
                operators.pop();
            }
            operators.push(currentOp);
        }
    }
    while (!operators.empty()) {
        std::string op(1, operators.top());
        rpn.push_back(op);
        operators.pop();
    }

    return rpn;
}

double MainWindow::calculateRPN(const std::vector<std::string> &rpn) {
    std::stack<double> numbers;

    for (int i = 0; i < rpn.size(); i++) {
        std::string token = rpn[i];

        if (isdigit(token[0]) || (token[0] == '-' && token.size() > 1)) {
            std::replace(token.begin(), token.end(), ',', '.');
            numbers.push(std::stod(token));
        }

        else {
            double b = numbers.top();
            numbers.pop();
            double a = numbers.top();
            numbers.pop();

            if (token == "+") numbers.push(a + b);
            else if (token == "-") numbers.push(a - b);
            else if (token == "*") numbers.push(a * b);
            else if (token == "/") numbers.push(a / b);
            else if (token == "^") numbers.push(pow(a, b));
        }
    }
    return numbers.top();
}






































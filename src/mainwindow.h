#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void buttonPressed();
    void backspacePressed();
    void clearPressed();
    void equalsPressed();

private:
    Ui::MainWindow *ui;
    QString getCorrectBracket(const QString &currentText);
    std::vector<std::string> tokenize(const std::string &expession);
    int getPriority(char op);
    std::vector<std::string> convertToRPN(const std::vector<std::string> &tokens);
    double calculateRPN(const std::vector<std::string> &rpn);

};
#endif // MAINWINDOW_H

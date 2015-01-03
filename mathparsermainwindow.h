#ifndef MATHPARSERMAINWINDOW_H
#define MATHPARSERMAINWINDOW_H

#include <QMainWindow>
#include "mathparser.h"

namespace Ui {
class MathParserMainWindow;
}

class MathParserMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MathParserMainWindow(QWidget *parent = 0);
    ~MathParserMainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MathParserMainWindow *ui;
    MathParser<double> mathParser;
    MathEval<double> mathEval;
};

#endif // MATHPARSERMAINWINDOW_H

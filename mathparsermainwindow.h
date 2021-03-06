#ifndef MATHPARSERMAINWINDOW_H
#define MATHPARSERMAINWINDOW_H

#include <QMainWindow>
#include "mathparser.h"
#include <QGraphicsView>

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
    void on_calculatePushButton_clicked();
    void on_variablesLineEdit_textEdited(const QString &arg1);
    void on_variablesComboBox_currentIndexChanged(int index);

private:
    Ui::MathParserMainWindow *ui;
    QGraphicsScene scene;
    void drawCoordinateSystem();
    void drawPlot();
    MathParser<double> mathParser;
    MathEval<double> mathEval;
    double *varptr[26];
};

#endif // MATHPARSERMAINWINDOW_H

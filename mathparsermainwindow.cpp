#include "mathparsermainwindow.h"
#include "ui_mathparsermainwindow.h"

MathParserMainWindow::MathParserMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MathParserMainWindow),
    mathParser(&mathEval)
{
    ui->setupUi(this);
}

MathParserMainWindow::~MathParserMainWindow()
{
    delete ui;
}

void MathParserMainWindow::on_pushButton_clicked()
{
    mathParser.setString(ui->formulaTextEdit->toPlainText());
    if(mathParser.parse())
    {
        mathEval.setVar('x',1.5);
        mathEval.run();
        ui->resultTextEdit->setPlainText(QString::number(mathEval.pop()));
    }
}

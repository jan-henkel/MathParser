#include "mathparsermainwindow.h"
#include "ui_mathparsermainwindow.h"

MathParserMainWindow::MathParserMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MathParserMainWindow),
    mathParser(&mathEval)
{
    ui->setupUi(this);
    for(char c='a';c<='z';++c)
    {
        varptr[(int)(c-'a')]=mathEval.getVarPtr(c);
        *(varptr[(int)(c-'a')])=0.;
        ui->variablesComboBox->addItem(QString(QChar(c)));
    }
}

MathParserMainWindow::~MathParserMainWindow()
{
    delete ui;
}

void MathParserMainWindow::on_calculatePushButton_clicked()
{
    mathParser.setString(ui->formulaLineEdit->text());
    if(mathParser.parse())
    {
        mathEval.run();
        ui->resultLineEdit->setText(QString::number(mathEval.result()));
    }
    else
        ui->resultLineEdit->setText("Invalid epression");
}

void MathParserMainWindow::on_variablesLineEdit_textEdited(const QString &arg1)
{
    if(ui->variablesComboBox->currentIndex()!=-1)
    {
        bool ok;
        double val;
        val=arg1.toDouble(&ok);
        if(ok)
            *(varptr[ui->variablesComboBox->currentIndex()])=val;
    }
}

void MathParserMainWindow::on_variablesComboBox_currentIndexChanged(int index)
{
    ui->variablesLineEdit->setText(QString::number(*varptr[index]));
}

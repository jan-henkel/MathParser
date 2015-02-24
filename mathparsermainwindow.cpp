#include "mathparsermainwindow.h"
#include "ui_mathparsermainwindow.h"

double rangeXlower=-10;
double rangeXupper=10;
double rangeYlower=-10;
double rangeYupper=10;
int nPlotPoints=500;

MathParserMainWindow::MathParserMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MathParserMainWindow),
    mathParser(&mathEval)
{
    ui->setupUi(this);
    ui->plotGraphicsView->setScene(&scene);

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

void MathParserMainWindow::drawCoordinateSystem()
{
    double xstep=(double)ui->plotGraphicsView->width()/(rangeXupper-rangeXlower);
    double ystep=(double)ui->plotGraphicsView->height()/(rangeYupper-rangeYlower);
    double barlength=20;
    scene.addLine(0,ui->plotGraphicsView->height()/2,ui->plotGraphicsView->width(),ui->plotGraphicsView->height()/2);
    scene.addLine(ui->plotGraphicsView->width()/2,0,ui->plotGraphicsView->width()/2,ui->plotGraphicsView->height());
    for(int x=0;x<(int)(rangeXupper-rangeXlower);++x)
        scene.addLine(x*xstep,ui->plotGraphicsView->height()/2-barlength/2,x*xstep,ui->plotGraphicsView->height()/2+barlength/2);
    for(int y=0;y<(int)(rangeYupper-rangeYlower);++y)
        scene.addLine(ui->plotGraphicsView->width()/2-barlength/2,y*ystep,ui->plotGraphicsView->width()/2+barlength/2,y*ystep);
}

void MathParserMainWindow::drawPlot()
{
    double *xptr=varptr[(int)('x'-'a')];
    double xtmp=*xptr;
    double xstep=(rangeXupper-rangeXlower)/(double)(nPlotPoints-1);
    double result;
    bool first=true;
    QPointF lastpoint;
    QPointF currentpoint;
    for(int i=0;i<nPlotPoints;++i)
    {
        *xptr=rangeXlower+xstep*i;
        mathEval.run();
        result=mathEval.result();
        if(result==result)
        {
            currentpoint=QPointF((double)i/(double)(nPlotPoints-1)*ui->plotGraphicsView->width(),(0.5-(result/(rangeYupper-rangeYlower)))*ui->plotGraphicsView->height());
            if(!first)
                scene.addLine(QLineF(lastpoint,currentpoint));
            lastpoint=currentpoint;
            first=false;
        }
    }
    *xptr=xtmp;
}

void MathParserMainWindow::on_calculatePushButton_clicked()
{
    mathParser.setString(ui->formulaLineEdit->text());
    if(mathParser.parse())
    {
        mathEval.run();
        ui->resultLineEdit->setText(QString::number(mathEval.result()));
        scene.clear();
        drawCoordinateSystem();
        drawPlot();
        ui->plotGraphicsView->setSceneRect(0,0,ui->plotGraphicsView->width(),ui->plotGraphicsView->height());
        ui->plotGraphicsView->update();
        update();
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

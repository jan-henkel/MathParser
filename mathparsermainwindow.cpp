#include "mathparsermainwindow.h"
#include "ui_mathparsermainwindow.h"

constexpr double rangeXlower=-10;
constexpr double rangeXupper=10;
constexpr double rangeYlower=-10;
constexpr double rangeYupper=10;
constexpr int nPlotPoints=500;

MathParserMainWindow::MathParserMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MathParserMainWindow),
    mathParser(&mathEval)
{
    ui->setupUi(this);
    ui->plotGraphicsView->setScene(&scene);

    for(char c='a';c<='z';++c)
    {
        varptr[static_cast<size_t>(c-'a')]=mathEval.getVarPtr(c);
        *(varptr[static_cast<size_t>(c-'a')])=0.;
        ui->variablesComboBox->addItem(QString(QChar(c)));
    }
}

MathParserMainWindow::~MathParserMainWindow()
{
    delete ui;
}

void MathParserMainWindow::drawCoordinateSystem()
{
    double xstep=static_cast<double>(ui->plotGraphicsView->width())/(rangeXupper-rangeXlower);
    double ystep=static_cast<double>(ui->plotGraphicsView->height())/(rangeYupper-rangeYlower);
    double barlength=20;
    scene.addLine(0,ui->plotGraphicsView->height()/2,ui->plotGraphicsView->width(),ui->plotGraphicsView->height()/2);
    scene.addLine(ui->plotGraphicsView->width()/2,0,ui->plotGraphicsView->width()/2,ui->plotGraphicsView->height());
    for(size_t x=0;x<static_cast<size_t>(rangeXupper-rangeXlower);++x)
        scene.addLine(x*xstep,ui->plotGraphicsView->height()/2-barlength/2,x*xstep,ui->plotGraphicsView->height()/2+barlength/2);
    for(size_t y=0;y<static_cast<size_t>(rangeYupper-rangeYlower);++y)
        scene.addLine(ui->plotGraphicsView->width()/2-barlength/2,y*ystep,ui->plotGraphicsView->width()/2+barlength/2,y*ystep);
}

void MathParserMainWindow::drawPlot()
{
    double *xptr=varptr[static_cast<size_t>('x'-'a')];
    double xtmp=*xptr;
    double xstep=(rangeXupper-rangeXlower)/static_cast<double>(nPlotPoints-1);
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
            currentpoint=QPointF(static_cast<double>(i)/(nPlotPoints-1)*ui->plotGraphicsView->width(),(0.5-(result/(rangeYupper-rangeYlower)))*ui->plotGraphicsView->height());
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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <algorithm>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->customPlot->setInteraction(QCP::iRangeDrag, true);
    ui->customPlot->setInteraction(QCP::iRangeZoom, true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnOpenDiagBtn_clicked()
{
    ui->txtCSVFilePath->setText(QFileDialog::getOpenFileName(this, tr("Open CSV file"), ".", tr("CSV Files (*.csv)")));
    ui->dbSpinMaxNofPoint->setValue(this->readCSVData() - 1);
}

int MainWindow::readCSVData(){

    if(ui->txtCSVFilePath->text() == "")
        QMessageBox::information(0, "Error", "No CSV file is specified.");

    QFile file(ui->txtCSVFilePath->text());
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    this->xAxisData.resize(1000000);
    this->yAxisData.resize(1000000);
    int i = 0;
    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList values = line.split(",");
        this->xAxisData[i] = (double) values[0].toDouble();
        this->yAxisData[i] = (double) values[1].toDouble();
        i += 1;
    }
    file.close();
    return(i);
}
QVector<double> MainWindow::slice(const QVector<double>& v, int start=0, int end=-1) {
    int oldlen = v.size();
    int newlen;

    if (end == -1 or end >= oldlen){
        newlen = oldlen-start;
    } else {
        newlen = end-start;
    }

    QVector<double> nv(newlen);

    for (int i=0; i<newlen; i++) {
        nv[i] = v[start+i];
    }
    return nv;
}
void MainWindow::on_btnPlot_clicked()
{
    this->readCSVData();
    this->xAxisData.resize(ui->dbSpinMaxNofPoint->value());
    this->yAxisData = this->slice(this->yAxisData, (int)ui->dbSpinStartNofPoint->value(), (int)ui->dbSpinMaxNofPoint->value());
    // create graph and assign data to it:
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(this->xAxisData, this->yAxisData);
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(*std::min_element(xAxisData.constBegin(), xAxisData.constEnd()) - 1, *std::max_element(xAxisData.constBegin(), xAxisData.constEnd()) + 1);
    ui->customPlot->yAxis->setRange(*std::min_element(yAxisData.constBegin(), yAxisData.constEnd()) - 1, *std::max_element(yAxisData.constBegin(), yAxisData.constEnd()) + 1);
    ui->customPlot->replot();
}

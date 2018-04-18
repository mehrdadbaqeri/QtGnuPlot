#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnOpenDiagBtn_clicked();

    void on_btnPlot_clicked();

private:
    Ui::MainWindow *ui;
    QVector<double> xAxisData;
    QVector<double> yAxisData;
    QVector<double> slice(const QVector<double>&, int, int);
    int readCSVData();
};

#endif // MAINWINDOW_H

#pragma once

#include <QMainWindow>
#include <vector>
#include "csv_reader.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    std::string inputString;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void generateColumnMeanStd(int col);
    //     void on_pushButton_SignUp_clicked();

private:
    Ui::MainWindow *ui;
    CSVReader csvReader;
    int pageIndex = 0,
        histogramIndex = 0,
        scatterXIndex = 0,
        scatterYIndex = 1,
        scatter_power = 1;
    void resetMenuSelected();
    void initialiseGraphs();
    void createRawDataView();
    void createHistogram(bool displayNormal);
    void createScatter();
    void createCorrelationMatrix();
    void createPca();
    // LoginDialog *loginDialog;
    // SignUpDialog *signUpDialog;
};
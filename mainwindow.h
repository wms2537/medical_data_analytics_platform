#pragma once

#include <QMainWindow>
#include <vector>
#include "csv_reader.h"
#include <QWidget>

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
    QWidget* pca3d;
    QWidget* cluster3d;
    bool ispca3d = false, iscluster3d = false, fileLoaded = false;
    CSVReader csvReader;
    int pageIndex = 0,
        histogramIndex = 0,
        scatterXIndex = 0,
        scatterYIndex = 1,
        scatter_power = 1,
        num_clusters = 2;
    void resetMenuSelected();
    void initialiseGraphs();
    void createRawDataView();
    void createHistogram(bool displayNormal);
    void createScatter();
    void createCorrelationMatrix();
    void createPca();
    void createClustering();
    void trainXgBoostAndCreateChart();
    // LoginDialog *loginDialog;
    // SignUpDialog *signUpDialog;
};
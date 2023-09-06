#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QBarSet>
#include <QBarSeries>
#include <QSplineSeries>
#include <QBarCategoryAxis>
#include <QCategoryAxis>
#include <QScatterSeries>
#include <Q3DScatter>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pageIndex = 0;
    ui->stackedWidget->setCurrentIndex(pageIndex);
    csvReader.loadFile("/Users/sohweimeng/Documents/cpp_dev/medical_data_analytics_platform/breast-cancer.csv");
    initialiseGraphs();
    connect(ui->actionLoad, &QAction::triggered, this, [=]
            {
        QString filename = QFileDialog::getOpenFileName( 
        this, 
        tr("Load CSV"), 
        QDir::currentPath(), 
        tr("CSV files (*.csv)") );
        csvReader.loadFile(filename.toStdString());
                initialiseGraphs(); });

    connect(ui->tableWidget->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(generateColumnMeanStd(int)));

    connect(ui->comboBox_histogram, &QComboBox::currentIndexChanged, this, [=](int index)
            { histogramIndex = index;
            createHistogram(ui->checkBox_histogram->isChecked()); });
    connect(ui->checkBox_histogram, &QCheckBox::stateChanged, this, [=](int state)
            { createHistogram(ui->checkBox_histogram->isChecked()); });

    connect(ui->comboBox_scatter_x, &QComboBox::currentIndexChanged, this, [=](int index)
            { 
                if(index == scatterYIndex) {
                    QMessageBox::warning(this, "Error", "X axis and Y axis cannot be the same column.");
                    return;
                }
                scatterXIndex = index;
            createScatter(); });
    connect(ui->comboBox_scatter_y, &QComboBox::currentIndexChanged, this, [=](int index)
            { 
                if(index == scatterXIndex) {
                    QMessageBox::warning(this, "Error", "Y axis and X axis cannot be the same column.");
                    return;
                }
                scatterYIndex = index;
            createScatter(); });
    connect(ui->spinBox_scatter_power, &QSpinBox::valueChanged, this, [=](int val)
            {
        scatter_power = val;
        createScatter(); });

    connect(ui->comboBox_correlation_type, &QComboBox::currentIndexChanged, this, [=](int index)
            { createCorrelationMatrix(); });
    connect(ui->comboBox_correlation_columns, &MultiSelectComboBox::selectionChanged, this, [=]
            { createCorrelationMatrix(); });

    connect(ui->comboBox_pca_columns, &MultiSelectComboBox::selectionChanged, this, [=]
            { createPca(); });
    connect(ui->comboBox_pca_dims, &QComboBox::currentIndexChanged, this, [=](int index)
            { createPca(); });

    connect(ui->label_menu_raw_data_view, &ClickableLabel::clicked, [=]
            { 
                if(!csvReader.fileSelected()) return;
                pageIndex = 1;
                resetMenuSelected();
                ui->label_menu_raw_data_view->setStyleSheet("background-color: gray;");
                ui->stackedWidget->setCurrentIndex(pageIndex); });
    connect(ui->label_menu_histogram, &ClickableLabel::clicked, [=]
            { 
                if(!csvReader.fileSelected()) return;
                pageIndex = 2;
                resetMenuSelected();
                ui->label_menu_histogram->setStyleSheet("background-color: gray;");
                ui->stackedWidget->setCurrentIndex(pageIndex); });
    connect(ui->label_menu_scatter_plot, &ClickableLabel::clicked, [=]
            { 
                if(!csvReader.fileSelected()) return;
                pageIndex = 3;
                resetMenuSelected();
                ui->label_menu_scatter_plot->setStyleSheet("background-color: gray;");
                ui->stackedWidget->setCurrentIndex(pageIndex); });
    connect(ui->label_menu_correlation_matrix, &ClickableLabel::clicked, [=]
            { 
                if(!csvReader.fileSelected()) return;
                pageIndex = 4;
                resetMenuSelected();
                ui->label_menu_correlation_matrix->setStyleSheet("background-color: gray;");
                ui->stackedWidget->setCurrentIndex(pageIndex); });
    connect(ui->label_menu_pca_analysis, &ClickableLabel::clicked, [=]
            { 
                if(!csvReader.fileSelected()) return;
                pageIndex = 5;
                resetMenuSelected();
                ui->label_menu_pca_analysis->setStyleSheet("background-color: gray;");
                ui->stackedWidget->setCurrentIndex(pageIndex); });
    connect(ui->label_menu_clustering, &ClickableLabel::clicked, [=]
            { 
                if(!csvReader.fileSelected()) return;
                pageIndex = 6;
                resetMenuSelected();
                ui->label_menu_clustering->setStyleSheet("background-color: gray;");
                ui->stackedWidget->setCurrentIndex(pageIndex); });
    connect(ui->label_menu_causal_analysis, &ClickableLabel::clicked, [=]
            { 
                if(!csvReader.fileSelected()) return;
                pageIndex = 7;
                resetMenuSelected();
                ui->label_menu_causal_analysis->setStyleSheet("background-color: gray;");
                ui->stackedWidget->setCurrentIndex(pageIndex); });
}

void MainWindow::resetMenuSelected()
{
    ui->label_menu_raw_data_view->setStyleSheet("");
    ui->label_menu_histogram->setStyleSheet("");
    ui->label_menu_scatter_plot->setStyleSheet("");
    ui->label_menu_correlation_matrix->setStyleSheet("");
    ui->label_menu_pca_analysis->setStyleSheet("");
    ui->label_menu_clustering->setStyleSheet("");
    ui->label_menu_causal_analysis->setStyleSheet("");
}

void MainWindow::initialiseGraphs()
{
    if (csvReader.getContent().size() == 0)
    {
        return;
    }
    ui->comboBox_histogram->clear();
    ui->comboBox_scatter_x->clear();
    ui->comboBox_scatter_y->clear();
    ui->comboBox_correlation_columns->clear();
    ui->comboBox_pca_columns->clear();
    for (std::string h : csvReader.getHeaders())
    {
        if (h.compare("id") == 0)
            continue;
        ui->comboBox_histogram->addItem(QString::fromStdString(h));
        ui->comboBox_scatter_x->addItem(QString::fromStdString(h));
        ui->comboBox_scatter_y->addItem(QString::fromStdString(h));
        ui->comboBox_correlation_columns->addItem(QString::fromStdString(h));
        if (h.compare("diagnosis") == 0)
            continue;
        ui->comboBox_pca_columns->addItem(QString::fromStdString(h));
    }
    for (int i = 0; i < 5; i++)
    {
        ui->comboBox_correlation_columns->setSelected(i, true);
        ui->comboBox_pca_columns->setSelected(i, true);
    }

    ui->comboBox_scatter_y->setCurrentIndex(scatterYIndex);
    createRawDataView();
    createHistogram(ui->checkBox_histogram->isChecked());
    createScatter();
    createCorrelationMatrix();
    createPca();
}

void MainWindow::createRawDataView()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(csvReader.getContent().size());
    ui->tableWidget->setColumnCount(csvReader.getContent()[0].size());
    for (int i = 0; i < csvReader.getContent().size(); i++)
    {
        for (int j = 0; j < csvReader.getContent()[0].size(); j++)
        {
            QTableWidgetItem *newItem = new QTableWidgetItem(QString::fromStdString(std::to_string(csvReader.getContent()[i][j])));
            ui->tableWidget->setItem(i, j, newItem);
        }
    }
    QStringList headerLabels;
    for (std::string h : csvReader.getHeaders())
    {
        headerLabels.push_back(QString::fromStdString(h));
    }

    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::createHistogram(bool displayNormal)
{
    int nbins = 10;
    if (histogramIndex == 0)
    {
        auto set0 = new QBarSet("B");
        auto set1 = new QBarSet("M");
        auto allData = csvReader.getContent();
        int bCount = 0, mCount = 0;
        for (auto row : allData)
        {
            if (row[1] == 0)
            {
                bCount++;
            }
            else
            {
                mCount++;
            }
        }
        *set0 << bCount;
        *set1 << mCount;
        auto barseries = new QBarSeries;
        barseries->append(set0);
        barseries->append(set1);
        auto chart = new QChart;
        chart->addSeries(barseries);
        chart->setTitle("Histogram of " + QString::fromStdString(csvReader.getHeaders()[histogramIndex + 1]));
        auto axisY = new QValueAxis;
        chart->addAxis(axisY, Qt::AlignLeft);
        barseries->attachAxis(axisY);
        axisY->setRange(0, max(bCount, mCount));

        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);

        ui->histogram->setChart(chart);
        return;
    }
    std::vector<float> bins;
    float startIntervals[30] = {5.0, 5.0, 40, 100, 0.05, 0, 0, 0, 0.105, 0.048, 0, 0, 0, 0, 0.0015, 0, 0, 0, 0, 0, 7, 10, 50, 0, 0.07, 0, 0, 0, 0, 0.05};
    float binSizes[30] = {2.5, 3.5, 15, 250, 0.012, 0.04, 0.045, 0.021, 0.02, 0.005, 0.3, 0.5, 2.2, 55, 0.003, 0.014, 0.04, 0.0053, 0.008, 0.003, 3, 4, 21, 500, 0.016, 0.11, 0.13, 0.03, 0.055, 0.16};
    QStringList categories;
    int values[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i <= 10; i++)
    {
        bins.push_back(startIntervals[histogramIndex - 1] + binSizes[histogramIndex - 1] * i);
        if (i != 0)
        {
            categories << QString::fromStdString(std::to_string(bins[i]));
        }
    }
    for (auto row : csvReader.getContent())
    {
        for (int i = 0; i < 10; i++)
        {
            if (row[histogramIndex + 1] < bins[i + 1])
            {
                values[i]++;
                break;
            }
        }
    }

    auto set = new QBarSet(QString::fromStdString(csvReader.getHeaders()[histogramIndex + 1]));
    int maxValue = 0;
    for (int i = 0; i < 10; i++)
    {
        *set << values[i];
        if (values[i] > maxValue)
        {
            maxValue = values[i];
        }
    }
    auto barseries = new QBarSeries;
    barseries->append(set);
    barseries->setBarWidth(1);
    auto chart = new QChart;
    chart->addSeries(barseries);
    chart->setTitle("Histogram of " + QString::fromStdString(csvReader.getHeaders()[histogramIndex + 1]));

    auto axisX = new QBarCategoryAxis;
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    barseries->attachAxis(axisX);
    axisX->setRange(QString::fromStdString(std::to_string(bins.front())), QString::fromStdString(std::to_string(bins.back())));

    auto axisY = new QValueAxis;
    chart->addAxis(axisY, Qt::AlignLeft);
    barseries->attachAxis(axisY);
    axisY->setRange(0, maxValue);

    if (displayNormal)
    {
        auto lineseries = new QSplineSeries;
        lineseries->setName("trend");
        MeanStdResult result = csvReader.getMeanStd(histogramIndex + 1);
        float mean = result.mean;
        float std = std::sqrt(result.variance);
        for (int i = 0; i < 10; i++)
        {
            float x = (bins[i] + bins[i + 1]) / 2.0;
            float pdf_gaussian = exp(-0.5 * pow((x - mean) / std, 2.0));
            lineseries->append(QPointF(i, pdf_gaussian * maxValue));
        }
        chart->addSeries(lineseries);
        lineseries->attachAxis(axisX);
        lineseries->attachAxis(axisY);
    }

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    ui->histogram->setChart(chart);
}

void MainWindow::createScatter()
{
    auto series0 = new QScatterSeries;
    series0->setName("plot");
    series0->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    series0->setMarkerSize(8.0);
    int index = 0;
    for (auto row : csvReader.getContent())
    {
        series0->append(row[scatterXIndex + 1], row[scatterYIndex + 1]);
        index++;
    }
    connect(series0, &QScatterSeries::hovered, this, [=](const QPointF &point, bool state)
            {
            if(state) {
                ui->label_scatter_coordinate->setText(QString::fromStdString("["+std::to_string(point.x())+","+std::to_string(point.y())+"]"));
            } });
    auto chart = new QChart;
    chart->addSeries(series0);
    chart->setTitle("Scatter Chart of " + QString::fromStdString(csvReader.getHeaders()[scatterYIndex + 1]) + " against " + QString::fromStdString(csvReader.getHeaders()[scatterXIndex + 1]));

    auto leastSqAndPR = csvReader.getLeastSquareAndPR(scatterXIndex + 1, scatterYIndex + 1, scatter_power);
    auto lineseries = new QSplineSeries;
    lineseries->setName("curve approx");

    for (auto x : std::get<3>(leastSqAndPR))
    {
        float y = 0;
        for (int j = 0; j <= scatter_power; j++)
        {
            y += std::get<0>(leastSqAndPR)[j] * std::pow(x, j);
        }
        if (y < 0)
            continue;
        std::cout << x << " " << y << endl;
        lineseries->append(QPointF(x, y));
        if (lineseries->points().size() == scatter_power)
            break;
    }
    float y = 0;
    for (int j = 0; j <= scatter_power; j++)
    {
        y += std::get<0>(leastSqAndPR)[j] * std::pow(std::get<3>(leastSqAndPR).back(), j);
    }
    std::cout << std::get<3>(leastSqAndPR).back() << " " << y << endl
              << endl;
    lineseries->append(QPointF(std::get<3>(leastSqAndPR).back(), y));
    chart->addSeries(lineseries);

    auto axisX = new QValueAxis;
    chart->addAxis(axisX, Qt::AlignBottom);
    series0->attachAxis(axisX);
    axisX->setRange(std::get<4>(leastSqAndPR), std::get<5>(leastSqAndPR));
    axisX->setTitleText(QString::fromStdString(csvReader.getHeaders()[scatterXIndex + 1]));

    auto axisY = new QValueAxis;
    chart->addAxis(axisY, Qt::AlignLeft);
    series0->attachAxis(axisY);
    axisY->setRange(std::get<6>(leastSqAndPR), std::get<7>(leastSqAndPR));
    axisY->setTitleText(QString::fromStdString(csvReader.getHeaders()[scatterYIndex + 1]));

    lineseries->attachAxis(axisX);
    lineseries->attachAxis(axisY);
    ui->label_scatter_p->setText(QString::fromStdString(std::to_string(std::get<1>(leastSqAndPR))));
    ui->label_scatter_r2->setText(QString::fromStdString(std::to_string(std::get<2>(leastSqAndPR))));
    chart->setDropShadowEnabled(false);
    ui->scatterPlot->setChart(chart);
}

void MainWindow::createCorrelationMatrix()
{
    auto selectedColumns = ui->comboBox_correlation_columns->getSelectedIndexes();

    std::vector<QString> axisLabels;
    for (auto col : selectedColumns)
    {
        axisLabels.push_back(QString::fromStdString(csvReader.getHeaders()[col]));
    }

    ui->correlation_matrix->setAxisLabels(axisLabels, axisLabels);
    auto res = csvReader.getCorrelationMatrix(selectedColumns, ui->comboBox_correlation_type->currentIndex() == 1);
    for (auto row : std::get<0>(res))
    {
        for (auto e : row)
            std::cout << e;
        std::cout << endl;
    }

    ui->correlation_matrix->setMinMax(std::get<1>(res), std::get<2>(res));
    ui->correlation_legend->setMinMax(std::get<1>(res), std::get<2>(res));
    ui->correlation_matrix->setData(std::get<0>(res));
}

void MainWindow::createPca()
{
    auto selectedColumns = ui->comboBox_pca_columns->getSelectedIndexes();
    if (ui->comboBox_pca_dims->currentIndex() == 0)
    {
        auto res = csvReader.getPca(selectedColumns, 2);

        auto seriesMalignant = new QScatterSeries;
        seriesMalignant->setName("Malignant");
        seriesMalignant->setMarkerShape(QScatterSeries::MarkerShapeCircle);
        seriesMalignant->setMarkerSize(8.0);
        seriesMalignant->setColor(QColor::fromRgb(255, 0, 0));

        auto seriesBenign = new QScatterSeries;
        seriesBenign->setName("Benign");
        seriesBenign->setMarkerShape(QScatterSeries::MarkerShapeCircle);
        seriesBenign->setMarkerSize(8.0);
        seriesBenign->setColor(QColor::fromRgb(0, 255, 0));

        for (int i = 0; i < res.size(); i++)
        {
            auto row = csvReader.getContent()[i];
            if (row[1] > 0)
            {
                seriesMalignant->append(res[i][0], res[i][1]);
            }
            else
            {
                seriesBenign->append(res[i][0], res[i][1]);
            }
        }
        connect(seriesMalignant, &QScatterSeries::clicked, this, [=](const QPointF &point)
                { QMessageBox::about(this, "Malignant", "data"); });

        auto chart = new QChart;
        chart->addSeries(seriesMalignant);
        chart->addSeries(seriesBenign);
        chart->createDefaultAxes();
        chart->setDropShadowEnabled(false);
        ui->pca_chart->setChart(chart);
        ui->pca_chart->show();
        ui->pca_3d_layout->removeItem(ui->pca_3d_layout->itemAt(0));
    }
    else
    {
        auto res = csvReader.getPca(selectedColumns, 3);
        Q3DScatter *scatter3d = new Q3DScatter();
        scatter3d->setFlags(scatter3d->flags() ^ Qt::FramelessWindowHint);

        QScatter3DSeries *seriesMalignant = new QScatter3DSeries;
        seriesMalignant->setName("Malignant");
        seriesMalignant->setBaseColor(QColor::fromRgb(255, 0, 0));
        seriesMalignant->setItemSize(5);
        QScatter3DSeries *seriesBenign = new QScatter3DSeries;
        seriesBenign->setName("Benign");
        seriesBenign->setBaseColor(QColor::fromRgb(0, 255, 0));
        seriesBenign->setItemSize(5);
        for (int i = 0; i < res.size(); i++)
        {
            auto row = csvReader.getContent()[i];
            if (row[1] > 0)
            {
                seriesMalignant->dataProxy()->addItem(QVector3D(res[i][0], res[i][1], res[i][2]));
            }
            else
            {
                seriesBenign->dataProxy()->addItem(QVector3D(res[i][0], res[i][1], res[i][2]));
            }
        }
        scatter3d->addSeries(seriesMalignant);
        scatter3d->addSeries(seriesBenign);
        scatter3d->setAspectRatio(1.0);
        scatter3d->setHorizontalAspectRatio(1.0);
        scatter3d->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);
        QVBoxLayout *layout = new QVBoxLayout();
        QWidget *scatterWindow = QWidget::createWindowContainer(scatter3d);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(scatterWindow->sizePolicy().hasHeightForWidth());
        scatterWindow->setSizePolicy(sizePolicy);
        ui->pca_3d_layout->addWidget(scatterWindow);
        ui->pca_chart->hide();
    }
}

void MainWindow::generateColumnMeanStd(int col)
{
    MeanStdResult result = csvReader.getMeanStd(col);
    QMessageBox::warning(this, "Mean Var", ("Mean of column " + result.columnName + " is " + std::to_string(result.mean) + "\n\nVariance of column " + result.columnName + " is " + std::to_string(result.variance)).c_str());
    std::cout << col;
}

MainWindow::~MainWindow()
{
    delete ui;
}
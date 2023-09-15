#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QBarSet>
#include <QHorizontalBarSeries>
#include <QBarSeries>
#include <QSplineSeries>
#include <QBarCategoryAxis>
#include <QCategoryAxis>
#include <QScatterSeries>
#include <Q3DScatter>
#include <QLegendMarker>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pageIndex = 0;
    ui->stackedWidget->setCurrentIndex(pageIndex);
    // csvReader.loadFile("/Users/sohweimeng/Documents/cpp_dev/medical_data_analytics_platform/breast-cancer.csv");
    // initialiseGraphs();
    // fileLoaded = true;
    connect(ui->actionLoad, &QAction::triggered, this, [=]
            {
                fileLoaded = false;
        QString filename = QFileDialog::getOpenFileName( 
        this, 
        tr("Load CSV"), 
        QDir::currentPath(), 
        tr("CSV files (*.csv)") );
        csvReader.loadFile(filename.toStdString());
                initialiseGraphs();
                fileLoaded = true; });

    connect(ui->pushButton_load_csv_data, &QPushButton::clicked, this, [=]
            {
        QString filename = QFileDialog::getOpenFileName( 
        this, 
        tr("Load CSV"), 
        QDir::currentPath(), 
        tr("CSV files (*.csv)") );
        csvReader.loadFile(filename.toStdString());
                initialiseGraphs();
                fileLoaded = true; });
    connect(ui->tableWidget->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(generateColumnMeanStd(int)));

    connect(ui->comboBox_histogram, &QComboBox::currentIndexChanged, this, [=](int index)
            { if(!fileLoaded) return;
                histogramIndex = index;
            createHistogram(ui->checkBox_histogram->isChecked()); });
    connect(ui->checkBox_histogram, &QCheckBox::stateChanged, this, [=](int state)
            { createHistogram(ui->checkBox_histogram->isChecked()); });

    connect(ui->comboBox_scatter_x, &QComboBox::currentIndexChanged, this, [=](int index)
            { 
                if(!fileLoaded) return;
                if(index == scatterYIndex) {
                    QMessageBox::warning(this, "Error", "X axis and Y axis cannot be the same column.");
                    return;
                }
                scatterXIndex = index;
            createScatter(); });
    connect(ui->comboBox_scatter_y, &QComboBox::currentIndexChanged, this, [=](int index)
            { 
                if(!fileLoaded) return;
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
            { if(!fileLoaded) return;
                createCorrelationMatrix(); });
    connect(ui->comboBox_correlation_columns, &MultiSelectComboBox::selectionChanged, this, [=]
            { if(!fileLoaded) return;
                createCorrelationMatrix(); });

    connect(ui->comboBox_pca_columns, &MultiSelectComboBox::selectionChanged, this, [=]
            { if(!fileLoaded) return;
                createPca(); });
    connect(ui->comboBox_pca_dims, &QComboBox::currentIndexChanged, this, [=](int index)
            { if(!fileLoaded) return;
                createPca(); });

    connect(ui->comboBox_clustering_columns, &MultiSelectComboBox::selectionChanged, this, [=]
            { if(!fileLoaded) return;
                createClustering(); });
    connect(ui->comboBox_clustering_algorithm, &QComboBox::currentIndexChanged, this, [=](int index)
            { if(!fileLoaded) return;
                createClustering(); });
    connect(ui->comboBox_clustering_dim, &QComboBox::currentIndexChanged, this, [=](int index)
            { if(!fileLoaded) return;
                createClustering(); });
    connect(ui->spinBox_num_clusters, &QSpinBox::valueChanged, this, [=](int val)
            {
        if(!fileLoaded) return;
        num_clusters = val;
        createClustering(); });

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

    connect(ui->pushButton_causal_train, &QPushButton::clicked, this, [=]
            { trainXgBoostAndCreateChart(); });
    connect(ui->pushButton_causal_scrolldown, &QPushButton::clicked, this, [=]
            { ui->causal_result->chart()->scroll(0, -20); });
    connect(ui->pushButton_causal_scrollup, &QPushButton::clicked, this, [=]
            { ui->causal_result->chart()->scroll(0, 20); });
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
        ui->comboBox_clustering_columns->addItem(QString::fromStdString(h));
    }
    for (int i = 0; i < 5; i++)
    {
        ui->comboBox_correlation_columns->setSelected(i, true);
        ui->comboBox_pca_columns->setSelected(i, true);
        ui->comboBox_clustering_columns->setSelected(i, true);
    }

    ui->comboBox_scatter_y->setCurrentIndex(scatterYIndex);
    createRawDataView();
    createHistogram(ui->checkBox_histogram->isChecked());
    createScatter();
    createCorrelationMatrix();
    createPca();
    createClustering();

    pageIndex = 1;
    ui->stackedWidget->setCurrentIndex(pageIndex);
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
    headerLabels.push_back("KMeans");
    headerLabels.push_back("DBSCAN");
    headerLabels.push_back("Spectral");
    headerLabels.push_back("Mean Shift");

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
        axisY->setRange(0, std::max(bCount, mCount));

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
        // std::cout << x << " " << y << endl;
        lineseries->append(QPointF(x, y));
        // if (lineseries->points().size() == scatter_power)
        //     break;
    }
    // float y = 0;
    // for (int j = 0; j <= scatter_power; j++)
    // {
    //     y += std::get<0>(leastSqAndPR)[j] * std::pow(std::get<3>(leastSqAndPR).back(), j);
    // }
    // // std::cout << std::get<3>(leastSqAndPR).back() << " " << y << endl
    // //   << endl;
    // lineseries->append(QPointF(std::get<3>(leastSqAndPR).back(), y));
    chart->addSeries(lineseries);

    auto axisX = new QValueAxis;
    chart->addAxis(axisX, Qt::AlignBottom);
    series0->attachAxis(axisX);
    lineseries->attachAxis(axisX);
    axisX->setRange(std::get<4>(leastSqAndPR), std::get<5>(leastSqAndPR));
    axisX->setTitleText(QString::fromStdString(csvReader.getHeaders()[scatterXIndex + 1]));

    auto axisY = new QValueAxis;
    chart->addAxis(axisY, Qt::AlignLeft);
    series0->attachAxis(axisY);
    lineseries->attachAxis(axisY);
    axisY->setRange(std::get<6>(leastSqAndPR), std::get<7>(leastSqAndPR));
    axisY->setTitleText(QString::fromStdString(csvReader.getHeaders()[scatterYIndex + 1]));

    
    
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
    // for (auto row : std::get<0>(res))
    // {
    // for (auto e : row)
    // std::cout << e;
    // std::cout << endl;
    // }

    ui->correlation_matrix->setMinMax(std::get<1>(res), std::get<2>(res));
    ui->correlation_legend->setMinMax(std::get<1>(res), std::get<2>(res));
    ui->correlation_matrix->setData(std::get<0>(res));
}

void MainWindow::createPca()
{
    if (ispca3d)
    {
        ui->pca_3d_layout->removeWidget(pca3d);
        pca3d->deleteLater();
        ispca3d = false;
    }
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
    }
    else
    {
        auto res = csvReader.getPca(selectedColumns, 3);
        Q3DScatter *scatter3d = new Q3DScatter();
        scatter3d->setFlags(scatter3d->flags() ^ Qt::FramelessWindowHint);

        QScatter3DSeries *seriesMalignant = new QScatter3DSeries;
        seriesMalignant->setName("Malignant");
        seriesMalignant->setBaseColor(QColor::fromRgb(255, 0, 0));
        seriesMalignant->setItemSize(0.1);
        QScatter3DSeries *seriesBenign = new QScatter3DSeries;
        seriesBenign->setName("Benign");
        seriesBenign->setBaseColor(QColor::fromRgb(0, 255, 0));
        seriesBenign->setItemSize(0.1);
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
        QWidget *scatterWindow = QWidget::createWindowContainer(scatter3d);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(scatterWindow->sizePolicy().hasHeightForWidth());
        scatterWindow->setSizePolicy(sizePolicy);
        ui->pca_3d_layout->addWidget(scatterWindow);
        ui->pca_chart->hide();
        pca3d = scatterWindow;
        ispca3d = true;
    }
}

void MainWindow::createClustering()
{
    // std::cout << "1" << endl;
    ui->cluster_legends->clear();
    // std::cout << "2" << endl;
    if (iscluster3d)
    {
        ui->cluster3d->removeWidget(cluster3d);
        cluster3d->deleteLater();
        iscluster3d = false;
    }
    // std::cout << "3" << endl;
    auto selectedColumns = ui->comboBox_pca_columns->getSelectedIndexes();
    std::vector<QColor> distinctColors = {
        QColor::fromRgb(255, 0, 0),     // Red
        QColor::fromRgb(0, 255, 0),     // Green
        QColor::fromRgb(0, 0, 255),     // Blue
        QColor::fromRgb(255, 255, 0),   // Yellow
        QColor::fromRgb(255, 0, 255),   // Magenta
        QColor::fromRgb(0, 255, 255),   // Cyan
        QColor::fromRgb(128, 0, 0),     // Maroon
        QColor::fromRgb(0, 128, 0),     // Olive
        QColor::fromRgb(0, 0, 128),     // Navy
        QColor::fromRgb(128, 128, 0),   // Olive Green
        QColor::fromRgb(128, 0, 128),   // Purple
        QColor::fromRgb(0, 128, 128),   // Teal
        QColor::fromRgb(255, 165, 0),   // Orange
        QColor::fromRgb(128, 128, 128), // Gray
        QColor::fromRgb(255, 192, 203), // Pink
        QColor::fromRgb(255, 140, 0),   // Dark Orange
        QColor::fromRgb(0, 128, 128),   // Dark Cyan
        QColor::fromRgb(139, 69, 19),   // Saddle Brown
        QColor::fromRgb(0, 255, 127),   // Spring Green
        QColor::fromRgb(218, 112, 214)  // Orchid
    };

    auto clusterRes = csvReader.clustering(static_cast<ClusteringAlgorithm>(ui->comboBox_clustering_algorithm->currentIndex()), selectedColumns, num_clusters);

    if (ui->comboBox_clustering_dim->currentIndex() == 0)
    {
        // std::cout << "4" << endl;
        auto res = csvReader.getPca(selectedColumns, 2);

        std::vector<QScatterSeries *> seriesList;

        for (int i = 0; i < res.size(); i++)
        {
            auto row = csvReader.getContent()[i];
            if (clusterRes[i] < 0)
                continue;
            while (clusterRes[i] >= seriesList.size())
            {
                auto myseries = new QScatterSeries;
                myseries->setName(QString::number(seriesList.size() + 1));
                myseries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
                myseries->setMarkerSize(5.0);
                myseries->setColor(distinctColors[seriesList.size() % 20]);
                seriesList.push_back(myseries);
            }
            seriesList[clusterRes[i]]->append(res[i][0], res[i][1]);
        }
        // connect(seriesMalignant, &QScatterSeries::clicked, this, [=](const QPointF &point)
        //         { QMessageBox::about(this, "Malignant", "data"); });

        auto chart = new QChart;
        for (auto s : seriesList)
        {
            chart->addSeries(s);
        }

        ui->cluster_legends->assignChart(chart);
        for (int i = 0; i < seriesList.size(); i++)
        {
            ui->cluster_legends->addSeries(seriesList[i], QString::number(i + 1));
        }
        chart->legend()->setVisible(false);

        chart->createDefaultAxes();
        chart->setDropShadowEnabled(false);
        ui->cluster2d->setChart(chart);
        ui->cluster2d->show();
    }
    else
    {
        auto res = csvReader.getPca(selectedColumns, 3);
        Q3DScatter *scatter3d = new Q3DScatter();
        scatter3d->setFlags(scatter3d->flags() ^ Qt::FramelessWindowHint);

        std::vector<QScatter3DSeries *> seriesList;
        for (int i = 0; i < res.size(); i++)
        {
            auto row = csvReader.getContent()[i];
            if (clusterRes[i] < 0)
                continue;
            while (clusterRes[i] >= seriesList.size())
            {
                QScatter3DSeries *myseries = new QScatter3DSeries;
                myseries->setName(QString::number(seriesList.size() + 1));
                myseries->setItemSize(0.1);
                myseries->setBaseColor(distinctColors[seriesList.size() % 20]);
                seriesList.push_back(myseries);
            }
            seriesList[clusterRes[i]]->dataProxy()->addItem(QVector3D(res[i][0], res[i][1], res[i][2]));
        }
        ui->cluster_legends->assignChart(scatter3d);
        for (size_t i = 0; i < seriesList.size(); i++)
        {
            scatter3d->addSeries(seriesList[i]);
            ui->cluster_legends->addSeries(seriesList[i], QString::number(i + 1));
        }

        scatter3d->setAspectRatio(1.0);
        scatter3d->setHorizontalAspectRatio(1.0);
        scatter3d->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);
        QWidget *scatterWindow = QWidget::createWindowContainer(scatter3d);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(scatterWindow->sizePolicy().hasHeightForWidth());
        scatterWindow->setSizePolicy(sizePolicy);
        ui->cluster3d->addWidget(scatterWindow);
        ui->cluster2d->hide();
        cluster3d = scatterWindow;
        iscluster3d = true;
    }
}

void MainWindow::trainXgBoostAndCreateChart()
{
    float trainRatio = 0.9;
    switch (ui->comboBox_causal_traintest->currentIndex())
    {
    case 0:
        trainRatio = 0.9;
        break;
    case 1:
        trainRatio = 0.8;
        break;
    case 2:
        trainRatio = 0.7;
        break;
    case 3:
        trainRatio = 0.5;
        break;
    default:
        break;
    }
    ui->progressBar_causal->setMaximum(ui->spinBox_causal_epoch->value());
    auto res = csvReader.trainModel(trainRatio, ui->spinBox_causal_epoch->value(), ui->spinBox_causal_depth->value(), [=](int epoch)
                                    { ui->progressBar_causal->setValue(epoch); });
    ui->label_causal_acc->setText(QString::fromStdString(std::to_string(std::get<1>(res))));
    ui->label_causal_auc->setText(QString::fromStdString(std::to_string(std::get<2>(res))));
    ui->label_causal_f1->setText(QString::fromStdString(std::to_string(std::get<3>(res))));

    QBarSet *barSet = new QBarSet("Importance Score");
    QBarCategoryAxis *axisY = new QBarCategoryAxis;
    QValueAxis *axisX = new QValueAxis;
    axisX->setLabelFormat("%.2f");
    axisX->setTitleText("Score");
    axisY->setLabelsAngle(0);
    axisY->setLabelsFont(QFont("Arial", 10));

    // Add data to the bar set

    std::vector<std::pair<QString, float>> label_pred_pairs;
    for (size_t i = 0; i < csvReader.getHeaders().size() - 2; ++i)
    {
        label_pred_pairs.emplace_back(QString::fromStdString(csvReader.getHeaders()[i + 2]), std::get<4>(res)[i]);
    }

    // Sort by prediction values in ascending order
    std::sort(label_pred_pairs.begin(), label_pred_pairs.end(),
              [](const std::pair<QString, float> &a, const std::pair<QString, float> &b)
              {
                  return a.second < b.second;
              });
    for (const auto &pair : label_pred_pairs)
    {
        barSet->append(pair.second);
        axisY->append(pair.first);
    }

    QHorizontalBarSeries *barSeries = new QHorizontalBarSeries;
    barSeries->append(barSet);
    // barSeries->setBarWidth(1);

    QChart *chart = new QChart();
    chart->addSeries(barSeries);
    chart->setTitle("Feature Contributions");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    chart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);
    chart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);
    axisX->setMin(0);
    axisX->applyNiceNumbers();

    // Create a chart view
    ui->causal_result->setChart(chart);
    ui->causal_result->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->causal_result->setRenderHint(QPainter::Antialiasing); // Enable antialiasing for smoother rendering
    QRectF rect = ui->causal_result->chart()->plotArea();
    rect.setHeight(rect.height() / 4.0);
    QPointF newCenter = ui->causal_result->chart()->plotArea().center();
    newCenter.setY(newCenter.y() / 1.8);
    rect.moveCenter(newCenter);
    ui->causal_result->chart()->zoomIn(rect);
}

void MainWindow::generateColumnMeanStd(int col)
{
    MeanStdResult result = csvReader.getMeanStd(col);
    QMessageBox::warning(this, "Mean Var", ("Mean of column " + result.columnName + " is " + std::to_string(result.mean) + "\n\nVariance of column " + result.columnName + " is " + std::to_string(result.variance)).c_str());
    // std::cout << col;
}

MainWindow::~MainWindow()
{
    delete ui;
}
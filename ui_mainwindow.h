/********************************************************************************
** Form generated from reading UI file 'mainwindowcLdlND.ui'
**
** Created by: Qt User Interface Compiler version 6.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MAINWINDOWCLDLND_H
#define MAINWINDOWCLDLND_H

#include <QtCharts/QChartView>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "clickable_label.h"
#include "heatmap_legend.h"
#include "heatmap_widget.h"
#include "legend.h"
#include "multiselect_combobox.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLoad;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_menu;
    QLabel *label;
    ClickableLabel *label_menu_raw_data_view;
    ClickableLabel *label_menu_histogram;
    ClickableLabel *label_menu_scatter_plot;
    ClickableLabel *label_menu_correlation_matrix;
    ClickableLabel *label_menu_pca_analysis;
    ClickableLabel *label_menu_clustering;
    ClickableLabel *label_menu_causal_analysis;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;
    QStackedWidget *stackedWidget;
    QWidget *page_no_data;
    QPushButton *pushButton_load_csv_data;
    QLabel *label_2;
    QWidget *page_raw_data_view;
    QVBoxLayout *verticalLayout;
    QLabel *label_7;
    QTableWidget *tableWidget;
    QWidget *page_histogram;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_11;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_8;
    QComboBox *comboBox_histogram;
    QCheckBox *checkBox_histogram;
    QChartView *histogram;
    QWidget *page_scatter_plot;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_10;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_9;
    QComboBox *comboBox_scatter_x;
    QLabel *label_12;
    QComboBox *comboBox_scatter_y;
    QLabel *label_15;
    QSpinBox *spinBox_scatter_power;
    QChartView *scatterPlot;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_13;
    QLabel *label_scatter_p;
    QLabel *label_14;
    QLabel *label_scatter_r2;
    QLabel *label_16;
    QLabel *label_scatter_coordinate;
    QWidget *page_correlation_matrix;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_17;
    QComboBox *comboBox_correlation_type;
    QLabel *label_18;
    MultiSelectComboBox *comboBox_correlation_columns;
    QHBoxLayout *horizontalLayout_6;
    HeatmapWidget *correlation_matrix;
    HeatmapLegend *correlation_legend;
    QWidget *page_pca_analysis;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_19;
    MultiSelectComboBox *comboBox_pca_columns;
    QLabel *label_20;
    QComboBox *comboBox_pca_dims;
    QChartView *pca_chart;
    QVBoxLayout *pca_3d_layout;
    QWidget *page_clustering;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_24;
    MultiSelectComboBox *comboBox_clustering_columns;
    QLabel *label_21;
    QComboBox *comboBox_clustering_algorithm;
    QLabel *label_22;
    QSpinBox *spinBox_num_clusters;
    QLabel *label_23;
    QComboBox *comboBox_clustering_dim;
    QChartView *cluster2d;
    QVBoxLayout *cluster3d;
    LegendWidget *cluster_legends;
    QWidget *page_causal_analysis;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_6;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_25;
    QComboBox *comboBox_causal_traintest;
    QLabel *label_26;
    QSpinBox *spinBox_causal_epoch;
    QLabel *label_27;
    QSpinBox *spinBox_causal_depth;
    QLabel *label_28;
    QProgressBar *progressBar_causal;
    QChartView *causal_result;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(842, 609);
        actionLoad = new QAction(MainWindow);
        actionLoad->setObjectName("actionLoad");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        verticalLayout_menu = new QVBoxLayout();
        verticalLayout_menu->setObjectName("verticalLayout_menu");
        verticalLayout_menu->setSizeConstraint(QLayout::SetFixedSize);
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(24);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_menu->addWidget(label);

        label_menu_raw_data_view = new ClickableLabel(centralwidget);
        label_menu_raw_data_view->setObjectName("label_menu_raw_data_view");
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_menu_raw_data_view->sizePolicy().hasHeightForWidth());
        label_menu_raw_data_view->setSizePolicy(sizePolicy1);

        verticalLayout_menu->addWidget(label_menu_raw_data_view);

        label_menu_histogram = new ClickableLabel(centralwidget);
        label_menu_histogram->setObjectName("label_menu_histogram");

        verticalLayout_menu->addWidget(label_menu_histogram);

        label_menu_scatter_plot = new ClickableLabel(centralwidget);
        label_menu_scatter_plot->setObjectName("label_menu_scatter_plot");

        verticalLayout_menu->addWidget(label_menu_scatter_plot);

        label_menu_correlation_matrix = new ClickableLabel(centralwidget);
        label_menu_correlation_matrix->setObjectName("label_menu_correlation_matrix");

        verticalLayout_menu->addWidget(label_menu_correlation_matrix);

        label_menu_pca_analysis = new ClickableLabel(centralwidget);
        label_menu_pca_analysis->setObjectName("label_menu_pca_analysis");

        verticalLayout_menu->addWidget(label_menu_pca_analysis);

        label_menu_clustering = new ClickableLabel(centralwidget);
        label_menu_clustering->setObjectName("label_menu_clustering");

        verticalLayout_menu->addWidget(label_menu_clustering);

        label_menu_causal_analysis = new ClickableLabel(centralwidget);
        label_menu_causal_analysis->setObjectName("label_menu_causal_analysis");

        verticalLayout_menu->addWidget(label_menu_causal_analysis);

        horizontalSpacer = new QSpacerItem(150, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        verticalLayout_menu->addItem(horizontalSpacer);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_menu->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout_menu);

        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(stackedWidget->sizePolicy().hasHeightForWidth());
        stackedWidget->setSizePolicy(sizePolicy2);
        page_no_data = new QWidget();
        page_no_data->setObjectName("page_no_data");
        pushButton_load_csv_data = new QPushButton(page_no_data);
        pushButton_load_csv_data->setObjectName("pushButton_load_csv_data");
        pushButton_load_csv_data->setGeometry(QRect(220, 310, 161, 32));
        label_2 = new QLabel(page_no_data);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(0, 200, 601, 51));
        label_2->setAlignment(Qt::AlignCenter);
        stackedWidget->addWidget(page_no_data);
        page_raw_data_view = new QWidget();
        page_raw_data_view->setObjectName("page_raw_data_view");
        verticalLayout = new QVBoxLayout(page_raw_data_view);
        verticalLayout->setObjectName("verticalLayout");
        label_7 = new QLabel(page_raw_data_view);
        label_7->setObjectName("label_7");
        label_7->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_7);

        tableWidget = new QTableWidget(page_raw_data_view);
        tableWidget->setObjectName("tableWidget");

        verticalLayout->addWidget(tableWidget);

        stackedWidget->addWidget(page_raw_data_view);
        page_histogram = new QWidget();
        page_histogram->setObjectName("page_histogram");
        verticalLayout_2 = new QVBoxLayout(page_histogram);
        verticalLayout_2->setObjectName("verticalLayout_2");
        label_11 = new QLabel(page_histogram);
        label_11->setObjectName("label_11");
        label_11->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_11);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_8 = new QLabel(page_histogram);
        label_8->setObjectName("label_8");
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy3);

        horizontalLayout_2->addWidget(label_8);

        comboBox_histogram = new QComboBox(page_histogram);
        comboBox_histogram->setObjectName("comboBox_histogram");

        horizontalLayout_2->addWidget(comboBox_histogram);

        checkBox_histogram = new QCheckBox(page_histogram);
        checkBox_histogram->setObjectName("checkBox_histogram");

        horizontalLayout_2->addWidget(checkBox_histogram);


        verticalLayout_2->addLayout(horizontalLayout_2);

        histogram = new QChartView(page_histogram);
        histogram->setObjectName("histogram");

        verticalLayout_2->addWidget(histogram);

        stackedWidget->addWidget(page_histogram);
        page_scatter_plot = new QWidget();
        page_scatter_plot->setObjectName("page_scatter_plot");
        verticalLayout_3 = new QVBoxLayout(page_scatter_plot);
        verticalLayout_3->setObjectName("verticalLayout_3");
        label_10 = new QLabel(page_scatter_plot);
        label_10->setObjectName("label_10");
        label_10->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_10);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label_9 = new QLabel(page_scatter_plot);
        label_9->setObjectName("label_9");
        sizePolicy3.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy3);

        horizontalLayout_3->addWidget(label_9);

        comboBox_scatter_x = new QComboBox(page_scatter_plot);
        comboBox_scatter_x->setObjectName("comboBox_scatter_x");

        horizontalLayout_3->addWidget(comboBox_scatter_x);

        label_12 = new QLabel(page_scatter_plot);
        label_12->setObjectName("label_12");
        sizePolicy3.setHeightForWidth(label_12->sizePolicy().hasHeightForWidth());
        label_12->setSizePolicy(sizePolicy3);

        horizontalLayout_3->addWidget(label_12);

        comboBox_scatter_y = new QComboBox(page_scatter_plot);
        comboBox_scatter_y->setObjectName("comboBox_scatter_y");

        horizontalLayout_3->addWidget(comboBox_scatter_y);

        label_15 = new QLabel(page_scatter_plot);
        label_15->setObjectName("label_15");
        sizePolicy3.setHeightForWidth(label_15->sizePolicy().hasHeightForWidth());
        label_15->setSizePolicy(sizePolicy3);

        horizontalLayout_3->addWidget(label_15);

        spinBox_scatter_power = new QSpinBox(page_scatter_plot);
        spinBox_scatter_power->setObjectName("spinBox_scatter_power");
        spinBox_scatter_power->setMinimum(1);

        horizontalLayout_3->addWidget(spinBox_scatter_power);


        verticalLayout_3->addLayout(horizontalLayout_3);

        scatterPlot = new QChartView(page_scatter_plot);
        scatterPlot->setObjectName("scatterPlot");

        verticalLayout_3->addWidget(scatterPlot);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        label_13 = new QLabel(page_scatter_plot);
        label_13->setObjectName("label_13");
        sizePolicy3.setHeightForWidth(label_13->sizePolicy().hasHeightForWidth());
        label_13->setSizePolicy(sizePolicy3);

        horizontalLayout_4->addWidget(label_13);

        label_scatter_p = new QLabel(page_scatter_plot);
        label_scatter_p->setObjectName("label_scatter_p");

        horizontalLayout_4->addWidget(label_scatter_p);

        label_14 = new QLabel(page_scatter_plot);
        label_14->setObjectName("label_14");
        sizePolicy3.setHeightForWidth(label_14->sizePolicy().hasHeightForWidth());
        label_14->setSizePolicy(sizePolicy3);

        horizontalLayout_4->addWidget(label_14);

        label_scatter_r2 = new QLabel(page_scatter_plot);
        label_scatter_r2->setObjectName("label_scatter_r2");

        horizontalLayout_4->addWidget(label_scatter_r2);

        label_16 = new QLabel(page_scatter_plot);
        label_16->setObjectName("label_16");

        horizontalLayout_4->addWidget(label_16);

        label_scatter_coordinate = new QLabel(page_scatter_plot);
        label_scatter_coordinate->setObjectName("label_scatter_coordinate");

        horizontalLayout_4->addWidget(label_scatter_coordinate);


        verticalLayout_3->addLayout(horizontalLayout_4);

        stackedWidget->addWidget(page_scatter_plot);
        page_correlation_matrix = new QWidget();
        page_correlation_matrix->setObjectName("page_correlation_matrix");
        verticalLayout_4 = new QVBoxLayout(page_correlation_matrix);
        verticalLayout_4->setObjectName("verticalLayout_4");
        label_3 = new QLabel(page_correlation_matrix);
        label_3->setObjectName("label_3");
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(label_3);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        label_17 = new QLabel(page_correlation_matrix);
        label_17->setObjectName("label_17");
        sizePolicy3.setHeightForWidth(label_17->sizePolicy().hasHeightForWidth());
        label_17->setSizePolicy(sizePolicy3);

        horizontalLayout_5->addWidget(label_17);

        comboBox_correlation_type = new QComboBox(page_correlation_matrix);
        comboBox_correlation_type->addItem(QString());
        comboBox_correlation_type->addItem(QString());
        comboBox_correlation_type->setObjectName("comboBox_correlation_type");

        horizontalLayout_5->addWidget(comboBox_correlation_type);

        label_18 = new QLabel(page_correlation_matrix);
        label_18->setObjectName("label_18");
        sizePolicy3.setHeightForWidth(label_18->sizePolicy().hasHeightForWidth());
        label_18->setSizePolicy(sizePolicy3);

        horizontalLayout_5->addWidget(label_18);

        comboBox_correlation_columns = new MultiSelectComboBox(page_correlation_matrix);
        comboBox_correlation_columns->setObjectName("comboBox_correlation_columns");

        horizontalLayout_5->addWidget(comboBox_correlation_columns);


        verticalLayout_4->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        correlation_matrix = new HeatmapWidget(page_correlation_matrix);
        correlation_matrix->setObjectName("correlation_matrix");
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(correlation_matrix->sizePolicy().hasHeightForWidth());
        correlation_matrix->setSizePolicy(sizePolicy4);

        horizontalLayout_6->addWidget(correlation_matrix);

        correlation_legend = new HeatmapLegend(page_correlation_matrix);
        correlation_legend->setObjectName("correlation_legend");
        QSizePolicy sizePolicy5(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(correlation_legend->sizePolicy().hasHeightForWidth());
        correlation_legend->setSizePolicy(sizePolicy5);

        horizontalLayout_6->addWidget(correlation_legend);


        verticalLayout_4->addLayout(horizontalLayout_6);

        stackedWidget->addWidget(page_correlation_matrix);
        page_pca_analysis = new QWidget();
        page_pca_analysis->setObjectName("page_pca_analysis");
        verticalLayout_5 = new QVBoxLayout(page_pca_analysis);
        verticalLayout_5->setObjectName("verticalLayout_5");
        label_4 = new QLabel(page_pca_analysis);
        label_4->setObjectName("label_4");
        label_4->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(label_4);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        label_19 = new QLabel(page_pca_analysis);
        label_19->setObjectName("label_19");
        sizePolicy3.setHeightForWidth(label_19->sizePolicy().hasHeightForWidth());
        label_19->setSizePolicy(sizePolicy3);

        horizontalLayout_7->addWidget(label_19);

        comboBox_pca_columns = new MultiSelectComboBox(page_pca_analysis);
        comboBox_pca_columns->setObjectName("comboBox_pca_columns");

        horizontalLayout_7->addWidget(comboBox_pca_columns);

        label_20 = new QLabel(page_pca_analysis);
        label_20->setObjectName("label_20");
        sizePolicy3.setHeightForWidth(label_20->sizePolicy().hasHeightForWidth());
        label_20->setSizePolicy(sizePolicy3);

        horizontalLayout_7->addWidget(label_20);

        comboBox_pca_dims = new QComboBox(page_pca_analysis);
        comboBox_pca_dims->addItem(QString());
        comboBox_pca_dims->addItem(QString());
        comboBox_pca_dims->setObjectName("comboBox_pca_dims");

        horizontalLayout_7->addWidget(comboBox_pca_dims);


        verticalLayout_5->addLayout(horizontalLayout_7);

        pca_chart = new QChartView(page_pca_analysis);
        pca_chart->setObjectName("pca_chart");
        pca_chart->setEnabled(true);

        verticalLayout_5->addWidget(pca_chart);

        pca_3d_layout = new QVBoxLayout();
        pca_3d_layout->setObjectName("pca_3d_layout");

        verticalLayout_5->addLayout(pca_3d_layout);

        stackedWidget->addWidget(page_pca_analysis);
        page_clustering = new QWidget();
        page_clustering->setObjectName("page_clustering");
        verticalLayout_6 = new QVBoxLayout(page_clustering);
        verticalLayout_6->setObjectName("verticalLayout_6");
        label_5 = new QLabel(page_clustering);
        label_5->setObjectName("label_5");
        label_5->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(label_5);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        label_24 = new QLabel(page_clustering);
        label_24->setObjectName("label_24");
        sizePolicy3.setHeightForWidth(label_24->sizePolicy().hasHeightForWidth());
        label_24->setSizePolicy(sizePolicy3);

        horizontalLayout_8->addWidget(label_24);

        comboBox_clustering_columns = new MultiSelectComboBox(page_clustering);
        comboBox_clustering_columns->setObjectName("comboBox_clustering_columns");
        sizePolicy.setHeightForWidth(comboBox_clustering_columns->sizePolicy().hasHeightForWidth());
        comboBox_clustering_columns->setSizePolicy(sizePolicy);

        horizontalLayout_8->addWidget(comboBox_clustering_columns);

        label_21 = new QLabel(page_clustering);
        label_21->setObjectName("label_21");
        sizePolicy3.setHeightForWidth(label_21->sizePolicy().hasHeightForWidth());
        label_21->setSizePolicy(sizePolicy3);

        horizontalLayout_8->addWidget(label_21);

        comboBox_clustering_algorithm = new QComboBox(page_clustering);
        comboBox_clustering_algorithm->addItem(QString());
        comboBox_clustering_algorithm->addItem(QString());
        comboBox_clustering_algorithm->addItem(QString());
        comboBox_clustering_algorithm->addItem(QString());
        comboBox_clustering_algorithm->setObjectName("comboBox_clustering_algorithm");
        sizePolicy1.setHeightForWidth(comboBox_clustering_algorithm->sizePolicy().hasHeightForWidth());
        comboBox_clustering_algorithm->setSizePolicy(sizePolicy1);

        horizontalLayout_8->addWidget(comboBox_clustering_algorithm);

        label_22 = new QLabel(page_clustering);
        label_22->setObjectName("label_22");
        sizePolicy3.setHeightForWidth(label_22->sizePolicy().hasHeightForWidth());
        label_22->setSizePolicy(sizePolicy3);

        horizontalLayout_8->addWidget(label_22);

        spinBox_num_clusters = new QSpinBox(page_clustering);
        spinBox_num_clusters->setObjectName("spinBox_num_clusters");
        spinBox_num_clusters->setMinimum(2);
        spinBox_num_clusters->setMaximum(20);

        horizontalLayout_8->addWidget(spinBox_num_clusters);

        label_23 = new QLabel(page_clustering);
        label_23->setObjectName("label_23");
        sizePolicy3.setHeightForWidth(label_23->sizePolicy().hasHeightForWidth());
        label_23->setSizePolicy(sizePolicy3);

        horizontalLayout_8->addWidget(label_23);

        comboBox_clustering_dim = new QComboBox(page_clustering);
        comboBox_clustering_dim->addItem(QString());
        comboBox_clustering_dim->addItem(QString());
        comboBox_clustering_dim->setObjectName("comboBox_clustering_dim");
        QSizePolicy sizePolicy6(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(comboBox_clustering_dim->sizePolicy().hasHeightForWidth());
        comboBox_clustering_dim->setSizePolicy(sizePolicy6);

        horizontalLayout_8->addWidget(comboBox_clustering_dim);


        verticalLayout_6->addLayout(horizontalLayout_8);

        cluster2d = new QChartView(page_clustering);
        cluster2d->setObjectName("cluster2d");

        verticalLayout_6->addWidget(cluster2d);

        cluster3d = new QVBoxLayout();
        cluster3d->setObjectName("cluster3d");

        verticalLayout_6->addLayout(cluster3d);

        cluster_legends = new LegendWidget(page_clustering);
        cluster_legends->setObjectName("cluster_legends");

        verticalLayout_6->addWidget(cluster_legends);

        stackedWidget->addWidget(page_clustering);
        page_causal_analysis = new QWidget();
        page_causal_analysis->setObjectName("page_causal_analysis");
        verticalLayout_7 = new QVBoxLayout(page_causal_analysis);
        verticalLayout_7->setObjectName("verticalLayout_7");
        label_6 = new QLabel(page_causal_analysis);
        label_6->setObjectName("label_6");
        label_6->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(label_6);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName("horizontalLayout_9");
        label_25 = new QLabel(page_causal_analysis);
        label_25->setObjectName("label_25");
        sizePolicy3.setHeightForWidth(label_25->sizePolicy().hasHeightForWidth());
        label_25->setSizePolicy(sizePolicy3);

        horizontalLayout_9->addWidget(label_25);

        comboBox_causal_traintest = new QComboBox(page_causal_analysis);
        comboBox_causal_traintest->addItem(QString());
        comboBox_causal_traintest->addItem(QString());
        comboBox_causal_traintest->addItem(QString());
        comboBox_causal_traintest->addItem(QString());
        comboBox_causal_traintest->setObjectName("comboBox_causal_traintest");

        horizontalLayout_9->addWidget(comboBox_causal_traintest);

        label_26 = new QLabel(page_causal_analysis);
        label_26->setObjectName("label_26");
        sizePolicy3.setHeightForWidth(label_26->sizePolicy().hasHeightForWidth());
        label_26->setSizePolicy(sizePolicy3);

        horizontalLayout_9->addWidget(label_26);

        spinBox_causal_epoch = new QSpinBox(page_causal_analysis);
        spinBox_causal_epoch->setObjectName("spinBox_causal_epoch");
        spinBox_causal_epoch->setValue(50);

        horizontalLayout_9->addWidget(spinBox_causal_epoch);

        label_27 = new QLabel(page_causal_analysis);
        label_27->setObjectName("label_27");
        sizePolicy3.setHeightForWidth(label_27->sizePolicy().hasHeightForWidth());
        label_27->setSizePolicy(sizePolicy3);

        horizontalLayout_9->addWidget(label_27);

        spinBox_causal_depth = new QSpinBox(page_causal_analysis);
        spinBox_causal_depth->setObjectName("spinBox_causal_depth");

        horizontalLayout_9->addWidget(spinBox_causal_depth);


        verticalLayout_7->addLayout(horizontalLayout_9);

        label_28 = new QLabel(page_causal_analysis);
        label_28->setObjectName("label_28");
        sizePolicy.setHeightForWidth(label_28->sizePolicy().hasHeightForWidth());
        label_28->setSizePolicy(sizePolicy);

        verticalLayout_7->addWidget(label_28);

        progressBar_causal = new QProgressBar(page_causal_analysis);
        progressBar_causal->setObjectName("progressBar_causal");
        progressBar_causal->setValue(24);

        verticalLayout_7->addWidget(progressBar_causal);

        causal_result = new QChartView(page_causal_analysis);
        causal_result->setObjectName("causal_result");

        verticalLayout_7->addWidget(causal_result);

        stackedWidget->addWidget(page_causal_analysis);

        horizontalLayout->addWidget(stackedWidget);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 842, 38));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionLoad);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(2);
        comboBox_clustering_algorithm->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionLoad->setText(QCoreApplication::translate("MainWindow", "Load", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Menu", nullptr));
        label_menu_raw_data_view->setText(QCoreApplication::translate("MainWindow", "Raw Data View", nullptr));
        label_menu_histogram->setText(QCoreApplication::translate("MainWindow", "Histogram", nullptr));
        label_menu_scatter_plot->setText(QCoreApplication::translate("MainWindow", "Scatter Plot", nullptr));
        label_menu_correlation_matrix->setText(QCoreApplication::translate("MainWindow", "Correlation Matrix", nullptr));
        label_menu_pca_analysis->setText(QCoreApplication::translate("MainWindow", "PCA Analysis", nullptr));
        label_menu_clustering->setText(QCoreApplication::translate("MainWindow", "Clustering", nullptr));
        label_menu_causal_analysis->setText(QCoreApplication::translate("MainWindow", "Causal Analysis", nullptr));
        pushButton_load_csv_data->setText(QCoreApplication::translate("MainWindow", "Load CSV Data", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "No Data Loaded", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Raw Data View", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "Histogram", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Column", nullptr));
        checkBox_histogram->setText(QCoreApplication::translate("MainWindow", "Show Normal Distribution Curve", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Scatter Plot", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "x-axis", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "y-axis", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "Power:", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "p: ", nullptr));
        label_scatter_p->setText(QString());
        label_14->setText(QCoreApplication::translate("MainWindow", "r^2:", nullptr));
        label_scatter_r2->setText(QString());
        label_16->setText(QCoreApplication::translate("MainWindow", "Point:", nullptr));
        label_scatter_coordinate->setText(QString());
        label_3->setText(QCoreApplication::translate("MainWindow", "Correlation Matrix", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "Type", nullptr));
        comboBox_correlation_type->setItemText(0, QCoreApplication::translate("MainWindow", "Covariance", nullptr));
        comboBox_correlation_type->setItemText(1, QCoreApplication::translate("MainWindow", "Pearson Correlation Coefficient", nullptr));

        label_18->setText(QCoreApplication::translate("MainWindow", "Columns", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "PCA Analysis", nullptr));
        label_19->setText(QCoreApplication::translate("MainWindow", "Columns", nullptr));
        label_20->setText(QCoreApplication::translate("MainWindow", "Dims", nullptr));
        comboBox_pca_dims->setItemText(0, QCoreApplication::translate("MainWindow", "2", nullptr));
        comboBox_pca_dims->setItemText(1, QCoreApplication::translate("MainWindow", "3", nullptr));

        label_5->setText(QCoreApplication::translate("MainWindow", "Clustering", nullptr));
        label_24->setText(QCoreApplication::translate("MainWindow", "Cols", nullptr));
        label_21->setText(QCoreApplication::translate("MainWindow", "Algo", nullptr));
        comboBox_clustering_algorithm->setItemText(0, QCoreApplication::translate("MainWindow", "K-Means", nullptr));
        comboBox_clustering_algorithm->setItemText(1, QCoreApplication::translate("MainWindow", "DBSCAN", nullptr));
        comboBox_clustering_algorithm->setItemText(2, QCoreApplication::translate("MainWindow", "Spectral", nullptr));
        comboBox_clustering_algorithm->setItemText(3, QCoreApplication::translate("MainWindow", "Mean-Shift", nullptr));

        label_22->setText(QCoreApplication::translate("MainWindow", "num clusters", nullptr));
        label_23->setText(QCoreApplication::translate("MainWindow", " dim", nullptr));
        comboBox_clustering_dim->setItemText(0, QCoreApplication::translate("MainWindow", "2", nullptr));
        comboBox_clustering_dim->setItemText(1, QCoreApplication::translate("MainWindow", "3", nullptr));

        label_6->setText(QCoreApplication::translate("MainWindow", "Causal Analysis", nullptr));
        label_25->setText(QCoreApplication::translate("MainWindow", "Train/Test Split", nullptr));
        comboBox_causal_traintest->setItemText(0, QCoreApplication::translate("MainWindow", "90%/10%", nullptr));
        comboBox_causal_traintest->setItemText(1, QCoreApplication::translate("MainWindow", "80%/20%", nullptr));
        comboBox_causal_traintest->setItemText(2, QCoreApplication::translate("MainWindow", "70%/30%", nullptr));
        comboBox_causal_traintest->setItemText(3, QCoreApplication::translate("MainWindow", "50%/50%", nullptr));

        label_26->setText(QCoreApplication::translate("MainWindow", "Epochs", nullptr));
        label_27->setText(QCoreApplication::translate("MainWindow", "Depth", nullptr));
        label_28->setText(QCoreApplication::translate("MainWindow", "Train Progress", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MAINWINDOWCLDLND_H

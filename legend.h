#pragma once
#include <QWidget>
#include <QChart>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QScatter3DSeries>
#include <QScatterSeries>

class LegendWidget : public QWidget {
    Q_OBJECT

public:
    LegendWidget(QWidget *parent = nullptr);

    // Add functions to manage legend items (e.g., add, remove)
    void assignChart(QObject* chart);
    void addSeries(QScatterSeries *series, const QString &label);
    void addSeries(QScatter3DSeries *series, const QString &label);
    void removeSeries(QAbstractSeries *series);
    void removeSeries(QScatter3DSeries *series);
    void clear();

private slots:
    void toggleSeriesVisibility();

private:
    QHBoxLayout *layout;
    QMap<QObject *, QCheckBox *> legendItems;

    // Store a reference to the chart for updating series visibility
    QObject *associatedChart;
    bool is2d = false;
};
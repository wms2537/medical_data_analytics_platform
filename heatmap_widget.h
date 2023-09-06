#pragma once

#include <QWidget>
#include <vector>
#include <QString>
#include <QColor>

class HeatmapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HeatmapWidget(QWidget *parent = nullptr);

    void setData(const std::vector<std::vector<float>>& data);
    void setAxisLabels(const std::vector<QString>& xLabels, const std::vector<QString>& yLabels);
    void setLegendColors(const QColor& lowColor, const QColor& highColor);
    void setMinMax(int min, int max);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int min = 0, max = 1;
    std::vector<std::vector<float>> correlationData;
    std::vector<QString> xAxisLabels;
    std::vector<QString> yAxisLabels;
    QColor legendLowColor;
    QColor legendHighColor;
};
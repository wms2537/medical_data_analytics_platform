#pragma once

#include <QWidget>
#include <QColor>

class HeatmapLegend : public QWidget
{
    Q_OBJECT

public:
    explicit HeatmapLegend(QWidget *parent = nullptr);

    void setColors(const QColor& lowColor, const QColor& highColor);
    void setMinMax(int min, int max);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QColor legendLowColor;
    QColor legendHighColor;
    int min = 0, max = 1;
};

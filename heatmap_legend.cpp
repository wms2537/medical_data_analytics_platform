#include "heatmap_legend.h"
#include <QPainter>

HeatmapLegend::HeatmapLegend(QWidget *parent)
    : QWidget(parent), legendLowColor(255, 255, 255), legendHighColor(255, 0, 0)
{
    setFixedWidth(30);
    // setFixedHeight(50); // Adjust the height of the legend as needed
}

void HeatmapLegend::setColors(const QColor &lowColor, const QColor &highColor)
{
    legendLowColor = lowColor;
    legendHighColor = highColor;
    update(); // Trigger a repaint.
}

void HeatmapLegend::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    // Calculate gradient for the legend.
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0.0, legendLowColor);
    gradient.setColorAt(1.0, legendHighColor);

    painter.fillRect(rect(), gradient);

    // Draw labels
    painter.setPen(Qt::black); // Set text color

    // Label positions (adjust as needed)
    QRectF labelRect1(1, height()-10, 28, 10);
    QRectF labelRect2(1, 1, 28, 10);

    QString label1 = QString::number(max);
    QString label2 = QString::number(min);

    // Draw labels
    int maxFontSize = 12; // Maximum allowed font size
    int minFontSize = 4;  // Minimum allowed font size
    int fontSize = maxFontSize;

    while (fontSize >= minFontSize)
    {
        QFont labelFont(painter.font());
        labelFont.setPointSize(fontSize);

        QFontMetrics labelFM(labelFont);

        if (labelFM.boundingRect(label1).width() <= 28)
        { // Adjust for cell padding
            painter.setFont(labelFont);
            painter.drawText(labelRect1, Qt::AlignCenter, label1);
            break;
        }

        --fontSize;
    }

    while (fontSize >= minFontSize)
    {
        QFont labelFont(painter.font());
        labelFont.setPointSize(fontSize);

        QFontMetrics labelFM(labelFont);

        if (labelFM.boundingRect(label2).width() <= 28)
        { // Adjust for cell padding
            painter.setFont(labelFont);
            painter.drawText(labelRect2, Qt::AlignCenter, label2);
            break;
        }

        --fontSize;
    }
}

void HeatmapLegend::setMinMax(int a, int b)
{
    min = a;
    max = b;
    update();
}
#include "heatmap_widget.h"
#include "heatmap_legend.h"
#include <QPainter>
#include <QFontMetrics>
#include <QHBoxLayout>

HeatmapWidget::HeatmapWidget(QWidget *parent)
    : QWidget(parent), legendLowColor(255, 255, 255), legendHighColor(255, 0, 0)
{
    // Initialize the correlation data (a sample 3x3 matrix).
    correlationData = {{1.0, 0.8, 0.6},
                       {0.8, 1.0, 0.4},
                       {0.6, 0.4, 1.0}};
}

void HeatmapWidget::setData(const std::vector<std::vector<float>>& data)
{
    // Set the correlation data to the provided data.
    correlationData = data;
    update(); // Trigger a repaint.
}

void HeatmapWidget::setAxisLabels(const std::vector<QString>& xLabels, const std::vector<QString>& yLabels)
{
    xAxisLabels = xLabels;
    yAxisLabels = yLabels;
    update(); // Trigger a repaint.
}

void HeatmapWidget::setLegendColors(const QColor& lowColor, const QColor& highColor)
{
    legendLowColor = lowColor;
    legendHighColor = highColor;
    update(); // Trigger a repaint.
}

void HeatmapWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    int numRows = correlationData.size();
    int numCols = (numRows > 0) ? correlationData[0].size() : 0;

    if (numRows == 0 || numCols == 0) {
        return;
    }

    // Decrease the font size for cell value labels
    QFont smallerFont = painter.font();
    smallerFont.setPointSize(8); // Set the desired font size

    painter.setFont(smallerFont);

    // Calculate cell width and height based on widget size.
    double cellWidth = static_cast<double>(width() - 30) / numCols; // Adjust for labels
    double cellHeight = static_cast<double>(height() - 30) / numRows; // Adjust for labels

    QFontMetrics fm(painter.font());
    int labelHeight = fm.height();
    int labelWidth = 0;

    for (int row = 0; row < numRows; ++row) {
        QString label = yAxisLabels.size() > row ? yAxisLabels[row] : QString();
        QRectF textRect(0, row * cellHeight + 30, cellHeight, 30);

        textRect.translate(-45, 30);

        // Rotate the painter for each label
        painter.save();
        painter.translate(textRect.center());
        painter.rotate(-90);
        painter.translate(-textRect.center());

        painter.drawText(textRect, Qt::AlignCenter, label);
        
        painter.restore();

        labelWidth = qMax(labelWidth, fm.boundingRect(label).width());  // Update the maximum label width
    }

    // Draw X-axis labels
    for (int col = 0; col < numCols; ++col) {
        QString label = xAxisLabels.size() > col ? xAxisLabels[col] : QString();
        QRectF textRect(col * cellWidth + 30 , 0, cellWidth, 30);
        painter.drawText(textRect, Qt::AlignCenter, label, nullptr);
    }

    // Draw the heatmap cells
    painter.setPen(Qt::black);
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            double value = correlationData[row][col];

            // Map the correlation value to a color gradient.
            double normalizedValue = (value-min) / (max-min);
            int red = static_cast<int>((1.0 - normalizedValue) * legendLowColor.red() + normalizedValue * legendHighColor.red());
            int green = static_cast<int>((1.0 - normalizedValue) * legendLowColor.green() + normalizedValue * legendHighColor.green());
            int blue = static_cast<int>((1.0 - normalizedValue) * legendLowColor.blue() + normalizedValue * legendHighColor.blue());

            QColor cellColor(red, green, blue);
            // Draw the heatmap cell
            QRectF cellRect(col * cellWidth + 30, row * cellHeight + 30, cellWidth, cellHeight);
            painter.fillRect(cellRect, cellColor);

            // Draw the cell value in the center
            QString cellValue = QString::number(value, 'f', 4); // Format the value
            QRectF textRect(cellRect);
            painter.drawText(textRect, Qt::AlignCenter, cellValue);
        }
    }
}

void HeatmapWidget::setMinMax(int a, int b) {
    min = a;
    max = b;
}
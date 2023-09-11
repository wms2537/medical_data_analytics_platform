#include "legend.h"


LegendWidget::LegendWidget(QWidget *parent)
    : QWidget(parent) {
    layout = new QHBoxLayout(this);
    setLayout(layout);
}

void LegendWidget::assignChart(QObject* chart) {
    associatedChart = chart;
}

void LegendWidget::addSeries(QScatterSeries *series, const QString &label) {
    is2d = true;
    QCheckBox *checkBox = new QCheckBox(label, this);
    QColor color = series->color();
    QString backgroundColor = QString("rgb(%1, %2, %3)").arg(color.red()).arg(color.green()).arg(color.blue());
    checkBox->setStyleSheet(QString("QCheckBox { background-color: %1; }").arg(backgroundColor));
    checkBox->setChecked(true); // Initially, show the series

    // Connect the checkbox signal to the slot for toggling visibility
    connect(checkBox, &QCheckBox::stateChanged, this, &LegendWidget::toggleSeriesVisibility);

    layout->addWidget(checkBox);
    legendItems[series] = checkBox;
}

void LegendWidget::addSeries(QScatter3DSeries *series, const QString &label) {
    is2d = false;
    QCheckBox *checkBox = new QCheckBox(label, this);
    QColor color = series->baseColor();
    QString backgroundColor = QString("rgb(%1, %2, %3)").arg(color.red()).arg(color.green()).arg(color.blue());
    checkBox->setStyleSheet(QString("QCheckBox { background-color: %1; }").arg(backgroundColor));
    checkBox->setChecked(true); // Initially, show the series

    // Connect the checkbox signal to the slot for toggling visibility
    connect(checkBox, &QCheckBox::stateChanged, this, &LegendWidget::toggleSeriesVisibility);

    layout->addWidget(checkBox);
    legendItems[series] = checkBox;
}

void LegendWidget::removeSeries(QAbstractSeries *series) {
    if (legendItems.contains(series)) {
        QCheckBox *checkBox = legendItems.take(series);
        layout->removeWidget(checkBox);
        checkBox->deleteLater();
    }
}

void LegendWidget::removeSeries(QScatter3DSeries *series) {
    if (legendItems.contains(series)) {
        QCheckBox *checkBox = legendItems.take(series);
        layout->removeWidget(checkBox);
        checkBox->deleteLater();
    }
}

void LegendWidget::clear() {
    for (auto series : legendItems.keys())
    {
        QCheckBox *checkBox = legendItems.take(series);
        layout->removeWidget(checkBox);
        checkBox->deleteLater();
    }
    associatedChart = nullptr;
}

void LegendWidget::toggleSeriesVisibility() {
    QCheckBox *senderCheckBox = qobject_cast<QCheckBox *>(sender());
    if (!senderCheckBox)
        return;

    // Find the series associated with the checkbox
    for (auto obj : legendItems.keys()) {
        if (legendItems[obj] == senderCheckBox) {
            QObject *series = qobject_cast<QObject *>(obj);
            if (series) {
                series->setProperty("visible", senderCheckBox->isChecked());
                // QMetaObject::invokeMethod(associatedChart, "updateSeriesVisibility");
            }
            return;
        }
    }
}
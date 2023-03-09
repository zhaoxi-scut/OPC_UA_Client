#ifndef EXPOSURE_SPIN_SLIDER_H
#define EXPOSURE_SPIN_SLIDER_H

#include <QWidget>

namespace Ui {
class ExposureSpinSlider;
}

class ExposureSpinSlider : public QWidget
{
    Q_OBJECT

public:
    explicit ExposureSpinSlider(QWidget *parent = nullptr);
    ~ExposureSpinSlider();

    void setExposure(uint16_t val);

signals:
    void valueChanged(uint16_t);

private:
    Ui::ExposureSpinSlider *ui;
};

#endif // EXPOSURE_SPIN_SLIDER_H

#ifndef GAIN_SPIN_SLIDER_H
#define GAIN_SPIN_SLIDER_H

#include <QWidget>

namespace Ui {
class GainSpinSlider;
}

class GainSpinSlider : public QWidget
{
    Q_OBJECT

public:
    explicit GainSpinSlider(QWidget *parent = nullptr);
    ~GainSpinSlider();

    void setGain(double val);

signals:
    void valueChanged(double);

private:
    Ui::GainSpinSlider *ui;
};

#endif // GAIN_SPIN_SLIDER_H

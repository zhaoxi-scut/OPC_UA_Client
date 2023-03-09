#ifndef DELAY_SPIN_SLIDER_H
#define DELAY_SPIN_SLIDER_H

#include <QWidget>

namespace Ui {
class DelaySpinSlider;
}

class DelaySpinSlider : public QWidget
{
    Q_OBJECT

public:
    explicit DelaySpinSlider(QWidget *parent = nullptr);
    ~DelaySpinSlider();

    void setValue(uint16_t val);

signals:
    void valueChanged(uint16_t val);

private:
    Ui::DelaySpinSlider *ui;
};

#endif // DELAY_SPIN_SLIDER_H

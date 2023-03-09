#ifndef LIGHT_SPIN_SLIDER_H
#define LIGHT_SPIN_SLIDER_H

#include <QWidget>

namespace Ui {
class LightSpinSlider;
}

class LightSpinSlider : public QWidget
{
    Q_OBJECT

public:
    explicit LightSpinSlider(QWidget *parent = nullptr);
    ~LightSpinSlider();

    void setValue(uint8_t val);

signals:
    void valueChanged(uint8_t val);

private:
    Ui::LightSpinSlider *ui;
};

#endif // LIGHT_SPIN_SLIDER_H

#include "delay_spin_slider.h"
#include "ui_delay_spin_slider.h"

DelaySpinSlider::DelaySpinSlider(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DelaySpinSlider)
{
    ui->setupUi(this);

    connect(ui->slider, &QSlider::valueChanged, this,
            [this](int val)
    {
        ui->spin->setValue(val);
        emit valueChanged(static_cast<uint16_t>(val));
    });
    connect(ui->spin, &QSpinBox::valueChanged, this,
            [this](int val)
    {
        ui->slider->setValue(val);
        emit valueChanged(static_cast<uint16_t>(val));
    });
}

DelaySpinSlider::~DelaySpinSlider()
{
    delete ui;
}

void DelaySpinSlider::setValue(uint16_t val)
{
    val = val > 3000 ? 3000 : (val < 1 ? 1 : val);
    ui->slider->setValue(static_cast<int>(val));
    ui->spin->setValue(static_cast<int>(val));
}

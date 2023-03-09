#include "light_spin_slider.h"
#include "ui_light_spin_slider.h"

LightSpinSlider::LightSpinSlider(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LightSpinSlider)
{
    ui->setupUi(this);

    connect(ui->slider, &QSlider::valueChanged, this,
            [this](int val)
    {
        ui->spin->setValue(val);
        emit valueChanged(static_cast<uint8_t>(val));
    });
    connect(ui->spin, &QSpinBox::valueChanged, this,
            [this](int val)
    {
        ui->slider->setValue(val);
        emit valueChanged(static_cast<uint8_t>(val));
    });
}

LightSpinSlider::~LightSpinSlider()
{
    delete ui;
}

void LightSpinSlider::setValue(uint8_t val)
{
    ui->slider->setValue(static_cast<int>(val));
    ui->spin->setValue(static_cast<int>(val));
}

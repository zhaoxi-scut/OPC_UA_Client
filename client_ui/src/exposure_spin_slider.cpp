#include "exposure_spin_slider.h"
#include "ui_exposure_spin_slider.h"

ExposureSpinSlider::ExposureSpinSlider(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExposureSpinSlider)
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

ExposureSpinSlider::~ExposureSpinSlider()
{
    delete ui;
}

void ExposureSpinSlider::setExposure(uint16_t val)
{
    val = val > 10000 ? 10000 : val;
    ui->slider->setValue(static_cast<int>(val));
    ui->spin->setValue(static_cast<int>(val));
}

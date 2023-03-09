#include "gain_spin_slider.h"
#include "ui_gain_spin_slider.h"

GainSpinSlider::GainSpinSlider(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GainSpinSlider)
{
    ui->setupUi(this);

    connect(ui->slider, &QSlider::valueChanged, this,
            [this](int val)
    {
        double e_val = static_cast<double>(val) / 100.0;
        ui->spin->setValue(e_val);
        emit valueChanged(e_val);
    });
    connect(ui->spin, &QDoubleSpinBox::valueChanged, this,
            [this](double val)
    {
        ui->slider->setValue(static_cast<int>(val * 100));
        emit valueChanged(val);
    });
}

void GainSpinSlider::setGain(double val)
{
    val = val > 3 ? 3 : (val < 0 ? 0 : val);
    ui->spin->setValue(val);
    ui->slider->setValue(static_cast<int>(val * 100));
}

GainSpinSlider::~GainSpinSlider()
{
    delete ui;
}

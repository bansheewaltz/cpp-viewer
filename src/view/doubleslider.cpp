#include "doubleslider.hpp"

DoubleSlider::DoubleSlider(QWidget *parent) : QSlider(parent) {
  connect(this, SIGNAL(valueChanged(int)), this, SLOT(notifyValueChanged(int)));
}

void DoubleSlider::setDoubleValue(double value) {
  double_value_ = value;
  int v = std::round(value * divisor_);
  this->setValue(v);
}

void DoubleSlider::notifyValueChanged(int value) {
  emit doubleValueChanged(double_value_);
}

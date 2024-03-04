#include "doubleslider.hpp"

DoubleSlider::DoubleSlider(QWidget *parent) : QSlider(parent) {
  connect(this, SIGNAL(valueChanged(int)), this, SLOT(notifyValueChanged(int)));
}

void DoubleSlider::setDoubleValue(double value) {
  int v = std::round(value * divisor);
  this->setValue(v);
}

void DoubleSlider::notifyValueChanged(int value) {
  double doubleValue = (float)value / this->divisor;
  emit doubleValueChanged(doubleValue);
}

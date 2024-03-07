#ifndef DOUBLESLIDER_HPP
#define DOUBLESLIDER_HPP

#include "QtCore/qtmetamacros.h"
#include "QtWidgets/qslider.h"

class DoubleSlider : public QSlider {
  Q_OBJECT

 public:
  DoubleSlider(QWidget *parent = nullptr);
  void setDivisor(int divisor) { divisor_ = divisor; }
  void setDoubleValue(double value);
  double double_value() { return double_value_; }

 signals:
  void doubleValueChanged(double value);

 public slots:
  void notifyValueChanged(int value);

 private:
  int divisor_;
  double double_value_;
};

#endif  // DOUBLESLIDER_HPP

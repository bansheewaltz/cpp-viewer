#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QPushButton>
#include <QSettings>
#include <QSpinbox>
#include <QTimer>

#include "doubleslider.hpp"
#include "gifimage/qgifimage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  QString file_name;
  QGifImage *gif;
  int gif_fps = 10;
  int gif_sec = 5;
  QSize gif_size = QSize(640, 480);
  QTimer gif_timer;
  int gif_frame_counter = 0;

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow *ui;

  void setupWidthControls(DoubleSlider *, QDoubleSpinBox *);
  void setupLocationControls(DoubleSlider *, QDoubleSpinBox *);
  void setupRotationControls(DoubleSlider *, QDoubleSpinBox *);
  void setupScaleControls(DoubleSlider *, QDoubleSpinBox *);
  void paintButton(QPushButton *b, const QColor &c);
  void resetSettings();
  void saveSettings(QString file_name);
  void loadSettings();
  void showFileStats();

 private slots:
  /* Display */
  void on_backgroundColorPicker_clicked();
  void on_lineColorPicker_clicked();
  void on_pointColorPicker_clicked();
  void on_displayLinesCB_toggled(bool checked);
  void on_displayPointsCB_toggled(bool checked);
  void on_pointSizeSlider_doubleValueChanged(double value);
  void on_pointStyleSquareCB_toggled(bool checked);
  void on_lineWidthSlider_doubleValueChanged(double value);
  void on_lineStyleDashedCB_toggled(bool checked);
  void on_orthographicProjButton_toggled(bool checked);
  void on_perspectiveProjButton_toggled(bool checked);
  /* Location */
  void on_xLocationSlider_doubleValueChanged(double value);
  void on_yLocationSlider_doubleValueChanged(double value);
  void on_zLocationSlider_doubleValueChanged(double value);
  void on_locationResetPushButton_clicked();
  /* Rotation */
  void on_xRotationSlider_doubleValueChanged(double angle);
  void on_yRotationSlider_doubleValueChanged(double angle);
  void on_zRotationSlider_doubleValueChanged(double angle);
  void on_rotationResetPushButton_clicked();
  /* Scale */
  void on_xScaleSlider_doubleValueChanged(double factor);
  void on_yScaleSlider_doubleValueChanged(double factor);
  void on_zScaleSlider_doubleValueChanged(double factor);
  void on_uScaleSlider_doubleValueChanged(double factor);
  void on_scaleResetPushButton_clicked();
  /* Actions */
  void on_openFilePushButton_released();
  void openFile();
  void on_gifCapturePushButton_released();
  void RecordGifFrame();
  void on_screenshotPushButton_released();
};

#endif  // MAINWINDOW_HPP

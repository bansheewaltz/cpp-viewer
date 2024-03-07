#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QPushButton>
#include <QSettings>
#include <QSpinbox>
#include <QTimer>

#include "doubleslider.hpp"
#include "facade.h"
#include "gifimage/qgifimage.h"
#include "params_dto.h"

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
  /* Display settings */
  void on_backgroundColorPicker_clicked();
  /* * Lines */
  void on_displayLinesCB_released();
  void on_lineColorPicker_clicked();
  void on_lineWidthSlider_doubleValueReleased();
  void on_lineStyleDashedCB_released();
  /* * Points */
  void on_displayPointsCB_released();
  void on_pointColorPicker_clicked();
  void on_pointSizeSlider_doubleValueReleased();
  void on_pointStyleSquareCB_released();

  /* Transformations */
  /* * Projection */
  void on_orthographicProjButton_released();
  void on_perspectiveProjButton_released();
  /* * Location */
  void on_xLocationSlider_doubleValueReleased();
  void on_yLocationSlider_doubleValueReleased();
  void on_zLocationSlider_doubleValueReleased();
  void on_locationResetPushButton_clicked();
  /* * Rotation */
  void on_xRotationSlider_doubleValueReleased();
  void on_yRotationSlider_doubleValueReleased();
  void on_zRotationSlider_doubleValueReleased();
  void on_rotationResetPushButton_clicked();
  /* * Scale */
  void on_xScaleSlider_doubleValueReleased();
  void on_yScaleSlider_doubleValueReleased();
  void on_zScaleSlider_doubleValueReleased();
  void on_uScaleSlider_doubleValueReleased();
  void on_scaleResetPushButton_clicked();

  /* Actions */
  void on_openFilePushButton_released();
  void openFile();
  void on_gifCapturePushButton_released();
  void RecordGifFrame();
  void on_screenshotPushButton_released();

 private:
  Facade facade_;
  RendererParamsDTO renderer_params_;
  SceneParamsDTO scene_params_;
};

#endif  // MAINWINDOW_HPP

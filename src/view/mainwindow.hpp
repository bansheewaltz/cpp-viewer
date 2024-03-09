#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QPushButton>
#include <QSettings>
#include <QSpinbox>
#include <QTimer>

#include "dtos.h"
#include "facade.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void ShowSceneStats();
  void UpdateRenderParams();
  void UpdateSceneTransforms();
  void ResetParameters();

 private slots:
  void OpenFile();
  // Display parameters
  void on_linesWidthSB_valueChanged(double t) { UpdateRenderParams(); }
  void on_pointsSizeSB_valueChanged(double t) { UpdateRenderParams(); }
  void on_linesChB_toggled(bool t) { UpdateRenderParams(); }
  void on_pointsChB_toggled(bool t) { UpdateRenderParams(); }
  void on_pointsCoB_currentIndexChanged(int t) { UpdateRenderParams(); }
  void on_linesCoB_currentIndexChanged(int t) { UpdateRenderParams(); }
  void on_resetPB_clicked() { ResetParameters(); }
  // Scene transforms
  void on_xTranslationSB_valueChanged(double t) { UpdateSceneTransforms(); }
  void on_yTranslationSB_valueChanged(double t) { UpdateSceneTransforms(); }
  void on_zTranslationSB_valueChanged(double t) { UpdateSceneTransforms(); }
  void on_xRotationSB_valueChanged(double t) { UpdateSceneTransforms(); }
  void on_yRotationSB_valueChanged(double t) { UpdateSceneTransforms(); }
  void on_zRotationSB_valueChanged(double t) { UpdateSceneTransforms(); }
  void on_xScaleSB_valueChanged(double t) { UpdateSceneTransforms(); }
  void on_yScaleSB_valueChanged(double t) { UpdateSceneTransforms(); }
  void on_zScaleSB_valueChanged(double t) { UpdateSceneTransforms(); }
  void on_uScaleSB_valueChanged(double t) { UpdateSceneTransforms(); }

 private:
  Ui::MainWindow *ui_;
  Facade facade_;
  RendererParamsDTO renderer_params_;
  SceneTransformsDTO scene_params_;
  QString filepath_;
};

#endif  // MAINWINDOW_HPP

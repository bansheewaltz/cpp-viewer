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

  void ShowFileStats();
  void OpenFile();

 private:
  Ui::MainWindow *ui;
  Facade facade_;
  RendererParamsDTO renderer_params_;
  SceneTransformsDTO scene_params_;
  QString filepath_;
};

#endif  // MAINWINDOW_HPP

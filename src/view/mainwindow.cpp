#include "mainwindow.hpp"

#include <QColor>
#include <QColorDialog>
#include <QFileDialog>
#include <QShortcut>

#include "./ui_mainwindow.h"
#include "file_reader/lowlevel_file_reader.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow) {
  ui_->setupUi(this);
  facade_.SetSceneRenderer(ui_->viewport);
  UpdateSceneTransforms();
  UpdateRenderParams();
  ui_->viewport->update();

  new QShortcut(QKeySequence("Ctrl+O"), this, SLOT(OpenFile()));

  facade_.SetFileReader(new LowlevelFileReader);
  facade_.CreateDefaultScene();
  ShowSceneStats();
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::ShowSceneStats() {
  auto scene_stats = facade_.scene_stats();
  QString stats =                                                     //
      filepath_.section("/", -1, -1).section(".", 0, 0) + " | " +     //
      "verts:" + QString::number(scene_stats.vertices_cnt) + " | " +  //
      "faces:" + QString::number(scene_stats.faces_cnt) + " | " +     //
      "edges:" + QString::number(scene_stats.edges_cnt);
  ui_->statisticsLabel->setText(stats);
}

void MainWindow::OpenFile() {
  QString dir = QDir::homePath() + "/Downloads/3Dmodels";
  auto filepath = QFileDialog::getOpenFileName(
      this, "Open 3d model", dir, "geometry definition file (*.obj)");
  if (filepath.isEmpty()) {
    return;
  }

  filepath_ = filepath;
  facade_.LoadScene(filepath_.toStdString());
  ShowSceneStats();
}

void MainWindow::UpdateRenderParams() {
  renderer_params_.line_width = ui_->linesWidthSB->value();
  renderer_params_.point_size = ui_->pointsSizeSB->value();
  renderer_params_.is_show_points = ui_->pointsChB->isChecked();
  renderer_params_.is_show_lines = ui_->linesChB->isChecked();
  if (ui_->pointsCoB->currentText() == "Circle")
    renderer_params_.point_style = RendererParamsDTO::PointStyle::CIRCLE;
  if (ui_->pointsCoB->currentText() == "Square")
    renderer_params_.point_style = RendererParamsDTO::PointStyle::SQUARE;
  if (ui_->linesCoB->currentText() == "Solid")
    renderer_params_.line_style = RendererParamsDTO::LineStyle::SOLID;
  if (ui_->linesCoB->currentText() == "Stipple")
    renderer_params_.line_style = RendererParamsDTO::LineStyle::DASHED;
  facade_.SetRendererParams(renderer_params_);
  ui_->viewport->update();
}

void MainWindow::UpdateSceneTransforms() {
  scene_params_.trnsx = ui_->xTranslationSB->value();
  scene_params_.trnsy = ui_->yTranslationSB->value();
  scene_params_.trnsz = ui_->zTranslationSB->value();
  scene_params_.rotx = ui_->xRotationSB->value();
  scene_params_.roty = ui_->yRotationSB->value();
  scene_params_.rotz = ui_->zRotationSB->value();
  scene_params_.scalex = ui_->xScaleSB->value();
  scene_params_.scaley = ui_->yScaleSB->value();
  scene_params_.scalez = ui_->zScaleSB->value();
  scene_params_.scaleu = ui_->uScaleSB->value();
  facade_.TransformScene(scene_params_);
  ui_->viewport->update();
}

void MainWindow::ResetParameters() {
  ui_->pointsCoB->setCurrentText("Circle");
  ui_->linesCoB->setCurrentText("Solid");
  ui_->linesWidthSB->setValue(1);
  ui_->pointsSizeSB->setValue(1);
  ui_->linesChB->setChecked(1);
  ui_->pointsChB->setChecked(1);
  ui_->xTranslationSB->setValue(0);
  ui_->yTranslationSB->setValue(0);
  ui_->zTranslationSB->setValue(0);
  ui_->xRotationSB->setValue(0);
  ui_->yRotationSB->setValue(0);
  ui_->zRotationSB->setValue(0);
  ui_->xScaleSB->setValue(1);
  ui_->yScaleSB->setValue(1);
  ui_->zScaleSB->setValue(1);
  ui_->uScaleSB->setValue(1);
  facade_.TransformScene(scene_params_);
  facade_.SetRendererParams(renderer_params_);
  ui_->viewport->update();
  ui_->centralwidget->setFocus();
}

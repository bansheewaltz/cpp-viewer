#include "mainwindow.hpp"

#include <QColor>
#include <QColorDialog>
#include <QFileDialog>
#include <QShortcut>

#include "./ui_mainwindow.h"
#include "file_reader/lowlevel_file_reader.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  new QShortcut(QKeySequence("Ctrl+O"), this, SLOT(OpenFile()));

  facade_.SetSceneRenderer(ui->viewport);
  facade_.SetFileReader(new LowlevelFileReader);
  facade_.CreateDefaultScene();
  // facade_.DrawScene();
  // ShowFileStats();
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::ShowSceneStats() {
  auto scene_stats = facade_.scene_stats();
  QString stats =                                                     //
      filepath_.section("/", -1, -1).section(".", 0, 0) + " | " +     //
      "verts:" + QString::number(scene_stats.vertices_cnt) + " | " +  //
      "faces:" + QString::number(scene_stats.faces_cnt) + " | " +     //
      "edges:" + QString::number(scene_stats.edges_cnt);
  ui->statisticsLabel->setText(stats);
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

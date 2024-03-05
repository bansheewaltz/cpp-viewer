#include "mainwindow.hpp"

#include <QColor>
#include <QColorDialog>
#include <QFileDialog>
#include <QShortcut>

#include "./ui_mainwindow.h"
#include "gifimage/qgifimage.h"

enum ControlSteps {
  LOCATION = 1000,
  ROTATION = 1800,
  SCALE = 1000,
  WIDTH = 200,
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  /* Set up primitives' size controls */
  setupWidthControls(ui->lineWidthSlider, ui->lineWidthSB);
  setupWidthControls(ui->pointSizeSlider, ui->pointSizeSB);
  /* Set up location controls */
  setupLocationControls(ui->xLocationSlider, ui->xLocationSB);
  setupLocationControls(ui->yLocationSlider, ui->yLocationSB);
  setupLocationControls(ui->zLocationSlider, ui->zLocationSB);
  /* Set up rotation controls */
  setupRotationControls(ui->xRotationSlider, ui->xRotationSB);
  setupRotationControls(ui->yRotationSlider, ui->yRotationSB);
  setupRotationControls(ui->zRotationSlider, ui->zRotationSB);
  /* Set up scale controls */
  setupScaleControls(ui->xScaleSlider, ui->xScaleSB);
  setupScaleControls(ui->yScaleSlider, ui->yScaleSB);
  setupScaleControls(ui->zScaleSlider, ui->zScaleSB);
  setupScaleControls(ui->uScaleSlider, ui->uScaleSB);
  /* Load settings */
  loadSettings();
  /* Paint the color picker buttons */
  paintButton(ui->backgroundColorPicker, ui->viewport->getBackgroundColor());
  paintButton(ui->lineColorPicker, ui->viewport->getLineColor());
  paintButton(ui->pointColorPicker, ui->viewport->getPointColor());
  /* Set up shortcuts */
  new QShortcut(QKeySequence(tr("Ctrl+O")), this, SLOT(openFile()));
  new QShortcut(QKeySequence(tr("L")), ui->xLocationSB, SLOT(setFocus()));
  new QShortcut(QKeySequence(tr("R")), ui->xRotationSB, SLOT(setFocus()));
  new QShortcut(QKeySequence(tr("S")), ui->xScaleSB, SLOT(setFocus()));
  new QShortcut(QKeySequence(tr("E")), ui->displayLinesCB, SLOT(toggle()));
  new QShortcut(QKeySequence(tr("V")), ui->displayPointsCB, SLOT(toggle()));
  connect(&gif_timer, SIGNAL(timeout()), this, SLOT(RecordGifFrame()));
}

MainWindow::~MainWindow() {
  this->saveSettings(nullptr);
  delete ui;
}

void MainWindow::resetSettings() {
  on_perspectiveProjButton_toggled(true);
  on_displayLinesCB_toggled(true);
  ui->lineWidthSB->setValue(1.0);
  on_displayPointsCB_toggled(false);
  ui->pointSizeSB->setValue(1.0);
  ui->xLocationSlider->setValue(0);
  ui->yLocationSlider->setValue(0);
  ui->zLocationSlider->setValue(0);
  ui->xRotationSlider->setValue(0);
  ui->yRotationSlider->setValue(0);
  ui->zRotationSlider->setValue(0);
  ui->xScaleSB->setValue(1);
  ui->yScaleSB->setValue(1);
  ui->zScaleSB->setValue(1);
  ui->uScaleSB->setValue(1);
}

void MainWindow::saveSettings(QString file_name) {
  QSettings settings("settings.conf", QSettings::IniFormat);
  if (file_name != nullptr) {
    settings.beginGroup("Filename");
    settings.setValue("model", file_name);
    settings.endGroup();
  }
  settings.beginGroup("MainWindow");
  settings.setValue("geometry", saveGeometry());
  settings.endGroup();
  settings.beginGroup("DrawSettings");
  settings.setValue("BackgroundColor", ui->viewport->getBackgroundColor());
  settings.setValue("DrawLines", ui->displayLinesCB->checkState());
  settings.setValue("DashedLines", ui->lineStyleDashedCB->checkState());
  settings.setValue("LinesColor", ui->viewport->getLineColor());
  settings.setValue("LinesWidth", ui->lineWidthSB->value());
  settings.setValue("DrawPoints", ui->displayPointsCB->checkState());
  settings.setValue("PointsType", ui->pointStyleSquareCB->checkState());
  settings.setValue("PointsColor", ui->viewport->getPointColor());
  settings.setValue("PointsSize", ui->pointSizeSB->value());
  settings.endGroup();
  settings.beginGroup("CameraProjection");
  settings.setValue("Perspective", ui->perspectiveProjButton->isChecked());
  settings.setValue("Orthographic", ui->orthographicProjButton->isChecked());
  settings.endGroup();
  settings.beginGroup("Location");
  settings.setValue("xLocationSB", ui->xLocationSB->value());
  settings.setValue("yLocationSB", ui->yLocationSB->value());
  settings.setValue("zLocationSB", ui->zLocationSB->value());
  settings.endGroup();
  settings.beginGroup("Rotation");
  settings.setValue("xRotationSB", ui->xRotationSB->value());
  settings.setValue("yRotationSB", ui->yRotationSB->value());
  settings.setValue("zRotationSB", ui->zRotationSB->value());
  settings.endGroup();
  settings.beginGroup("Scale");
  settings.setValue("xScaleSB", ui->xScaleSB->value());
  settings.setValue("yScaleSB", ui->yScaleSB->value());
  settings.setValue("zScaleSB", ui->zScaleSB->value());
  settings.setValue("uScaleSB", ui->uScaleSB->value());
  settings.endGroup();
}

void MainWindow::loadSettings() {
  if (QFile("settings.conf").exists() == false) {
    resetSettings();
    return;
  }

  QSettings settings("settings.conf", QSettings::IniFormat);
  settings.beginGroup("MainWindow");
  if (settings.contains("geometry"))
    restoreGeometry(settings.value("geometry").toByteArray());
  settings.endGroup();
  settings.beginGroup("DrawSettings");
  if (settings.contains("BackgroundColor"))
    ui->viewport->setBackgroundColor(
        settings.value("BackgroundColor").value<QColor>());
  if (settings.contains("DrawLines"))
    ui->displayLinesCB->setCheckState(
        settings.value("DrawLines").value<Qt::CheckState>());
  if (settings.contains("DashedLines"))
    ui->lineStyleDashedCB->setCheckState(
        settings.value("DashedLines").value<Qt::CheckState>());
  if (settings.contains("LinesColor"))
    ui->viewport->setLineColor(settings.value("LinesColor").value<QColor>());
  if (settings.contains("LinesWidth"))
    ui->lineWidthSB->setValue(settings.value("LinesWidth").toDouble());
  if (settings.contains("DrawPoints"))
    ui->displayPointsCB->setCheckState(
        settings.value("DrawPoints").value<Qt::CheckState>());
  if (settings.contains("PointsType"))
    ui->pointStyleSquareCB->setCheckState(
        settings.value("PointsType").value<Qt::CheckState>());
  if (settings.contains("PointsColor"))
    ui->viewport->setPointColor(settings.value("PointsColor").value<QColor>());
  if (settings.contains("PointsSize"))
    ui->pointSizeSB->setValue(settings.value("PointsSize").toDouble());
  settings.endGroup();
  settings.beginGroup("CameraProjection");
  if (settings.contains("Orthographic"))
    ui->orthographicProjButton->toggled(
        settings.value("Orthographic").toBool());
  if (settings.contains("Perspective"))
    ui->perspectiveProjButton->toggled(settings.value("Perspective").toBool());
  settings.endGroup();
  settings.beginGroup("Location");
  if (settings.contains("xLocationSB"))
    ui->xLocationSB->setValue(settings.value("xLocationSB").toDouble());
  if (settings.contains("yLocationSB"))
    ui->yLocationSB->setValue(settings.value("yLocationSB").toDouble());
  if (settings.contains("zLocationSB"))
    ui->zLocationSB->setValue(settings.value("zLocationSB").toDouble());
  settings.endGroup();
  settings.beginGroup("Rotation");
  if (settings.contains("xRotationSB"))
    ui->xRotationSB->setValue(settings.value("xRotationSB").toDouble());
  if (settings.contains("yRotationSB"))
    ui->yRotationSB->setValue(settings.value("yRotationSB").toDouble());
  if (settings.contains("zRotationSB"))
    ui->zRotationSB->setValue(settings.value("zRotationSB").toDouble());
  settings.endGroup();
  settings.beginGroup("Scale");
  if (settings.contains("xScaleSB"))
    ui->xScaleSB->setValue(settings.value("xScaleSB").toDouble());
  if (settings.contains("yScaleSB"))
    ui->yScaleSB->setValue(settings.value("yScaleSB").toDouble());
  if (settings.contains("zScaleSB"))
    ui->zScaleSB->setValue(settings.value("zScaleSB").toDouble());
  if (settings.contains("uScaleSB"))
    ui->uScaleSB->setValue(settings.value("uScaleSB").toDouble());
  settings.endGroup();
  settings.beginGroup("Filename");
  if (settings.contains("model")) {
    file_name = settings.value("model").toString();
    ui->viewport->setFileName(file_name.toStdString());
    ui->viewport->loadModel();
    showFileStats();
  }
  settings.endGroup();
}

void MainWindow::showFileStats() {
  QString stats =                                                         //
      file_name.section("/", -1, -1).section(".", 0, 0) + " | " +         //
      "verts:" + QString::number(ui->viewport->vertices_count) + " | " +  //
      "faces:" + QString::number(ui->viewport->faces_count) + " | " +     //
      "edges:" + QString::number(ui->viewport->edges_count);
  ui->statisticsLabel->setText(stats);
}

/* GUI behaviour related helper functions */

void MainWindow::paintButton(QPushButton *b, const QColor &c) {
  QString qss = QString(
                    "background-color: %1; border-width: 2px;"
                    "border-style: solid; border-radius: 5px;")
                    .arg(c.name());
  b->setStyleSheet(qss);
}

QColor convertColorToGreyscale(const QColor &c) {
  // formula from the documentaiton:
  // https://doc.qt.io/archives/qt-4.8/qcolor.html#qGray
  int greyv = (c.red() * 11 + c.green() * 16 + c.blue() * 5) / 32;
  /* Set the bounds */
  static const int lbound = 0.43 * 255;
  static const int ubound = 0.81 * 255;
  if (greyv < lbound) greyv = lbound;
  if (greyv > ubound) greyv = ubound;
  return QColor(greyv, greyv, greyv);
}

void setLayoutWidgetsState(const QLayout *layout, bool state) {
  for (int i = 0; i < layout->count(); ++i) {
    QWidget *widget = layout->itemAt(i)->widget();
    if (widget != NULL)
      widget->setEnabled(state);
    else {
      QLayout *inner = layout->itemAt(i)->layout();
      setLayoutWidgetsState(inner, state);
    }
  }
}

void syncSliderWithSB(DoubleSlider *s, QDoubleSpinBox *sb) {
  QObject::connect(s, &DoubleSlider::doubleValueChanged, sb,
                   &QDoubleSpinBox::setValue);
  QObject::connect(sb, &QDoubleSpinBox::valueChanged, s,
                   &DoubleSlider::setDoubleValue);
}

/* GUI behaviour related signal functions */

void MainWindow::on_backgroundColorPicker_clicked() {
  QColor prev_color = ui->viewport->getBackgroundColor();
  QColor color = QColorDialog::getColor(prev_color, this);
  if (!color.isValid()) return;
  ui->viewport->setBackgroundColor(color);
  ui->viewport->update();
  paintButton(ui->backgroundColorPicker, color);
}
void MainWindow::on_lineColorPicker_clicked() {
  QColor prev_color = ui->viewport->getLineColor();
  QColor color = QColorDialog::getColor(prev_color, this);
  if (!color.isValid()) return;
  ui->viewport->setLineColor(color);
  ui->viewport->update();
  paintButton(ui->lineColorPicker, color);
}
void MainWindow::on_pointColorPicker_clicked() {
  QColor prev_color = ui->viewport->getPointColor();
  QColor color = QColorDialog::getColor(prev_color, this);
  if (!color.isValid()) return;
  ui->viewport->setPointColor(color);
  ui->viewport->update();
  paintButton(ui->pointColorPicker, color);
}

void MainWindow::on_orthographicProjButton_toggled(bool checked) {
  if (checked) ui->viewport->setProjectionType(ProjectionType::ORTHOGONAL);
  ui->orthographicProjButton->setChecked(checked);
  ui->viewport->update();
}
void MainWindow::on_perspectiveProjButton_toggled(bool checked) {
  if (checked) ui->viewport->setProjectionType(ProjectionType::PERSPECTIVE);
  ui->perspectiveProjButton->setChecked(checked);
  ui->viewport->update();
}

void MainWindow::on_displayLinesCB_toggled(bool checked) {
  /* Set the state in the viewport */
  ui->viewport->setLineDisplayEnabled(checked);
  ui->viewport->update();
  /* Set the checkbox check state */
  ui->displayLinesCB->setChecked(checked);
  /* Disable the settings' frame */
  setLayoutWidgetsState(ui->lineSettingsLayout, checked);
  ui->lineStyleDashedCB->setEnabled(checked);
  /* Update the button color */
  QColor res_color = ui->viewport->getLineColor();
  if (checked == false) {
    res_color = convertColorToGreyscale(res_color);
  }
  paintButton(ui->lineColorPicker, res_color);
}
void MainWindow::on_displayPointsCB_toggled(bool checked) {
  /* Set the state in the viewport */
  ui->viewport->setPointDisplayEnabled(checked);
  ui->viewport->update();
  /* Set the checkbox check state */
  ui->displayPointsCB->setChecked(checked);
  /* Disable the settings' frame */
  setLayoutWidgetsState(ui->pointSettingsLayout, checked);
  ui->pointStyleSquareCB->setEnabled(checked);
  /* Update the button color */
  QColor res_color = ui->viewport->getPointColor();
  if (checked == false) {
    res_color = convertColorToGreyscale(res_color);
  }
  paintButton(ui->pointColorPicker, res_color);
}

/* Primitives' style related functions */

void MainWindow::on_pointStyleSquareCB_toggled(bool checked) {
  ui->viewport->setPointStyle(checked == true ? PointStyle::SQUARE
                                              : PointStyle::CIRCLE);
  ui->viewport->update();
}
void MainWindow::on_lineStyleDashedCB_toggled(bool checked) {
  ui->viewport->setLineStyle(checked == true ? LineStyle::DASHED
                                             : LineStyle::SOLID);
  ui->viewport->update();
}

/* Primitives' size related functions */

void MainWindow::setupWidthControls(DoubleSlider *s, QDoubleSpinBox *sb) {
  syncSliderWithSB(s, sb);
  /* Set up the spinbox */
  const unsigned int steps_count = ControlSteps::WIDTH;
  const float sb_limit = 20.0f;
  const float sb_step = sb_limit / steps_count;
  sb->setSingleStep(sb_step);
  sb->setDecimals(1);
  sb->setMinimum(1.0);
  sb->setMaximum(+sb_limit);
  /* Set up the slider */
  s->setMinimum(10);
  s->setMaximum(+steps_count);
  // internally the slider is of int type but emits the signal of type double
  s->divisor = steps_count / sb_limit;
  s->setDoubleValue(9.9);  // placeholder value to fix not emitted valueChanged
}
void MainWindow::on_lineWidthSlider_doubleValueChanged(double value) {
  ui->viewport->setLineWidth(value);
  ui->viewport->update();
}
void MainWindow::on_pointSizeSlider_doubleValueChanged(double value) {
  ui->viewport->setPointSize(value);
  ui->viewport->update();
}

/* Location related functions */

void MainWindow::setupLocationControls(DoubleSlider *s, QDoubleSpinBox *sb) {
  syncSliderWithSB(s, sb);
  /* Set up the spinbox */
  const unsigned int steps_count = ControlSteps::LOCATION;
  const float sb_limit = 1.0f;
  const float sb_step = sb_limit / steps_count;
  sb->setSingleStep(sb_step);
  sb->setDecimals(3);
  sb->setMinimum(-sb_limit);
  sb->setMaximum(+sb_limit);
  /* Set up the slider */
  s->setMinimum(-steps_count);
  s->setMaximum(+steps_count);
  // internally the slider is of int type but emits the signal of type double
  s->divisor = steps_count;
}
void MainWindow::on_xLocationSlider_doubleValueChanged(double value) {
  ui->viewport->setTranslationX(value);
  ui->viewport->update();
}
void MainWindow::on_yLocationSlider_doubleValueChanged(double value) {
  ui->viewport->setTranslationY(value);
  ui->viewport->update();
}
void MainWindow::on_zLocationSlider_doubleValueChanged(double value) {
  ui->viewport->setTranslationZ(value);
  ui->viewport->update();
}
void MainWindow::on_locationResetPushButton_clicked() {
  ui->xLocationSlider->setValue(0);
  ui->yLocationSlider->setValue(0);
  ui->zLocationSlider->setValue(0);
}

/* Rotation related functions */

void MainWindow::setupRotationControls(DoubleSlider *s, QDoubleSpinBox *sb) {
  syncSliderWithSB(s, sb);
  /* Set up the spinbox */
  const unsigned int steps_count = ControlSteps::ROTATION;
  const float sb_limit = 180.0f;
  const float sb_step = sb_limit / steps_count;
  sb->setSingleStep(sb_step);
  sb->setDecimals(1);
  sb->setMinimum(-sb_limit);
  sb->setMaximum(+sb_limit);
  /* Set up the slider */
  s->setMinimum(-steps_count);
  s->setMaximum(+steps_count);
  // internally the slider is of int type but emits the signal of type double
  s->divisor = steps_count / sb_limit;
}
void MainWindow::on_xRotationSlider_doubleValueChanged(double value) {
  ui->viewport->setRotationX(value);
  ui->viewport->update();
}
void MainWindow::on_yRotationSlider_doubleValueChanged(double value) {
  ui->viewport->setRotationY(value);
  ui->viewport->update();
}
void MainWindow::on_zRotationSlider_doubleValueChanged(double value) {
  ui->viewport->setRotationZ(value);
  ui->viewport->update();
}
void MainWindow::on_rotationResetPushButton_clicked() {
  ui->xRotationSlider->setValue(0);
  ui->yRotationSlider->setValue(0);
  ui->zRotationSlider->setValue(0);
}

/* Scale related functions */

void MainWindow::setupScaleControls(DoubleSlider *s, QDoubleSpinBox *sb) {
  syncSliderWithSB(s, sb);
  /* Set up the spinbox */
  const unsigned int steps_count = ControlSteps::SCALE;
  const float sb_limit = 10.0f;
  const float sb_step = sb_limit / steps_count;
  sb->setSingleStep(sb_step);
  sb->setDecimals(2);
  sb->setMinimum(0.01f);
  sb->setMaximum(10.00f);
  /* Set up the slider */
  s->setMinimum(1);
  s->setMaximum(steps_count);
  // internally the slider is of int type but emits the signal of type double
  s->divisor = steps_count / sb_limit;
  s->setDoubleValue(1.0f);
}
void MainWindow::on_xScaleSlider_doubleValueChanged(double value) {
  ui->viewport->setScaleX(value);
  ui->viewport->update();
}
void MainWindow::on_yScaleSlider_doubleValueChanged(double value) {
  ui->viewport->setScaleY(value);
  ui->viewport->update();
}
void MainWindow::on_zScaleSlider_doubleValueChanged(double value) {
  ui->viewport->setScaleZ(value);
  ui->viewport->update();
}
void MainWindow::on_uScaleSlider_doubleValueChanged(double value) {
  ui->viewport->setScaleU(value);
  ui->viewport->update();
}
void MainWindow::on_scaleResetPushButton_clicked() {
  ui->xScaleSB->setValue(1);
  ui->yScaleSB->setValue(1);
  ui->zScaleSB->setValue(1);
  ui->uScaleSB->setValue(1);
}

void MainWindow::on_openFilePushButton_released() { MainWindow::openFile(); }
void MainWindow::openFile() {
  QString dir = QDir::homePath() + "/Downloads/3Dmodels";
  this->file_name = QFileDialog::getOpenFileName(
      this, "Open 3d model", dir, "geometry definition file (*.obj)");
  if (this->file_name.isEmpty()) {
    return;
  }

  this->saveSettings(file_name);
  ui->viewport->setFileName(file_name.toStdString());
  ui->viewport->loadModel();
  showFileStats();
}

void MainWindow::on_screenshotPushButton_released() {
  QImage screenshot = ui->viewport->grabFramebuffer();
  QString dir = QDir::homePath();
  QString extension = "*.jpeg;;*.bmp";
  QString file_name = QFileDialog::getSaveFileName(this, tr("Save screenshot"),
                                                   dir, extension, &extension);
  if (file_name.isEmpty()) {
    return;
  }

  extension.remove(0, 2);
  if (file_name.right(extension.length() + 1) != ("." + extension)) {
    file_name = file_name + "." + extension;
  }
  screenshot.save(file_name, extension.toStdString().c_str(), 80);
}

void MainWindow::on_gifCapturePushButton_released() {
  gif = new QGifImage();
  gif_timer.start(1000 / gif_fps);
}
void MainWindow::RecordGifFrame() {
  if (gif_frame_counter < gif_fps * gif_sec) {
    gif_frame_counter++;
    gif->setDefaultDelay(1000 / gif_fps);
    QImage frame = ui->viewport->grabFramebuffer().scaled(gif_size);
    gif->addFrame(frame);
  } else {
    gif_timer.stop();
    gif_frame_counter = 0;
    QString dir = QDir::homePath();
    QString file_name = QFileDialog::getSaveFileName(this, "Save Gif", dir,
                                                     "Gif files (*.gif)");
    if (!file_name.isEmpty()) {
      gif->save(file_name);
    }
    delete gif;
  }
}

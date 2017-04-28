#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"


#include <model/qenvironement.h>
#include <gui/qenvironementeditor.h>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    const bool loaded = QEnvironementInstance->loadEnvironementFromFile();
    if (!loaded) {
        qWarning() << "Error: we could not restore the last saved environement";
    }

    ui->setupUi(this);
    initUi();
    loadUi();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUi() {
    connect(ui->actionEnvironement, &QAction::triggered, [&](bool) {
        QEnvironementEditor editor;
        editor.exec();
        ui->beaconsPanel->invalidate();
    });

    connect(ui->actionOpen, &QAction::triggered, [&](bool) {
        const QString fileName = QFileDialog::getOpenFileName(this,
                tr("Open Environement File"), "", QString("Environement (*%1)").arg(ENVIRONEMENT_FORMAT));
        if (!fileName.isEmpty()) {
            if (!QEnvironementInstance->loadEnvironementFromFile(fileName)) {
                QMessageBox::information(this, tr("Unable to open file"), "The file is corrupted");
            }
        }
    });

    connect(ui->actionSave, &QAction::triggered, [&](bool) {
        QString fileName = QFileDialog::getSaveFileName(this,
                tr("Open Environement File"), "", QString("Environement (*%1)").arg(ENVIRONEMENT_FORMAT));

        if (!fileName.endsWith(ENVIRONEMENT_FORMAT))
            fileName += ENVIRONEMENT_FORMAT;

        if (!fileName.isEmpty()) {
            if (!QEnvironementInstance->saveEnvironementInFile(fileName)) {
                QMessageBox::information(this, tr("Unable to open file"), "The file is corrupted");
            }
        }
    });
}

void MainWindow::loadUi() {
    QSettings settings;
    settings.beginGroup("UI");
    restoreGeometry(settings.value("geometry", saveGeometry()).toByteArray());
    restoreState(settings.value("state", saveState()).toByteArray());
    ui->verticalSplitter->restoreState(
                settings.value("vSplitter", ui->verticalSplitter->saveState())
                .toByteArray());
    ui->horizontalSplitter->restoreState(
                settings.value("hSplitter", ui->horizontalSplitter->saveState())
                .toByteArray());
    settings.endGroup();
}

void MainWindow::saveUi() {
    QSettings settings;
    settings.beginGroup("UI");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
    settings.setValue("vSplitter", ui->verticalSplitter->saveState());
    settings.setValue("hSplitter", ui->horizontalSplitter->saveState());
    settings.endGroup();
    settings.sync();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (!QEnvironementInstance->saveEnvironementInFile()) {
        qWarning() << "Error: could not save the current data model";
    }
    saveUi();
    QMainWindow::closeEvent(event);
}


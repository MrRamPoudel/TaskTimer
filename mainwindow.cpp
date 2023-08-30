#include "mainwindow.h"
#include "./ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lcdNumber->display("00:00:00");
    timer = new QTimer(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::startTimer() {
    ui->textEdit->hide();
    startTime = std::chrono::steady_clock::now();
    connect(timer, &QTimer::timeout, this, &MainWindow::showTime);
    timer->start(1000);
}

void MainWindow::stopTimer() {
    timer->stop();
    disconnect(timer, &QTimer::timeout, this, &MainWindow::showTime);
    startTime = std::chrono::steady_clock::now();
    ui->lcdNumber->display("00:00:00");
    ui->textEdit->show();
}

void MainWindow::showTime() {

}

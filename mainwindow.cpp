#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFinalState>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lcdNumber->display("00:00:00");
    timer = new QTimer(this);
    //create states
    QState* s0 = new QState();
    QState* s1 = new QState();
    QState* s2 = new QState();
    QFinalState * s3 = new QFinalState();
    s1->assignProperty(ui->pushButton, "text", "Stop");
    s2->assignProperty(ui->pushButton, "text", "Start");
    s0->addTransition(ui->pushButton, &QPushButton::clicked,s1);
    s1->addTransition(ui->pushButton, &QPushButton::clicked,s2);
    s2->addTransition(ui->pushButton, &QPushButton::clicked,s1);

    //Add states to machines
    timeState.addState(s0);
    timeState.addState(s1);
    timeState.addState(s2);
    timeState.addState(s3);

    //Set initial state
    timeState.setInitialState(s0);

    //connect state to actions
    connect(s1, &QState::entered, this, &MainWindow::startTimer);
    connect(s2, &QState::entered, this, &MainWindow::stopTimer);

    timeState.start();
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
    auto rightNow = std::chrono::steady_clock::now();
    auto timeDiff = std::chrono::duration_cast<std::chrono::seconds>(rightNow - startTime);
    auto convertedDiff = QString::fromStdString(std::format("{0:%H:%M:%S}", timeDiff));
    ui->lcdNumber->display(convertedDiff);
}

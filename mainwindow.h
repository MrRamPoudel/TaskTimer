#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <chrono>
#include <QTimer>
#include <QStateMachine>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer * timer;
    std::chrono::steady_clock::time_point startTime;
    QStateMachine timeState;
    void showTime();

private slots:
    void startTimer();
    void stopTimer();
};
#endif // MAINWINDOW_H

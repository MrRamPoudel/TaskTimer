#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <chrono>
#include <QTimer>
#include <QStateMachine>
#include "helper.h"

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
    std::map<std::string, std::string> config = Helper::readEnvFile(".env");
    QTimer * timer;
    std::chrono::steady_clock::time_point startTime;
    QStateMachine timeState;
    void submitEntry(const int & problemNumber, const std::chrono::time_point<std::chrono::system_clock> & dateCompleted, const long & elapsedTime);;
    void showTime();
private slots:
    void startTimer();
    void stopTimer();
};
#endif // MAINWINDOW_H

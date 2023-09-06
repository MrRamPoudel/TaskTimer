#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <chrono>
#include <QTimer>
#include <QStateMachine>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include "database.h"
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
    void showTime();
    Database db;
private slots:
    void startTimer();
    void stopTimer();
};
#endif // MAINWINDOW_H

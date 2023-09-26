#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <map>
#include <string>
#include <nlohmann/json.hpp>
#include <curl/curl.h>

using json = nlohmann::json;

// partial specialization (full specialization works too)
namespace nlohmann {
template<typename Clock, typename Duration>
struct adl_serializer<std::chrono::time_point<Clock, Duration>> {
    static void to_json(json& j, const std::chrono::time_point<Clock, Duration>& tp) {
        j = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
    }
};
}

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

    // Assign property and transition
    s1->assignProperty(ui->pushButton, "text", "Stop");
    s2->assignProperty(ui->pushButton, "text", "Start");
    s0->addTransition(ui->pushButton, &QPushButton::clicked,s1);
    s1->addTransition(ui->pushButton, &QPushButton::clicked,s2);
    s2->addTransition(ui->pushButton, &QPushButton::clicked,s1);

    //Add states to machines
    timeState.addState(s0);
    timeState.addState(s1);
    timeState.addState(s2);
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

void MainWindow::submitEntry(const int &problemNumber, const std::chrono::time_point<std::chrono::system_clock> &dateCompleted, const long &elapsedTime)
{
    using json = nlohmann::json;
    json data;
    data["dateCompleted"] = dateCompleted;
    data["problemNumber"] = problemNumber;
    data["elapsedTaskTime"] = elapsedTime;
    std::string jsonData = data.dump();
    CURL * curl;
    CURLcode res;
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:3000/createEntry");
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        res = curl_easy_perform(curl);

        if(res != CURLE_OK){
            qDebug() <<"curl_easy_perform() failed\n";
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

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
    auto diffTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
    auto elapsedTaskTime = std::format("{0:%H:%M:%S}",std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now()-startTime));
    auto now = std::chrono::system_clock::now();
    int problemNum =  ui->textEdit->toPlainText().toInt();
    submitEntry(problemNum,now, diffTime);
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

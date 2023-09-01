#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <map>
#include <string>
#include "helper.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lcdNumber->display("00:00:00");
    try
    {
        auto envVariables = Helper::readEnvFile(".env");
        std::string mongoUri = envVariables["MONGODB_URI"];

        // Create an instance.
        mongocxx::instance inst{};

        // Replace the connection string with your MongoDB deployment's connection string.
        const auto uri = mongocxx::uri{envVariables["MONGODB_URI"]};

        // Set the version of the Stable API on the client.
        mongocxx::options::client client_options;
        const auto api = mongocxx::options::server_api{ mongocxx::options::server_api::version::k_version_1 };
        client_options.server_api_opts(api);

        // Setup the connection and get a handle on the "admin" database.
        mongocxx::client conn{ uri, client_options };
        mongocxx::database db = conn["gymdb"];

        // Ping the database.
        const auto ping_cmd = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("ping", 1));
        db.run_command(ping_cmd.view());
        qDebug("Pinged your deployment. You successfully connected to MongoDB!");
    }
    catch (const std::exception& e)
    {
        // Handle errors.
        qDebug("Exception: ", qPrintable(e.what() ));
    }

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

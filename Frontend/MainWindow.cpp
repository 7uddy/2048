#include "MainWindow.h"
#include <QMenuBar>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    centralWidget(new QWidget(this)),
    boardLayout(new QGridLayout),
    gameLogic(new game::Game(4)),
    listener(std::make_shared<GameListener>())
{
    setCentralWidget(centralWidget);
    this->setMinimumSize(600, 400);
    this->setMaximumSize(1000, 600);
    centralWidget->setLayout(boardLayout);



    this->gameLogic->AddListener(listener);
    this->initializeGameBoard();

    // Menu bar setup
    QMenu* gameMenu = menuBar()->addMenu(tr("&Game"));
    QAction* resetAction = gameMenu->addAction(tr("&Reset"));
    QAction* exitAction = gameMenu->addAction(tr("&Exit"));

    connect(listener.get(), &GameListener::notifyMoveDone, this, [this]() {updateGameBoard(); });
    connect(listener.get(), &GameListener::notifyGameReset, this, [this]() {updateGameBoard(); });

    
    connect(resetAction, &QAction::triggered, this, &MainWindow::initializeGameBoard);
    connect(exitAction, &QAction::triggered, this, &MainWindow::close);
}

MainWindow::~MainWindow()
{
    delete boardLayout;
}

void MainWindow::initializeGameBoard()
{
    std::string boardString = this->gameLogic->GetBoard();
    std::istringstream stream(boardString);
    std::string token;

    int row = 0, column = 0;
    while (stream >> token)
    {
        QLabel* tile = new QLabel();
        QString text = QString::fromUtf8(token.c_str());
        tile->setText(text);
        this->boardLayout->addWidget(tile, row, column);

        column++;
        if (column == 4)
        {
            column = 0;
            row++;
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Up) {
        this->gameLogic->ApplyMove(game::Movement::UP);
    }
    else if (event->key() == Qt::Key_Down) {
        this->gameLogic->ApplyMove(game::Movement::DOWN);
    }
    else if (event->key() == Qt::Key_Left) {
        this->gameLogic->ApplyMove(game::Movement::LEFT);
    }
    else if (event->key() == Qt::Key_Right) {
        this->gameLogic->ApplyMove(game::Movement::RIGHT);
    }
    else if (event->key() == Qt::Key_Space) {
        this->gameLogic->ResetGame();
    }
    else {
        std::cout << "A fost apăsată o altă tastă!" << std::endl;
    }

    QMainWindow::keyPressEvent(event);
}


void MainWindow::updateGameBoard()
{
    std::string boardString = this->gameLogic->GetBoard();
    std::istringstream stream(boardString);
    std::string token;

    int row = 0, column = 0;
    while (stream >> token)
    {
        QString text = QString::fromUtf8(token.c_str());
        QLabel* tile = qobject_cast<QLabel*>(this->boardLayout->itemAtPosition(row, column)->widget());
        tile->setText(text);
        this->boardLayout->addWidget(tile, row, column);

        column++;
        if (column == 4)
        {
            column = 0;
            row++;
        }
    }
}

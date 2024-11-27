#include "MainWindow.h"
#include <QMenuBar>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    centralWidget(new QWidget(this)),
    boardLayout(new QGridLayout),
    gameLogic(new game::Game(4))
{
    setCentralWidget(centralWidget);
    this->setMinimumSize(600, 400);
    this->setMaximumSize(1000, 600);
    centralWidget->setLayout(boardLayout);

    gameLogic->SetBoard("2 2 4 2\n4 8 2 4\n2 4 2 2\n2 2 2 2");

    this->initializeGameBoard();

    // Menu bar setup
    QMenu* gameMenu = menuBar()->addMenu(tr("&Game"));
    QAction* resetAction = gameMenu->addAction(tr("&Reset"));
    QAction* exitAction = gameMenu->addAction(tr("&Exit"));

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

        this->boardLayout->addWidget(tile,row,column);
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

}


void MainWindow::updateGameBoard()
{

}

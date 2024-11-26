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

void MainWindow::keyPressEvent(QKeyEvent* event)
{

}

void MainWindow::initializeGameBoard()
{

}

void MainWindow::updateGameBoard()
{

}

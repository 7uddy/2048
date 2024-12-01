#include "MainWindow.h"
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    centralWidget(new QWidget(this)),
    boardLayout(new QGridLayout),
    gameLogic(new game::Game(4)),
    listener(std::make_shared<GameListener>())
{
    setCentralWidget(centralWidget);
    this->setMinimumSize(800, 600);
    this->setMaximumSize(1000, 800);
    centralWidget->setLayout(boardLayout);

    this->gameLogic->AddListener(listener);
    this->InitializeGameBoard();

    // Menu bar setup

    QMenu* gameMenu = menuBar()->addMenu(tr("&Game"));
    QAction* saveAction = gameMenu->addAction(tr("&Save"));
    QAction* loadAction = gameMenu->addAction(tr("&Load"));
    QAction* resetAction = gameMenu->addAction(tr("&Reset"));
    QAction* exitAction = gameMenu->addAction(tr("&Exit"));

    connect(listener.get(), &GameListener::notifyMoveDone, this, [this]() {UpdateGameBoard(); });
    connect(listener.get(), &GameListener::notifyGameReset, this, [this]() {UpdateGameBoard(); });

    //TODO implement board reset action
    connect(resetAction, &QAction::triggered, this, &MainWindow::InitializeGameBoard);
    connect(saveAction, &QAction::triggered, this, &MainWindow::SaveGameBoard);
    connect(loadAction, &QAction::triggered, this, &MainWindow::LoadGameBoard);
    connect(exitAction, &QAction::triggered, this, &MainWindow::close);
}

MainWindow::~MainWindow()
{
    delete boardLayout;
}

QString MainWindow::GenerateColor(int value)
{
    if (value == 0) {
        return "#BEC7D5"; // Default background for empty tiles
    }

    // Calculate a unique hue based on the logarithm of the value
    double hue = fmod((log2(value) * 40), 360.0); // Scale and wrap hue within [0, 360]
    double saturation = 0.7; // Keep saturation constant (70%)
    double lightness = 0.6;  // Keep lightness constant (60%)

    // Convert HSL to RGB using Qt's QColor
    QColor color;
    color.setHslF(hue / 360.0, saturation, lightness);
    return color.name();
}




void MainWindow::UpdateTileColor(QLabel* label, const QString& color) 
{
    QString currentStyle = label->styleSheet();

    // Find and replace only the background-color property
    QRegularExpression re("background-color: #[A-Fa-f0-9]{6};"); // Regex for current color
    QString newStyle = currentStyle;

    if (re.match(currentStyle).hasMatch()) {
        // Replace the existing background-color
        newStyle.replace(re, QString("background-color: %1;").arg(color));
    }
    else {
        // Append the new background-color property if it doesn't exist
        newStyle.append(QString("background-color: %1;").arg(color));
    }

    label->setStyleSheet(newStyle);
}

void MainWindow::InitializeGameBoard()
{
    this->gameLogic->ResetGame();

    // Clear existing items if necessary
    while (QLayoutItem* item = boardLayout->takeAt(0)) {
        delete item->widget();  // Deletes the QLabel or widget in the grid
        delete item;
    }

    m_scoreLabel = new QLabel("Score: 0", this);
    m_scoreLabel->setAlignment(Qt::AlignCenter);
    m_scoreLabel->setStyleSheet("font-size: 20px; font-weight: bold; margin: 5px;");
    boardLayout->addWidget(m_scoreLabel, 0, 0, 1, m_boardSize); // Add score label spanning all columns


    std::string boardString = this->gameLogic->GetBoard();
    std::istringstream stream(boardString);
    std::string token;

    //Start row count from 1, as row 0 has the score
    int row = 1, column = 0;
    while (stream >> token)
    {
        //Create a new tile to add as a widget in the grid, set it's text as the one in the game
        QLabel* tile = new QLabel();
        QString text = QString::fromUtf8(token.c_str());

        this->boardLayout->addWidget(tile, row, column);

        //Set the text to 0 so empty tiles will get a basic color, center the text
        tile->setText(text.toInt() > 0 ? QString::number((text).toInt()) : "");
        tile->setAlignment(Qt::AlignCenter);
        

        QString color = this->GenerateColor(text.toInt());

        //Set a stylesheet for each tile, passing the generated color as a parameter
        tile->setStyleSheet(QString(
            "QLabel { "
            "border: 2px solid #BBADA0; "  // Border color
            "border-radius: 10px; "        // Rounded corners
            "background-color: %1; "      // Dynamic background
            "font-size: 24px; "           // Font size
            "font-weight: bold; "         // Bold text
            "}"
        ).arg(color));


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


void MainWindow::SaveGameBoard() {
    QString filePath = QFileDialog::getSaveFileName(this, "Choose save location ", "", "Text (*.txt)");
    if (filePath.isEmpty()) {
        return;
    }

    try {
        gameLogic->SetPathToBoardFile(filePath.toStdString());
        gameLogic->SaveGameStateInFile();
        QMessageBox::information(this, "Saved succesfully ", "The game state was saved succesfully! ");
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Save Error", QString("There was a problem: %1").arg(e.what()));
    }
}

void MainWindow::LoadGameBoard() {
    QString filePath = QFileDialog::getOpenFileName(this, "Choose load file ", "", "Text (*.txt)");
    if (filePath.isEmpty()) {
        return;
    }
    try {
        gameLogic->SetPathToBoardFile(filePath.toStdString());
        gameLogic->ReadGameStateFromFile();
        UpdateGameBoard();
        QMessageBox::information(this, "Load succesfully ", "The game state was loaded succesfully! ");
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Load Error", QString("There was a problem: %1").arg(e.what()));
    }

}

void MainWindow::UpdateGameBoard()
{
    this->m_scoreLabel->setText(QString("Score: %1").arg(this->gameLogic->GetScore()));
    std::string boardString = this->gameLogic->GetBoard();
    std::istringstream stream(boardString);
    std::string token;

    //Start row count from 1, as row 0 has the score
    int row = 1, column = 0;
    while (stream >> token)
    {
        //Same as before, generate a QString for each label, cast every grid widget as a lavel and style based on value
        QString text = QString::fromUtf8(token.c_str());
        QLabel* tile = qobject_cast<QLabel*>(this->boardLayout->itemAtPosition(row, column)->widget());
        tile->setText(text.toInt() > 0 ? QString::number((text).toInt()) : "");
        tile->setAlignment(Qt::AlignCenter);
        this->boardLayout->addWidget(tile, row, column);

        QString color = this->GenerateColor(text.toInt());

        //Update a tile's color without changing it's entire stylesheet
        UpdateTileColor(tile, color);

        column++;
        if (column == 4)
        {
            column = 0;
            row++;
        }
    }
}

bool MainWindow::IsBoardEmpty() {
    return this->boardLayout->count() == 0;
}

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

QString MainWindow::generateColor(int value)
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

void MainWindow::updateTileColor(QLabel* label, const QString& color) 
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

void MainWindow::initializeGameBoard()
{
    std::string boardString = this->gameLogic->GetBoard();
    std::istringstream stream(boardString);
    std::string token;

    int row = 0, column = 0;
    while (stream >> token)
    {
        //Create a new tile to add as a widget in the grid, set it's text as the one in the game
        QLabel* tile = new QLabel();
        QString text = QString::fromUtf8(token.c_str());

        //Set the text to 0 so empty tiles will get a basic color
        tile->setText(text.toInt() > 0 ? QString::number((text).toInt()) : "");
        this->boardLayout->addWidget(tile, row, column);
        
        //Generate a color based on the value
        QString color = this->generateColor(text.toInt());

        tile->setStyleSheet(QString(
            "QLabel { "
            "border: 2px solid #BBADA0; "  // Border color
            "border-radius: 10px; "        // Rounded corners
            "background-color: %1; "      // Dynamic background
            "color: %2; "                 // Text color
            "font-size: 24px; "           // Font size
            "font-weight: bold; "         // Bold text
            "}"
        )
            .arg(color)
            .arg(text.toInt() > 4 ? "#F9F6F2" : "#776E65")); // Light or dark text based on value

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
        tile->setText(text.toInt() > 0 ? QString::number((text).toInt()) : "");
        tile->setAlignment(Qt::AlignCenter);
        this->boardLayout->addWidget(tile, row, column);

        QString color = this->generateColor(text.toInt());

        updateTileColor(tile, color);

        column++;
        if (column == 4)
        {
            column = 0;
            row++;
        }
    }
}

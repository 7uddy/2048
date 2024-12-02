#include "MainWindow.h"



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
    this->InitialiseGameBoard();

    // Menu bar setup

    QMenu* gameMenu = menuBar()->addMenu(tr("&Game"));

    QAction* saveAction = gameMenu->addAction(tr("&Save"));
    QAction* loadAction = gameMenu->addAction(tr("&Load"));
    QAction* resetAction = gameMenu->addAction(tr("&Reset"));
    QAction* exitAction = gameMenu->addAction(tr("&Exit"));

    connect(listener.get(), &GameListener::notifyMoveDone, this, [this]() {UpdateGameBoard(); });
    connect(listener.get(), &GameListener::notifyGameReset, this, [this]() {UpdateGameBoard(); });
    connect(listener.get(), &GameListener::notifyGameOver, this, [this]() 
        {
            UpdateGameBoard();
            QMessageBox::information(this, "Joc Terminat", "Felicitari! Jocul s-a terminat."); 
            gameLogic->ResetGame();
        }
    );

    //TODO implement board reset action
    connect(resetAction, &QAction::triggered, this, &MainWindow::InitialiseGameBoard);
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

QString MainWindow::DarkenColor(const QString& base, int percentage)
{
    QColor color(base);
    color = color.darker(percentage); // Darken by percentage (e.g., 120% is darker)
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

void MainWindow::InitialiseGameBoard()
{
    this->m_selectedPosition1.row = 0;
    this->m_selectedPosition1.column = 0;
    this->m_selectedPosition2.row = 0;
    this->m_selectedPosition2.column = 0;
    this->m_isFirstSelection = true;

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

    m_undoLabel = new QLabel("Undos Left: 2 \n (Press U to undo)", this);
    m_undoLabel->setAlignment(Qt::AlignRight);
    m_undoLabel->setStyleSheet("font-size: 13px; font-weight: bold; margin: 5px;");
    boardLayout->addWidget(m_undoLabel, 0, 0, 1, m_boardSize); 

    m_swapLabel = new QLabel("Swaps Left: 2 \n (Click two tiles to swap them)", this);
    m_swapLabel->setAlignment(Qt::AlignLeft);
    m_swapLabel->setStyleSheet("font-size: 13px; font-weight: bold; margin: 5px;");
    boardLayout->addWidget(m_swapLabel, 0, 0, 1, m_boardSize);


    std::string boardString = this->gameLogic->GetBoard();
    std::istringstream stream(boardString);
    std::string token;

    //Start row count from 1, as row 0 has the score
    int row = 1, column = 0;
    while (stream >> token)
    {
        // Create a new tile as a QPushButton to add as a widget in the grid
        QPushButton* tile = new QPushButton();
        QString text = QString::fromUtf8(token.c_str());

        this->boardLayout->addWidget(tile, row, column);

        tile->setFixedSize(200, 120);

        // Set the text to 0 so empty tiles will get a basic color, center the text
        tile->setText(text.toInt() > 0 ? QString::number((text).toInt()) : "");
        tile->setEnabled(true); // Disable buttons to make them behave like tiles
        tile->setFocusPolicy(Qt::NoFocus);

        connect(tile, &QPushButton::clicked, this, [this, row, column]() {
            this->HandleTileClick(row, column);
            });

        QString color = this->GenerateColor(text.toInt());

        // Set a stylesheet for each tile, passing the generated color as a parameter
        UpdateTileStyleSheet(tile, color);

        column++;
        if (column == 4)
        {
            column = 0;
            row++;
        }
    }
}

void MainWindow::HandleTileClick(int row, int column)
{
    game::Position currentPosition(row - 1, column); // Adjust if Position uses zero-based 

    if (m_isFirstSelection) {
        m_selectedPosition1 = currentPosition;
        QPushButton* clickedButton = qobject_cast<QPushButton*>(this->boardLayout->itemAtPosition(row, column)->widget());
        
        //if value is 0, ignore
        if (clickedButton->text().toInt() == 0)
            return;

        QString darkColor = DarkenColor(GenerateColor(clickedButton->text().toInt()));
        UpdateTileStyleSheet(clickedButton, darkColor);
        m_isFirstSelection = false;
    }
    else {
        QPushButton* clickedButton = qobject_cast<QPushButton*>(this->boardLayout->itemAtPosition(row, column)->widget());
        
        //if value is 0, ignore
        if (clickedButton->text().toInt() == 0)
        {
            m_isFirstSelection = true;
            this->UpdateGameBoard();
            return;
        }

        m_isFirstSelection = true;
        m_selectedPosition2 = currentPosition;
    
        // Perform the tile swap
        if(m_selectedPosition1.row != m_selectedPosition2.row || m_selectedPosition1.column != m_selectedPosition2.column)
            this->gameLogic->ApplySwitchTiles(m_selectedPosition1, m_selectedPosition2);

        // Update the game board visually
        UpdateGameBoard();
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
        this->InitialiseGameBoard();
    }
    else if (event->key() == Qt::Key_U) {
        this->gameLogic->ApplyUndo();
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
    // Update score and undo labels
    game::GameState currentGameState = this->gameLogic->GetGameState();
    this->m_scoreLabel->setText(QString("Score: %1").arg(currentGameState.score));
    this->m_undoLabel->setText(QString("Undos Left: %1 \n (Press U to undo)").arg(currentGameState.timesLeftToUseUndo));
    this->m_swapLabel->setText(QString("Swaps Left: %1 \n (Click two tiles to swap them)").arg(currentGameState.timesLeftToUseSwap));

    std::string boardString = currentGameState.board;
    std::istringstream stream(boardString);
    std::string token;

    // Start row count from 1, as row 0 has the score
    int row = 1, column = 0;
    while (stream >> token)
    {
        // Get the button at the current position in the grid
        QPushButton* tile = qobject_cast<QPushButton*>(this->boardLayout->itemAtPosition(row, column)->widget());

        if (tile) {
            // Update button text
            QString text = QString::fromUtf8(token.c_str());
            tile->setText(text.toInt() > 0 ? QString::number((text).toInt()) : "");

            // Generate color based on tile value
            QString color = this->GenerateColor(text.toInt());

            // Update button's stylesheet
            UpdateTileStyleSheet(tile, color);
        }
        if (!(currentGameState.timesLeftToUseSwap > 0u)) {
            UpdateTileState(row, column, false);
        }
        else {
            UpdateTileState(row, column, true);
        }

        column++;
        if (column == 4)
        {
            column = 0;
            row++;
        }
    }
}

void MainWindow::UpdateTileStyleSheet(QPushButton* tile, QString color)
{
    tile->setStyleSheet(QString(
        "QPushButton { "
        "border: 2px solid #BBADA0; "  // Border color
        "border-radius: 10px; "        // Rounded corners
        "background-color: %1; "      // Dynamic background
        "font-size: 24px; "           // Font size
        "font-weight: bold; "         // Bold text
        "} "
    ).arg(color));
}

void MainWindow::UpdateTileState(int row, int column, bool value)
{
    for (int row = 1; row <= 4; ++row) {
        for (int column = 0; column < 4; ++column) {
            QPushButton* tile = qobject_cast<QPushButton*>(this->boardLayout->itemAtPosition(row, column)->widget());
            if (tile) {
                tile->setEnabled(value); // Toggle interaction
            }
        }
    }
}

bool MainWindow::IsBoardEmpty() {
    return this->boardLayout->count() == 0;
}

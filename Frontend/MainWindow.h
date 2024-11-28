#include <QMainWindow>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include "GameListener.h"
#include "Game.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT;

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    QWidget* centralWidget;
    QGridLayout* boardLayout;
    QLabel* boardTiles[4][4];
    game::Game* gameLogic;
    std::shared_ptr<GameListener> listener;

    bool hasChildren();
    void initializeGameBoard();
    void updateGameBoard();
};
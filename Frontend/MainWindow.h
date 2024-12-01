#include <QMainWindow>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QRegularExpression>
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
    QLabel* m_scoreLabel;
    game::Game* gameLogic;
    std::shared_ptr<GameListener> listener;
    unsigned int m_boardSize = 4;

    void UpdateTileColor(QLabel*,const QString&);
    QString GenerateColor(int);
    void InitializeGameBoard();
    void ResetGameBoard();
    void SaveGameBoard();
    void LoadGameBoard();
    void UpdateGameBoard();
    bool IsBoardEmpty();
};
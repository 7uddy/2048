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
    QLabel* m_undoLabel;
    QLabel* m_swapLabel;
    game::Game* gameLogic;
    std::shared_ptr<GameListener> listener;
    unsigned int m_boardSize = 4;

    game::Position m_selectedPosition1;
    game::Position m_selectedPosition2;
    bool m_isFirstSelection = true;

    void UpdateTileColor(QLabel*,const QString&);
    QString GenerateColor(int);
    void InitializeGameBoard();
    void HandleTileClick(int,int);
    void ResetGameBoard();
    void SaveGameBoard();
    void LoadGameBoard();
    void UpdateGameBoard();
    bool IsBoardEmpty();
};
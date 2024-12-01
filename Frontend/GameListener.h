#include <QObject>
#include "IGameListener.h"

class GameListener : public QObject, public game::IGameListener
{
	Q_OBJECT
public:
	// Inherited via IGameListener
	void OnMoveDone() override;
	void OnGameOver() override;
	void OnGameReset() override;
	void OnEnableUndoMove(bool isEnabled) override;
	void OnEnableSwapTileMove(bool isEnabled) override;
signals:
	void notifyMoveDone();
	void notifyGameOver();
	void notifyGameReset();
	void enableUndoButton(bool isEnabled);
	void enableSwapTileButton(bool isEnabled);

};
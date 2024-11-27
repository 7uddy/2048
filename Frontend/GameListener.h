#include <QObject>
#include "IGameListener.h"

class GameListener : public QObject, public game::IGameListener
{
	Q_OBJECT
public:
	// Inherited via IGameListener
	void OnMoveDone() override;
	void OnGameOver() override;
signals:
	void notifyMoveDone();
	void notifyGameOver();
};
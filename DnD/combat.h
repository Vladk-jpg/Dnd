#ifndef COMBAT_H
#define COMBAT_H

#include <QObject>
#include "dice.h"
#include "entity.h"
#include "player.h"

class Combat : public QObject
{
    Q_OBJECT
public:
    Combat(Player *);

    void start(Entity *);

private:
    Player *player;
    Entity *enemy;
    int needRoll;
    int phase;
    bool yourTurn;
    int playerLastRoll;
    int enemyLastRoll;
    int round;

    int bonus();

public slots:
    void handleRoll(int, int);

signals:
    void sendText(QString);
    void enemyRoll(int);
    void gameOver();
};

#endif // COMBAT_H

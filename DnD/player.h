#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QVector>
#include "constants.h"
#include "entity.h"

class Player : public Entity
{
    Q_OBJECT
public:
    Player();
    Player(QString, QString, QString, int, int, int, int, int, int, int);

    void setup(Player *);
    void setup(QString, QString, QString, int, int, int, int, int, int, int);
    QString getGameClass();
    void setGameClass(QString gameClass);
    int getLvl();
    int getExp();
    int getMaxExp();
    void useItem(int i);
    void addExp(int);

    QVector<Item *> inventory;

private:
    QString gameClass;
    int healDice;
    int lvl = 1;
    int exp = 0;
    QVector<int> maxExp;
};

#endif // PLAYER_H

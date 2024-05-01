#ifndef WORLD_H
#define WORLD_H

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "armor.h"
#include "combat.h"
#include "entity.h"
#include "player.h"
#include "potion.h"
#include "weapon.h"

struct Place
{
    QString name;
    QString description;
    QVector<QString> exits;
    QVector<QString> npcs;
    int type;
    int lvl;
};
struct Dialog
{
    QString name;
    struct DialogNode
    {
        int id;
        QString option;
        QString dialog;
        QVector<int> options;
        int event;
        bool was = false;
    };
    QVector<DialogNode> nodes;
};

class World : public QObject
{
    Q_OBJECT
public:
    World(Player *, Combat *);
    World();

    Weapon *longSword = new Weapon(1, "Длинный меч", "Урон 1D8", D8, DAMAGE);

    Armor *chain = new Armor(2, "Кольчуга", "Защита 16", 16, DEFENCE);
    Armor *leatherArmor = new Armor(3, "Кожаная броня", "Защита 12", 12, DEFENCE);

    Potion *SmallHealPotion = new Potion(4, "Малое лечебное зелье", "Восстанавливает 4 HP", 4, HEAL);

    Item *getItem(int id);

private:
    Dialog dialog;
    QVector<int> possibleWays;
    QMap<QString, Entity *> entities;
    QMap<QString, bool> defeated;
    QMap<int, Item *> items;
    QMap<QString, Place *> places;

    Player *player;
    Place *currentPlace;
    QString currentEntity;
    Combat *combat;

    int state{CREATION};

    QMap<QString, Place *> parsePlaces(const QString &filePath);
    QMap<QString, Entity *> parseEntities(const QString &filePath);
    Dialog parseDialog(const QString &filePath);

    void goToPlace(QString place);
    void talkToNPC(QString npc);
    void fightToNPC(QString npc);
    void startDialog();
    void findPossibleWays();
    void checkWay(int id);
    void chooseOption(QString option);

public slots:
    void handleCommand(QString command);
    void handleFightEnd(QString name);
signals:
    void sendText(QString, QColor);
    void createPlayer();
};

#endif // WORLD_H

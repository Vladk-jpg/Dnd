#ifndef WORLD_H
#define WORLD_H

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "armor.h"
#include "combat.h"
#include "dialog.h"
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

class World : public QObject
{
    Q_OBJECT
public:
    World(Player *, Combat *);
    World();

    Weapon *longSword = new Weapon("Длинный меч", "Урон 1D8", D8, DAMAGE);

    Armor *chain = new Armor("Кольчуга", "Защита 16", 16, DEFENCE);
    Armor *leatherArmor = new Armor("Кожаная броня", "Защита 12", 12, DEFENCE);

    Potion *SmallHealPotion = new Potion("Малое лечебное зелье", "Восстанавливает 4 HP", 4, HEAL);

    Item *getItem(QString name);
    void goToPlace(QString place);
    void talkToNPC(QString npc);
    void fightToNPC(QString npc);

private:
    QMap<QString, Dialog *> dialogs;
    QMap<QString, Entity *> entities;
    QMap<QString, bool> defeated;
    QMap<QString, Item *> items;
    QMap<QString, Place *> places;

    Player *player;
    Place *currentPlace;
    Combat *combat;

    int state{CREATION};

    QMap<QString, Place *> parsePlaces(const QString &filePath);

public slots:
    void handleCommand(QString command);
    void handleFightEnd(QString name);
signals:
    void sendText(QString, QColor);
    void createPlayer();
};

#endif // WORLD_H

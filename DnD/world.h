#ifndef WORLD_H
#define WORLD_H

#include "armor.h"
#include "combat.h"
#include "dialog.h"
#include "entity.h"
#include "player.h"
#include "potion.h"
#include "weapon.h"

struct Place
{
    Place(QString placeName,
          QString descr,
          QVector<QString> another,
          QVector<QString> npcArr,
          int placeType)
    {
        name = placeName;
        description = descr;
        exits = another;
        npcs = npcArr;
        type = placeType;
    }

    QString name;
    QString description;
    QVector<QString> exits;
    QVector<QString> npcs;
    int type;
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

    Entity *goblin = new Entity("Голлум", "Гоблин", 7, -1, 2, 0, 0, -1, -1, 4);

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

public slots:
    void handleCommand(QString command);
    void handleFightEnd(QString name);
signals:
    void sendText(QString);
    void createPlayer();
};

#endif // WORLD_H

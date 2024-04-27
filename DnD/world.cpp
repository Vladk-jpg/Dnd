#include "world.h"

World::World(Player *player, Combat *combat)
{   
    this->player = player;
    this->combat = combat;
    goblin->setDamage(D6);
    goblin->setDefence(15);
    entities.insert(goblin->getName(), goblin);
    defeated.insert(goblin->getName(), 0);
    places.insert("Вернживальд",
                  new Place("Вернживальд",
                            "Славный город",
                            QVector<QString>{"A", "B", "C"},
                            QVector<QString>{"Голлум"},
                            CITY));
    currentPlace = places["Вернживальд"];

    connect(combat, &Combat::fightEnd, this, &World::handleFightEnd);
}

World::World() = default;

Item *World::getItem(QString name)
{
    return items[name];
}

void World::goToPlace(QString place)
{
    bool isFind = false;
    for (int i = 0; i < currentPlace->exits.count(); i++) {
        if (place == currentPlace->exits[i]) {
            isFind = true;
            break;
        }
    }
    if (isFind) {
        if (currentPlace->type == CITY && places[place]->type == CITY) {
            //какое-то случайное событие
        }
        currentPlace = places[place];
    } else {
        emit sendText("Такого места не существует или вы не можете до него добраться");
    }
}

void World::talkToNPC(QString npc)
{
    bool isFind = false;
    for (int i = 0; i < currentPlace->npcs.count(); i++) {
        if (npc == currentPlace->npcs[i]) {
            isFind = true;
            break;
        }
    }
    if (isFind) {
        state = DIALOG;
        //начало диалога
    } else {
        emit sendText("Такого NPC не существует или вы не можете до него добраться");
    }
}

void World::fightToNPC(QString npc)
{
    bool isFind = false;
    for (int i = 0; i < currentPlace->npcs.count(); i++) {
        if (npc == currentPlace->npcs[i]) {
            isFind = true;
            break;
        }
    }
    if (isFind) {
        if (defeated[npc]) {
            emit sendText(npc + " больше нет с нами");
        } else {
            combat->start(entities[npc]);
            state = FIGHT;
        }
    } else {
        emit sendText("Такого NPC не существует или вы не можете до него добраться");
    }
}

void World::handleCommand(QString command)
{
    if (state == FREE) {
        if (command.mid(0, 3) == "/go") {
            QString place = command.mid(4);
            goToPlace(place);
        } else if (command == "/info") {
            QString info = "Список мест, в которые можно пойти:\n";
            for (int i = 0; i < currentPlace->exits.size(); i++) {
                info.append(QString::number(i + 1) + " " + currentPlace->exits[i] + "\n");
            }
            info.append("Список NPC:\n");
            for (int i = 0; i < currentPlace->npcs.size(); i++) {
                if (!defeated[currentPlace->npcs[i]]) {
                    info.append(QString::number(i + 1) + " " + currentPlace->npcs[i] + "\n");
                }
            }
            emit sendText(info);
        } else if (command.mid(0, 5) == "/talk") {
            QString npc = command.mid(6);
            talkToNPC(npc);
        } else if (command.mid(0, 6) == "/fight") {
            QString npc = command.mid(7);
            fightToNPC(npc);
        } else if (command == "/create") {
            emit sendText("Вы уже создали персонажа");
        } else {
            emit sendText("Команда не найдена");
        }
    } else if (state == CREATION) {
        if (command == "/create") {
            emit createPlayer();
            state = FREE;
        } else {
            emit sendText("Команда не найдена");
        }
    } else if (state == DIALOG) {
        if (command.mid(0, 7) == "/choose") {
        } else {
            emit sendText("Команда не найдена");
        }
    }
}

void World::handleFightEnd(QString name)
{
    defeated[name] = true;
    state = FREE;
}

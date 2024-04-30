#include "world.h"

World::World(Player *player, Combat *combat)
{   
    this->player = player;
    this->combat = combat;

    entities.insert("Гоблин",
                    new Entity("Гоблин", "Злобный карлик", 7, -1, 2, 0, 0, -1, -1, 4, 15, D6));

    QString filePath = "D:/BSUIR/OAIP/CourseWorkGitHub/Dnd/Sources/JSONS/places.json";
    places = parsePlaces(filePath);
    currentPlace = places["Распутье"];

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
        if (places[place]->lvl <= player->getLvl()) {
            if (places[place]->type == ENVIRONMENT) {
                //какое-то случайное событие
            }
            currentPlace = places[place];
            emit sendText(currentPlace->description, Qt::black);
        } else {
            emit sendText("У вас недостаточно уровней, чтобы добраться до этого места\n",
                          Qt::darkYellow);
        }
    } else {
        emit sendText("Такого места не существует или вы не можете до него добраться\n", Qt::red);
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
        emit sendText("Такого NPC не существует или вы не можете до него добраться\n", Qt::red);
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
        combat->start(entities[npc]);
        state = FIGHT;
    } else {
        emit sendText("Такого NPC не существует или вы не можете до него добраться\n", Qt::red);
    }
}

QMap<QString, Place *> World::parsePlaces(const QString &filePath)
{
    QMap<QString, Place *> placesMap;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Не удалось открыть файл";
        return placesMap;
    }

    QByteArray jsonData = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);

    if (!doc.isArray()) {
        qDebug() << "Корневой элемент не является массивом";
        return placesMap;
    }

    QJsonArray placeArray = doc.array();
    for (const auto &placeValue : placeArray) {
        if (!placeValue.isObject()) {
            qDebug() << "Элемент места не является объектом";
            continue;
        }

        QJsonObject placeObject = placeValue.toObject();

        Place *place = new Place;
        place->name = placeObject.value("name").toString();
        place->description = placeObject.value("description").toString();
        place->type = placeObject.value("type").toInt();
        place->lvl = placeObject.value("lvl").toInt();

        QJsonArray exitsArray = placeObject.value("exits").toArray();
        for (const auto &exitValue : exitsArray) {
            if (exitValue.isString()) {
                place->exits.append(exitValue.toString());
            }
        }

        QJsonArray npcsArray = placeObject.value("npcs").toArray();
        for (const auto &npcValue : npcsArray) {
            if (npcValue.isString()) {
                place->npcs.append(npcValue.toString());
            }
        }

        placesMap.insert(place->name, place);
    }

    file.close();
    return placesMap;
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
                info.append(QString::number(i + 1) + " " + currentPlace->exits[i] + " (Уровень "
                            + QString::number(places[currentPlace->exits[i]]->lvl) + ")\n");
            }
            if (currentPlace->type != ENVIRONMENT) {
                info.append("Список NPC:\n");
                for (int i = 0; i < currentPlace->npcs.size(); i++) {
                    if (!defeated[currentPlace->npcs[i]]) {
                        info.append(QString::number(i + 1) + " " + currentPlace->npcs[i] + "\n");
                    }
                }
            }
            emit sendText(info, Qt::black);
        } else if (command.mid(0, 5) == "/talk") {
            QString npc = command.mid(6);
            talkToNPC(npc);
        } else if (command.mid(0, 6) == "/fight") {
            QString npc = command.mid(7);
            fightToNPC(npc);
        } else if (command == "/create") {
            emit sendText("Вы уже создали персонажа\n", Qt::red);
        } else {
            emit sendText("Команда не найдена\n", Qt::red);
        }
    } else if (state == CREATION) {
        if (command == "/create") {
            emit createPlayer();
            state = FREE;
        } else {
            emit sendText("Команда не найдена\n", Qt::red);
        }
    } else if (state == DIALOG) {
        if (command.mid(0, 7) == "/choose") {
        } else {
            emit sendText("Команда не найдена\n", Qt::red);
        }
    }
}

void World::handleFightEnd(QString name)
{
    defeated[name] = true;
    state = FREE;
}

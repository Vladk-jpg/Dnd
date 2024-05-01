#include "world.h"

World::World(Player *player, Combat *combat)
{   
    this->player = player;
    this->combat = combat;

    // entities.insert("Гоблин",
    //                 new Entity("Гоблин", "Злобный карлик", 7, -1, 2, 0, 0, -1, -1, 4, 15, D6));

    QString filePath = "D:/BSUIR/OAIP/CourseWorkGitHub/Dnd/Sources/JSONS/places.json";
    places = parsePlaces(filePath);
    entities = parseEntities("D:/BSUIR/OAIP/CourseWorkGitHub/Dnd/Sources/JSONS/entities.json");

    currentPlace = places["Распутье"];

    connect(combat, &Combat::fightEnd, this, &World::handleFightEnd);
}

World::World() = default;

Item *World::getItem(int id)
{
    return items[id];
}

void World::goToPlace(QString place)
{
    bool isNumber = false;
    bool isExists = false;
    int var = place.toInt(&isNumber);
    if (isNumber && var > 0 && var <= currentPlace->exits.count()) {
        isExists = true;
    }
    if (isExists) {
        place = currentPlace->exits[var - 1];
        if (places[place]->lvl <= player->getLvl()) {
            if (places[place]->type == ENVIRONMENT) {
                //какое-то случайное событие
            }
            currentPlace = places[place];
            emit sendText(currentPlace->description + "\n", Qt::black);
            QString info;
            if (currentPlace->type != ENVIRONMENT) {
                info.append("Список NPC с которыми можно поговорить:\n");
                for (int i = 0; i < currentPlace->npcs.size(); i++) {
                    if (!defeated[currentPlace->npcs[i]]) {
                        info.append(QString::number(i + 1) + " " + currentPlace->npcs[i] + "\n");
                    }
                }
            }
            emit sendText(info + "\n", Qt::black);
        } else {
            emit sendText("У вас недостаточно уровней, чтобы добраться до этого места\n",
                          Qt::darkYellow);
        }
    } else {
        emit sendText("Ведите пожалуйста число соответствующее месту\n", Qt::red);
    }
}

void World::talkToNPC(QString npc)
{
    bool isNumber = false;
    bool isExists = false;
    int var = npc.toInt(&isNumber);
    if (isNumber && var > 0 && var <= currentPlace->npcs.count()) {
        isExists = true;
    }
    if (isExists) {
        npc = currentPlace->npcs[var - 1];
        if (!defeated[npc]) {
            state = DIALOG;
            currentEntity = QString::number(var - 1);
            dialog = parseDialog("D:/BSUIR/OAIP/CourseWorkGitHub/Dnd/Sources/JSONS/"
                                 + entities[currentPlace->npcs[var - 1]]->getRace() + ".json");
            startDialog();
        } else {
            emit sendText(npc + " больше нет с нами\n", Qt::black);
        }
    } else {
        emit sendText("Ведите пожалуйста число соответствующее НПС\n", Qt::red);
    }
}

void World::fightToNPC(QString npc)
{
    bool isNumber = false;
    bool isExists = false;
    int var = npc.toInt(&isNumber);
    if (isNumber && var > 0 && var <= currentPlace->npcs.count()) {
        isExists = true;
    }
    if (isExists) {
        npc = currentPlace->npcs[var - 1];
        if (!defeated[npc]) {
            combat->start(entities[npc]);
            state = FIGHT;
        } else {
            emit sendText(npc + " больше нет с нами\n", Qt::black);
        }
    } else {
        emit sendText("Такого NPC не существует или вы не можете до него добраться\n", Qt::red);
    }
}

void World::startDialog()
{
    findPossibleWays();
    dialog.nodes[0].was = true;
    emit sendText(currentPlace->npcs[currentEntity.toInt()] + ":\n", Qt::darkBlue);
    emit sendText(dialog.nodes[0].dialog + "\n", Qt::black);
    for (const int var : possibleWays) {
        emit sendText(QString::number(var) + " " + dialog.nodes[var].option + "\n", Qt::black);
    }
}

void World::findPossibleWays()
{
    possibleWays.clear();
    checkWay(0);
}

void World::checkWay(int id)
{
    for (const int &node : dialog.nodes[id].options) {
        if (dialog.nodes[node].was) {
            checkWay(node);
        } else if (!possibleWays.contains(node)) {
            possibleWays.append(node);
        }
    }
}

void World::chooseOption(QString option)
{
    bool isNumber = false;
    bool isExists = false;
    int var = option.toInt(&isNumber);
    if (isNumber && var > 0 && possibleWays.contains(var)) {
        isExists = true;
    }
    if (isExists) {
        if (!dialog.nodes[var].event) {
            dialog.nodes[var].was = true;
            emit sendText(currentPlace->npcs[currentEntity.toInt()] + ":\n", Qt::darkBlue);
            emit sendText(dialog.nodes[var].dialog + '\n', Qt::black);
            findPossibleWays();
            for (const int var : possibleWays) {
                emit sendText(QString::number(var) + " " + dialog.nodes[var].option + "\n",
                              Qt::black);
            }
        } else {
            //прописать механику ивентов
        }
    } else {
        emit sendText("Ведите пожалуйста число соответствующее выбору\n", Qt::red);
        for (const int var : possibleWays) {
            emit sendText(QString::number(var) + " " + dialog.nodes[var].option + "\n", Qt::black);
        }
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

QMap<QString, Entity *> World::parseEntities(const QString &filePath)
{
    QMap<QString, Entity *> entityMap;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Не удалось открыть файл";
        return entityMap;
    }

    QByteArray jsonData = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);

    if (!doc.isArray()) {
        qDebug() << "Корневой элемент не является массивом";
        return entityMap;
    }

    QJsonArray entityArray = doc.array();
    for (const auto &entityValue : entityArray) {
        if (!entityValue.isObject()) {
            qDebug() << "Элемент места не является объектом";
            continue;
        }

        QJsonObject placeObject = entityValue.toObject();

        Entity *entity = new Entity;
        entity->setName(placeObject.value("name").toString());
        entity->setRace(placeObject.value("race").toString());
        entity->setHealth(placeObject.value("health").toInt());
        entity->setMod(STRENGTH, placeObject.value("str").toInt());
        entity->setMod(DEXTERITY, placeObject.value("dex").toInt());
        entity->setMod(CONSTITUTION, placeObject.value("con").toInt());
        entity->setMod(INTELLEGENCE, placeObject.value("intel").toInt());
        entity->setMod(WISDOM, placeObject.value("wis").toInt());
        entity->setMod(CHARISMA, placeObject.value("cha").toInt());
        entity->setDanger(placeObject.value("danger").toInt());
        entity->setDefence(placeObject.value("defence").toInt());
        entity->setDamage(placeObject.value("damage").toInt());

        entityMap.insert(entity->getName(), entity);
        defeated.insert(entity->getName(), false);
    }

    file.close();
    return entityMap;
}

Dialog World::parseDialog(const QString &filePath)
{
    Dialog dialog;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Не удалось открыть файл";
        return dialog;
    }

    QByteArray jsonData = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);

    if (!doc.isObject()) {
        qDebug() << "Корневой элемент не является объектом";
        return dialog;
    }

    QJsonObject rootObject = doc.object();
    dialog.name = rootObject.value("name").toString();

    QJsonArray nodesArray = rootObject.value("dialog_nodes").toArray();
    for (const auto &nodeValue : nodesArray) {
        if (!nodeValue.isObject()) {
            qDebug() << "Элемент узла диалога не является объектом";
            continue;
        }

        QJsonObject nodeObject = nodeValue.toObject();

        Dialog::DialogNode dialogNode;
        dialogNode.id = nodeObject.value("id").toInt();
        dialogNode.option = nodeObject.value("option").toString();
        dialogNode.dialog = nodeObject.value("dialog").toString();
        dialogNode.event = nodeObject.value("event").toInt();

        QJsonArray optionsArray = nodeObject.value("options").toArray();
        for (const auto &optionValue : optionsArray) {
            if (optionValue.isDouble()) {
                dialogNode.options.append(optionValue.toInt());
            }
        }

        dialog.nodes.append(dialogNode);
    }

    file.close();
    return dialog;
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
            QString option = command.mid(8);
            chooseOption(option);

        } else if (command.mid(0, 6) == "/fight") {
            state = FIGHT;
            fightToNPC(QString::number(currentEntity.toInt() + 1));

        } else if (command.mid(0, 6) == "/end") {
            state = FREE;
            emit sendText("Диолог закончился", Qt::black);

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

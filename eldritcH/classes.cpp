#include "classes.h"
#include "effects.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>
#include <QFile>
#include <random>
#include <QMessageBox>
#include <QTextCodec>


inline Storage storage;                                                                       //storage
inline Deck deck;
inline QList <Player> pls;
inline Board board;

typedef void(*funccard)(Player&);
typedef void(*funcchar)();
funccard m_effects[] = {m_0};
funccard m_effects_back[] = {};
funccard b_effects[] = {};
funccard b_effects_back[] = {};
funccard a_effects[] = {a_0,a_1,a_2};
funccard art_effects[] = {};
funcchar char_specabil[] = {};
funcchar char_pasabil[] = {};


void Board::initBoard()  //лучше  все грузить  с  файла опять
{
board.graph[0]=Board::Node(0,Board::city,0,"Archam");
board.graph[1]=Board::Node(1,Board::city,-1,"1");
board.graph[2]=Board::Node(2,Board::sea,-1,"2");
board.graph[3]=Board::Node(3,Board::jungle,-1,"3");
board.graph[4]=Board::Node(4,Board::jungle,-1,"4");
board.graph[0].heighbours.append(Board::Road(Board::train,&board.graph[1]));
board.graph[0].heighbours.append(Board::Road(Board::ferry,&board.graph[2]));
board.graph[0].heighbours.append(Board::Road(Board::train,&board.graph[3]));
board.graph[1].heighbours.append(Board::Road(Board::train,&board.graph[0]));
board.graph[1].heighbours.append(Board::Road(Board::sea,&board.graph[2]));
board.graph[2].heighbours.append(Board::Road(Board::sea,&board.graph[1]));
board.graph[2].heighbours.append(Board::Road(Board::sea,&board.graph[0]));
board.graph[2].heighbours.append(Board::Road(Board::sea,&board.graph[3]));
board.graph[3].heighbours.append(Board::Road(Board::sea,&board.graph[2]));
board.graph[3].heighbours.append(Board::Road(Board::train,&board.graph[0]));
board.graph[3].heighbours.append(Board::Road(Board::dirt,&board.graph[4]));
board.graph[4].heighbours.append(Board::Road(Board::dirt,&board.graph[3]));

//загрузить резерв (отдельная функция)

}






void Storage::load()
{
    QString parsed;
    QFile file("arsenal.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    parsed = file.readAll();
    file.close();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(parsed.toUtf8(), &error);
    //qdebug?
    if (doc.isObject()){
        QJsonObject bigjson = doc.object();
        QJsonArray jsonarray = bigjson["arsenal"].toArray();
        foreach (const QJsonValue &value, jsonarray)
        {
        if (value.isObject())
        {
        QJsonObject obj = value.toObject();
        storage.data_things.push_back(Arsenal(obj["ID"].toInt(),obj["header"].toString(),obj["description"].toString()));
        }
        }
    }
    //
    file.setFileName("characters.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    parsed = file.readAll();
    file.close();
    doc = QJsonDocument::fromJson(parsed.toUtf8(), &error);
    //qdebug?
    if (doc.isObject()){
        QJsonObject bigjson = doc.object();
        QJsonArray jsonarray = bigjson["characters"].toArray();
        foreach (const QJsonValue &value, jsonarray)
        {
        if (value.isObject())
        {
        QJsonObject obj = value.toObject();
        Player chara = Player(obj["id"].toInt());
        chara.name=obj["name"].toString();
        chara.bio=obj["bio"].toString();
        chara.start_loadout=obj["start_loadout"].toString();
        chara.death_body=obj["death_body"].toString();
        chara.death_mind=obj["death_mind"].toString();
        QJsonArray arr =obj["improvements"].toArray();
        for (int i =0; i<5;i++){
            chara.improvements[i]=arr[i].toInt();
        }
        chara.health=obj["health"].toInt();
        chara.sanity=obj["sanity"].toInt();
        chara.clues=obj["clues"].toInt();
        arr =obj["chars"].toArray();
        for (int i =0; i<5;i++){
            chara.chars[i]=arr[i].toInt();
        }
        //карты выдавать уже после того как собран дек?  переделать  под  забор  с массива
        if (obj["hand_magic"].toString()!="none")
            chara.start_magic.insert(obj["hand_magic"].toInt());
        if (obj["hand_things"].toString()!="none")
            chara.start_things.insert(obj["hand_things"].toInt());
        if (obj["hand_arts"].toString()!="none")
            chara.start_arts.insert(obj["hand_arts"].toInt());
        storage.data_characters.push_back(chara);
        storage.namemap_chara[chara.name]=chara.id;
        }
        }
    }
}

void Storage::shuffle()
{
    srand(time(NULL));
    //static std::random_device rng;
    //static std::mt19937 eng(rng());
    std::random_shuffle(storage.data_things.begin(),storage.data_things.end());
    Deck::updateDeck();
}

void Deck::updateDeck()
{
    for (int i=0;i<storage.data_things.size();i++)
    {
        deck.deck_things.push(&storage.data_things[i]);
    }
}

void Deck::pullReserve()
{
    for (int i = 0;i<4;i++)
    if (deck.reserve[i].card==nullptr) {
        if (!deck.deck_things.isEmpty()) {
            deck.reserve[i].card=static_cast<Arsenal*>(deck.deck_things.pop());
        }
    }
}

void addPlayer (int charID, QString Username)
{
    if (pls.size()<8)
    {
        Player newpl = storage.data_characters[charID];
        newpl.username=Username;
        pls.append(newpl);
    }
}

void Player::takeStartCards()
{
    if (!this->start_magic.isEmpty()){
   /* QStack<Magic>::iterator itm = *deck.deck_magic.begin();
    while (itm!=*deck.deck_magic.end())
    {
        if (this->start_magic.contains(itm->id))
        {

            this->hand_magic.push_back(itm);
            int fordelete = deck.deck_magic.indexOf(itm);  //хренотень  такая, все фигня, юзаю стэк,  но  методы вектора,  в  чем  смысл(((
            deck.deck_magic.removeAt(fordelete);
        }
    }*/
    for (qsizetype i=0;i<deck.deck_magic.size();i++)
    {
        if (this->start_magic.contains(deck.deck_magic.at(i)->id))
        {
            this->hand_magic.push_back(deck.deck_magic.at(i));           //хренотень  такая, все фигня, юзаю стэк,  но  методы вектора,  в  чем  смысл(((
            deck.deck_magic.removeAt(i);
        }
    }
    }

    if (!this->start_things.isEmpty()){
        for (qsizetype i=0;i<deck.deck_things.size();i++)
        {
            if (this->start_things.contains(deck.deck_things.at(i)->id))
            {
                this->hand_things.push_back(deck.deck_things.at(i));           //хренотень  такая, все фигня, юзаю стэк,  но  методы вектора,  в  чем  смысл(((
                this->hand_things.back()->playAction(*this);
                deck.deck_things.removeAt(i);
            }
        }
    }
}

void pullStartLoadout()
{
 QList<Player>::iterator it = pls.begin();
 while (it<pls.end()){
     it->takeStartCards();
     it++;
 }
}

void Magic::playAction(Player& pl)
{
    (*m_effects[this->id])(pl);
}
void Magic::turnAround()
{
    //(*m_effects_back[this->id])();
}
void BadStateCard::playAction(Player& pl)
{
    (*b_effects[this->id])(pl);
}
void BadStateCard::turnAround()
{
    //(*b_effects_back[this->id])();
}
void Arsenal::playAction(Player& pl)
{
    (*a_effects[this->id])(pl);
}
void Artifacts::playAction(Player& pl)
{
    (*art_effects[this->id])(pl);
}

void Character::specialAbility()
{
    (*char_specabil[this->id])();
}

void Character::passiveAbility()
{
    (*char_pasabil[this->id])();
}

void Player::modifyStats(int Type, int value)
{
    this->modifications[Type]+=value;
}

void Player::throwDices(int Type)
{
    this->currentCheck=0;
    //srand(time(NULL));
    int thisthrow;
    this->dices.erase(this->dices.begin(),this->dices.end());
    for (int i=0;i<this->chars[Type]+this->improvements[Type]+this->modifications[Type];i++)
    {
        thisthrow = rand()%6+1;
        this->dices.push_back(thisthrow);
        if (thisthrow>4&&thisthrow<7)
            this->currentCheck++;
    }
}

void Player::move(int Destination)
{
 this->currentLocation=&board.graph[Destination];
}

void Player::sleep()
{
    if (!currentLocation->hasMonsters)
    {
        if (sanity<maxSanity) sanity++;
        if (health<maxHealth) health++;
    }
}

void Player::goTrade() //кнопка  - бросок кубиков,  потом из резерва подсвечиваются  те на  что хватает или сброс
{
this->throwDices(Player::trade);
}

void Player::goExchange(Player &pl) //передать игрока на выбор
{

}

void Player::getTicket() //кнопка какой  взять
{

}

void Player::takeSCard(int Type)
{
    switch(Type)
    {
    case 0:{
        if(!deck.deck_magic.isEmpty())
        {
        this->hand_magic.push_back(deck.deck_magic.pop());
        //this->hand_magic.back()->playAction(*this);
        }
        break;
    }
    case 1:{
        if(!deck.deck_bads.isEmpty())
        {
        this->hand_bads.push_back(deck.deck_bads.pop());
        this->hand_bads.back()->playAction(*this);
        }
        break;
    }
    case 2:{
        if(!deck.deck_things.isEmpty())
        {
        this->hand_things.push_back(deck.deck_things.pop());
        this->hand_things.back()->playAction(*this);
        }
        break;
    }
   case 3:{
        if(!deck.deck_arts.isEmpty())
        {
        this->hand_arts.push_back(deck.deck_arts.pop());
        this->hand_arts.back()->playAction(*this);
        }
        break;
    }
}
}

QString bool2string (bool bol)
{
    if (bol) return "true";
    return "false";
}

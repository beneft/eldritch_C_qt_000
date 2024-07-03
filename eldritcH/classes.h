#ifndef CLASSES_H
#define CLASSES_H

#include <QStack>
#include <QVector>
#include <QMap>
#include <QSet>

                                                    //GAME
class CardsStorage;
class Arsenal;
class Player;
class Character;

QString bool2string (bool bol);

class Game{
public:

};


class Board{
public:
struct Node;
struct Road{    
     Node* where;
     int how;
     Road(int Type, Node* Destination){
         how = Type;
         where=Destination;
     }
};
struct Node {
    int id;
    int surface;
    int cityid=-1;
    int gatetype=-1;
    QString name;
    QList <Road> heighbours;
    QList <int*> monsters; //изменить тип
    bool hasMonsters = false;
    bool isNamedCity = false;
    bool hasExpedition = false;
    bool hasClue = false;
    bool hasRumor = false;
    bool hasMainObjective = false;
    bool hasTentacles = false;
    bool hasGates = false;
    Node (){};
    Node (int ID, int Surface, int cityID, QString Namefordepiction)
    {
        id=ID;
        surface=Surface;
        cityid=cityID;
        name=Namefordepiction;
    };
    QString gatherText()
    {
        QString surftext;
        switch (surface){
        case (Board::city) :{surftext="city";break;}
        case (Board::sea) :{surftext="sea";break;}
        case (Board::jungle) :{surftext="jungle";break;}
        }
        QString text = "surface: " + surftext + "\nhas monsters " + bool2string(hasMonsters) + "\nhas expedition " + bool2string((hasExpedition));
        text+= "\nhas clue " + bool2string(hasClue) + "\nhas rumor " + bool2string(hasRumor) + "\nhas main objective " + bool2string(hasMainObjective);
        text+= "\nhas texntacles " +  bool2string(hasTentacles) + "\nhas gates " + bool2string(hasGates);
        return text;
    }
};
Node graph[5];
public:
enum {city,jungle,sea};
enum {dirt,train,ferry};
Board(){};
static void initBoard();
};

                                                    //SMALLCARDS
class Card{
public:
//protected:
    QString header;
    //QString description;
    QString desc_back  = nullptr;
    int id;
    bool isAction;
//public:
    QString description;
    //enum {magic,bads,things,arts};
    Card(){};
    Card(int ID,QString head,QString desc){
        id=ID;
        header=head;
        description=desc;
    };
    Card(int ID, QString head,QString desc,QString descb):Card(ID,head,desc){
        desc_back=descb;
    };
    virtual void playAction(Player&)=0;
    virtual void turnAround(){throw "You called an undefined method (Card)!";};
    QString gatherText ()
    {
        QString text = header + "\n" + description;
        if (desc_back != nullptr) text+="\n" + desc_back;
        return text;
    }
};
class Magic:public Card{
public:
    Magic(){};
    Magic(int ID,QString head,QString desc,QString descb):Card(ID,head,desc,descb)  {}
    void playAction(Player&) final;
    void turnAround() final;
};
class BadStateCard:public Card{
public:
    //enum{legs,spine,inner,etc};
    //int b_type=-1;
    BadStateCard(){};
    BadStateCard(int ID,QString head,QString desc,QString descb):Card(ID,head,desc,descb)    {};
    void playAction(Player&) final;
    void turnAround() final;
};
class Artifacts:public Card{
public:
    Artifacts(){};
    Artifacts(int ID,QString head,QString desc):Card(ID,head,desc)      //TODO добавить кост карточек
    {
    };
    void playAction(Player&) final;
};
class Arsenal:public Card{
public:
    Arsenal(){};
    Arsenal(int ID,QString head,QString desc):Card(ID,head,desc)
    {
        id=ID;
    };
    void playAction(Player&) final;
};


                                                    //CONTACTS
/*class Contact{
protected:
    QString description;
    QString description_second=nullptr;
    QString description_third=nullptr;
    int id;
public:
    virtual void playContact();
};
class TripleContact:public Contact{
private:
    bool isWorldwide;
public:
    enum{deflt,city,};
    void playContact() final;
};
class TwoWaysContact:public Contact{
private:
    bool isGoodFate;
public:
    void doFate();
    void playContact() final;
};
class Event{

};*/

                                                    //OTHER
class Storage{
public:
QVector <Magic> data_magic;
QVector <BadStateCard> data_bads;
QVector <Arsenal> data_things;
QVector <Artifacts> data_arts;
/*QVector <TripleContact> data_c_default;
QVector <TripleContact> data_c_city_red;
QVector <TripleContact> data_c_city_purp;
QVector <TripleContact> data_c_city_green;
QVector <TripleContact> data_c_search;
QVector <TwoWaysContact> data_c_gates;
QVector <TwoWaysContact> data_c_arts;
QVector <Contact> data_c_elder;
QVector <Event> data_c_mistery;*/
QVector <Player> data_characters;
QMap <QString,int> namemap_chara;
static void shuffle();
static void load();
};

class Deck{
public:
    enum {m,b,t,a,md,bd,td,ad};
QStack <Card*> deck_magic;
QStack <Card*> deck_bads;
QStack <Card*> deck_things;
QStack <Card*> deck_arts;
QStack <Card*> deck_magic_dead;
QStack <Card*> deck_bads_dead;
QStack <Card*> deck_things_dead;
QStack <Card*> deck_arts_dead;
//QMap <int,QStack<Card*>> map = {{m,deck_magic},{b,deck_bads},{t,deck_things},{a,deck_arts},{md,deck_magic_dead},{bd,deck_bads_dead},{td,deck_things_dead},{ad,deck_arts_dead}};
/*QStack <TripleContact*> c_default;
QStack <TripleContact*> c_city_red;
QStack <TripleContact*> c_city_purp;
QStack <TripleContact*> c_city_green;
QStack <TripleContact*> c_search;
QStack <TwoWaysContact*> c_gates;
QStack <TwoWaysContact*> c_arts;
QStack <Contact*> c_elder;
QStack <Event*> c_mistery;*/
struct reserve_slot{
    Arsenal* card = nullptr;
    bool canBeBought=false;
    reserve_slot(Arsenal* Card)
    {
        card = Card;
    }
    reserve_slot(){};
};
reserve_slot reserve[4];
static void pullReserve();
static void updateDeck();
};

                                                //CHARACTER
class Character{
//private:
public:
    int id;
    QString name;
    QString bio;
    QString start_loadout;
    QString death_body;
    QString death_mind;
    int improvements[5]={0,0,0,0,0};
    int extrathrows[5]={0,0,0,0,0};
    int modifications[5]={0,0,0,0,0};
    int health;
    int sanity;
    int maxHealth;
    int maxSanity;
    /*short book;
    short trade;
    short eyes;
    short strength;
    short will;*/
    Board::Node* currentLocation;
    int chars[5]={0,0,0,0,0};
    int clues=0;
    int tickets[2] = {none,none};
    bool isDead =false;
    bool isArrested = false;
    bool isOutOfHere = false;
    bool isCursed =  false;
    bool isBlessed = false;
//public:
    enum {none,ferry,train};
    QSet<int> start_magic;
    QSet<int> start_things;
    QSet<int> start_arts;
    Character(){};
    Character(int ID){id=ID;}
    void specialAbility();
    void passiveAbility();
};
class Player: public Character{
public:
//private:
    int currentCheck;
    QVector <int> dices;
    bool availableContacts[8]={0,0,0,0,0,0,0,0};
//public:
    enum skills {book,trade,eyes,strength,will};
    enum scardtype {m,b,t,a};
    enum subtypes {any,thing,shiny,weapon,ally,books,ritual,aura,trauma,madness,illness};
    Player(){};
    Player(int ID):Character(ID){};
    QString username;
    QVector <Card*> hand_magic;
    QVector <Card*> hand_bads;
    QVector <Card*> hand_things;
    QVector <Card*> hand_arts;
    void throwDices(int Type);
    void pullContact();
    void modifyStats(int Type,int value);
    void move(int nodeID);
    void goTrade();
    void takeStartCards();
    void takeSCard(int Type);
    void takeSCard(int Type, int subtype);
    void destroySCard (int Type);
    void destroySCard (int Type, int subtype);
    void sleep();
    void goExchange(Player&);
    void getTicket();
};
void addPlayer (int charID, QString Username);
void pullStartLoadout ();




#endif // CLASSES_H


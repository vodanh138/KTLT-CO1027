#ifndef _KNIGHT2_H_
#define _KNIGHT2_H_

#include "main.h"

// #define DEBUG

enum ItemType {/* TODO: */Antidote = 0, PhoenixDownI, PhoenixDownII, PhoenixDownIII, PhoenixDownIV };
enum KnightType { PALADIN = 0, LANCELOT, DRAGON, NORMAL };
enum enemy { MadBear = 0, Bandit, LordLupin, Elf, Troll, Tornbery, QueenOfCards, NinaDeRings, DurianGarden, OmegaWeapon, Hades };
class BaseItem;
class Events;
class BaseKnight;
class BaseBag {
    BaseKnight* knight;
    int maxsize;
    int num = 0;
public:
    BaseItem* bag = nullptr;
    BaseBag(BaseKnight*, int, int);
    virtual bool insertFirst(BaseItem*);
    virtual BaseItem* get(ItemType);
    virtual string toString() const;
    friend class KnightAdventure;
};



class BaseKnight {
protected:
    int id;
    int hp;
    int maxhp;
    int level;
    int gil;
    int antidote;
    BaseBag* bag;
    KnightType knightType;
    bool poison;
public:
    static BaseKnight* create(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI);
    string toString() const;
    friend class KnightAdventure;
    friend class ArmyKnights;
    friend class BaseBag;
    friend class antidote;
    friend class phoenixDownI;
    friend class phoenixDownII;
    friend class phoenixDownIII;
    friend class phoenixDownIV;
};

class BaseOpponent {
public:
    int level;
    enemy name;
    int gil;
    int dam;
    friend class ArmyKnights;
};
class MadBear :public BaseOpponent {
public:

    MadBear(int a) {
        this->level = a;
        name = enemy::MadBear;
        this->gil = 100;
        this->dam = 10;
    }
};
class Bandit :public BaseOpponent {
public:


    Bandit(int a) {
        this->gil = 150;
        this->dam = 15;
        level = a;
        name = enemy::Bandit;
    }
};
class LordLupin :public BaseOpponent {
public:


    LordLupin(int a) {
        this->gil = 450;
        this->dam = 45;
        level = a;
        this->name = enemy::LordLupin;
    }
};
class Elf :public BaseOpponent {
public:


    Elf(int a) {
        this->gil = 750;
        this->dam = 75;
        level = a;
        name = enemy::Elf;
    }
};
class Troll :public BaseOpponent {
public:


    Troll(int a) {
        this->gil = 800;
        this->dam = 95;
        level = a;
        name = enemy::Troll;
    }
};
class  Tornbery :public BaseOpponent {
public:

    Tornbery(int a) {
        level = a;
        name = enemy::Tornbery;
    }
};
class  QueenofCards :public BaseOpponent {
public:

    QueenofCards(int a) {
        level = a;
        name = enemy::QueenOfCards;
    }
};
class  NinadeRings :public BaseOpponent {
public:

    NinadeRings() {
        name = enemy::NinaDeRings;
    }
};
class  DurianGarden :public BaseOpponent {
public:

    DurianGarden() {
        name = enemy::DurianGarden;
    }
};
class  OmegaWeapon :public BaseOpponent {
public:

    OmegaWeapon() {
        name = enemy::OmegaWeapon;
        level = 10;
    }
};
class   Hades :public BaseOpponent {
public:

    Hades() {
        name = enemy::Hades;
        level = 10;
    }
};


class ArmyKnights {
    int num = 0;
    BaseKnight* baseknight = nullptr;
    bool shield = false;
    bool spear = false;
    bool hair = false;
    bool sword = false;
public:
    ArmyKnights(const string& file_armyknights);
    ~ArmyKnights();
    bool fight(BaseOpponent* opponent);
    bool adventure(Events* events);
    int count() const;
    BaseKnight* lastKnight() const;
    bool hasPaladinShield() const;
    bool hasLancelotSpear() const;
    bool hasGuinevereHair() const;
    bool hasExcaliburSword() const;
    void printInfo() const;
    void printResult(bool win) const;
    friend class KnightAdventure;
    void gaingil(int gil) {
        if (lastKnight()->gil + gil > 999)
        {
            lastKnight()->gil = 999;
            gil = lastKnight()->gil + gil - 999;
            if (num >= 1)
            {
                num--;
                gaingil(gil);
                num++;
            }
        }
        else
            lastKnight()->gil += gil;
    }
};





class BaseItem {
    ItemType item;
    BaseItem* next;
public:
    BaseItem() {
        this->item = Antidote;
        this->next = nullptr;
    };
    BaseItem(ItemType item) {
        this->item = item;
        this->next = nullptr;
    }
    virtual bool canUse(BaseKnight* knight) = 0;
    virtual void use(BaseKnight* knight) = 0;
    friend class BaseBag;
    friend class ArmyKnights;
    friend class KnightAdventure;
};
class antidote :public BaseItem {
public:
    const ItemType item = Antidote;
    BaseItem* next;
    bool canUse(BaseKnight* knight) {
        return false;
    }
    void use(BaseKnight* knight) {
        knight->antidote--;
    }
};
class phoenixDownI :public BaseItem {
public:
    bool canUse(BaseKnight* knight) {
        if (knight->hp <= 0)
            return true;
        return false;
    }
    void use(BaseKnight* knight) {
        knight->hp = knight->maxhp;
    }
};
class phoenixDownII :public BaseItem {
public:
    bool canUse(BaseKnight* knight) {
        if (knight->hp < knight->maxhp / 4)
            return true;
        return false;
    };
    void use(BaseKnight* knight) {
        knight->hp = knight->maxhp;
    };
};

class phoenixDownIII :public BaseItem {
public:
    bool canUse(BaseKnight* knight) {
        if (knight->hp < knight->maxhp / 3)
            return true;
        return false;
    };
    void use(BaseKnight* knight) {
        if (knight->hp <= 0)
            knight->hp = knight->maxhp / 3;
        else
            knight->hp = knight->hp + knight->maxhp / 4;
    };
};

class phoenixDownIV :public BaseItem {
public:
    bool canUse(BaseKnight* knight) {
        if (knight->hp < knight->maxhp / 2)
            return true;
    };
    void use(BaseKnight* knight) {
        if (knight->hp <= 0)
            knight->hp = knight->maxhp / 2;
        else
            knight->hp = knight->hp + knight->maxhp / 5;
    };
};







class Events {
    int* event;
    int numev;
public:
    Events(string file) {
        string input;
        ifstream in(file);
        getline(in, input);
        int numev = 0;
        for (int j = 0; input[j]; j++)
        {
            numev = numev * 10 + input[j] - '0';
        }
        int* arr = new int[numev];
        this->numev = numev;
        getline(in, input);
        int index = 0;
        int eventnum = -1;
        int event = 0;
        while (input[index])
        {
            if (input[index] == ' ')
            {
                eventnum++;
                arr[eventnum] = event;
                event = 0;
            }
            else if (input[index] >= '0' && input[index] <= '9')
            {
                event = event * 10 + input[index] - '0';
            }
            index++;
        }
        eventnum++;
        arr[eventnum] = event;
        this->event = arr;
    }
    ~Events() {
        delete[] event;
    }
    int count() const {
        return this->numev;
    };
    int get(int i) const {
        return this->event[i];
    };
    friend class KnightAdventure;
};

class KnightAdventure {
private:
    ArmyKnights* armyKnights;
    Events* events;
    int* ev;
    int num;
public:
    KnightAdventure();
    ~KnightAdventure(); // TODO:
    void loadArmyKnights(const string&);
    void loadEvents(const string&);
    void run();
};

#endif // _KNIGHT2_H_
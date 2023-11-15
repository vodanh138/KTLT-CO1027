#include "knight2.h"

/* * * BEGIN implementation of class BaseBag * * */
BaseBag::BaseBag(BaseKnight* knight, int pnx, int anti) {
	this->knight = knight;
	KnightType k = this->knight->knightType;
	if (k == 1)
		maxsize = 16;
	else if (k == 2)
		maxsize = 14;
	else if (k == 3)
		maxsize = 19;
	else
		maxsize = -1;
	for (int i = 0; i < pnx; i++)
	{
		BaseItem* phoenix = new phoenixDownI();
		phoenix->item = PhoenixDownI;
		insertFirst(phoenix);
	}
	if (k == DRAGON)
		return;
	for (int i = 0; i < anti; i++)
	{
		BaseItem* anti = new antidote();
		anti->item = Antidote;
		insertFirst(anti);
	}
}
bool BaseBag::insertFirst(BaseItem* Item) {
	if (maxsize == -1 || num < maxsize)
	{
		Item->next = bag;
		bag = Item;
		num++;
		return true;
	}
	return false;
}
BaseItem* BaseBag::get(ItemType itemType) {
	BaseItem* walker = bag;
	if (walker == nullptr)
		return nullptr;
	if (walker->item == itemType)
	{
		bag = bag->next;
		num--;
		return walker;
	}
	else if (walker->next != nullptr)
	{
		while (walker != nullptr)
		{
			if (walker->item == itemType)
			{
				ItemType tmp = walker->item;
				walker->item = bag->item;
				bag->item = tmp;
				walker = bag;
				bag = bag->next;
				num--;
				return walker;
			}
			walker = walker->next;
		}
		return walker;
	}
	return walker;
}
string BaseBag::toString() const {
	string s = "Bag[count=";
	s += to_string(num) + ";";
	BaseItem* walker = bag;
	while (walker != nullptr)
	{
		if (walker->item == 0)
			s += "Antidote";
		else if (walker->item == 1)
			s += " PhoenixI";
		else if (walker->item == 2)
			s += " PhoenixII";
		else if (walker->item == 3)
			s += " PhoenixIII";
		else if (walker->item == 4)
			s += " PhoenixIV";
		if (walker->next != nullptr)
			s += ",";
		walker = walker->next;
	}
	s += "]";
	return s;
}
/* * * END implementation of class BaseBag * * */

/* * * BEGIN implementation of class BaseKnight * * */
string BaseKnight::toString() const {
	string typeString[4] = { "PALADIN", "LANCELOT", "DRAGON", "NORMAL" };
	// inefficient version, students can change these code
	//      but the format output must be the same
	string s("");
	s += "[Knight:id:" + to_string(id)
		+ ",hp:" + to_string(hp)
		+ ",maxhp:" + to_string(maxhp)
		+ ",level:" + to_string(level)
		+ ",gil:" + to_string(gil)
		+ "," + bag->toString()
		+ ",knight_type:" + typeString[knightType]
		+ "]";
	return s;
}
BaseKnight* BaseKnight::create(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI) {
	BaseKnight* newknight = new BaseKnight;
	newknight->id = id;
	newknight->hp = maxhp;
	newknight->maxhp = maxhp;
	newknight->level = level;
	newknight->gil = gil;
	newknight->antidote = antidote;
	newknight->poison = false;
	if (maxhp == 2 || maxhp == 3)
		newknight->knightType = PALADIN;
	else if (maxhp == 888)
		newknight->knightType = LANCELOT;
	else if (maxhp == 345 || maxhp == 354 || maxhp == 435 || maxhp == 453 || maxhp == 543 || maxhp == 534)
	{
		newknight->knightType = DRAGON;
	}
	else
		newknight->knightType = NORMAL;
	for (int m = 2; m <= sqrt(maxhp); m++)
	{
		if (maxhp % m == 0)
			break;
		if (m + 1 > sqrt(maxhp))
			newknight->knightType = PALADIN;
	}
	BaseBag* bag = new BaseBag(newknight, phoenixdownI, antidote);
	newknight->bag = bag;
	return newknight;
}
/* * * END implementation of class BaseKnight * * */

/* * * BEGIN implementation of class ArmyKnights * * */
int ArmyKnights::count() const {
	return num;
}
bool ArmyKnights::hasPaladinShield() const {
	return shield;
}
bool ArmyKnights::hasLancelotSpear() const {
	return spear;
}
bool ArmyKnights::hasGuinevereHair() const {
	return hair;
}
bool ArmyKnights::hasExcaliburSword() const {
	return sword;
}


bool ArmyKnights::adventure(Events* events) {
	for (int i = 0; i < events->count(); i++)
	{
		cout << events->get(i) << endl;
	}
	return true;
}


void ArmyKnights::printInfo() const {
	cout << "No. knights: " << this->count();
	if (this->count() > 0) {
		BaseKnight* lknight = lastKnight(); // last knight
		cout << ";" << lknight->toString();
	}
	cout << ";PaladinShield:" << this->hasPaladinShield()
		<< ";LancelotSpear:" << this->hasLancelotSpear()
		<< ";GuinevereHair:" << this->hasGuinevereHair()
		<< ";ExcaliburSword:" << this->hasExcaliburSword()
		<< endl
		<< string(50, '-') << endl;
}

void ArmyKnights::printResult(bool win) const {
	cout << (win ? "WIN" : "LOSE") << endl;
}

ArmyKnights::ArmyKnights(const string& file_armyknights) {
	string input;
	ifstream in(file_armyknights);
	getline(in, input);
	int numknight = 0;
	for (int j = 0; input[j]; j++)
	{
		numknight = numknight * 10 + input[j] - '0';
	}
	this->num = numknight;
	static BaseKnight* knight = new BaseKnight[numknight];
	for (int j = 0; j < numknight; j++)
	{
		getline(in, input);
		int i = 0;
		int HP = 0;
		int level = 0;
		int gil = 0;
		int antidote = 0;
		int pnx = 0;
		while (input[i] && input[i] != ' ')
		{
			int x = input[i] - '0';
			HP = HP * 10 + x;
			i++;
		}
		i++;
		HP = min(HP, 999);
		while (input[i] && input[i] != ' ')
		{
			int x = input[i] - '0';
			level = level * 10 + x;
			i++;
		}
		i++;
		level = min(level, 10);
		while (input[i] && input[i] != ' ')
		{
			int x = input[i] - '0';
			pnx = pnx * 10 + x;
			i++;
		}
		i++;
		pnx = min(pnx, 5);
		while (input[i] && input[i] != ' ')
		{
			int x = input[i] - '0';
			gil = gil * 10 + x;
			i++;
		}
		i++;
		gil = min(gil, 999);
		while (input[i] && input[i] != ' ')
		{
			int x = input[i] - '0';
			antidote = antidote * 10 + x;
			i++;
		}
		antidote = min(antidote, 5);
		*(knight + j) = *(BaseKnight::create(j + 1, HP, level, gil, antidote, pnx));
		this->baseknight = knight;
	}
}
ArmyKnights::~ArmyKnights() {
}
BaseKnight* ArmyKnights::lastKnight() const {
	if (num == 0)
		return NULL;
	return (baseknight + num - 1);
}
bool ArmyKnights::fight(BaseOpponent* opponent) {
	if (opponent->name >= 0 || opponent->name <= 6)
	{
		if (lastKnight()->level >= opponent->level)
			return true;
		else
			return false;
	}
	else if (opponent->name == 7)
	{
		if (lastKnight()->gil >= 50 && lastKnight()->hp < lastKnight()->maxhp / 3)
			return true;
		return false;
	}
	else if (opponent->name == 8)
		return true;
	else if (opponent->name == 9)
	{
		if (lastKnight()->level == 10 && lastKnight()->hp == lastKnight()->maxhp || lastKnight()->knightType == DRAGON)
			return true;
		else
			return false;
	}
	else if (opponent->name == 10)
	{
		if (lastKnight()->level == 10 || lastKnight()->knightType == PALADIN && lastKnight()->level >= 8)
			return true;
		else
			return false;
	}
	return false;

}

/* * * END implementation of class ArmyKnights * * */

/* * * BEGIN implementation of class KnightAdventure * * */

/**************************************************************************************************************************/
KnightAdventure::KnightAdventure() {
	this->armyKnights = nullptr;
	this->events = nullptr;
	this->ev = nullptr;
	this->num = 0;
}
KnightAdventure::~KnightAdventure() {
	delete[] this->ev;
}
void KnightAdventure::loadArmyKnights(const string& file)
{
	ArmyKnights* armyKnights = new ArmyKnights(file);
	this->armyKnights = armyKnights;
}

void KnightAdventure::loadEvents(const string& file)
{
	Events* events = new Events(file);
	this->events = events;
	this->num = this->events->count();
	int* arr = new int[num];
	for (int i = 0; i < num; i++)
	{
		arr[i] = this->events->get(i);
	}
	this->ev = arr;
}
void KnightAdventure::run()
{
	int uhp = 5000;
	bool u = false;
	bool ten = false;
	bool eleven = false;
	BaseItem* phoenix2 = new phoenixDownII();
	phoenix2->item = PhoenixDownII;
	BaseItem* phoenix3 = new phoenixDownIII();
	phoenix3->item = PhoenixDownIII;
	BaseItem* phoenix4 = new phoenixDownIV();
	phoenix4->item = PhoenixDownIV;
	for (int i = 0; i < this->num; i++)
	{
		if (ev[i] == 10 && !ten)
		{
			BaseOpponent* op = new class::OmegaWeapon();
			if (this->armyKnights->fight(op))
			{
				this->armyKnights->lastKnight()->level = 10;
				this->armyKnights->lastKnight()->gil = 999;
			}
			else
				this->armyKnights->lastKnight()->hp = 0;
			ten = true;
		}
		if (ev[i] == 11 && !eleven)
		{
			BaseOpponent* op = new class ::Hades();
			if (this->armyKnights->fight(op) && !this->armyKnights->hasPaladinShield())
			{
				this->armyKnights->shield == true;
			}
			else
				this->armyKnights->lastKnight()->hp = 0;
			eleven = true;
		}
		if (ev[i] == 99)
		{
			u = true;
			if (this->armyKnights->hasExcaliburSword())
				uhp = 0;
			if (this->armyKnights->hasGuinevereHair() &&
				this->armyKnights->hasLancelotSpear() &&
				this->armyKnights->hasPaladinShield())
			{
				for (int i = this->armyKnights->num - 1; i >= 0; i--)
				{
					if (this->armyKnights->baseknight[i].knightType != NORMAL)
					{
						int dam;
						if (this->armyKnights->baseknight[i].knightType == LANCELOT)
							dam = this->armyKnights->baseknight[i].hp * this->armyKnights->baseknight[i].level * 0.05;
						if (this->armyKnights->baseknight[i].knightType == PALADIN)
							dam = this->armyKnights->baseknight[i].hp * this->armyKnights->baseknight[i].level * 0.06;
						if (this->armyKnights->baseknight[i].knightType == DRAGON)
							dam = this->armyKnights->baseknight[i].hp * this->armyKnights->baseknight[i].level * 0.075;
						uhp -= dam;
						if (uhp > 0)
						{
							for (int j = i; j < this->armyKnights->num - 1; j++)
							{
								this->armyKnights->baseknight[j] = this->armyKnights->baseknight[j + 1];
							}
							this->armyKnights->num--;
						}
						else
							break;
					}
				}
			}
		}
		switch (this->ev[i])
		{
			int x;
			BaseOpponent* op;
		case 1:
			x = (i + ev[i]) % 10 + 1;
			op = new class::MadBear(x);
			if (this->armyKnights->fight(op))
				this->armyKnights->gaingil(op->gil);
			else
				this->armyKnights->lastKnight()->hp -= (op->dam * (-this->armyKnights->lastKnight()->level + op->level));
			break;
		case 2:
			x = (i + ev[i]) % 10 + 1;
			op = new class::Bandit(x);
			if (this->armyKnights->fight(op))
				this->armyKnights->gaingil(op->gil);
			else
				this->armyKnights->lastKnight()->hp -= (op->dam * (-this->armyKnights->lastKnight()->level + op->level));
			break;
		case 3:
			x = (i + ev[i]) % 10 + 1;
			op = new class::LordLupin(x);
			if (this->armyKnights->fight(op))
				this->armyKnights->gaingil(op->gil);
			else
				this->armyKnights->lastKnight()->hp -= (op->dam * (-this->armyKnights->lastKnight()->level + op->level));
			break;
		case 4:
			x = (i + ev[i]) % 10 + 1;
			op = new class::Elf(x);
			if (this->armyKnights->fight(op))
				this->armyKnights->gaingil(op->gil);
			else
				this->armyKnights->lastKnight()->hp -= (op->dam * (-this->armyKnights->lastKnight()->level + op->level));
			break;
		case 5:
			x = (i + ev[i]) % 10 + 1;
			op = new class::Troll(x);
			if (this->armyKnights->fight(op))
				this->armyKnights->gaingil(op->gil);
			else
				this->armyKnights->lastKnight()->hp -= (op->dam * (-this->armyKnights->lastKnight()->level + op->level));
			break;
		case 6:
			x = (i + ev[i]) % 10 + 1;
			op = new class::Tornbery(x);
			if (this->armyKnights->fight(op))
				this->armyKnights->lastKnight()->level = min(10, this->armyKnights->lastKnight()->level + 1);
			else
			{
				BaseItem* y = this->armyKnights->lastKnight()->bag->get(Antidote);
				if (y)
					y->use(this->armyKnights->lastKnight());
				else if (this->armyKnights->lastKnight()->poison == false && this->armyKnights->lastKnight()->knightType != DRAGON)
				{
					this->armyKnights->lastKnight()->poison = true;
					if (this->armyKnights->lastKnight()->bag->num <= 3)
					{
						this->armyKnights->lastKnight()->bag->num == 0;
						this->armyKnights->lastKnight()->bag = nullptr;
					}
					else
					{
						this->armyKnights->lastKnight()->bag->bag = this->armyKnights->lastKnight()->bag->bag->next->next->next;
						this->armyKnights->lastKnight()->bag->num -= 3;
					}
				}
			}
			break;
		case 7:
			x = (i + ev[i]) % 10 + 1;
			op = new class::QueenofCards(x);
			if (this->armyKnights->fight(op))
				this->armyKnights->gaingil(this->armyKnights->lastKnight()->gil);
			else
				this->armyKnights->lastKnight()->gil /= 2;
			break;
		case 8:
			op = new class::NinadeRings();
			if (this->armyKnights->fight(op))
			{
				this->armyKnights->lastKnight()->hp += this->armyKnights->lastKnight()->maxhp / 5;
				this->armyKnights->lastKnight()->gil -= 50;
			}
			break;
		case 9:
			op = new class::DurianGarden();
			if (this->armyKnights->fight(op))
			{
				this->armyKnights->lastKnight()->hp = this->armyKnights->lastKnight()->maxhp;
			}
			break;
		case 95:
			if (!this->armyKnights->hasPaladinShield())
				this->armyKnights->shield = true;
			break;
		case 96:
			if (!this->armyKnights->hasLancelotSpear())
				this->armyKnights->spear = true;
			break;
		case 97:
			if (!this->armyKnights->hasGuinevereHair())
				this->armyKnights->hair = true;
			break;
		case 98:
			if (!this->armyKnights->hasExcaliburSword())
				this->armyKnights->sword = true;
			break;
		case 112:
			this->armyKnights->lastKnight()->bag->insertFirst(phoenix2);
			break;
		case 113:
			this->armyKnights->lastKnight()->bag->insertFirst(phoenix3);
			break;
		case 114:
			this->armyKnights->lastKnight()->bag->insertFirst(phoenix4);
			break;
		}
		if (ev[i] >= 1 && ev[i] <= 6 || ev[i] == 10 || ev[i] == 11)
		{
			BaseItem* walker = this->armyKnights->lastKnight()->bag->bag;
			while (walker != nullptr)
			{
				if (walker->canUse(this->armyKnights->lastKnight()))
				{
					this->armyKnights->lastKnight()->bag->get(walker->item)->use(this->armyKnights->lastKnight());
					break;
				}
				walker = walker->next;
			}
			while (this->armyKnights->lastKnight()->hp <= 0)
			{
				if (walker->canUse(this->armyKnights->lastKnight()))
					this->armyKnights->lastKnight()->bag->get(walker->item)->use(this->armyKnights->lastKnight());
				walker = walker->next;
			}
			if (this->armyKnights->lastKnight()->hp <= 0 && this->armyKnights->lastKnight()->gil >= 100)
			{
				this->armyKnights->lastKnight()->gil -= 100;
				this->armyKnights->lastKnight()->hp = this->armyKnights->lastKnight()->maxhp / 2;
			}
			if (this->armyKnights->lastKnight()->hp <= 0)
				this->armyKnights->num--;
		}
		cout << this->armyKnights->num << endl;
		this->armyKnights->printInfo();
		if (u)
		{
			if (uhp <= 0)
				this->armyKnights->printResult(1);
			else
				this->armyKnights->printResult(0);
		}
	}
}
/**************************************************************************************************************************/
/* * * END implementation of class KnightAdventure * * */

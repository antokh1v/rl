#pragma once
#include <utility>
#include <iostream>
#include <memory>
#include <vector>
#include <iterator>
#include <queue>
#include "Map.h"
class Wall;
class Arrow;
class Zombie;
class Knight;
class Princess;
class Dragon;
class Aidkit;


class Arrow : public GameObject {
public:
	Arrow(std::pair<int, int> go, std::pair<int, int> pos) :GameObject() {
		this->go = go;
		this->SetPos(pos);
		SetHp(10);
		SetDefoult_Damage(10);
		SetSym('A');
	};
	void Collide(GameObject& a)override {
		a.Collide(*this);
	};

	void Collide(Knight& a)override {
		this->SetHp(0);
	}
	void Collide(Princess& a)override {
		this->SetHp(0);
	}
	void Collide(Zombie& a)override {
		this->SetHp(0);
	}
	void Collide(Dragon& a)override {
		this->SetHp(0);
	}
	void Collide(Arrow& a)override {
		this->SetHp(0);
	}
	void Collide(Wall& a)override {
		this->SetHp(0);
	}
	void Collide(AidKit& a)override {
		this->SetHp(0);
	}
	void AI() override {
		SetGoTo(go);
		if (this->GetPos().first == 0 || this->GetPos().second == 0 || this->GetPos().first == 19 || this->GetPos().second == 19) {
			this->SetHp(0);
		}
	};
private:
	std::pair<int, int> go;
};

class AidKit : public GameObject {
public:
	AidKit(int x, int y) :GameObject() {
		this->SetPos(std::pair<int,int>(x,y));
		SetHp(1);
		SetDefoult_Damage(-100);
		SetSym('H');
	};
	void Collide(GameObject& a)override {
		a.Collide(*this);
	};

	void Collide(Knight& a);
	void Collide(Princess& a)override {
		this->SetHp(0);
	}
	void Collide(Zombie& a)override {
		this->SetHp(0);
	}
	void Collide(Dragon& a)override {
		this->SetHp(0);
	}
	void Collide(Arrow& a)override {
		this->SetHp(0);
	}
	void Collide(Wall& a)override {
		this->SetHp(0);
	}
	void Collide(AidKit& a)override {
		this->SetHp(0);
	}
};

class Character: public GameObject{
 public:
	 Character(int hp, int mana, int damage, char sym, int x, int y):GameObject(hp,sym,x,y) {
		 
		 SetDamage(damage);
		 SetMana(mana);
		 
	 }
	 Character(int x, int y):GameObject(x,y) {
		 SetHp(100);
		 SetDamage(10);
		 SetMana(100);
	 };	 
	 
	 
	 
	 virtual ~Character() = default;
};

class Monster :public Character {
public:
	Monster(int hp, int mana, int damage, char sym, int x, int y):Character(hp, mana, damage, sym, x, y) {
		SetDefoult_Damage(10);
	}

	virtual void SetMap(std::shared_ptr<Map> mymaps) {
		lab = std::vector<std::vector<int>>(mymaps->GetSize(), std::vector<int>(mymaps->GetSize(), 0));
		for (int i = 0; i < mymaps->GetSize(); i++) {
			for (int j = 0; j < mymaps->GetSize(); j++) {
				if (mymaps->GetMap()[i][j] == '.') {
					lab[i][j] = -1;
				}
				else {
					lab[i][j] = -2;
				}
			}
		}
		lab[this->GetPos().first][this->GetPos().second] = 0;
		this->map = mymaps;
	}
	virtual void RefreshMap() {
		lab = std::vector<std::vector<int>>(map->GetSize(), std::vector<int>(map->GetSize(), 0));
		for (int i = 0; i < map->GetSize(); i++) {
			for (int j = 0; j < map->GetSize(); j++) {
				if (map->GetMap()[i][j] == '.' || map->GetMap()[i][j] == 'K') {
					lab[i][j] = -1;
				}
				else {
					lab[i][j] = -2;
				}
			}
		}
		lab[this->GetPos().first][this->GetPos().second] = 0;
		this->map = map;
	}
	virtual std::vector<std::vector<int>> GetMap() {
		return lab;
	}
	virtual std::shared_ptr<GameObject> GetTarget() {
		return target;
	}
	virtual void SetTarget(std::shared_ptr<GameObject> t) {
		target = t;
	}

private:
	std::vector<std::vector<int>> lab;
	std::shared_ptr<GameObject> target;
	std::shared_ptr<Map> map;
};
class Zombie :public Monster {
public:
	Zombie(int hp, int mana, int damage, char sym, int x, int y) :Monster(hp, mana, damage, sym, x, y) {

	}
	void AI() override {
		RefreshMap();

		this->SetGoTo(bfs(this->GetPos(), this->GetTarget()->GetPos()));

	}
	void Collide(GameObject& a)override {
		a.Collide(*this);
	};

	void Collide(Knight& a);
	void Collide(Princess& a)override {
		;
	}
	void Collide(Zombie& a)override {
		;
	}
	void Collide(Dragon& a)override {
		;
	}
	void Collide(Arrow& a)override {
		this->SetHp(this->GetHp() - a.GetDamage());
		a.Collide(*this);
	}
	void Collide(Wall& a)override {
		;
	}
	void Collide(AidKit& a)override {
		this->SetHp(this->GetHp() - a.GetDamage());
	}
private:
	std::pair<int, int> bfs(std::pair<int, int> start, std::pair<int, int> finish)
	{
		std::vector<std::vector<int>> mymap = this->GetMap();
		int dx[] = { 1,0,0,-1 };
		int dy[] = { 0,-1,1,0 };
		int wavevalue = 0;

		std::queue<std::pair<std::pair<int, int>, int>> que;
		que.emplace(start, wavevalue);

		while (!que.empty()) {

			auto x = que.front().first;
			wavevalue = que.front().second + 1;
			for (int i = 0; i < 4; i++) {
				auto to = std::make_pair(x.first + dx[i], x.second + dy[i]);
				if (x.first + dx[i] >= 0 && x.second + dy[i] >= 0 && x.first + dx[i] < mymap.size() && x.second + dy[i] < mymap.size()) {
					if (mymap[to.first][to.second] != -2 && mymap[to.first][to.second] == -1) {
						que.emplace(std::make_pair(to.first, to.second), wavevalue);
						mymap[to.first][to.second] = wavevalue;
					}
				}
			}
			if (que.front().first.first == finish.first && que.front().first.second == finish.second) break;
			if (!que.empty()) que.pop();

		}
		std::queue<std::pair<int, int>> q;
		std::pair<int, int> point = GetTarget()->GetPos();
		for (int j = 0; j <= 20; j++) {
			for (int i = 0; i < 4; i++) {
				auto to = std::make_pair(point.first + dx[i], point.second + dy[i]);
				if (point.first + dx[i] >= 0 && point.second + dy[i] >= 0 && point.first + dx[i] < mymap.size() && point.second + dy[i] < mymap.size()) {
					if (mymap[point.first][point.second] - mymap[to.first][to.second] == 1) {
						q.push(point);
						point = to;
					}
				}
			}
		}
		std::pair<int, int> go = std::make_pair<int, int>(0, 0);
		if (!q.empty()) go = std::make_pair<int, int>(q.back().first - this->GetPos().first, q.back().second - this->GetPos().second);
		for (int i = 0; i < 4; i++) {
			if (go.first == dx[i] && go.second == dy[i]) {
				return go;
			}
		}
		return std::make_pair<int, int>(0, 0);
	}
};
class Knight :public Character {
 public:
	Knight(int hp, int mana, int damage, char sym, int x, int y):Character(hp,mana,damage,sym,x,y){		
		SetDefoult_Damage(10);
	}	
	Knight(int x, int y) :Character(x,y) {		
		SetDefoult_Damage(10);
		SetSym('K');		
	}
	
	void Collide(GameObject& a)override {
		a.Collide(*this);
	};

	void Collide(Knight& a)override {
		;
	}
	void Collide(Princess& a);
	void Collide(Zombie& a)override {
		this->SetHp(this->GetHp() - a.GetDamage());
	}
	void Collide(Dragon& a);

	void Collide(Arrow& a)override {
		this->SetHp(this->GetHp() - a.GetDamage());
		a.Collide(*this);
	}
	void Collide(Wall& a)override {
		;
	}
	void Collide(AidKit& a)override {
		this->SetHp(this->GetHp() - a.GetDamage());
	}
};



std::pair<int, int> MakeShoot(int a ,int b) {
	int first = 0;
	int second = 0;
	if(a > 0){
		first = 1;
	}
	else if(a < 0){
		first = -1;
	}
	if (b > 0) {
		second = 1;
	}
	else if (b < 0) {
		second = -1;
	}
	return std::pair<int, int>(first, second);
}

class Dragon :public Monster {
public:
	Dragon(int hp, int mana, int damage, char sym, int x, int y) :Monster(hp, mana, damage, sym, x, y) {

	}
	void Collide(GameObject& a)override {
		a.Collide(*this);
	};
	void AI() {
		std::vector<std::vector<int>> mymap = this->GetMap();
		if (GetTarget()->GetPos().first == GetPos().first || GetTarget()->GetPos().second == GetPos().second) {
			SetShootTo(MakeShoot(-(GetPos().first - GetTarget()->GetPos().first), -(GetPos().second - GetTarget()->GetPos().second)));
			cooldown++;
			if (cooldown == 2) {
				SetShoot(true);
				cooldown = 0;
			}
			
		}
	}


	void Collide(Knight& a)override {
		this->SetHp(this->GetHp() - a.GetDamage());
	}
	void Collide(Princess& a)override {
		;
	}
	void Collide(Zombie& a)override {
		this->SetHp(this->GetHp() - a.GetDamage());
	}
	void Collide(Dragon& a)override {
		;
	}
	void Collide(Arrow& a)override {
		this->SetHp(this->GetHp() - a.GetDamage());
		a.Collide(*this);
	}
	void Collide(Wall& a)override {
		;
	}
	void Collide(AidKit& a)override {
		this->SetHp(this->GetHp() - a.GetDamage());
	}
private:
	int cooldown = 0;
};
	



class Wall :public GameObject {
public:
	Wall(int hp, char sym, int x, int y) :GameObject(hp, sym, x, y) {
		SetHp(10000);
	};
	void Collide(GameObject& a)override {
		a.Collide(*this);
	};

	void Collide(Knight& a)override {
		;
	}
	void Collide(Princess& a)override {
		;
	}
	void Collide(Zombie& a)override {
		;
	}
	void Collide(Dragon& a)override {
		;
	}
	void Collide(Arrow& a)override {
		a.Collide(*this);
	}
	void Collide(Wall& a)override {
		;
	}
	void Collide(AidKit& a)override {
		this->SetHp(this->GetHp() - a.GetDamage());
	}
};


void Zombie::Collide(Knight& a) {
	this->SetHp(this->GetHp() - a.GetDamage());
}

void AidKit::Collide(Knight& a) {
	a.SetHp(a.GetHp() + 100);
	this->SetHp(0);
}
class Mission :public GameObject {
public:
	Mission(int x, int y) :GameObject() {		
		this->SetPos((std::pair<int, int>(x, y)));
		SetHp(10);
		SetDefoult_Damage(10);
		SetSym('P');
	};
	
	bool Check() {
		return missions;
	}
	void SetTarget(bool a) {
		missions = a;
	}
private:
	bool missions = false;
};

class Princess :public Mission {
public:
	Princess(int x, int y) :Mission(x,y) {
		;
	};
	void Collide(GameObject& a)override {
		a.Collide(*this);
	};

	void Collide(Knight& a)override {
		SetTarget(true);
	}
	void Collide(Princess& a)override {
		SetTarget(true);
	}
	void Collide(Zombie& a)override {
		;
	}
	void Collide(Dragon& a)override {
		;
	}
	void Collide(Arrow& a)override {
		a.Collide(*this);
	}
	void Collide(Wall& a)override {
		;
	}
	void Collide(AidKit& a)override {
		;
	}

};

void Knight::Collide(Princess& a) {
	a.Collide(*this);
}
void Knight::Collide(Dragon& a) {
	a.Collide(*this);
}
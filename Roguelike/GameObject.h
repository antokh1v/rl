#pragma once
#include <utility>
#include <iostream>
#include <memory>
#include <vector>
#include <iterator>
#include <queue>

class Wall;
class Arrow;
class Zombie;
class Knight;
class Princess;
class Dragon;
class AidKit;

class GameObject {
public:
	GameObject() {
		hp = 1;
		max_hp = 1;
	}
	GameObject(int hp, char sym, int x, int y) {
		this->hp = hp;
		this->max_hp = hp;
		this->x = x;
		this->y = y;
		this->sym = sym;
	}
	GameObject(int x, int y) {
		hp = 100;
		max_hp = 100;
		sym = 'E';
		this->x = x;
		this->y = y;
	}
	virtual std::pair<int, int>GetPos() {
		return std::make_pair(x, y);
	}
	virtual int GetHp() {
		return hp;
	}
	virtual char GetSym() {
		return sym;
	}
	virtual void SetHp(int hp) {
		this->hp = hp;
		if (this->hp > max_hp) {
			this->hp = max_hp;
		}
	}
	virtual bool CheckHp() {
		if (hp <= 0) {
			SetSym('0');
			return false;
		}
		return true;
	}

	virtual void SetSym(char ch) {
		this->sym = ch;
	}
	virtual void SetGoTo(std::pair<int, int> move) {
		GoTo.first = move.first;
		GoTo.second = move.second;
	}
	virtual std::pair<int, int> GetGoTo() {
		return GoTo;
	}

	virtual void SetShootTo(std::pair<int, int> move) {
		ShootTo.first = move.first;
		ShootTo.second = move.second;
	}
	virtual std::pair<int, int> GetShootTo() {
		return ShootTo;
	}

	virtual void Collide( GameObject& a)  = 0;

	virtual void Collide( Knight& a)  = 0;
	virtual void Collide( Princess& a)  = 0;
	virtual void Collide( Zombie& a)  = 0;
	virtual void Collide( Dragon& a)  = 0;
	virtual void Collide( Arrow& a)  = 0;
	virtual void Collide( Wall& a)  = 0;
	virtual void Collide( AidKit& a)  = 0;

	virtual void AI() {
		;
	};
	virtual int GetDamage() {
		return damage;
	}

	virtual void SetDamage(int atc) {
		this->damage = atc;
		
	}
	virtual void SetDefoult_Damage(int atc) {
		this->damage = atc;
	}
	virtual int GetDefoult_Damage() {
		return defoult_damage;
	}
	virtual void SetMana(int mana) {
		this->mana = mana;
	}
	void SetPos(std::pair<int,int> pos) {
		x = pos.first;
		y = pos.second;
	}
	bool GetShoot() {
		return shoot;
	};
	void SetShoot(bool a) {
		shoot = a;
	};
	virtual void Cooldown() {
		;
	}
private:
	bool shoot = false;
	std::pair<int, int> GoTo = std::make_pair<int, int>(0, 0);
	std::pair<int, int> ShootTo = std::make_pair<int, int>(0, 0);
	int mana;
	int damage;
	int defoult_damage;
	int hp;
	int max_hp;
	char sym;
	int x;
	int y;
};
#pragma once
#pragma once
#include <utility>
#include <iostream>
#include <memory>
#include <vector>
#include <conio.h>
#include <iterator>
#include <queue>
#include "Character.h"
class Map;


class GameLoop {
public:
	GameLoop() {
		map = std::make_shared<Map>(Map(20));
		hero_interface = std::make_shared<InterfaceHero>(InterfaceHero());
		map->init();
	}
	void Input() {
		if (_kbhit()) {
			switch (_getch())
			{
			case 'w':
				Hero->SetGoTo(std::make_pair(-1, 0));
				break;
			case 'a':
				Hero->SetGoTo(std::make_pair(0, -1));
				break;
			case 's':
				Hero->SetGoTo(std::make_pair(1, 0));
				break;
			case 'd':
				Hero->SetGoTo(std::make_pair(0, 1));
				break;
			case 77:
				Shoot(std::make_pair(0, 1), std::make_pair<int,int>(Hero->GetPos().first , Hero->GetPos().second + 1));
				ContinueGame();
				break;
			case 75:
				Shoot(std::make_pair(0, -1), std::make_pair<int, int>(Hero->GetPos().first , Hero->GetPos().second - 1));
				ContinueGame();
				break;
			case 72:
				Shoot(std::make_pair(-1, 0), std::make_pair<int, int>(Hero->GetPos().first - 1, Hero->GetPos().second));
				ContinueGame();
				break;
			case 80:
				Shoot(std::make_pair(1, 0), std::make_pair<int, int>(Hero->GetPos().first + 1, Hero->GetPos().second));
				ContinueGame();
				break;
			}
			ContinueGame();
		}

	};
	void GoTo() {
		for (int i = 0; i < characters.size(); i++) {
			if (CheckCollide(characters[i]->GetGoTo(), characters[i])) {
				characters[i]->SetDamage(0);
				characters[i]->SetPos(std::make_pair<int, int>(characters[i]->GetPos().first + characters[i]->GetGoTo().first, characters[i]->GetPos().second + characters[i]->GetGoTo().second));
				
			}
			characters[i]->SetGoTo(std::make_pair<int, int>(0, 0));
		}

	}
	void Shoot(std::pair<int,int> go, std::pair<int, int> pos) {
		if (go.first + pos.first >= 0 && go.first + pos.first < map->GetSize() && go.second + pos.second >= 0 && go.second + pos.second < map->GetSize()) {
			characters.push_back(std::make_unique<Arrow>(go, pos));
		}
		
	}
	void Attack(std::shared_ptr<GameObject> a, std::shared_ptr<GameObject> b) {
		a->Collide(*b);
	}
	bool CheckCollide(std::pair<int, int> point, std::shared_ptr<GameObject> obj) {
		if (point.first + obj->GetPos().first >= 0 && point.first + obj->GetPos().first < map->GetSize() && point.second + obj->GetPos().second >= 0 && point.second + obj->GetPos().second < map->GetSize()) {
			for (const auto pch : characters) {
				if (pch->GetPos().first == (point.first + obj->GetPos().first) && pch->GetPos().second == (point.second + obj->GetPos().second)) {
					if (pch != obj) {
						obj->SetDamage(10);
						Attack(obj, pch);
						return false;
					}
					
				}
			}
			return true;
		}
		if (obj->GetSym() == 'K') {
			map->infinity(point);
		}
		return false;
	}
	void AI() {
		for (unsigned int i = 0; i < characters.size(); i++) {
			characters[i]->AI();
		}
	}
	void ContinueGame() {
		this->GoTo();
		map->ChangeMap(characters);
		map->Draw_map();
		ObserveMap();
		for (unsigned int i = 0; i < characters.size(); i++) {
			if (!characters[i]->CheckHp()) {
				
				characters.erase(characters.begin() + i);
			}
		}
		AI();
		for (unsigned int i = 0; i < characters.size(); i++) {
			if (characters[i]->GetShoot()) {
				Shoot(characters[i]->GetShootTo(), std::make_pair<int,int>(characters[i]->GetPos().first + characters[i]->GetShootTo().first, characters[i]->GetPos().second + characters[i]->GetShootTo().second));
				characters[i]->SetShoot(false);
			}
		}
		
		map->Draw_map();
		
		hero_interface->ShowCharacter();
		if (mission->Check()) {
			EndGame();
			system("cls");
			std::cout << "You win";
			exit;
		}
		
	}

	void GameProcess() {
		while (!pause) {
			Input();
		}
	}

	template<class T>
	void AddEnemy(T* m) {
		m->SetMap(map);
		m->SetTarget(Hero);
		std::shared_ptr<T> monster = std::make_unique<T>(*m);
		characters.push_back(monster);
		enemy.push_back(monster);
	}
	void ObserveMap() {
		
		map->Observe(Hero->GetPos());
		
	}
	void StartGame() {
		characters.push_back(std::make_unique<Knight>(10000, 100, 10, 'K', 19, 0));	
		characters.push_back(std::make_unique<Wall>(100, 'W', 0, 1));
		//characters.push_back(std::make_unique<Wall>(100, 'W', 1, 0));
		Hero = characters[0];
		hero_interface->SetCharacter(characters[0]);
		characters.push_back(std::make_unique<Wall>(100, 'W', 1, 3));
		characters.push_back(std::make_unique<Wall>(100, 'W', 9, 10));
		characters.push_back(std::make_unique<Wall>(100, 'W', 8, 10));
		characters.push_back(std::make_unique<Wall>(100, 'W', 7, 10));

		characters.push_back(std::make_unique<Wall>(100, 'W', 10, 0));
		characters.push_back(std::make_unique<Wall>(100, 'W', 10, 1));
		characters.push_back(std::make_unique<AidKit>(1, 5));
		mission = std::make_unique<Princess>(19, 19);
		characters.push_back(mission);
		AddEnemy<Zombie>(new Zombie(100, 100, 10, 'Z', 10, 8));
		AddEnemy<Zombie>(new Zombie(100, 100, 10, 'Z', 12, 3));
		//AddEnemy<Zombie>(new Zombie(100, 100, 10, 'Z', 5, 5));
		//AddEnemy<Zombie>(new Zombie(100, 100, 10, 'Z', 0, 10));
		AddEnemy<Dragon>(new Dragon(100, 100, 10, 'D', 2, 10));
		map->ChangeMap(characters);
		map->Draw_map();
		GameProcess();
	}
	void EndGame() {
		pause = true;
	}
private:
	std::shared_ptr<Mission> mission;
	std::string Text;
	std::vector<std::shared_ptr<GameObject>> characters;
	std::vector<std::shared_ptr<Monster>> enemy;
	std::shared_ptr<Map> map;
	std::shared_ptr<InterfaceHero> hero_interface;
	std::shared_ptr<GameObject> Hero;
	bool pause = false;
	bool Prapere = false;
};


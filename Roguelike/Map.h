#pragma once
#include <utility>
#include <iostream>
#include <memory>
#include <vector>
#include <iterator>
#include <queue>

#include "GameObject.h"



class Map {
public:
	class Place {
	public:
		Place() {
			place = '.';
		}
		Place(GameObject& ch) {
			place = ch.GetSym();
		}
		void SetChar(char ch) {
			place = ch;
		};
		char GetChar() {
			if (CheckPlace()) {
				return place;
			}
			return '#';
		};
		bool operator==(char ch) {
			return place == ch;
		}
		void SetPlace(bool a) {
			check = a;
		}
		bool CheckPlace() {
			return check;
		}

	private:
		char place = '.';
		bool check = false;
	};
public:
	Map() {
		size = 20;
	}
	Map(int size) {
		this->size = size;
	}
	void init() {		
		map = std::vector<std::vector<Place>>(size, std::vector<Place>(size, Place()));
	}
	void RefreshMap() {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				map[i][j].SetChar('.');
			}
		}
	}
	void Observe_LU(std::pair<int,int> pos, int depth) {
		if (depth == 0) return;
		if (pos.first - 1 >= 0 && pos.second - 1 >= 0) {
			
			map[pos.first - 1][pos.second - 1].SetPlace(true);
			if (CheckPlace(pos.first - 1, pos.second - 1)) return;
			Observe_LU(std::make_pair<int, int>(pos.first - 1, pos.second - 1), depth - 1);
		}
		
	}
	void Observe_LD(std::pair<int, int> pos, int depth) {
		if (depth == 0) return;
		if (pos.first + 1 < size && pos.second - 1 >= 0) {
			
			map[pos.first + 1][pos.second - 1].SetPlace(true);
			if (CheckPlace(pos.first + 1, pos.second - 1)) return;
			Observe_LD(std::make_pair<int, int>(pos.first + 1, pos.second - 1), depth - 1);
		}
		
	}
	void Observe_RU(std::pair<int, int> pos, int depth) {
		if (depth == 0) return;
		if (pos.first - 1 >= 0 && pos.second + 1 < size) {
			
			map[pos.first - 1][pos.second+1].SetPlace(true);
			if (CheckPlace(pos.first - 1, pos.second+1)) return;
			Observe_RU(std::make_pair<int, int>(pos.first - 1, pos.second + 1), depth - 1);
		}
		
	}
	void Observe_RD(std::pair<int, int> pos, int depth) {
		if (depth == 0) return;
		if (pos.first + 1 < size && pos.second + 1 < size) {
			
			map[pos.first + 1][pos.second + 1].SetPlace(true);
			if (CheckPlace(pos.first + 1, pos.second + 1)) return;
			Observe_RD(std::make_pair<int, int>(pos.first + 1, pos.second + 1), depth - 1);
		}
		
	}
	void Observe(std::pair<int, int> pos) {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				map[i][j].SetPlace(false);
			}		
		}
		int distanse = 10;
		map[pos.first][pos.second].SetPlace(true);
		for (int i = 0; i <= distanse; i++) {
			if (pos.first - i >= 0) {
				
				map[pos.first - i][pos.second].SetPlace(true);
				if (CheckPlace(pos.first - i , pos.second)) break;
				
				Observe_RU(std::pair<int, int>(pos.first - i, pos.second), distanse - i);
				Observe_LU(std::pair<int, int>(pos.first - i, pos.second), distanse - i);
			}
		}
		for (int i = 0; i <= distanse; i++) {
			if (pos.second - i >= 0) {
				map[pos.first][pos.second - i].SetPlace(true);
				if (CheckPlace(pos.first , pos.second - i)) break;
				Observe_LU(std::pair<int, int>(pos.first , pos.second - i), distanse - i);
				Observe_LD(std::pair<int, int>(pos.first , pos.second - i), distanse - i);
			}
		}

		for (int i = 0; i <= distanse; i++) {
			if (pos.second + i < size) {
				 
				map[pos.first ][pos.second + i].SetPlace(true);
				if (CheckPlace(pos.first , pos.second + i)) break;				
				Observe_RU(std::pair<int, int>(pos.first, pos.second + i), distanse - i);
				Observe_RD(std::pair<int, int>(pos.first, pos.second + i), distanse - i);
				
			}
		}
		for (int i = 0; i <= distanse; i++) {
			if (pos.first + i < size) {
				map[pos.first + i][pos.second].SetPlace(true);
				if (CheckPlace(pos.first + i , pos.second)) break;
				Observe_LD(std::pair<int, int>(pos.first + i, pos.second), distanse - i);
				Observe_RD(std::pair<int, int>(pos.first + i, pos.second), distanse - i);
			}
		}
	}
	void Draw_map() {
		system("cls");
		RefreshMap();
		ChangeMap(characters);
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				std::cout << map[i][j].GetChar() << " ";
			}
			std::cout << "\n";
		}
	}
	std::vector<std::vector<Place>> GetMap() {
		return map;
	}
	void ChangeMap(std::vector<std::shared_ptr<GameObject>>& ch) {
		characters = ch;
		for (const auto& pch : ch) {
			if (pch->GetPos().first >= 0 && pch->GetPos().first < size && pch->GetPos().second >= 0 && pch->GetPos().second < size) {
				map[pch->GetPos().first][pch->GetPos().second].SetChar(pch->GetSym());
			}
			
		}
	}
	int GetSize() {
		return this->size;
	}
	bool CheckPlace(int x, int y) {
		for (const auto pch : characters) {
			if (pch->GetPos().first == x && pch->GetPos().second == y) {
				if (pch->GetSym() == 'K') {
					return false;
				}
				return true;
			}
		}
		return false;
	}
	void infinity(std::pair<int,int> move) {		
		if (move.first == -1) {
			for (const auto pch : characters) {
				if (pch->GetSym() != 'K') {
					pch->SetPos(std::pair<int, int>(pch->GetPos().first + 1, pch->GetPos().second));
				}							
			}
		}
		if (move.first == 1) {
			for (const auto pch : characters) {
				if (pch->GetSym() != 'K') {
					pch->SetPos(std::pair<int, int>(pch->GetPos().first - 1, pch->GetPos().second));
				}
			}
		}
		if (move.second == -1) {
			for (const auto pch : characters) {
				if (pch->GetSym() != 'K') {
					pch->SetPos(std::pair<int, int>(pch->GetPos().first , pch->GetPos().second + 1));
				}
			}
		}
		if (move.second == 1) {
			for (const auto pch : characters) {
				if (pch->GetSym() != 'K') {
					pch->SetPos(std::pair<int, int>(pch->GetPos().first, pch->GetPos().second - 1));
				}
			}
		}
		RefreshMap();
	}
private:
	int size;
	std::vector<std::vector<Place>> map;
	std::vector<std::shared_ptr<GameObject>> characters;
};

class InterfaceHero {
public:
	InterfaceHero(std::shared_ptr<GameObject> Hero) {
		this->Hero = Hero;
	}
	InterfaceHero() {
		;
	};
	void ShowCharacter() {
		std::cout << "HP " << Hero->GetHp() << "\n";
	}
	void SetCharacter(std::shared_ptr<GameObject> hero) {
		Hero = hero;
	}

private:
	std::shared_ptr<GameObject> Hero;
};
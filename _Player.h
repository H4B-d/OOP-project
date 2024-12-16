#pragma once
#include <string.h>
#include <sstream>
#include <iostream>
#include <time.h>

using namespace std;

class Player
{
public:
private:
	string _name;
	int _step;
	int _win;
	int _lose;
	int _draw;
	int _time;
public:
	string GetName();
	void SetName(string);
	void Input();
	void IncreStep();
	void DecreStep() { _step--; }
	int GetStep();
	void SetStep(int value) { _step = value; }
	void InitStep() { _step = 0; }
	void SetWin(int value) { _win = value; }
	void SetLose(int value) { _lose = value; }
	void SetDraw(int value) { _draw = value; }
	int GetWin() { return _win; }
	int GetLose() { return _lose; }
	int GetDraw() { return _draw; }
	void IncreWin() { _win++; }
	void IncreLose() { _lose++; }
	void IncreDraw() { _draw++; }
	int GetTime() { return _time; }
	void SetTime(int value) { _time = value; }
	void ResetTime() { _time = 30 * 60; }
};


string Player::GetName()
{
	return _name;
}

void Player::SetName(string value)
{
	_name = value;
	_time = 30 * 60;
}

void Player::Input()
{
	//cin.ignore();
	getline(cin, _name);
	_time = 30 * 60;
}

void Player::IncreStep()
{
	_step++;
}

int Player::GetStep()
{
	return _step;
}

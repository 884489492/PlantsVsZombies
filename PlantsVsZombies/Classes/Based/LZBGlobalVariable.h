#pragma once
#include "ui/CocosGUI.h"
#include "LZBUserInformation.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#define GAME_FONT_NAME_1 Global::getInstance()->userInformation->getGameText().find("TTF��������1")->second->text
#define GAME_FONT_NAME_2 Global::getInstance()->userInformation->getGameText().find("TTF��������2")->second->text
#define MYRELEASE 0

using namespace std;
using namespace cocos2d;

class Global
{
public:
	static Global* getInstance();
	void resumeProhibit();
	void checkAnimationInterval();

public:
	int prohibitId;
	UserInformation* userInformation;

private:
	Global();
	~Global();

private:
	static Global* _instance;    /* ���� */
	float _sumAnimationInterval;
	int _frequence;
};

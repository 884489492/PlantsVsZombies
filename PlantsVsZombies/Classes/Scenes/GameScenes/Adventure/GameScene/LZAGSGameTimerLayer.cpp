﻿/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.1.20
 *Email: 2117610943@qq.com
 */

#include "LZAGSGameTimerLayer.h"
#include "Based/LZUserData.h"

int GSGameTimerLayer::breakThroughTime = 0;

GSGameTimerLayer::GSGameTimerLayer():
	_global(Global::getInstance())
{
}

GSGameTimerLayer::~GSGameTimerLayer()
{
	breakThroughTime = 0;
}

bool GSGameTimerLayer::init()
{
	if (!Layer::init())return false;

	createTimer();

	return true;
}

void GSGameTimerLayer::createTimer()
{
	// 每30秒自动存档
	schedule([=](float) {
		UserData::getInstance()->createNewLevelDataDocument();
		UserData::getInstance()->caveLevelData(_global->userInformation->getCurrentCaveFileLevelWorldName());
		}, 30, "caveGameData");

	schedule([this](float) {
		_global->checkAnimationInterval();
		++breakThroughTime;
		}, 1.f, "FPS");
}

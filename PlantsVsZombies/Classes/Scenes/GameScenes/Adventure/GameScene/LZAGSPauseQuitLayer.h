﻿/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Scenes/MainMenuScene/LZOptionsSence.h"

using namespace cocos2d;

enum class PauseQuitLayer_Button
{
	查看图鉴 = 0,
	从新开始,
	退出游戏,
	按键说明,
	返回游戏
};

class GSPauseQuitLayer :public OptionsMenu
{
public:
	CREATE_FUNC(GSPauseQuitLayer);
	static void pauseLayer();
	static void resumeLayer();
	static void setPause(const bool pause);
	static bool getIsPause();
	
CC_CONSTRUCTOR_ACCESS:
	GSPauseQuitLayer();
	~GSPauseQuitLayer();
	virtual bool init() override;

protected:
	virtual void createButton(const Vec2& vec2, const std::string name, PauseQuitLayer_Button type);
	virtual void createDialog() override;
	virtual void popSceneAnimation();
	virtual void showPrompt();
	virtual void openHandBook();
	virtual void setRestart();
	virtual void setQuitGame();
	virtual void keyDescription();
	virtual void returnGame();

protected:
	EventListenerTouchOneByOne* _touchListener;
	LayerColor* _promptLayer;
	static bool _isPause;
	static string _layerName[7];
	char* _levelName;
};
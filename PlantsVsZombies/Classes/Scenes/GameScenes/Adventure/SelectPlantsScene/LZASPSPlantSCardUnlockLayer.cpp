﻿/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.1.31
 *Emal: 2117610943@qq.com
 */

#include "LZASPSPlantSCardUnlockLayer.h"
#include "LZASPSSpriteLayer.h"
#include "Based/LZPlayMusic.h"
#include "Based/LZMouseEventControl.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSAnimationLayer.h"
#include "Plants/LZPlants.h"

vector<PlantsType> PlantsCardUnlock::unlockPlantsCard;

PlantsCardUnlock::PlantsCardUnlock():
	_plantsCardUnlockDialog(nullptr),
	_scrollView(nullptr),
	_plant(nullptr),
	_plantAnimation(nullptr),
	_button(nullptr),
	_winSize(Director::getInstance()->getWinSize())
{
}

PlantsCardUnlock::~PlantsCardUnlock()
{
}

bool PlantsCardUnlock::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;

	Dialog::createShieldLayer(this);

	createDialog();
	
	return true;
}

void PlantsCardUnlock::createDialog()
{
	auto text = _global->userInformation->getGameText();

	_plantsCardUnlockDialog = Sprite::createWithSpriteFrameName("LevelObjiectives.png");
	_plantsCardUnlockDialog->setScale(2.0f);
	this->addChild(_plantsCardUnlockDialog);

	auto question = Text::create(text.find("恭喜！解锁了新的植物！")->second->text, GAME_FONT_NAME_1, text.find("恭喜！解锁了新的植物！")->second->fontsize);
	question->setTextColor(Color4B(0, 255, 255, 255));
	question->setPosition(Vec2(_plantsCardUnlockDialog->getContentSize().width / 2, _plantsCardUnlockDialog->getContentSize().height - 70));
	_plantsCardUnlockDialog->addChild(question);

	/* 创建监听 */
	createTouchtListener(_plantsCardUnlockDialog);

	/* 创建按钮 */
	createButtons();
	showUnlockPlant();
	showDialogAction();
}

void PlantsCardUnlock::createButtons()
{
	_button = Button::create("ButtonNew.png", "ButtonNew2.png", "",cocos2d::ui::Widget::TextureResType::PLIST);
	_button->setPosition(Vec2(_plantsCardUnlockDialog->getContentSize().width / 2, 10));
	_button->setScale(0.25f);
	_button->setTitleFontSize(70);
	_button->setTitleFontName(GAME_FONT_NAME_1);
	_button->setTitleText(_global->userInformation->getGameText().find("确定")->second->text);
	_button->setTitleColor(Color3B::BLACK);
	_plantsCardUnlockDialog->addChild(_button);

	_button->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
		{
			switch (type)
			{
			case Widget::TouchEventType::BEGAN: PlayMusic::playMusic("bleep");                                  break;
			case Widget::TouchEventType::ENDED: showNextUnlockPlant(); _button->setTouchEnabled(false);         break;
			}
		});
}

void PlantsCardUnlock::showUnlockPlant()
{
	auto emitter = ParticleSystemQuad::create("resources/Text/ExplodingRing.reanim.compiled");
	emitter->retain();
	emitter->setTotalParticles(1000);
	emitter->setLifeVar(5);
	emitter->setSpeedVar(400);
	emitter->setSpeed(500);
	emitter->setPosition(getContentSize() / 2.f);
	emitter->setAutoRemoveOnFinish(true);
	Director::getInstance()->getRunningScene()->addChild(emitter, 6);

	_plant = GSAnimationLayer::createDifferentPlants(unlockPlantsCard.back(), animationLayerInformation);
	_plant->setPlantNode(_plantsCardUnlockDialog);
	_plant->setPlantPosition(Vec2(_plantsCardUnlockDialog->getContentSize().width / 2.f, 70));
	_plant->createPlantAnimation();
	_plant->setPlantNoramlAnimation();
	_plantAnimation = _plant->getPlantAnimation();
	_plantAnimation->getChildByName("SplashOfSoil")->setVisible(false);
}

void PlantsCardUnlock::showNextUnlockPlant()
{
	if (unlockPlantsCard.size() > 0)
	{
		unlockPlantsCard.pop_back();
	}
	
	fadeOutAndShowNewDialog();
}

void PlantsCardUnlock::showDialogAction()
{
	PlayMusic::playMusic("achievement");
	_plantsCardUnlockDialog->setPosition(_winSize / 2.f);
	_plantsCardUnlockDialog->setScale(0.f);
	_plantsCardUnlockDialog->runAction(Sequence::create(
		EaseBounceOut::create(ScaleTo::create(0.3f, 2.f)),
		CallFunc::create([=]()
			{
				_button->setTouchEnabled(true);
			}), nullptr));
}

void PlantsCardUnlock::fadeOutAndShowNewDialog()
{
	_plantsCardUnlockDialog->runAction(Sequence::create(EaseExponentialIn::create(Spawn::create(
		ScaleTo::create(0.3f, 0.f), MoveTo::create(0.3f, _winSize / 2.f),
		CallFunc::create([=]()
			{
				if (!unlockPlantsCard.size())
				{
					_plantsCardUnlockDialog->getParent()->runAction(FadeOut::create(0.3f));
				}
			}), nullptr)),
		CallFunc::create([=]()
			{
				if (unlockPlantsCard.size())
				{
					if (_plantAnimation)
					{
						_plantAnimation->removeFromParent();
						delete _plant;
						_plant = nullptr;
					}
					showDialogAction();
					showUnlockPlant();
				}
				else
				{
					SPSSpriteLayer::resumeButtonHoverEffect();
					showDeleteDialogAction(_plantsCardUnlockDialog);
				}
			}), nullptr));
}

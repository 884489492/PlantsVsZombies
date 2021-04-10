/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.09
 *Email: 2117610943@qq.com
 */

#include "LZTSPSRequirementLayer.h"
#include "Scenes/GameScenes/Adventure/SelectPlantsScene/LZASPSSpriteLayer.h"
#include "Based/LZGlobalVariable.h"
#include "Based/LZPlayMusic.h"

TSPSRequriementLayer::TSPSRequriementLayer():
	_objiectives(nullptr)
{
}

TSPSRequriementLayer::~TSPSRequriementLayer()
{
}

bool TSPSRequriementLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;

	createShieldLayer(this);
	createDialog();
	createText();
	showButton();

	return true;
}

void TSPSRequriementLayer::createDialog()
{
	_objiectives = Scale9Sprite::createWithSpriteFrameName("LevelObjiectives.png");
	_objiectives->setPosition(Director::getInstance()->getWinSize() / 2.0f);
	_objiectives->setScale(2.0f);
	this->addChild(_objiectives);

	showDialogAction(_objiectives);
	createTouchtListener(_objiectives);
}

void TSPSRequriementLayer::createText()
{
	auto objiectivesText = Text::create();
	objiectivesText->setString(_global->userInformation->getGameText().find("ģʽ˵��")->second->text);
	objiectivesText->setFontName(GAME_FONT_NAME_1);
	objiectivesText->setFontSize(_global->userInformation->getGameText().find("ģʽ˵��")->second->fontsize);
	objiectivesText->setScale(0.5f);
	objiectivesText->setColor(Color3B(0, 255, 255));
	objiectivesText->setPosition(Vec2(_objiectives->getContentSize().width / 2, 245));
	_objiectives->addChild(objiectivesText);

	auto str = _global->userInformation->getGameText().find("ģʽ����")->second;
	auto information = Text::create(str->text, GAME_FONT_NAME_1, str->fontsize);
	information->setColor(Color3B::BLACK);
	information->setGlobalZOrder(10);
	information->setTextVerticalAlignment(TextVAlignment::CENTER);
	information->setTextHorizontalAlignment(TextHAlignment::CENTER);
	information->setTextAreaSize(Size(_objiectives->getContentSize().width - 85, 150));
	information->setPosition(Vec2(_objiectives->getContentSize().width / 2.0f, _objiectives->getContentSize().height / 2.0f - 30));
	_objiectives->addChild(information);
}

void TSPSRequriementLayer::showButton()
{
	auto button = Button::create("Continue1.png", "Continue.png", "", TextureResType::PLIST);
	button->setTitleText(Global::getInstance()->userInformation->getGameText().find("ȷ��")->second->text);
	button->setTitleFontName(GAME_FONT_NAME_1);
	button->setTitleFontSize(Global::getInstance()->userInformation->getGameText().find("ȷ��")->second->fontsize);
	button->setTitleColor(Color3B::YELLOW);
	button->setPosition(Vec2(_objiectives->getContentSize().width / 2.0f, 10));
	button->setOpacity(0);
	button->setScale(0.5f);
	button->runAction(FadeIn::create(0.5f));
	button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("tap");
				break;
			case ui::Widget::TouchEventType::ENDED:
				showDeleteDialogAction(_objiectives);
				this->runAction(Sequence::create(Spawn::create(DelayTime::create(0.2f), FadeOut::create(0.2f), nullptr),
					CallFunc::create([&]()
						{
							SPSSpriteLayer::resumeButtonHoverEffect();
							this->removeFromParent();
						}), nullptr));
				break;
			}
		});
	_objiectives->addChild(button);
}

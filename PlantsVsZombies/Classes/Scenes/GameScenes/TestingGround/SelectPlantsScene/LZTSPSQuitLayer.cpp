/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.07
 *Email: 2117610943@qq.com
 */

#include "LZTSPSQuitLayer.h"
#include "Scenes/GameScenes/Adventure/SelectPlantsScene/LZASPSSpriteLayer.h"
#include "Scenes/MainMenuScene/LZMainMenu.h"
#include "Based/LZPlayMusic.h"

bool TSPSQuitLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;

	createShieldLayer(this);
	createDialog();

	return true;
}

void TSPSQuitLayer::createButton(const Vec2& vec2, const std::string name, PauseQuitLayer_Button button_type)
{
	/* �����������˵���ť */
	auto button = ui::Button::create("ButtonNew2.png", "ButtonNew.png", "", TextureResType::PLIST);
	auto label = Label::createWithTTF(name, GAME_FONT_NAME_1, _global->userInformation->getGameText().find("������Ϸ")->second->fontsize);
	label->enableShadow(Color4B(0, 0, 0, 200));//������Ӱ
	label->setScale(2.0f);
	button->setTitleLabel(label);
	button->setTitleColor(Color3B::WHITE);
	button->setPosition(vec2);
	button->setScale(0.5f);
	_option->addChild(button);

	button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("gravebutton");
				break;
			case ui::Widget::TouchEventType::ENDED:
				switch (button_type)
				{
				case PauseQuitLayer_Button::������Ϸ:
					SPSSpriteLayer::resumeButtonHoverEffect();
					this->removeFromParent();
					break;
				case PauseQuitLayer_Button::�˳���Ϸ:
					SPSSpriteLayer::setListenerMutexNumber();
					Director::getInstance()->replaceScene(TransitionFade::create(0.5f, MainMenu::create()));
					break;
				default:
					break;
				}
			}
		});
}
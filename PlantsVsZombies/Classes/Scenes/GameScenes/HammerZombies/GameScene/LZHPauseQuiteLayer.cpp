/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.09
 *Email: 2117610943@qq.com
 */

#include "LZHPauseQuiteLayer.h"
#include "LZHammerZombiesScene.h"
#include "Scenes/MainMenuScene/LZMainMenu.h"
#include "Based/LZPlayMusic.h"
#include "spine/spine.h"
#include "spine/spine-cocos2dx.h"

using namespace spine;

HPauseQuitLayer::HPauseQuitLayer()
{
}

HPauseQuitLayer::~HPauseQuitLayer()
{
}

bool HPauseQuitLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;

	createDialog();

	return true;
}

void HPauseQuitLayer::createDialog()
{
	_option = Sprite::createWithSpriteFrameName("LevelObjiectivesBg.png");
	_option->setPosition(_director->getWinSize() / 2);
	_option->setScale(0.9f);
	this->addChild(_option);

	auto PauseAnimation = SkeletonAnimation::createWithData(
		_global->userInformation->getAnimationData().find("PauseAnimation")->second);
	PauseAnimation->setAnimation(0, "animation", true);
	PauseAnimation->setPosition(Vec2(530, 650));
	_option->addChild(PauseAnimation);

	/* �����ƶ����� */
	_touchListener = createTouchtListener(_option);

	/* ���������� */
	auto musicslider = createSlider(Vec2(600, 520), Vec2(150, 520),
		_global->userInformation->getGameText().find("����")->second->text, OptionScene_Slider::����,
		Sprite::createWithSpriteFrameName("bgFile.png"),
		Sprite::createWithSpriteFrameName("progressFile.png"),
		Sprite::createWithSpriteFrameName("thumbFile.png"),
		nullptr,
		true);
	auto SoundEffectslider = createSlider(Vec2(600, 450), Vec2(150, 450),
		_global->userInformation->getGameText().find("��Ч")->second->text, OptionScene_Slider::��Ч,
		Sprite::createWithSpriteFrameName("bgFile.png"),
		Sprite::createWithSpriteFrameName("progressFile.png"),
		Sprite::createWithSpriteFrameName("thumbFile.png"),
		nullptr,
		true);

	musicslider->setScale(1.2f);
	SoundEffectslider->setScale(1.2f);

	/* ������ѡ�� */
	auto check = createCheckBox(Vec2(350, 380), Vec2(150, 380), _global->userInformation->getGameText().find("��Ϣ")->second->text, OptionScene_CheckBox::��ʾ��Ϣ, "CheckBox2", "CheckBox", true);
	auto check1 = createCheckBox(Vec2(800, 380), Vec2(600, 380), _global->userInformation->getGameText().find("ȫ��")->second->text, OptionScene_CheckBox::ȫ��, "CheckBox2", "CheckBox", true);
	auto check2 = createCheckBox(Vec2(350, 310), Vec2(150, 310), _global->userInformation->getGameText().find("��֡��")->second->text, OptionScene_CheckBox::��֡��, "CheckBox2", "CheckBox", true);
	auto check3 = createCheckBox(Vec2(800, 310), Vec2(600, 310), _global->userInformation->getGameText().find("�����ʾ")->second->text, OptionScene_CheckBox::�������, "CheckBox2", "CheckBox", true);
	auto check4 = createCheckBox(Vec2(350, 240), Vec2(150, 240), _global->userInformation->getGameText().find("������ʾ")->second->text, OptionScene_CheckBox::������ʾ, "CheckBox2", "CheckBox", true);
	auto check5 = createCheckBox(Vec2(800, 240), Vec2(600, 240), _global->userInformation->getGameText().find("��ֱͬ��")->second->text, OptionScene_CheckBox::��ֱͬ��, "CheckBox2", "CheckBox", true);

	check->setScale(0.6f);
	check1->setScale(0.6f);
	check2->setScale(0.6f);
	check3->setScale(0.6f);
	check4->setScale(0.6f);
	check5->setScale(0.6f);

	/* ������ť */
	createButton(Vec2(210, 170), _global->userInformation->getGameText().find("�鿴ͼ��")->second->text, PauseQuitLayer_Button::�鿴ͼ��);
	createButton(Vec2(520, 170), _global->userInformation->getGameText().find("���¿�ʼ")->second->text, PauseQuitLayer_Button::���¿�ʼ);
	createButton(Vec2(830, 170), _global->userInformation->getGameText().find("������Ϸ")->second->text, PauseQuitLayer_Button::������Ϸ);
	createButton(Vec2(365, 70), _global->userInformation->getGameText().find("����˵��")->second->text, PauseQuitLayer_Button::����˵��);
	createButton(Vec2(665, 70), _global->userInformation->getGameText().find("�˳�")->second->text, PauseQuitLayer_Button::�˳���Ϸ);
}

void HPauseQuitLayer::createButton(const Vec2& vec2, const std::string name, PauseQuitLayer_Button button_type)
{
	/* �����������˵���ť */
	auto button = ui::Button::create("ButtonNew2.png", "ButtonNew.png", "", TextureResType::PLIST);
	auto label = Label::createWithTTF(name, GAME_FONT_NAME_1, _global->userInformation->getGameText().find("�˳�������")->second->fontsize);
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
				case PauseQuitLayer_Button::�鿴ͼ��: openHandBook();   break;
				case PauseQuitLayer_Button::���¿�ʼ: setRestart();     break;
				case PauseQuitLayer_Button::�˳���Ϸ: setQuitGame();    break;
				case PauseQuitLayer_Button::����˵��: keyDescription(); break;
				case PauseQuitLayer_Button::������Ϸ: returnGame();     break;
				default: break;
				}
			}
		});
}

void HPauseQuitLayer::setRestart()
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.5f, HammerZombiesScene::create()));
}

void HPauseQuitLayer::setQuitGame()
{
	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, MainMenu::createScene()));
}

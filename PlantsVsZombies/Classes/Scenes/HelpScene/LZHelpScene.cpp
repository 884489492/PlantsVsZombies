/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.7.1
 *Email: 2117610943@qq.com
 */
#include "ui/CocosGUI.h"

#include "LZHelpScene.h"
#include "Scenes/MainMenuScene/LZMainMenu.h"
#include "Scenes/EasterEggsScene/LZGameEasterEggs.h"
#include "Scenes/SelectWorldScene/LZSelectWorldScene.h"
#include "Based/LZPlayMusic.h"
#include "Based/LZMouseEventControl.h"

string INFORMATION_TEXT = { "\
        ����Ϸȫ������������\"�������\"��LZ������������ɡ�����Ϸ�뵱ǰ���Լ������ڶ�İ治ͬ���˳����ǻ���ԭ����Ϸ�Ķ��ο��������Ǵ��´��㿪ʼ������\
����˵�˳�����ԭ�����û���κι�ϵ���ó���ʹ��Cocos2dx��Ϸ��������Լ�C++����������ɡ�Ŀǰ�ó��������Դ�����Ѿ���Դ��github�Լ�gitee�ϡ�\n\
        ����ϷĿǰ�Ѿ��ж�����Ϸģʽ�����Ų�ͬ�������Ϸ�ؿ������ų������ٹ���������Ϸ�ؿ������ų����ͼ��10 * 18������Ϸģʽ�����š�����ʬ���޾������Ϸģʽ�ȵ����ࡣ\
����Ϸʹ�õľ��󲿷�ͼƬ�����֡���Ч���������Դ�ļ���Դ��ԭ��һ�����������Լ���������Ϸ��Դ���ٲ����ռ��������Լ��Լ�������\n\
	    �˳���ʹ��ԭ����Ϸ�ز����Բ���������ҵ��;���������Ը����������д���Ϸ����ʹ����Դ����ʱ����ΪĬ��ͬ���Э�飬�������˳���Ϸ��ɾ����\
��Ŀ��ַ����ֱ����github��gitee�������ؼ���\"PlantsVsZombies\"����������������µ�ַ��https://github.com/ErLinErYi/PlantsVsZombies \n\
		Ŀǰ�� 15 �ֽ�ʬ�� 24 ��ֲ��������������������ޣ���Щ��ʬ��ֲ�ﶯ������ʵ�ֻ򶯻���ԭ����ںܴ�ƫ�\
��ʹ��spine����DragonBones������2D������Ŀǰ�Ѿ��ж�����Ϸģʽ�ͳ������ٸ���Ϸ�ؿ���\nQQ:2117610943 \nQQȺ:926707238 \n��ַ:https://gitlz.gitee.io/lz \n\n"
};

Scene* HelpScene::createHelpScene()
{
	return HelpScene::create();
}

bool HelpScene::init()
{
	if (!Scene::init()) return false;

	PlayMusic::playMusic("paper");
	
	createBackground();
	createParticle();
	createText();
	createButton();
	
	return true;
}

void HelpScene::createBackground()
{
	SelectWorldScene::createGalaxy(this);
}

void HelpScene::createText()
{
	/* �������ֱ��� */
	auto note = Sprite::createWithSpriteFrameName("ZombieNote.png");
	note->setScale(1.7f);
	note->setScaleX(2.2f);
	note->setPosition(Vec2(_size.width / 2 + 50, _size.height / 2));
	this->addChild(note);

	addScrollView();

	FileUtils::getInstance()->getStringFromFile("history.pak", [=](string history)
		{
			auto helptext = Label::createWithTTF(INFORMATION_TEXT + (history.empty() ? "\t\t\t\t\t\t\t\t\t\t\t\t\t�ı�����ʧ�ܣ�" : history), GAME_FONT_NAME_1, 35);
			helptext->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
			helptext->setColor(Color3B::BLACK);
			helptext->setMaxLineWidth(1100);
			_textScrollView->setInnerContainerSize(helptext->getContentSize());
			_textScrollView->addChild(helptext);
			helptext->setPosition(Vec2(600, _textScrollView->getInnerContainerSize().height));
			helptext->setLineBreakWithoutSpace(true);
		});

	addMouseEvent();
}

void HelpScene::createGithubUrl()
{
	auto github = Button::create();
	github->setTitleFontName(GAME_FONT_NAME_1);
	github->setTitleFontSize(35);
	github->setTitleText("����˴���github��Ŀ");
	github->setPosition(Vec2(1220, 740));
	github->setTitleColor(Color3B(0, 255, 255));
	github->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
		{
			switch (type)
			{
			case Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("tap2");
				break;
			case Widget::TouchEventType::ENDED:
				Application::getInstance()->openURL("https://github.com/ErLinErYi/PlantsVsZombies");
				break;
			}
		});
	this->addChild(github);
	
	auto draw = DrawNode::create();
	draw->drawLine(Vec2(0, 5), Vec2(github->getContentSize().width, 5), Color4F(0, 1, 1, 1));
	draw->setLineWidth(2);
	github->addChild(draw);
}

void HelpScene::createButton()
{
	/* ������ť */
	auto button = Button::create("SeedChooser_Button2.png", "SeedChooser_Button2_Glow.png", "", TextureResType::PLIST);
	button->setTitleText(_global->userInformation->getGameText().find("���˵�")->second->text);
	button->setTitleColor(Color3B(0, 255, 255));
	button->setTitleFontSize(_global->userInformation->getGameText().find("���˵�")->second->fontsize);
	button->setScale(2.0f);
	button->setPosition(Vec2(_size.width / 2, 100));
	this->addChild(button);
	button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
		{
			switch (type)
			{
			case Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("tap2");
				break;
			case Widget::TouchEventType::ENDED:
				Director::getInstance()->replaceScene(MainMenu::createScene());
				break;
			}
		});

}

void HelpScene::createParticle()
{
	/*auto particle = ParticleSystemQuad::create("resources/Text/Particle.plist");
	particle->setPosition(Vec2(0, -30));
	this->addChild(particle);*/
}

void HelpScene::addScrollView()
{
	_textScrollView = ui::ScrollView::create();
	_textScrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	_textScrollView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_textScrollView->setContentSize(Size(1280.f, 570.f));
	_textScrollView->setPosition(_size / 2.0f);
	_textScrollView->setBounceEnabled(true);
	_textScrollView->setScrollBarPositionFromCorner(Vec2(20, 0));
	_textScrollView->setScrollBarWidth(10);
	_textScrollView->setScrollBarColor(Color3B::BLACK);
	this->addChild(_textScrollView);
}

void HelpScene::addMouseEvent()
{
	/* ��껬������ */
	auto mouse = EventListenerMouse::create();
	mouse->onMouseScroll = [=](Event* event)
	{
		auto mouseEvent = static_cast<EventMouse*>(event);
		float movex = mouseEvent->getScrollY() * 5;

		MouseEventControl::mouseScrollControlListener(_textScrollView, movex, ui::ScrollView::Direction::VERTICAL);
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouse, _textScrollView);
}

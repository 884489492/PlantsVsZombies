/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.6.25
 *Email: 2117610943@qq.com
 */

#include "LZGameEasterEggs.h"

#include "Scenes/MainMenuScene/LZQuitScene.h"
#include "Based/LZPlayMusic.h"
#include "Based/LZVideoPlayer.h"

GameEasterEggs::GameEasterEggs():
    _global(Global::getInstance())
,   _audioId(-1)
{
}

GameEasterEggs::~GameEasterEggs()
{
}

bool GameEasterEggs::init()
{
	if (!Scene::init())return false;

#if MYRELEASE
	playVideo();
#endif

#ifndef VIDEO_TEST
	createEggText();
	musicCallBack();
#else
#   if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	createSlider();
    playVideo();
#   endif
#endif

	return true;
}

void GameEasterEggs::createEggText()
{
	_audioId = PlayMusic::changeBgMusic("ZombiesWinEggs", false);
	AudioEngine::setVolume(_audioId, 1.0f);

	const auto sumTime = QuitMenu::getSumRunTime();
	const int hour = sumTime / 3600;
	const int min = (sumTime - hour * 3600) / 60;
	const int sec = sumTime - hour * 3600 - min * 60;

	/* ���ֲʵ� */
	string buff = { "      �������������ף����ͨ����ð��ģʽ�����м򵥹ؿ������滹�����ѹؿ���������Ϸģʽ������ȥ��ս����л���ܹ�����ʱ������ע����Ϸ��\
���ǵĹ�ע���ҿ�����Ϸ���������������Ϊ���ǵĹ�ע����ʹ�������㹻������ʹ�Ҽ��������ȥ��������Ϸ���ĸ��á�\n\
      ֲ���ս��ʬ�����Ϸ������ϲ������Ϸ֮һ���ܾ���ǰ�Ҿ�ϣ���Լ��ܹ���������һ��ͬ����Ϸ�������Ǹ�ʱ���������ޣ�\
ֱ��ȥ����٣�2018�꣩�ҿ�ʼ������������˴�Լһ�����¡����������������ޣ�����������Ϸ����û�п����ԡ���󲻵ò�������������\
�����Ҳ�û�������ķ�����ʱ��һ�꣨2019�꣩������Ҵ��¿�ʼ����ֲ���ս��ʬ�������������µ�ѧϰ�뿪����������������һ������\
�����ԵĲ��԰档��Ϸ������һ�����Ѷ��Ҽ����Ĺ��̣��ڼ��������������⣬�й�������Ҳ�й��˷ܡ������Ҽ���������������и�����\
�Ĺؿ�������ȥ��ս���������Ͱɣ�\n\
      Ŀǰ��16�ֽ�ʬ��24��ֲ�ֲ��ͽ�ʬ�Ķ������Ǳ������ģ��������������������ޣ���Щ��ʬ��ֲ�ﶯ������ʵ�֡�\
��ʹ��spine����DragonBones������2D�����������ĸ����ǲ���ʱ�ģ����ܻ����������qq��2117610943 QQȺ��926707238\n\n" };

	auto eggstext = Text::create("", GAME_FONT_NAME_1, 36);
	eggstext->setString(buff + "��Ϸ�����о��� " + std::to_string(_global->userInformation->getBreakThroughNumbers()) +
		" �δ���ʧ�ܣ�\n��Ϸ������ʹ�� " + std::to_string(_global->userInformation->getUsePlantsNumbers()) +
		" ��ֲ����з�����\n��Ϸ�����й��� " + std::to_string(_global->userInformation->getKillZombiesNumbers()) + " ����ʬ�����ˣ�\n" +
		UserDefault::getInstance()->getStringForKey("FIRSTRUNTIME") + " �����һ�����д���Ϸ��ʱ�䣡\n" +
		"��ĿǰΪֹ���Ѿ���Ϸ�� " + to_string(hour) + "Сʱ " + to_string(min) + "���� " + to_string(sec) + "�룡");
	eggstext->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.0f, -430));
	eggstext->setColor(Color3B(0, 255, 255));
	eggstext->enableGlow(Color4B::BLUE);
	eggstext->setTextAreaSize(Size(1700, 880));
	addChild(eggstext);

	eggstext->runAction(Sequence::create(MoveTo::create(25.0f, Director::getInstance()->getWinSize() / 2.0f),
		CallFunc::create([=]()
			{
				auto text = Text::create("�������������ֲ������", GAME_FONT_NAME_1, 36);
				text->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.0f, 70));
				text->setColor(Color3B::WHITE);
				text->setName("Text");
				this->addChild(text);
				if (UserDefault::getInstance()->getBoolForKey("EASTEREGGSHOW"))text->setVisible(false);

				eggstext->runAction(RepeatForever::create(Sequence::create(
					CallFunc::create([=]()
						{
							static int I = 0;
							if (I % 3 == 0 || I % 5 == 0 || I % 7 == 0)++I;
							else I += 2;
							const auto size = eggstext->getString().size();
							for (unsigned int i = 0; i < size; ++i)
							{
								auto letter = eggstext->getLetter(i);
								if (letter)
								{
									letter->runAction(Sequence::create(DelayTime::create(0.05f * i),
										Spawn::create(JumpBy::create(0.7f, Vec2(0, 0), 50, 1), RotateBy::create(0.7f, 360),
											Sequence::create(TintTo::create(0.35f, Color3B::RED), 
												TintTo::create(0.35f, I % 2 == 0 ? Color3B::GREEN : Color3B(0, 255, 255)), nullptr), nullptr), nullptr));
								}
							}
						}), DelayTime::create(eggstext->getString().size() * 0.05f / 3.0f), nullptr)));

				eggstext->runAction(RepeatForever::create(Sequence::create(
					CallFunc::create([=]()
						{
							const auto size1 = text->getString().size();
							for (unsigned int i = 0; i < size1; ++i)
							{
								auto letter = text->getLetter(i);
								if (letter)
								{
									letter->runAction(Sequence::create(DelayTime::create(0.1f * i), JumpBy::create(0.5f, Vec2(0, 0), 30, 1), nullptr));
								}
							}
						}), DelayTime::create(text->getString().size() * 0.1f), nullptr)));

			}), nullptr));
}

void GameEasterEggs::musicCallBack()
{
	if (UserDefault::getInstance()->getBoolForKey("EASTEREGGSHOW"))
	{
		backButton();
	}
	else
	{
		AudioEngine::setFinishCallback(_audioId, [=](int i, string name)
			{
				if (this->getChildByName("Text"))
					this->getChildByName("Text")->setVisible(false);

				PlayMusic::changeBgMusic("mainmusic", true);

				backButton();

				UserDefault::getInstance()->setBoolForKey("EASTEREGGSHOW", true);
			});
	}
}

void GameEasterEggs::backButton()
{
	/* ������ť */
	auto button = ui::Button::create("SeedChooser_Button2.png", "SeedChooser_Button2_Glow.png", "", TextureResType::PLIST);
	button->setTitleText(_global->userInformation->getGameText().find("�˳�")->second->text);
	button->setTitleColor(Color3B(0, 255, 255));
	button->setTitleFontSize(_global->userInformation->getGameText().find("�˳�")->second->fontsize);
	button->setScale(2.0f);
	button->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2, 30));
	this->addChild(button);
	button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("tap2");
				PlayMusic::changeBgMusic("mainmusic", true);
				break;
			case ui::Widget::TouchEventType::ENDED:
				Director::getInstance()->popScene();
				break;
			}
		});
}

#if MYRELEASE
void GameEasterEggs::playVideo()
{
	_pPlayer = VideoPlayer::instance();
	this->addChild(_pPlayer);

	string str = FileUtils::getInstance()->fullPathForFilename(_global->userInformation->getGameText().find("Video��ַ")->second->text);

	auto n = str.find("/");
	while (n != string::npos)
	{
		str.replace(n, 1, "\\");
		n = str.find("/");
	}

	_pPlayer->play(str.c_str());

#if MYRELEASE
#else
	schedule([=](float)
		{
			auto value = _pPlayer->getPrecent();
			_slider->setPercent(value == -1 ? 0 : value);
		}, 1.f, CC_REPEAT_FOREVER, 5.f, "video");
#endif
}
#endif

#ifdef VIDEO_TEST
void GameEasterEggs::createSlider()
{
	_slider = Slider::create();
	_slider->loadBarTexture("bgFile.png", TextureResType::PLIST);
	_slider->loadSlidBallTextureNormal("thumbFile.png", TextureResType::PLIST);
	_slider->loadProgressBarTexture("progressFile.png", TextureResType::PLIST);
	_slider->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.f, 25));
	_slider->setLocalZOrder(1);
	_slider->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
		{
			auto slider = (Slider*)sender;
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				_pPlayer->setPrecent(slider->getPercent() / 100.f);
				break;
			}
		});
	this->addChild(_slider);

	auto KeyBoard = EventListenerKeyboard::create();
	KeyBoard->onKeyReleased = [&](EventKeyboard::KeyCode code, Event* event)
	{
		switch (code)
		{
		case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			_pPlayer->setVideoTime(_pPlayer->getVideoTime() - 30000);
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			_pPlayer->setVideoTime(_pPlayer->getVideoTime() + 30000);
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
			_pPlayer->setVolume(_pPlayer->getVolume() + 5);
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			_pPlayer->setVolume(_pPlayer->getVolume() - 5);
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
			_pPlayer->changeVideoState();
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE:
			Director::getInstance()->popScene();
			break;
		default:
			break;
		}
	};
	_director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(KeyBoard, this);
}
#endif
/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.7.15
 *Emal: 2117610943@qq.com
 */

#include "AudioEngine.h"
#include "tinyxml2/tinyxml2.h"

#include "Based/LZBLevelData.h"
#include "Based/LZBUserInformation.h"
#include "Based/LZBPlayMusic.h"
#include "LZSLLoadingScene.h"
#include "LZSLSelectLanguageLayer.h"

#define MYDEBUG 1

bool LoadingScene::loadingFinished = false;

LoadingScene::LoadingScene() :
	_loadFileNumbers(0),
	_textNumbers(0),
	_allFileNumbers(0),
	_musicNumbers(0),
	_animationNumbers(0),
	_imageNumbers(0),
	_delayTime(0.08f),
	_loadingPrecent(0),
	_label(nullptr),
	_loadingBar(nullptr),
	_listener(nullptr),
	_global(Global::getInstance()),
	_director(Director::getInstance()),
	_files(FileUtils::getInstance()),
	_userData(UserData::getInstance())
{
	SpriteFrameCache::getInstance()->
		addSpriteFramesWithFile("resources/images/LoadingScene/LoadingScene.plist");
	_downloader.reset(new network::Downloader());
}

LoadingScene::~LoadingScene()
{
	SpriteFrameCache::getInstance()->
		removeSpriteFramesFromFile("resources/images/LoadingScene/LoadingScene.plist");
}

Scene* LoadingScene::createLaodingScene()
{
	return LoadingScene::create();
}

bool LoadingScene::init()
{
	if (!Scene::init())return false;
	
#if MYRELEASE
#   if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	checkEdition();          /* ���汾 */
#   endif
#endif
	setRunFirstTime();       /* ��ȡ��һ������ʱ�� */
	setSystem();             /* ����ϵͳ���� */
	loadUserData();          /* �����û���Ϣ */
	showLoadingBackGround(); /* չʾ���ؽ��� */
	calculateFileNumbers();  /* �����ļ����� */

	return true;
}

void LoadingScene::setSystem()
{
	/* ���ù�� */
	_director->getOpenGLView()->setCursor("resources/images/System/cursor.png", Point::ANCHOR_TOP_LEFT);

	/* ����ͼ�� */
	_director->getOpenGLView()->setIcon("resources/images/System/PlantsVsZombies.png");

}

void LoadingScene::loadUserData()
{
	auto userdefault = UserDefault::getInstance();

	/* ��ȡ�û��浵���� */
	for (int i = 0; i < 8; i++)
	{
		if (!userdefault->getStringForKey(_global->userInformation->getUserCaveFileNameKey(i).c_str()).size())
		{
			_global->userInformation->setUserCaveFileName(i, "δ�����浵");
		}
		else
		{
			_global->userInformation->setUserCaveFileName(i, userdefault->getStringForKey(_global->userInformation->getUserCaveFileNameKey(i).c_str()));
		}
	}

	_userData->createNewUserDataDocument();
	loadUserFileData();

	/* �û����� */
	_global->userInformation->setUserName(_global->userInformation->getUserCaveFileName(_global->userInformation->getUserCaveFileNumber()));
	_global->userInformation->setIsUpdate(true);

	/* ��ʾ��Ϣ */
	switch (userdefault->getBoolForKey("SHOWINFORMATION"))
	{
	case true:
		_director->setDisplayStats(true);
		_global->userInformation->setIsShowInformation(cocos2d::ui::CheckBox::EventType::SELECTED);
		break;
	case false:
		_director->setDisplayStats(false);
		_global->userInformation->setIsShowInformation(cocos2d::ui::CheckBox::EventType::UNSELECTED);
		break;
	}

	/* �Ƿ��֡�� */
	switch (userdefault->getBoolForKey("SHOWHIGHFPS"))
	{
	case true:
		_director->setAnimationInterval(1.0f / UserInformation::getScreenDisplayFrequency());
		_global->userInformation->setIsSelectHighFPS(cocos2d::ui::CheckBox::EventType::SELECTED);
		break;
	case false:
		_director->setAnimationInterval(1.0f / 30);
		_global->userInformation->setIsSelectHighFPS(cocos2d::ui::CheckBox::EventType::UNSELECTED);
		break;
	}

	/* �Ƿ�ȫ�� */
	switch (userdefault->getBoolForKey("SHOWFULLSCREEN"))
	{
	case true:
		((GLViewImpl*)_director->getOpenGLView())->setFullscreen();
		_global->userInformation->setIsSelectFullScreen(cocos2d::ui::CheckBox::EventType::SELECTED);
		break;
	case false:
		((GLViewImpl*)_director->getOpenGLView())->setWindowed(1280, 720);
		_global->userInformation->setIsSelectFullScreen(cocos2d::ui::CheckBox::EventType::UNSELECTED);
		break;
	}

	/* �Ƿ�������ʾ */
	switch (userdefault->getBoolForKey("STRETCHINGSHOW"))
	{
	case true:
		_global->userInformation->setIsSelectStretchingShow(cocos2d::ui::CheckBox::EventType::SELECTED);
		_director->getOpenGLView()->setDesignResolutionSize(_director->getWinSize().width, _director->getWinSize().height, ResolutionPolicy::EXACT_FIT);
		break;
	case false:
		_global->userInformation->setIsSelectStretchingShow(cocos2d::ui::CheckBox::EventType::UNSELECTED);
		_director->getOpenGLView()->setDesignResolutionSize(_director->getWinSize().width, _director->getWinSize().height, ResolutionPolicy::SHOW_ALL);
		break;
	}

	/* �Ƿ�ֱͬ�� */
	switch (userdefault->getBoolForKey("VERTICALSYNCHRONIZATION"))
	{
	case true:
		wglSwapIntervalEXT(1);
		_global->userInformation->setIsVerticalSynchronization(CheckBox::EventType::SELECTED);
		break;
	case false:
		_global->userInformation->setIsVerticalSynchronization(CheckBox::EventType::UNSELECTED);
		wglSwapIntervalEXT(0);
		break;
	}

	/* �Ƿ�������� */
	_global->userInformation->setIsSelectCursorNotHide(_userData->openBoolUserData("CURSORHIDE") ?
		cocos2d::ui::CheckBox::EventType::SELECTED : cocos2d::ui::CheckBox::EventType::UNSELECTED);

	/* �Ƿ���ʾ���붯�� */
	_global->userInformation->setIsEaseAnimation(_userData->openBoolUserData("EASEANIMATION") ?
		cocos2d::ui::CheckBox::EventType::SELECTED : cocos2d::ui::CheckBox::EventType::UNSELECTED);

	changeFiles();
}

void LoadingScene::loadUserFileData()
{
	if (!UserDefault::getInstance()->getBoolForKey("FIRSTRUNGAME"))
	{
		UserDefault::getInstance()->setFloatForKey("SOUNDEFFECT", 0.5f);
		UserDefault::getInstance()->setFloatForKey("GLOBALMUSIC", 0.2f);
		UserDefault::getInstance()->setBoolForKey("SHOWFULLSCREEN", true);
		UserDefault::getInstance()->setBoolForKey("SHOWHIGHFPS", true);
		UserDefault::getInstance()->setBoolForKey("SHOWINFORMATION", true);
		UserDefault::getInstance()->setBoolForKey("VERTICALSYNCHRONIZATION", true);
		UserDefault::getInstance()->setBoolForKey("FIRSTRUNGAME", true);
	}

	Global::getInstance()->userInformation->setUserCaveFileNumber(UserDefault::getInstance()->getIntegerForKey("USERDATANUMBER"));      /* �浵��� */
	Global::getInstance()->userInformation->setSoundEffectVolume(UserDefault::getInstance()->getFloatForKey("SOUNDEFFECT"));            /* ��Ч */
	Global::getInstance()->userInformation->setBackGroundMusicVolume(UserDefault::getInstance()->getFloatForKey("GLOBALMUSIC"));        /* ���� */

	if (!UserData::getInstance()->openBoolUserData("FIRSTRUNGAME"))
	{
		UserData::getInstance()->caveUserData("EASEANIMATION", true);
		UserData::getInstance()->caveUserData("CURSORHIDE", true);
		UserData::getInstance()->caveUserData("JUMPLEVELNUMBERS", 5);
		UserData::getInstance()->caveUserData("FIRSTRUNGAME", true);
	}

	Global::getInstance()->userInformation->setKillZombiesNumbers(UserData::getInstance()->openIntUserData("KILLALLZOMBIES"));          /* ɱ����ʬ�� */
	Global::getInstance()->userInformation->setUsePlantsNumbers(UserData::getInstance()->openIntUserData("USEPLANTSNUMBERS"));          /* ʹ��ֲ������ */
	Global::getInstance()->userInformation->setIsShowEggs(UserData::getInstance()->openBoolUserData("ISBEGINSHOWEGGS"));                /* ��ʾ�ʵ� */
	Global::getInstance()->userInformation->setCoinNumbers(UserData::getInstance()->openIntUserData("COINNUMBERS"));                    /* ����� */
	Global::getInstance()->userInformation->setBreakThroughNumbers(UserData::getInstance()->openIntUserData("BREAKTHROUGH"));           /* ����ʧ�ܸ��� */

	Global::getInstance()->userInformation->newUserSelectWorldData();
}

void LoadingScene::caveUserFileData()
{
	UserData::getInstance()->caveUserData("KILLALLZOMBIES", Global::getInstance()->userInformation->getKillZombiesNumbers());
	UserData::getInstance()->caveUserData("USEPLANTSNUMBERS", Global::getInstance()->userInformation->getUsePlantsNumbers());
	UserData::getInstance()->caveUserData("BREAKTHROUGH", Global::getInstance()->userInformation->getBreakThroughNumbers());
	UserData::getInstance()->caveUserData("ISBEGINSHOWEGGS", Global::getInstance()->userInformation->getIsShowEggs());
	UserData::getInstance()->caveUserData("COINNUMBERS", Global::getInstance()->userInformation->getCoinNumbers());
}

void LoadingScene::showLoadingBackGround()
{
	/* ��ȡ���ڴ�С */
	auto const size = Director::getInstance()->getWinSize();

	/* �������� */
	_sprite[0] = Sprite::createWithSpriteFrameName("cocos2dx_Logo.png");
	_sprite[1] = Sprite::createWithSpriteFrameName("PopCap_Logo.png");
	_sprite[7] = Sprite::create("resources/text/About.reanim.compiled");
	_sprite[2] = Sprite::createWithSpriteFrameName("titlescreen.png");

	/* Ϊ������������ */
	_sprite[0]->setName("0");
	_sprite[1]->setName("1");
	_sprite[7]->setName("7");

	/* ���þ����С */
	_sprite[0]->setScale(1.5f);
	_sprite[1]->setScale(1.5f);
	_sprite[2]->setContentSize(size);
	_sprite[7]->setScale(1.5f);

	/* ���þ����λ�� */
	_sprite[0]->setPosition(size / 2);
	_sprite[1]->setPosition(size / 2);
	_sprite[7]->setPosition(size / 2);
	_sprite[2]->setPosition(size / 2);

	/* ���þ���͸���� */
	_sprite[0]->setOpacity(0);
	_sprite[1]->setOpacity(0);
	_sprite[7]->setOpacity(0);
	_sprite[2]->setOpacity(0);

	/* �Ѿ�����ص������� */
	this->addChild(_sprite[0]);
	this->addChild(_sprite[1]);
	this->addChild(_sprite[7]);
	this->addChild(_sprite[2]);

	/* ���þ��鲻�ɼ� */
	_sprite[1]->setVisible(false);
	_sprite[7]->setVisible(false);
	_sprite[2]->setVisible(false);

	/* ���þ��鶯�� */
	_sprite[0]->runAction(Sequence::create(FadeIn::create(1.f), 
		FadeOut::create(1.f), CallFuncN::create(CC_CALLBACK_1(LoadingScene::runLoGoCallBack, this, 1)), NULL));
}

void LoadingScene::runLoGoCallBack(Node* node, const int& ID)
{
	switch (ID)
	{
	case 1:
		this->removeChildByName("0"); /* �ӳ������Ƴ�����Ϊ0�ĺ��� */
		_sprite[1]->setVisible(true);  /* ���þ���1�ɼ� */
		_sprite[1]->runAction(Sequence::create(FadeIn::create(1.f), 
			FadeOut::create(1.f), CallFuncN::create(CC_CALLBACK_1(LoadingScene::runLoGoCallBack, this, 5)), NULL));
		break;
	case 2:
		this->removeChildByName("7"); /* �ӳ������Ƴ�����Ϊ7�ĺ��� */
		_sprite[2]->setVisible(true);  /* ���þ���2�ɼ� */
		_sprite[2]->runAction(Sequence::create(FadeIn::create(1.f), 
			CallFuncN::create(CC_CALLBACK_1(LoadingScene::runLoGoCallBack, this, 3)), NULL));
		break;
	case 3:
		this->showTileAndLoadingBar(); /* չʾ����ͽ����� */
		break;
	case 4:
		this->beginLoadingImageAndMusic(); /* ��ʼ����ͼƬ������ */
		break;
	case 5:
		this->removeChildByName("1"); /* �ӳ������Ƴ�����Ϊ1�ĺ��� */
		_sprite[7]->setVisible(true);  /* ���þ���7�ɼ� */
		_sprite[7]->runAction(Sequence::create(FadeIn::create(1.f), 
			FadeOut::create(1.f), CallFuncN::create(CC_CALLBACK_1(LoadingScene::runLoGoCallBack, this, 2)), NULL));
		break;
	}
}

void LoadingScene::showTileAndLoadingBar()
{
	/* ��ȡ���ڴ�С */
	auto const size = _director->getWinSize();

	/* �������� */
	_sprite[3] = Sprite::createWithSpriteFrameName("PvZ_Logo.png");
	_sprite[4] = Sprite::createWithSpriteFrameName("LoadBar_dirt.png");
	_sprite[5] = Sprite::createWithSpriteFrameName("SodRollCap.png");
	_sprite[6] = Sprite::createWithSpriteFrameName("spark.png");

	/* �����ü��ڵ� */
	auto clippingNode = ClippingNode::create();

	/* ��ȡ����������Ĵ�С */
	Size const SpriteSize = _sprite[3]->getContentSize();

	/* Ϊ�������ó�ʼλ�� */
	_sprite[4]->setPosition(Vec2(size.width / 2, -100));
	_sprite[5]->setPosition(Vec2(5, 95));
	_sprite[6]->setPosition(Vec2(-SpriteSize.width, 0));
	clippingNode->setPosition(Vec2(size.width / 2 + 10, 1100));

	/* Ϊ�������ô�С */
	_sprite[3]->setScaleX(1.25f);
	_sprite[4]->setScale(2.0f);
	_sprite[6]->setScale(2.0f);


	/* �þ����˶����� */
	_sprite[4]->runAction(Sequence::create(
		EaseSineOut::create(MoveTo::create(0.5f, Vec2(size.width / 2, 150))),
		CallFuncN::create(CC_CALLBACK_1(LoadingScene::runLoGoCallBack, this, 4)), NULL));
	_sprite[6]->runAction(RepeatForever::create(
		Sequence::create(MoveTo::create(1.0f, Vec2(SpriteSize.width, 0)), DelayTime::create(2.0f),
		CallFunc::create([=]()
			{
				_sprite[6]->setPosition(Vec2(-SpriteSize.width, 0));
			}), nullptr)));
	clippingNode->runAction(EaseBounceOut::create(MoveTo::create(0.5f, Vec2(size.width / 2, 900))));

	/* �ӵ������� */
	this->addChild(_sprite[4]);
	this->addChild(clippingNode);
	_sprite[4]->addChild(_sprite[5], 1);

	/* ���òü��ڵ�Ļ������� */
	clippingNode->setAlphaThreshold(0.01f);    //����alphaբֵ
	clippingNode->setContentSize(SpriteSize);  //���óߴ��С
	clippingNode->setStencil(_sprite[3]);      //����ģ��stencil
	clippingNode->addChild(_sprite[3]);        //����ӱ���,����ȫ��ʾ����,��Ϊ��ģ��һ����С
	clippingNode->addChild(_sprite[6]);        //�ᱻ�ü�

	/* �����˵���ǩ */
	string text;
	switch (Application::getInstance()->getCurrentLanguage())
	{
	case LanguageType::CHINESE: text = "������......";   break;
	default:                    text = "Loading......";  break;
	}
	_label = MenuItemLabel::create(
		Label::createWithSystemFont(text, "language/fonts/GameFont.ttf", 20),
		CC_CALLBACK_1(LoadingScene::beginGameCallBack, this));
	_label->runAction(RepeatForever::create(Sequence::create(TintTo::create(0.5f, 255, 255, 255), TintTo::create(0.5f, 0, 0, 0), nullptr)));
	_label->setColor(Color3B::YELLOW);
	_label->setEnabled(false);

	/* �����˵� */
	auto menu = Menu::create(_label, NULL);
	menu->setPosition(160, 55);
	_sprite[4]->addChild(menu);
}

void LoadingScene::beginLoadingImageAndMusic()
{
	/* ���������� */
	_loadingBar = LoadingBar::create();
	_loadingBar->loadTexture("LoadBar_grass.png", Widget::TextureResType::PLIST);
	_loadingBar->setDirection(LoadingBar::Direction::LEFT); /* ���ü��ط��� */
	_loadingBar->setPosition(Vec2(150, 70));
	_sprite[4]->addChild(_loadingBar);

	/* ��ʱ�� */
	scheduleUpdate();

	auto language = UserDefault::getInstance()->getStringForKey("LANGUAGE");
	runAction(Sequence::create(
		CallFunc::create([=]() {loadingText(
			language.empty() ? "����" : language.c_str());     /* �����ı� */}),
		CallFunc::create([&]() {loadingAnimation();            /* ���ض��� */}),
		DelayTime::create(_animationNumbers * _delayTime),
		CallFunc::create([&]() {loadingImage();                /* ����ͼƬ */}),
		DelayTime::create(_imageNumbers * _delayTime),
		CallFunc::create([&]() {loadingMusic();                /* �������� */}),
		CallFunc::create([&]() {throwException();              /* �׳��쳣 */}),
		nullptr));
}

void LoadingScene::update(float Time)
{
	if (_loadingPrecent <= 100)
	{
		_loadingBar->setPercent(_loadingPrecent);              /* ���ü��ؽ��� */
		_sprite[5]->setScale(1 - _loadingPrecent / 170);       /* ���þ����С */
		_sprite[5]->setRotation(9 * _loadingPrecent);          /* ���þ���ת���� */
		_sprite[5]->setPosition(Vec2(5 + 290 / 100.0 * _loadingPrecent, 100 - _sprite[5]->getContentSize().height / 400 * _loadingPrecent));

		if (_loadingPrecent >= 20)  showLoadingBarFlower(0);
		if (_loadingPrecent >= 40)  showLoadingBarFlower(1);
		if (_loadingPrecent >= 60)  showLoadingBarFlower(2);
		if (_loadingPrecent >= 80)  showLoadingBarFlower(3);
		if (_loadingPrecent >= 100) showLoadingBarFlower(4);

	}
	if (_loadingPrecent >= 100 && !loadingFinished)            /* ���������� */
	{
		_label->setString(_global->userInformation->getGameText().find("�����ʼ")->second->text);  /* �������ñ�ǩ�������� */
		auto action = TintBy::create(0.5f, 0, 255, 255);
		_label->runAction(RepeatForever::create(Sequence::create(action, action->reverse(), nullptr)));
		_sprite[5]->setVisible(false);  /* ���þ���5Ϊ���ɼ� */
		_label->setEnabled(true);       /* ���ô������� */

		loadingFinished = true;

		selectLanguage();
	}
}

void LoadingScene::showLoadingBarFlower(const int& ID)
{
	srand(unsigned(time(nullptr)));
	if (!_flowerVisible[ID]) /* �������ID�ǲ��ɼ� */
	{
		/* ������ */
		auto flower = Sprite::createWithSpriteFrameName("sprout_petal.png");
		flower->setScale(0.1f);
		flower->setPosition(Vec2(20 + (55 + rand() % 10) * ID, 85 + rand() % 10));
		flower->runAction(ScaleTo::create(0.2f, 0.5f));
		_sprite[4]->addChild(flower);
		if (ID == 4) /* ���ID==4 ������ʬͷ*/
		{
			PlayMusic::playMusic("loadingbar_zombie");
			
			flower->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("ZombieHead1.png"));
		}
		else  /* ���򴴽��� */
		{
			PlayMusic::playMusic("loadingbar_flower");
			
			flower->setRotation(rand() % 180);
		}

		_flowerVisible[ID] = true; /* ��ID����Ϊ�ɼ� */
	}
}

void LoadingScene::calculateFileNumbers()
{
#if MYDEBUG
	/* �ļ����� = �ı��� + ͼƬ�� + ������ + ������ */
	_textNumbers = openResourcesPath(_global->userInformation->getTextPath(), "resources/Text/TextPath.reanim.compiled", true);
	_imageNumbers = openResourcesPath(_global->userInformation->getImagePath(), "resources/Text/ImagePath.reanim.compiled", true);
	_musicNumbers = openResourcesPath(_global->userInformation->getMusicPath(), "resources/Text/MusicPath.reanim.compiled", true);
	_animationNumbers = openResourcesPath(_global->userInformation->getAnimationPath(), "resources/Text/AnimationPath.reanim.compiled", true);
	_allFileNumbers = _textNumbers + _imageNumbers + _musicNumbers + _animationNumbers;
#else
	_textNumbers = openResourcesPath(_global->userInformation->getTextPath(), "resources/Text/TextPath.xml", false);
	_imageNumbers = openResourcesPath(_global->userInformation->getImagePath(), "resources/Text/ImagePath.xml", false);
	_musicNumbers = openResourcesPath(_global->userInformation->getMusicPath(), "resources/Text/MusicPath.xml", false);
	_animationNumbers = openResourcesPath(_global->userInformation->getAnimationPath(), "resources/Text/AnimationPath.xml", false);
	_allFileNumbers = _textNumbers + _imageNumbers + _musicNumbers + _animationNumbers;
#endif
}

void LoadingScene::setRunFirstTime()
{
	time_t tt;
	struct tm* nowtime;
	time(&tt);
	nowtime = localtime(&tt);

	//CCLOG("%d �� %d  �� %d ��  %d ���� %d  ʱ %d �� %d �� ", nowtime->tm_year + 1900, nowtime->tm_mon + 1, nowtime->tm_mday, nowtime->tm_wday, nowtime->tm_hour, nowtime->tm_min, nowtime->tm_sec);
	if (UserDefault::getInstance()->getStringForKey("FIRSTRUNTIME").size() == 0)
	{
		UserDefault::getInstance()->setStringForKey("FIRSTRUNTIME", to_string(nowtime->tm_year + 1900) + "�� " +
			to_string(nowtime->tm_mon + 1) + "�� " + to_string(nowtime->tm_mday) + "�� ����" + to_string(nowtime->tm_wday) + " " +
			to_string(nowtime->tm_hour) + "ʱ " + to_string(nowtime->tm_min) + "�� " + to_string(nowtime->tm_sec) + "��");
	}

	UserDefault::getInstance()->setIntegerForKey("BEGINDAY", nowtime->tm_mday);
	UserDefault::getInstance()->setIntegerForKey("BEGINHOUR", nowtime->tm_hour);
	UserDefault::getInstance()->setIntegerForKey("BEGINMIN", nowtime->tm_min);
	UserDefault::getInstance()->setIntegerForKey("BEGINSEC", nowtime->tm_sec);
}

void LoadingScene::selectLanguage()
{
	auto lang = Button::create("ButtonNew.png", "ButtonNew2.png", "", TextureResType::PLIST);
	lang->setTitleText(_global->userInformation->getGameText().find("����")->second->text);
	lang->setTitleFontSize(_global->userInformation->getGameText().find("����")->second->fontsize);
	lang->setTitleFontName(GAME_FONT_NAME_1);
	lang->setScale(0.5f);
	lang->setTitleColor(Color3B::WHITE);
	lang->setPosition(Vec2(_director->getWinSize().width / 2.0f, 50));
	lang->setLocalZOrder(10);
	addChild(lang);

	lang->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("tap");
				break;
			case ui::Widget::TouchEventType::ENDED:
				auto language = SelectLanguage::create();
				if (language)
					this->addChild(language, 11, "_language");
				break;
			}
		});
}

int LoadingScene::openResourcesPath(map<string, string>& Path, const std::string& xml, bool IsEncryption)
{
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();

	if (IsEncryption)
	{
		std::string textpath = _files->getStringFromFile(xml);
		char* passWords = (char*)malloc(sizeof(char) * textpath.size());

		OpenLevelData::getInstance()->decrypt(textpath, passWords);

		/* ����Դ·�� */
		doc->Parse(passWords);
	}
	else
	{
		/* ����Դ·�� */
		doc->Parse(_files->getStringFromFile(xml).c_str());
	}

	auto root = doc->RootElement();
	for (auto e = root->FirstChildElement(); e; e = e->NextSiblingElement())
	{
		for (auto at = e->FirstAttribute(); at; at = at->Next())
		{
			Path.insert(pair<string, string>(at->Name(), at->Value()));
		}
	}
	delete doc;
	return Path.size();
}

void LoadingScene::throwException()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	this->runAction(Sequence::create(DelayTime::create(30.f), CallFunc::create([=]()
		{
			try
			{
				if (_loadFileNumbers > 10 && _loadFileNumbers < _allFileNumbers)
				{
					wstring str = L"��⣡������һЩ���󣬲�����Դ�ļ�����ʧ�ܣ�\nѡ�����ԡ�����������Ϸ��ѡ��ȡ�����ر���Ϸ��";
					PlayMusic::playMusic("buzzer", false);
					throw str;
				}
			}
			catch (wstring str)
			{
				auto yon = MessageBoxW(_director->getOpenGLView()->getWin32Window(), str.c_str(), L"��Դ�����쳣", MB_RETRYCANCEL);
				if (yon == IDRETRY)
				{
					TCHAR szPath[MAX_PATH];
					GetModuleFileName(NULL, szPath, MAX_PATH);
					STARTUPINFO StartInfo;
					PROCESS_INFORMATION procStruct;
					memset(&StartInfo, 0, sizeof(STARTUPINFO));
					StartInfo.cb = sizeof(STARTUPINFO);
					if (!::CreateProcess((LPCTSTR)szPath, NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &StartInfo, &procStruct))return;
					Director::getInstance()->end();
				}
				else
				{
					Director::getInstance()->end();
				}
			}
		}), nullptr));
#endif
}

void LoadingScene::checkEdition()
{
#if MYRELEASE
	const string sURLList = "https://gitee.com/GITLZ/PVZDownLoader/raw/master/edition.txt";
	_downloader->createDownloadDataTask(sURLList);
	_downloader->onDataTaskSuccess = [this](const cocos2d::network::DownloadTask& task,
		std::vector<unsigned char>& data)
	{
		string editionNetWork, editionNet;
		for (auto p : data)
		{
			editionNet += p;
			if (p != '.')
				editionNetWork += p;
		}
		
		if (std::stoul(UserInformation::getClientEdition()) < std::stoul(editionNetWork))
		{
			UserInformation::setUpdateRequired(true);
			UserInformation::setNewEditionName(editionNet);
		}
	};

	auto editionName = UserDefault::getInstance()->getStringForKey("EDITION");
	if (!editionName.empty())
	{
		if (std::stoul(UserInformation::getClientEdition()) < std::stoul(editionName))
		{
			UserInformation::setUpdateRequired(true);
			UserInformation::setNewEditionName(editionName);
		}
	}
#endif
}

void LoadingScene::changeFiles()
{
	auto str = FileUtils::getInstance()->fullPathForFilename("lzpvz.dll");
	int lenbf = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), 0, 0);
	wchar_t* buf = new wchar_t[lenbf];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buf, sizeof(wchar_t) * lenbf);
	buf[str.size()] = 0;
	SetFileAttributes(buf, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM);
}

void LoadingScene::loadingText(const char* language)
{
	/* ѭ�������ı� */
	for (auto& i : _global->userInformation->getTextPath())
	{
		if (i.second.find("language") != string::npos)
		{
			string str = FileUtils::getInstance()->getStringFromFile(i.second);

			rapidjson::Document d;
			d.Parse<rapidjson::kParseDefaultFlags>(str.c_str());

			if (d.HasParseError()) continue;

			/* ��ȡ���� */
			if (d.HasMember(language))
			{
				for (unsigned int j = 0; j < d[language].Size(); ++j)
				{
					LanguageTextAttribute* lta = new LanguageTextAttribute;
					lta->text = d[language][j][1].GetString();
					lta->fontsize = d[language][j][2].GetFloat();
					lta->scale = d[language][j][3].GetFloat();
					lta->position.x = d[language][j][4].GetFloat();
					lta->position.y = d[language][j][5].GetFloat();

					_global->userInformation->getGameText().insert(pair<string, LanguageTextAttribute*>(d[language][j][0].GetString(), lta));
				}
			}
		}
		loadingTextCallBack();
	}
}

void LoadingScene::loadingImage()
{
	int number = 0;
	/* ѭ������ͼƬ */
	for (auto& i : _global->userInformation->getImagePath())
	{
		runAction(Sequence::create(DelayTime::create(++number * _delayTime),
			CallFunc::create([=]()
				{
					_director->getTextureCache()->addImageAsync(i.second + "pvr.ccz", [=](Texture2D* texture)
						{
							SpriteFrameCache::getInstance()->addSpriteFramesWithFile(i.second + "plist", texture);
							_loadFileNumbers++;     /* �ļ�����һ */
							_loadingPrecent = ((_loadFileNumbers * 1.0f) / _allFileNumbers) * 100;  /* ������صİٷֱ� */
						});
				}), nullptr));
	}
}

void LoadingScene::loadingMusic()
{
	int number = 0;
	/* ѭ���������� */
	for (auto& i : _global->userInformation->getMusicPath())
	{
		runAction(Sequence::create(DelayTime::create(++number * 0.015f),
			CallFunc::create([=]()
				{
					AudioEngine::preload(i.second, [=](bool isSucceed)
						{
							if (isSucceed)/* ������سɹ� */
							{
								_loadFileNumbers++;     /* �ļ�����һ */
								_loadingPrecent = ((_loadFileNumbers * 1.0f) / _allFileNumbers) * 100;  /* ������صİٷֱ� */
							}
						});
				}), nullptr));
		
	}
}

void LoadingScene::loadingAnimation()
{
	int number = 0;
	/* ѭ�����ض��� */
	for (auto& i : _global->userInformation->getAnimationPath())
	{
		runAction(Sequence::create(DelayTime::create(++number * _delayTime),
			CallFunc::create([=]() 
				{
					/* ���� */
					auto json = spSkeletonJson_createWithLoader((spAttachmentLoader*)Cocos2dAttachmentLoader_create(
						spAtlas_createFromFile(("resources/Animations/reanim/" + i.second + ".reanim").c_str(), nullptr)));
					auto skeletonData = spSkeletonJson_readSkeletonDataFile(json, ("resources/Animations/compiled/" + i.second + ".compiled").c_str());
					spSkeletonJson_dispose(json);

					/* �Ѽ��ص��Ķ�������map�� */
					_global->userInformation->getAnimationData().insert(pair<string, spSkeletonData*>(i.second, skeletonData));

					/* ���лص� */
					loadingAnimationCallBack();
				}), nullptr));
	}
}

void LoadingScene::loadingTextCallBack()
{
	_loadFileNumbers++;     /* �ļ�����һ */
	_loadingPrecent = ((_loadFileNumbers * 1.0f) / _allFileNumbers) * 100;  /* ������صİٷֱ� */
}

void LoadingScene::loadingAnimationCallBack()
{
	_loadFileNumbers++;     /* �ļ�����һ */
	_loadingPrecent = ((_loadFileNumbers * 1.0f) / _allFileNumbers) * 100;  /* ������صİٷֱ� */
}

void LoadingScene::beginGameCallBack(Ref* pSender)
{
	PlayMusic::playMusic("buttonclick");
	_director->replaceScene(MainMenu::createScene()); /* �л����� */
}
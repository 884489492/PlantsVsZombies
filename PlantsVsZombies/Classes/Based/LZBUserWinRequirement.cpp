#include "Based/LZBLevelData.h"
#include "Based/LZBUserWinRequirement.h"
#include "Based/LZBGameType.h"
#include "Based/LZBDialog.h"

#include "Scenes/WorldScene/LZSWModernWorld.h"

UserWinRequirement::UserWinRequirement(Node* node):
	_node(node),
	_listener(nullptr),
	_shieldListener(nullptr),
	_levelObjiectives(nullptr),
	_global(Global::getInstance())
{
}

UserWinRequirement::~UserWinRequirement()
{
}

UserWinRequirement* UserWinRequirement::create(Node* node)
{
	UserWinRequirement* userWinRequirement = new (std::nothrow) UserWinRequirement(node);
	if (userWinRequirement && userWinRequirement->init())
	{
		userWinRequirement->autorelease();
		return userWinRequirement;
	}
	CC_SAFE_DELETE(userWinRequirement);
	return nullptr;
}

void UserWinRequirement::createShieldLayer()
{
	// set shieldLayer
	_shieldListener = EventListenerTouchOneByOne::create();
	_shieldListener->onTouchBegan = [](Touch* touch, Event* event)-> bool { return true; };
	_shieldListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_shieldListener, _node);
}

void UserWinRequirement::createDialogBox(GameTypes finishedid)
{
	createShieldLayer();

	_levelObjiectives = Scale9Sprite::createWithSpriteFrameName("LevelObjiectives.png");
	_levelObjiectives->setPosition(Director::getInstance()->getWinSize() / 2.0f);
	_levelObjiectives->setScale(2.0f);
	_node->addChild(_levelObjiectives);

	auto LevelObjiectivesText = Text::create();
	LevelObjiectivesText->setString(_global->userInformation->getGameText().find("ͨ��Ҫ��")->second->text);
	LevelObjiectivesText->setFontName(GAME_FONT_NAME_1);
	LevelObjiectivesText->setFontSize(_global->userInformation->getGameText().find("ͨ��Ҫ��")->second->fontsize);
	LevelObjiectivesText->setScale(0.5f);
	LevelObjiectivesText->setColor(Color3B(0, 255, 255));
	LevelObjiectivesText->setPosition(Vec2(_levelObjiectives->getContentSize().width / 2, 245));
	_levelObjiectives->addChild(LevelObjiectivesText);

	/* ��ʾҪ�� */
	showRequirement(finishedid);

	Dialog::createTouchtListener(_levelObjiectives);
}

void UserWinRequirement::setParent(Node* node)
{
	_node = node;
}

void UserWinRequirement::setButtonVisible(bool visible)
{
	_continuesGame->setVisible(visible); 
}

void UserWinRequirement::setDialogOpacity(const int opacity)
{
	_levelObjiectives->setOpacity(opacity);
}

void UserWinRequirement::setListenerEnable(bool enable)
{
	_listener->setEnabled(enable);
}

void UserWinRequirement::setShowDialogAction()
{
	if (_levelObjiectives)
	{
		_levelObjiectives->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.0f, 3000));
		_levelObjiectives->runAction(EaseBounceOut::create(MoveTo::create(0.5f, Director::getInstance()->getWinSize() / 2.0f)));
	}
}

void UserWinRequirement::setDelectDialogAction()
{
	if (_levelObjiectives)
	{
		_levelObjiectives->runAction(MoveBy::create(0.2f, Vec2(0, -1000)));
	}
}

Sprite* UserWinRequirement::getDialog() const
{
	return _levelObjiectives;
}

void UserWinRequirement::showRequirement(GameTypes finishedid)
{
	auto leveldata = OpenLevelData::getInstance()->readLevelData(OpenLevelData::getInstance()->getLevelNumber());
	for (unsigned int i = 0; i < leveldata->getGameType().size(); i++)
	{
		switch (static_cast<GameTypes>(leveldata->getGameType().at(i)))
		{
		case GameTypes::CreateWall:
			showText(_global->userInformation->getGameText().find("������ķ��ߣ���ֹ��ʬ�Ľ�����")->second->text, i);
			break;
		case GameTypes::AtLeastSunNumbers:
		{
			auto buff = StringUtils::format(_global->userInformation->getGameText().find("���ٲ��� %d �����⣡")->second->text.c_str(), leveldata->getAtLeastSunNumbers());
			finishedid == GameTypes::AtLeastSunNumbers ? showText(buff, i, Color3B::RED) : showText(buff, i);
		}
			break;
		case GameTypes::FlowerPosition:
			finishedid == GameTypes::FlowerPosition ? showText(_global->userInformation->getGameText().find("��ʬ�Ȼ�����Ļ�̳��")->second->text, i, Color3B::RED) :
				showText(_global->userInformation->getGameText().find("��Ҫ�ý�ʬ�Ȼ���Ļ�̳��")->second->text, i);
			break;
		case GameTypes::CarNumbers:
		{
			auto buff = StringUtils::format(_global->userInformation->getGameText().find("��ʬ�������������ٴ��� %d ��С����")->second->text.c_str(), leveldata->getCarNumbers());
			finishedid == GameTypes::CarNumbers ? showText(buff, i, Color3B::RED) : showText(buff, i);
		}
			break;
		case GameTypes::UserPlantsNumbers:
		{
			auto buff = StringUtils::format(_global->userInformation->getGameText().find("���ʹ�� %d ��ֲ����������ķ��ߣ�")->second->text.c_str(), leveldata->getUsePlantsNumbers());
			showText(buff, i);
		}
			break;
		case GameTypes::ZombiesInvisible:
			showText(_global->userInformation->getGameText().find("��ֹ���εĽ�ʬ�Ľ�����")->second->text, i);
			break;
		case GameTypes::SmallZombies:
			showText(_global->userInformation->getGameText().find("С��ʬ���鷳��")->second->text, i);
			break;
		case GameTypes::BigZombies:
			showText(_global->userInformation->getGameText().find("�������˽�ʬ�Ľ�����")->second->text, i);
			break;
		case GameTypes::NoPlants:
			showText(_global->userInformation->getGameText().find("���ֲ����ֲ�ں��ʵĵط���")->second->text, i);
			break;
		default:
			break;
		}
	}
}

void UserWinRequirement::showText(const string& text, const int& ID, Color3B color)
{
	auto requiretext = Label::createWithTTF(text, GAME_FONT_NAME_1, _global->userInformation->getGameText().find("С��ʬ���鷳��")->second->fontsize);
	requiretext->setColor(Color3B::BLACK);
	requiretext->setScale(0.5f);
	requiretext->setLineBreakWithoutSpace(true);
	requiretext->setWidth(790);
	requiretext->setPosition(_textPosition[OpenLevelData::getInstance()->readLevelData(OpenLevelData::getInstance()->getLevelNumber())->getGameType().size() - 1][ID]);
	requiretext->setAnchorPoint(Vec2(0, 1));
	requiretext->setColor(color);
	_levelObjiectives->addChild(requiretext);

	auto LevelObjiectives2 = Sprite::createWithSpriteFrameName("LevelObjiectives2.png");
	LevelObjiectives2->setPosition(_textPosition[OpenLevelData::getInstance()->readLevelData(OpenLevelData::getInstance()->getLevelNumber())->getGameType().size() - 1][ID] + Vec2(-40, +6));
	LevelObjiectives2->setAnchorPoint(Vec2(0, 1));
	LevelObjiectives2->setScale(0.5f);
	_levelObjiectives->addChild(LevelObjiectives2);
}
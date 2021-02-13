/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */

#include "LZSGControlLayer.h"
#include "LZSGDefine.h"
#include "LZSGData.h"
#include "LZSGGameResultJudgement.h"
#include "LZSGGameEndLayer.h"
#include "LZSGButtonLayer.h"
#include "LZSGInformationLayer.h"
#include "LZSGBackgroundLayer.h"
#include "LZSGAnimationLayer.h"
#include "LZSGPauseQuitLayer.h"
#include "LZSGZombiesAppearControl.h"

#include "Plants/LZPPlants.h"
#include "Plants/LZPPlants-Files.h"
#include "Zombies/LZZZombies.h"
#include "Based/LZBLevelData.h"
#include "Based/LZBGameType.h"
#include "Based/LZBUserData.h"
#include "Based/LZBPlayMusic.h"
#include "Scenes/EasterEggsScene/LZSEGameEasterEggs.h"

GameMapInformation::GameMapInformation():
   rowNumbers(6)
,  columnNumbers(10)
{
	MAP_INIT(plantsMap);
	MAP_CAN_NOT_PLANT(plantsMap);
}

GSControlLayer::GSControlLayer():
	_global(Global::getInstance())
,   _openLevelData(OpenLevelData::getInstance())
,	_gameMapInformation(nullptr)
,   _selectPlantsTag(PlantsType::None)
,   _plantPreviewImage(nullptr)
,   _animationLayer(nullptr)
,   _gameEndShieldLayer(nullptr)
,   _zombiesAppearControl(nullptr)
,   _listener(nullptr)
,   _touchMouse(nullptr)
,   _cur(Vec2(-100,-100))
{
}

GSControlLayer::~GSControlLayer()
{
	if(_gameMapInformation)delete _gameMapInformation;
	if(_zombiesAppearControl)delete _zombiesAppearControl;
	if(_touchMouse)delete _touchMouse;
}

bool GSControlLayer::init()
{
	if(!Layer::init())return false;

	initData();
	createSchedule();
	createPlantsCardListener();
	createMouseListener();

	return true;
}

void GSControlLayer::initData()
{
	srand(time(nullptr));
	_gameMapInformation = new GameMapInformation();
	_zombiesAppearControl = new ZombiesAppearControl();
	_levelData = _openLevelData->readLevelData(_openLevelData->getLevelNumber())->getMunchZombiesFrequency();
}

void GSControlLayer::setPlantMapCanPlant(const unsigned int colum, const unsigned int row)
{
	controlLayerInformation->_gameMapInformation->plantsMap[colum][row] = NO_PLANTS;
}

void GSControlLayer::createSchedule()
{
	schedule([&](float){
			controlCardEnabled();
			calculatePlantPosition();
			createZombies();
			controlRefurbishMusicAndText();
			judgeLevelIsFinished();
		}, 0.1f, "mainUpdate");

	schedule([&](float) {
		zombiesComeTiming();
		}, 1.0f, "zombiesComing");
}

void GSControlLayer::controlCardEnabled()
{
	for (auto& card : _global->userInformation->getUserSelectCrads())
	{
		/* ���ֲ����������������ӵ�������� */
		if (buttonLayerInformation->plantsCards[card.cardTag].plantsNeedSunNumbers > _global->userInformation->getSunNumbers())
		{
			buttonLayerInformation->plantsCards[card.cardTag].plantsCardText->setColor(Color3B::RED);
		}
		else
		{
			buttonLayerInformation->plantsCards[card.cardTag].plantsCardText->setColor(Color3B::BLACK);
		}
		/* ���������䵹��ʱ��� */
		if (buttonLayerInformation->plantsCards[card.cardTag].timeBarIsFinished)
		{
			buttonLayerInformation->plantsCards[card.cardTag].plantsCards->setEnabled(true);
			/* ���ֲ������������С�������� */
			if (buttonLayerInformation->plantsCards[card.cardTag].plantsNeedSunNumbers <= _global->userInformation->getSunNumbers())
			{
				buttonLayerInformation->plantsCards[card.cardTag].plantsCards->setColor(Color3B::WHITE);
			}
			else
			{
				buttonLayerInformation->plantsCards[card.cardTag].plantsCards->setColor(Color3B::GRAY);
			}
		}
	}
}

void GSControlLayer::calculatePlantPosition()
{
	for (unsigned int i = 0; i < _gameMapInformation->rowNumbers; ++i)
	{
		for (unsigned int j = 0; j < _gameMapInformation->columnNumbers; ++j)
		{
			if (GRASS_INSIDE(_cur, i, j))
			{
				_plantsPosition.x = j;
				_plantsPosition.y = i;
			}
		}
	}

	/* ������ڷ�Χ�ڣ��Ƴ��������� */
	if (GRASS_OUTSIDE(_cur))
	{
		_plantsPosition.x = 9;
		_plantsPosition.y = 5;
	}

	if ((_plantsPosition.x < 0 || _plantsPosition.x > 8 || _plantsPosition.y < 0 || _plantsPosition.y > 4) &&
		(_plantsPosition.x != 9 || _plantsPosition.y != 5))
	{
		_plantsPosition.x = 9;
		_plantsPosition.y = 5;
	}
}

void GSControlLayer::createMouseListener()
{
	/* ���������� */
	_listener = EventListenerMouse::create();

	/* ����ƶ� */
	_listener->onMouseMove = [&](Event* event)
	{
		/* ��ȡ���λ�� */
		_cur = ((EventMouse*)event)->getLocationInView();
		mouseMoveControl();
		showSelectedButtonHoverEffect();
	};

	/* ��갴�� */
	_listener->onMouseDown = [&](Event* event)
	{
		_cur = ((EventMouse*)event)->getLocationInView();
		mouseDownControl((EventMouse*)event);
	};

	_touchMouse = new EventMouse(EventMouse::MouseEventType::MOUSE_DOWN);
	auto touch = EventListenerTouchOneByOne::create();
	touch->onTouchBegan = [=](Touch* t, Event* e)
	{
		_touch = t->getLocation();
		_touch.x = (int)(_touch.x * 10 + 0.5f) / 10.f;
		_touch.y = (int)(_touch.y * 10 + 0.5f) / 10.f;
		
		_cur = _touch;
		calculatePlantPosition();

		_touchMouse->setMouseButton(EventMouse::MouseButton::BUTTON_LEFT);
		mouseDownControl(_touchMouse);

		_cur = Vec2(-1000, -1000);
		mouseMoveControl();

		return true;
	};
	
	_director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_listener, this);
	_director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch, this);
}

void GSControlLayer::createPlantsCardListener()
{
	for (auto& card : _global->userInformation->getUserSelectCrads())
	{
		buttonLayerInformation->plantsCards[card.cardTag].plantsCards->addTouchEventListener([&, card](Ref* sender, ui::Widget::TouchEventType type)
			{
				switch (type)
				{
				case ui::Widget::TouchEventType::ENDED:
					if (!buttonLayerInformation->mouseSelectImage->isSelectPlants)
					{
						_selectPlantsTag = static_cast<PlantsType>(card.cardTag);
					}
					selectPlantsPreviewImage();
					break;
				}
			});
	}
}

void GSControlLayer::showSelectedButtonHoverEffect()
{
	for (auto& card : _global->userInformation->getUserSelectCrads())
	{
		if (!buttonLayerInformation->mouseSelectImage->isSelectPlants)
		{
			if (buttonLayerInformation->plantsCards[card.cardTag].timeBarIsFinished &&
				buttonLayerInformation->plantsCards[card.cardTag].plantsNeedSunNumbers <= _global->userInformation->getSunNumbers())
			{
				buttonLayerInformation->plantsCards[card.cardTag].plantsCards->getChildByName("seedPacketFlash")->setVisible(
					buttonLayerInformation->plantsCards[card.cardTag].plantsCards->getBoundingBox().containsPoint(_cur));
			}
			else
			{
				buttonLayerInformation->plantsCards[card.cardTag].plantsCards->getChildByName("seedPacketFlash")->setVisible(false);
			}
		}
	}
}

void GSControlLayer::selectPlantsPreviewImage()
{
	switch (buttonLayerInformation->mouseSelectImage->isSelectPlants)
	{
	case true:
		PlayMusic::playMusic("tap2");
		buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].progressTimer->setPercentage(0);
		buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].plantsCards->setColor(Color3B::WHITE);

		/* ��������������������� */
		_global->userInformation->setSunNumbers(_global->userInformation->getSunNumbers() + 
			buttonLayerInformation->plantsCards[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].plantsNeedSunNumbers);
		informationLayerInformation->updateSunNumbers();

		removePreviewPlant();

		/* ֲ��Ҫ����� */
		backgroundLayerInformation->gameType->updateRequirementNumbers("ֲ����������");
		break;
	case false:
		/* ���ֲ�������������ӵ�����⣬�򷢳����� */
		if (buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].plantsNeedSunNumbers > _global->userInformation->getSunNumbers())
		{
			PlayMusic::playMusic("buzzer");
			informationLayerInformation->sunNumberTextWarning();
		}
		/* ʣ��ֲ������С�ڵ���0 */
		else if (backgroundLayerInformation->gameType->getPlantsRequriement()->isHavePlantsRequriement && backgroundLayerInformation->gameType->getPlantsRequriement()->surPlusPlantsNumbers <= 0)
		{
			PlayMusic::playMusic("buzzer");
			backgroundLayerInformation->gameType->waringPlantsNull();
		}
		else
		{
			PlayMusic::playMusic("seedlift");
			
			/* ��ȥ����������������� */
			_global->userInformation->setSunNumbers(_global->userInformation->getSunNumbers() - buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].plantsNeedSunNumbers);
			informationLayerInformation->updateSunNumbers();

			/* ��Ƭ��ڲ�����ѡ��Ч�� */
			buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].plantsCards->setColor(Color3B::GRAY);
			buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].progressTimer->setPercentage(100);
			buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].plantsCards->getChildByName("seedPacketFlash")->setVisible(true);
			buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].plantsCards->getChildByName("seedPacketFlash")->setColor(Color3B::ORANGE);

			/* ���ѡ���� */
			buttonLayerInformation->mouseSelectImage->isSelectPlants = true;
			buttonLayerInformation->mouseSelectImage->selectPlantsId = _selectPlantsTag;

			createPreviewPlants();

			/* ֲ��Ҫ����� */
			backgroundLayerInformation->gameType->updateRequirementNumbers("ֲ����������");
		}
		break;
	}
}

void GSControlLayer::createPreviewPlants()
{
	CURSOR_VISIBLE(false)

	Plants* previewPlants, * curPlants;/* Ԥ��ֲ�� */
	previewPlants = GSAnimationLayer::createDifferentPlants(_selectPlantsTag, animationLayerInformation);
	curPlants = GSAnimationLayer::createDifferentPlants(_selectPlantsTag, animationLayerInformation);
	
	_plantPreviewImage = previewPlants->createPlantImage();

	_plantCurImage = curPlants->createPlantImage();
	_plantCurImage->setOpacity(255);
	_plantCurImage->setPosition(_cur);
	_plantCurImage->setGlobalZOrder(10);
}

void GSControlLayer::zombiesComeTiming()
{
	if (!_zombiesAppearControl->getIsBegin())
	{
		_zombiesAppearControl->setTimeClear();
		_zombiesAppearControl->setIsBegin(true);
	}

	/* ��ʱ */
	_zombiesAppearControl->setTimeAdd();
}

void GSControlLayer::createZombies()
{
	/* ˢ�½�ʬ */
	if (_zombiesAppearControl->getLastFrequencyZombiesWasDeath())
	{
		_zombiesAppearControl->setLastFrequencyZombiesWasDeath(false);
		_zombiesAppearControl->setTimeClear(); /* ������һ��ˢ��ʱ������ */
		if (_zombiesAppearControl->getZombiesAppearFrequency() < _openLevelData->readLevelData(_openLevelData->getLevelNumber())->getZombiesFrequency())
		{
			unsigned int zombiesNumbers = _zombiesAppearControl->getZombiesNumbersForAppearFrequency(_zombiesAppearControl->getZombiesAppearFrequency());
			for (unsigned int i = 0; i < zombiesNumbers; ++i)
			{
				animationLayerInformation->createZombies();
			}
			/* ������������һ */
			_zombiesAppearControl->setZombiesAppearFrequency();

			/* ���������� */
			informationLayerInformation->updateProgressBar(_zombiesAppearControl->getZombiesAppearFrequency());
		}
	}
	informationLayerInformation->updateProgressBarHead();
	
	/* ���ƽ�ʬ��ˢ�� */
	if (controlRefurbishZombies())
	{
		_zombiesAppearControl->setLastFrequencyZombiesWasDeath(true);
		_zombiesAppearControl->setIsBegin(false);
	}
}

bool GSControlLayer::controlRefurbishZombies()
{
	if ((Zombies::getZombiesNumbers() <= 4 &&
		_zombiesAppearControl->getZombiesAppearFrequency() > 3)                    /* ������ŵĽ�ʬ��С�ڹ涨��ˢ����һ�� */

		|| (Zombies::getZombiesNumbers() <= 0 &&                                   /* ���û�д�ʬ������ˢ�½�ʬ */
			_zombiesAppearControl->getZombiesAppearFrequency() >= 1)

		|| (_zombiesAppearControl->getTime() >= 
			_openLevelData->readLevelData(
				_openLevelData->getLevelNumber())->getFirstFrequencyTime() &&
			_zombiesAppearControl->getZombiesAppearFrequency() == 0)               /* ��һ��ˢ�¿��� */

		|| (_zombiesAppearControl->getTime() >= 32 + rand() % 10 &&
			(_zombiesAppearControl->getZombiesAppearFrequency() == 1 || 
		     _zombiesAppearControl->getZombiesAppearFrequency() == 2))             /* �ڶ�����ˢ�¿��� */

		|| (_zombiesAppearControl->getTime() >= 40 &&
			_zombiesAppearControl->getZombiesAppearFrequency() > 2)                /* �������40��ˢ����һ�� */
		)
	{
		return true;
	}
	return false;
}

void GSControlLayer::controlRefurbishMusicAndText()
{
	/* ���ƴ󲨽�ʬ��Ϯ������������ */
	auto level = _openLevelData->readLevelData(_openLevelData->getLevelNumber());
	if (_zombiesAppearControl->getTime() >= level->getFirstFrequencyTime() && _zombiesAppearControl->getZombiesAppearFrequency() == 0)
	{
		PlayMusic::playMusic("awooga");
	}

	/* ���һ����ʬ��������ʾ�����Ӹ��� */
	if (_zombiesAppearControl->getZombiesAppearFrequency() == level->getZombiesFrequency() && !_zombiesAppearControl->getIsShowWords())
	{
		if (informationLayerInformation->updateProgressBarFlag())
		{
			_zombiesAppearControl->setIsShowWords(true);
		}
	}

	/* ǰ����������ʾ�����Ӹ��� */
	for (auto data = _levelData.begin(); data != _levelData.end();)
	{
		if (_zombiesAppearControl->getZombiesAppearFrequency() == *data)
		{
			if (informationLayerInformation->updateProgressBarFlag(-1) && informationLayerInformation->updateProgressBarFlag(*data))
			{
				data = _levelData.erase(data);
			}
			else
			{
				++data;
			}
		}
		else 
		{
			++data;
		}
	}
}

void GSControlLayer::updateFlag()
{
	for (auto data : _levelData)
	{
		if (_zombiesAppearControl->getZombiesAppearFrequency() > data)
		{
			informationLayerInformation->updateProgressBarFlag(data);
		}
	}
	if (_zombiesAppearControl->getIsShowWords())
	{
		informationLayerInformation->updateProgressBarFinalFlag();
	}
}

bool GSControlLayer::judgeMousePositionIsInMap()
{
	return (_plantsPosition.x >= 0 && _plantsPosition.x <= 8 && _plantsPosition.y >= 0 && _plantsPosition.y <= 4) ? true : false;
}

bool GSControlLayer::judgeMousePositionIsCanPlant()
{
	return (_gameMapInformation->plantsMap[static_cast<unsigned int>(_plantsPosition.y)][static_cast<unsigned int>(_plantsPosition.x)] != CAN_NOT_PLANT /* �����ڲ�����ֲ ��������ֲ�ķ�Χ���ڣ�*/
		&& _gameMapInformation->plantsMap[static_cast<unsigned int>(_plantsPosition.y)][static_cast<unsigned int>(_plantsPosition.x)] == NO_PLANTS)     /* ����ֲ�ķ�Χ�ڻ�û����ֲ */
		? true : false;
}

bool GSControlLayer::judgeMousePositionHavePlant()
{
	return (_gameMapInformation->plantsMap[static_cast<unsigned int>(_plantsPosition.y)][static_cast<unsigned int>(_plantsPosition.x)] != CAN_NOT_PLANT
		&& _gameMapInformation->plantsMap[static_cast<unsigned int>(_plantsPosition.y)][static_cast<unsigned int>(_plantsPosition.x)] != NO_PLANTS)
		? true : false;
}

void GSControlLayer::removePreviewPlant()
{
	/* �Ƴ�Ԥ��ֲ�� */
	_plantPreviewImage->removeFromParent();
	_plantCurImage->removeFromParent();
	buttonLayerInformation->mouseSelectImage->isSelectPlants = false;
	CURSOR_VISIBLE(true)
}

void GSControlLayer::removeShovel()
{
	buttonLayerInformation->mouseSelectImage->isSelectShovel = false;
	_director->getOpenGLView()->setCursor("resources/images/System/cursor.png", Point::ANCHOR_TOP_LEFT);
}

void GSControlLayer::removeMouseListener()
{
	if (_listener)
	{
		_director->getEventDispatcher()->removeEventListener(_listener);
	}
}

void GSControlLayer::recoveryPlantsColor()
{
	for (unsigned int i = 0; i < _gameMapInformation->rowNumbers; ++i)
	{
		for (unsigned int j = 0; j < _gameMapInformation->columnNumbers; ++j)
		{
			if (_gameMapInformation->plantsMap[i][j] != CAN_NOT_PLANT && _gameMapInformation->plantsMap[i][j] != NO_PLANTS)
			{
				auto plant = _animationLayer->getChildByTag(SET_TAG(Vec2(j, i)));
				if (plant)
				{
					plant->setColor(Color3B::WHITE);
				}
			}
		}
	}
}

void GSControlLayer::judgeLevelIsFinished()
{
	/* �ؿ����� */
	if (ZombiesGroup.size() <= 0 && _zombiesAppearControl->getZombiesAppearFrequency() >=
		_openLevelData->readLevelData(_openLevelData->getLevelNumber())->getZombiesFrequency())
	{
		CURSOR_VISIBLE(true)

		setGameEnd();

		auto judgeUserWin = new GSGameResultJudgement();
		auto winOrLose = judgeUserWin->judgeUserIsWin();
		if (winOrLose == GameTypes::None)
		{
			if (_global->userInformation->getCurrentPlayLevels() >= 52)
			{
				_director->getInstance()->pushScene(TransitionFade::create(0.5f, GameEasterEggs::createScene()));
			}
			_gameEndShieldLayer->successfullEntry();
		}
		else
		{
			_gameEndShieldLayer->breakThrough(winOrLose); /* ����ʧ�� */
		}
		delete judgeUserWin;
	}
}

void GSControlLayer::setGameEnd()
{
	GSPauseQuitLayer::pauseLayer();

	_gameEndShieldLayer = GSGameEndLayer::create();
	_director->getRunningScene()->addChild(_gameEndShieldLayer, 10, "gameEndShieldLayer");
}

void GSControlLayer::mouseMoveControl()
{
	/* ������ѡ����ֲ�� */
	if (buttonLayerInformation->mouseSelectImage->isSelectPlants)
	{
		int posX = static_cast<int>(_plantsPosition.x);
		int posY = static_cast<int>(_plantsPosition.y);
		if (posX >= 0 && posY >= 0 && posX < 9 && posY < 5)
		{
			if (_gameMapInformation->plantsMap[posY][posX] != NO_PLANTS)
			{
				_plantPreviewImage->setPosition(Vec2(-1000, -1000));
			}
			else
			{
				auto size = _plantPreviewImage->getContentSize() / 2.f;
				_plantPreviewImage->setPosition(Vec2(GRASS_POSITION_LEFT + 122 *
					_plantsPosition.x + size.width, 110 + 138 * (_plantsPosition.y + 1) - size.height));
			}
		}
		else
		{
			_plantPreviewImage->setPosition(Vec2(-1000, -1000));
		}
		_plantCurImage->setPosition(_cur + Vec2(0, 30));
	}

	/* ������в��� */
	if (buttonLayerInformation->mouseSelectImage->isSelectShovel)
	{
		if (!_animationLayer)
		{
			_animationLayer = _director->getRunningScene()->getChildByName("animationLayer");
		}

		/* ѭ����ֲ��ָ���ԭ������ɫ */
		recoveryPlantsColor();

		if (judgeMousePositionIsInMap() && judgeMousePositionHavePlant())  /* ����ڵ�ͼ��Χ�� && ����ֲ�� */
		{
			auto plant = _animationLayer->getChildByTag(SET_TAG(_plantsPosition));
			if (plant)
			{
				plant->setColor(Color3B(100, 100, 100));
			}
		}
	}
}

void GSControlLayer::mouseDownControl(EventMouse* eventmouse)
{
	if (eventmouse->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) /* ������Ҽ����� */
	{	
		if (buttonLayerInformation->mouseSelectImage->isSelectPlants)/* �������ֲ�� */
		{
			PlayMusic::playMusic("tap2");
			buttonLayerInformation->plantsCards[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].progressTimer->setPercentage(0);
			buttonLayerInformation->plantsCards[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].plantsCards->setColor(Color3B::WHITE);

			/* ��������������������� */
			_global->userInformation->setSunNumbers(_global->userInformation->getSunNumbers() +
				plantsCardInformation[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].plantsNeedSunNumbers);
			informationLayerInformation->updateSunNumbers();

			/* ֲ��Ҫ����� */
			backgroundLayerInformation->gameType->updateRequirementNumbers("ֲ����������");

			removePreviewPlant();
		}

		if (buttonLayerInformation->mouseSelectImage->isSelectShovel) /* ������в��� */
		{
			removeShovel();
			recoveryPlantsColor();
		}
	}

	if (eventmouse->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) /* ������������ */
	{
		if (buttonLayerInformation->mouseSelectImage->isSelectPlants)
		{
			if (judgeMousePositionIsInMap() && judgeMousePositionIsCanPlant()) /* ����ڵ�ͼ��Χ�� && ������ֲֲ�� */
			{
				/* ��¼ʹ��ֲ������ */
				UserData::getInstance()->caveUserData("USEPLANTSNUMBERS", ++_global->userInformation->getUsePlantsNumbers());

				/* ��ֲֲ�� */
				animationLayerInformation->plantPlants();

				/* ��ͼ��¼��ֲ��ֲ�� */
				_gameMapInformation->plantsMap[static_cast<unsigned int>(_plantsPosition.y)][static_cast<unsigned int>(_plantsPosition.x)] =
					static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId);

				/* ���õ���ʱ���Ұ�ť������ */
				unsigned int plantsTag = static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId);
				buttonLayerInformation->plantsCards[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].timeBarIsFinished = false;
				buttonLayerInformation->plantsCards[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].plantsCards->setEnabled(false);
				buttonLayerInformation->plantsCards[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].plantsCards->setColor(Color3B::GRAY);
				buttonLayerInformation->plantsCards[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].progressTimer->runAction(
					Sequence::create(ProgressFromTo::create(plantsCardInformation[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].plantsCoolTime, 100, 0),
						CallFunc::create([=]() { buttonLayerInformation->plantsCards[plantsTag].timeBarIsFinished = true;}), nullptr)
				);
				removePreviewPlant();
			}
			else
			{
				if (_cur.x > CARD_BAR_RIGHT)
				{
					PlayMusic::playMusic("buzzer");
					/* ������ɫ�ָ� */
					buttonLayerInformation->plantsCards[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].progressTimer->setPercentage(0);
					buttonLayerInformation->plantsCards[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].plantsCards->setColor(Color3B::WHITE);

					/* ��ʾ��Ϣ */
					informationLayerInformation->createPromptText();

					removePreviewPlant();

					/* ��������������������� */
					_global->userInformation->setSunNumbers(_global->userInformation->getSunNumbers() +
						plantsCardInformation[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].plantsNeedSunNumbers);
					informationLayerInformation->updateSunNumbers();

					/* ֲ��Ҫ����� */
					backgroundLayerInformation->gameType->updateRequirementNumbers("ֲ����������");
				}
			}
		}
		if (buttonLayerInformation->mouseSelectImage->isSelectShovel) /* ������в��� */
		{
			PlayMusic::playMusic("plant2");
			if (judgeMousePositionIsInMap() && judgeMousePositionHavePlant())    /* ����ڵ�ͼ��Χ�� && ����ֲ�� */
			{
				animationLayerInformation->deletePlants();/* ����ֲ�� */
			}
			removeShovel();
			recoveryPlantsColor();
		}
	}

	if (_selectPlantsTag != PlantsType::None) 
	{
		buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].plantsCards->getChildByName("seedPacketFlash")->setColor(Color3B::WHITE);
		buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].plantsCards->getChildByName("seedPacketFlash")->setVisible(false);
	}
}
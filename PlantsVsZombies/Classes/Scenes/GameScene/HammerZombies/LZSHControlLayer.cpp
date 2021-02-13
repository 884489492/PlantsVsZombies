/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.09
 *Email: 2117610943@qq.com
 */

#include "LZSHControlLayer.h"
#include "LZSHGameEndLayer.h"
#include "LZSHButtonLayer.h"
#include "LZSHAnimationLayer.h"
#include "LZSHInformationLayer.h"
#include "../LZSGAnimationLayer.h"
#include "../LZSGInformationLayer.h"
#include "../LZSGZombiesAppearControl.h"
#include "../LZSGData.h"
#include "../LZSGDefine.h"
#include "Zombies/LZZZombies.h"
#include "Based/LZBPlayMusic.h"
#include "Based/LZBUserData.h"
#include "Based/LZBMouseEventControl.h"

HControlLayer::HControlLayer() :
	_hammer(nullptr),
	_attack(100),
	_frequencyZombiesNumbers(5),
	_maxFrequencyNumbers(5),
	_currentLevelNumber(1),
	_zombiesTypeNumbers(1),
	_currentLevelZombiesSpeed(1.5),
	_isShowHammerButton(false)
{
	srand(time(nullptr));
	createHammerAnimation();
	createMouseListener();
	calculateZombiesData();
}

HControlLayer::~HControlLayer()
{
}

bool HControlLayer::init()
{
	if (!Layer::init())return false;

	initData();
	createSchedule();
	
	return true;
}

void HControlLayer::initData()
{
	srand(time(nullptr));
	_zombiesAppearControl = new ZombiesAppearControl();
}

void HControlLayer::createSchedule()
{
	schedule([&](float) {
		createZombies();
		controlRefurbishMusicAndText();
		judgeLevelIsFinished();
		}, 0.1f, "mainUpdate");

	schedule([&](float) {
		zombiesComeTiming();
		judgeZombiesWin();
		}, 1.0f, "zombiesComing");

	runAction(Sequence::create(DelayTime::create(1.f), 
		CallFunc::create([=]()
			{
				MouseEventControl::goodsRecovery(goodsLayerInformation, _hammer);
			}), nullptr));
}

void HControlLayer::createHammerAnimation()
{
	_hammer = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("Hammer")->second);
	_hammer->setAnimation(0, "Hammer_Normal", false);
	_hammer->setSkin("skin" + to_string(HButtonLayer::selectedHammer));
	_hammer->setScale(1.5f);
	_hammer->setPosition(Vec2(-100,-100));
	_hammer->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	_hammer->setGlobalZOrder(2);
	_hammer->update(0);
	this->addChild(_hammer);
}

void HControlLayer::updateHammerInformation()
{
	_hammer->setSkin("skin" + to_string(HButtonLayer::selectedHammer));
	_attack = hammerInformation[HButtonLayer::selectedHammer].lowAttack + min(hammerInformation[HButtonLayer::selectedHammer].HigtAttack,
		hammerInformation[HButtonLayer::selectedHammer].lowAttack + _currentLevelNumber * 15);
}

void HControlLayer::createMouseListener()
{
	/* ���������� */
	auto listener = EventListenerMouse::create();

	/* ����ƶ� */
	listener->onMouseMove = [&](Event* event)
	{
		/* ��ȡ���λ�� */
		_cur = ((EventMouse*)event)->getLocationInView();
		_hammer->setPosition(_cur - Vec2(20, 60));

		changeCursor();
		showSelectedButtonHoverEffect();
	};

	/* ��갴�� */
	listener->onMouseDown = [=](Event* event)
	{
		_cur = ((EventMouse*)event)->getLocationInView();
		_hammer->setAnimation(0, "Hammer_Attack", false);
		judgeHammerZombies(_cur);

		if (_hammer->isVisible())PlayMusic::playMusic("swing");
	};

	_director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void HControlLayer::judgeHammerZombies(const Vec2& position)
{
	for (auto zombie : ZombiesGroup)
	{
		if (zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() && _hammer->isVisible())
		{
			auto& box = zombie->getZombieAnimation()->getBoundingBox();
			if (_hammer->getBoundingBox().intersectsRect(Rect(box.origin.x + 60, box.origin.y + 50, box.size.width, box.size.height - 50)))
			{
				PlayMusic::playMusic("bonk");
				zombie->setZombieHurtBlink();
				attackZombies(zombie);
				showPowImage(zombie, position);
				break;
			}
		}
	}
}

void HControlLayer::changeCursor()
{
	if (_cur.x > GRASS_POSITION_LEFT - 100 && _cur.y > GRASS_POSITION_BOTTOM &&
		_cur.x < GRASS_POSITION_RIGHT + 100 && _cur.y < GRASS_POSITION_TOP + 150)
	{
		_director->getOpenGLView()->setCursorVisible(false);
		_hammer->setVisible(true);
		hideHammerButton();
	}
	else
	{
		_director->getOpenGLView()->setCursorVisible(true);
		_hammer->setVisible(false);
		showHammerButton();
	}
}

void HControlLayer::showSelectedButtonHoverEffect()
{
	for (auto button : HButtonLayer::hammerButton)
	{
		if (button->isEnabled() && HButtonLayer::selectedHammer != button->getTag())
		{
			button->getChildByName("SeedPacketFlash")->
				setVisible(button->getBoundingBox().containsPoint(_cur));
		}

		if (button->getBoundingBox().containsPoint(_cur))
		{
			dynamic_cast<HInformationLayer*>(informationLayerInformation)->showHammerInformation(button);
		}
	}
}

void HControlLayer::attackZombies(Zombies* zombie)
{
	if (zombie->getZombieCurrentBodyShieldVolume() < _attack)     /* �����ǰ���廤��Ѫ��С�ڱ�ը�˺� */
	{
		if (zombie->getZombieCurrentBodyShieldVolume() +
			zombie->getZombieCurrentHeadShieldVolume() < _attack) /* �����ǰ���廤�ܼ�ͷ������Ѫ��С�ڱ�ը�˺� */
		{
			if (zombie->getZombieCurrentBodyShieldVolume() +
				zombie->getZombieCurrentHeadShieldVolume() +
				zombie->getZombieCurrentBloodVolume() <= _attack) /* �����ʬ����Ѫ��С�ڱ�ը�˺�����ʬ������ */
			{
				zombie->setZombieDeath(true);
			}
			else
			{
				/* ���㽩ʬ����Ѫ�� */
				zombie->setZombieCurrentBloodVolume(
					zombie->getZombieCurrentBodyShieldVolume() +
					zombie->getZombieCurrentHeadShieldVolume() +
					zombie->getZombieCurrentBloodVolume() - _attack);
				zombie->setZombieCurrentHeadShieldVolume(0);
				zombie->setZombieCurrentBodyShieldVolume(0);
			}
		}
		else
		{
			/* ���㽩ʬ����ʣ��Ѫ�� */
			zombie->setZombieCurrentHeadShieldVolume(
				zombie->getZombieCurrentBodyShieldVolume() +
				zombie->getZombieCurrentHeadShieldVolume() - _attack);
			zombie->setZombieCurrentBodyShieldVolume(0);
		}
	}
	else
	{
		zombie->setZombieCurrentBodyShieldVolume(
			zombie->getZombieCurrentBodyShieldVolume() - _attack);
	}
}

void HControlLayer::createZombies()
{
	/* ˢ�½�ʬ */
	if (_zombiesAppearControl->getLastFrequencyZombiesWasDeath())
	{
		_zombiesAppearControl->setLastFrequencyZombiesWasDeath(false);
		_zombiesAppearControl->setTimeClear(); /* ������һ��ˢ��ʱ������ */
		if (static_cast<unsigned int>(_zombiesAppearControl->getZombiesAppearFrequency()) < _maxFrequencyNumbers)
		{
			unsigned int zombiesNumbers = _frequencyZombiesNumbers + _zombiesAppearControl->getZombiesAppearFrequency() / 2;
			zombiesNumbers = zombiesNumbers + rand() % (zombiesNumbers / 3);
			for (unsigned int i = 0; i < zombiesNumbers; ++i)
			{
				dynamic_cast<HAnimationLayer*>(animationLayerInformation)->createZombies(
					_currentLevelZombiesSpeed, _zombiesAppearControl->getZombiesAppearFrequency() * 0.01f, _zombiesTypeNumbers);
			}
			/* ������������һ */
			_zombiesAppearControl->setZombiesAppearFrequency();

			/* ���������� */
			informationLayerInformation->updateProgressBar(_zombiesAppearControl->getZombiesAppearFrequency(), _maxFrequencyNumbers);
		}
		ZombiesGroup.sort(Zombies::compare);
	}
	informationLayerInformation->updateProgressBarHead();

	/* ���ƽ�ʬ��ˢ�� */
	if (controlRefurbishZombies())
	{
		_zombiesAppearControl->setLastFrequencyZombiesWasDeath(true);
		_zombiesAppearControl->setIsBegin(false);
	}
}

bool HControlLayer::controlRefurbishZombies()
{
	if ((Zombies::getZombiesNumbers() <= 4 &&
		_zombiesAppearControl->getZombiesAppearFrequency() > 3)                    /* ������ŵĽ�ʬ��С�ڹ涨��ˢ����һ�� */

		|| (Zombies::getZombiesNumbers() <= 0 &&                                   /* ���û�д�ʬ������ˢ�½�ʬ */
			_zombiesAppearControl->getZombiesAppearFrequency() > 0)

		|| (_zombiesAppearControl->getTime() >= 5 &&
			_zombiesAppearControl->getZombiesAppearFrequency() == 0)               /* ��һ��ˢ�¿��� */

		|| (_zombiesAppearControl->getTime() >= 10 &&
			_zombiesAppearControl->getZombiesAppearFrequency() > 0)                /* �������10��ˢ����һ�� */
		)
	{
		return true;
	}
	return false;
}

void HControlLayer::controlRefurbishMusicAndText()
{
	/* ���ƴ󲨽�ʬ��Ϯ������������ */
	if (_zombiesAppearControl->getTime() >= 5 && _zombiesAppearControl->getZombiesAppearFrequency() == 0)
	{
		PlayMusic::playMusic("awooga");
	}

	/* ���һ����ʬ��������ʾ�����Ӹ��� */
	if (_zombiesAppearControl->getZombiesAppearFrequency() == _maxFrequencyNumbers && !_zombiesAppearControl->getIsShowWords())
	{
		if (informationLayerInformation->updateProgressBarFlag())
		{
			_zombiesAppearControl->setIsShowWords(true);
		}
	}
}

void HControlLayer::judgeLevelIsFinished()
{
	/* �ؿ����� */
	if (ZombiesGroup.size() <= 0 && static_cast<unsigned int>(_zombiesAppearControl->getZombiesAppearFrequency()) >= _maxFrequencyNumbers)
	{
		_hammer->setVisible(false);
		UserData::getInstance()->caveUserData("HAMMERZOMBIES_LEVEL_NUMBER", static_cast<int>(_currentLevelNumber + 1));

		auto gameEndShieldLayer = HGameEndLayer::create();
		_director->getRunningScene()->addChild(gameEndShieldLayer, 10, "gameEndShieldLayer");
		gameEndShieldLayer->successfullEntry();
	}
}

void HControlLayer::judgeZombiesWin()
{
	for (auto zombie : ZombiesGroup)
	{
		if (zombie->getZombieIsSurvive() && zombie->getZombiePositionX() < GRASS_POSITION_LEFT - 60)
		{
			_hammer->setVisible(false);
			auto gameEndShieldLayer = HGameEndLayer::create();
			_director->getRunningScene()->addChild(gameEndShieldLayer, 10, "gameEndShieldLayer");
			gameEndShieldLayer->breakThrough();
		}
	}
}

void HControlLayer::showPowImage(Zombies* zombie, const Vec2& position)
{
	auto pow = Sprite::createWithSpriteFrameName("Pow.png");
	pow->setPosition(position - Vec2(60, 40));
	pow->setOpacity(0);
	pow->setLocalZOrder(zombie->getZombieAnimation()->getLocalZOrder() + 1);
	pow->runAction(Sequence::create(FadeIn::create(0.1f), DelayTime::create(0.3f), FadeOut::create(0.2f),
		CallFunc::create([pow]()
			{
				pow->removeFromParent();
			}), nullptr));
	animationLayerInformation->addChild(pow);
}

void HControlLayer::calculateZombiesData()
{
	_currentLevelNumber = max(UserData::getInstance()->openIntUserData("HAMMERZOMBIES_LEVEL_NUMBER"), 1);
	_maxFrequencyNumbers += min(static_cast<int>(_currentLevelNumber / 2), 40);
	_zombiesTypeNumbers = min(static_cast<int>(_zombiesTypeNumbers + _currentLevelNumber / 3), 14);
	_currentLevelZombiesSpeed += _currentLevelNumber * 0.02f;
	_attack = hammerInformation[HButtonLayer::selectedHammer].lowAttack + min(hammerInformation[HButtonLayer::selectedHammer].HigtAttack,
		hammerInformation[HButtonLayer::selectedHammer].lowAttack + _currentLevelNumber * 15);
}

void HControlLayer::showHammerButton()
{
	if (!_isShowHammerButton)
	{
		for (auto button : HButtonLayer::hammerButton)
		{
			button->stopAllActions();
			button->runAction(Sequence::create(MoveTo::create(0.1f, Vec2(90, 1030 - 100 * button->getTag())), nullptr));
		}
		_isShowHammerButton = true;
	}
}

void HControlLayer::hideHammerButton()
{
	if (_isShowHammerButton)
	{
		for (auto button : HButtonLayer::hammerButton)
		{
			button->stopAllActions();
			button->runAction(Sequence::create(MoveTo::create(0.1f, Vec2(-90, 1030 - 100 * button->getTag())), nullptr));
		}
		dynamic_cast<HInformationLayer*>(informationLayerInformation)->deleteHammerInformation();
		_isShowHammerButton = false;
	}
}

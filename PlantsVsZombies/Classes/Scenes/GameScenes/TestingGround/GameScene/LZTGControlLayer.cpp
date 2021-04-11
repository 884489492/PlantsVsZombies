/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.08
 *Email: 2117610943@qq.com
 */

#include "LZTGControlLayer.h"
#include "LZTGAnimationLayer.h"
#include "LZTGInformationLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSZombiesAppearControl.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSAnimationLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSButtonLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSInformationLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSDefine.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Based/LZPlayMusic.h"

TGControlLayer::TGControlLayer()
{
}

TGControlLayer::~TGControlLayer()
{
}

bool TGControlLayer::init()
{
	if (!Layer::init())return false;

	initData();
	createSchedule();
	createPlantsCardListener();
	createMouseListener();

	return true;
}

void TGControlLayer::initData()
{
	srand(time(nullptr));
	gameMapInformation = new GameMapInformation();
	MAP_INIT(gameMapInformation->plantsMap)

	_zombiesAppearControl = new ZombiesAppearControl();
}

void TGControlLayer::createSchedule()
{
	schedule([&](float) {
		controlCardEnabled();
		createZombies();
		controlRefurbishMusicAndText();
		}, 0.1f, "mainUpdate");

	schedule([&](float) {
		zombiesComeTiming();
		}, 1.0f, "zombiesComing");
}

void TGControlLayer::createZombies()
{
	/* ˢ�½�ʬ */
	if (_zombiesAppearControl->getLastFrequencyZombiesWasDeath())
	{
		_zombiesAppearControl->setLastFrequencyZombiesWasDeath(false);
		_zombiesAppearControl->setTimeClear(); /* ������һ��ˢ��ʱ������ */
		
		const auto frequence = _zombiesAppearControl->getZombiesAppearFrequency();
		const auto number = frequence + rand() % 5;
		unsigned int zombiesNumbers = number <= 25 ? number : 20 + rand() % 5;
		zombiesNumbers += frequence / 25;
		for (unsigned int i = 0; i < zombiesNumbers; ++i)
		{
			dynamic_cast<TGAnimationLayer*>(animationLayerInformation)->createZombies(static_cast<unsigned>(frequence / 3));
		}
		/* ������������һ */
		_zombiesAppearControl->setZombiesAppearFrequency();
		dynamic_cast<TGInformationLayer*>(informationLayerInformation)->updateText();
	}
	
	/* ���ƽ�ʬ��ˢ�� */
	if (controlRefurbishZombies())
	{
		_zombiesAppearControl->setLastFrequencyZombiesWasDeath(true);
		_zombiesAppearControl->setIsBegin(false);
	}
}

bool TGControlLayer::controlRefurbishZombies()
{
	if ((Zombies::getZombiesNumbers() <= 4 &&
		_zombiesAppearControl->getZombiesAppearFrequency() > 3)                    /* ������ŵĽ�ʬ��С�ڹ涨��ˢ����һ�� */

		|| (Zombies::getZombiesNumbers() <= 0 &&                                   /* ���û�д�ʬ������ˢ�½�ʬ */
			_zombiesAppearControl->getZombiesAppearFrequency() >= 1)

		|| (_zombiesAppearControl->getTime() >= 10 &&
			_zombiesAppearControl->getZombiesAppearFrequency() == 0)               /* ��һ��ˢ�¿��� */

		|| (_zombiesAppearControl->getTime() >= 30 &&
			_zombiesAppearControl->getZombiesAppearFrequency() > 0)                /* �������40��ˢ����һ�� */
		)
	{
		return true;
	}
	return false;
}

void TGControlLayer::controlRefurbishMusicAndText()
{
	/* ���ƴ󲨽�ʬ��Ϯ������������ */
	if (_zombiesAppearControl->getTime() >= 10 && _zombiesAppearControl->getZombiesAppearFrequency() == 0)
	{
		PlayMusic::playMusic("awooga");
	}
}

void TGControlLayer::selectPlantsPreviewImage()
{
	switch (buttonLayerInformation->mouseSelectImage->isSelectPlants)
	{
	case true:
		PlayMusic::playMusic("tap2");
		buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].progressTimer->setPercentage(0);
		buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].plantsCards->setColor(Color3B::WHITE);

		removePreviewPlant();
		break;
	case false:
		PlayMusic::playMusic("seedlift");

		/* ��Ƭ��ڲ�����ѡ��Ч�� */
		buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].plantsCards->setColor(Color3B::GRAY);
		buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].progressTimer->setPercentage(100);
		buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].plantsCards->getChildByName("seedPacketFlash")->setVisible(true);
		buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].plantsCards->getChildByName("seedPacketFlash")->setColor(Color3B::ORANGE);

		/* ���ѡ���� */
		buttonLayerInformation->mouseSelectImage->isSelectPlants = true;
		buttonLayerInformation->mouseSelectImage->selectPlantsId = _selectPlantsTag;

		createPreviewPlants();
		break;
	}
}

void TGControlLayer::mouseLeftButtonDownControl()
{
	if (buttonLayerInformation->mouseSelectImage->isSelectPlants)
	{
		if (judgeMousePositionIsInMap() && judgeMousePositionIsCanPlant() && _cur.x > CARD_BAR_RIGHT) /* ����ڵ�ͼ��Χ�� && ������ֲֲ�� */
		{
			/* ��ֲֲ�� */
			animationLayerInformation->plantPlants();

			/* ��ͼ��¼��ֲ��ֲ�� */
			gameMapInformation->plantsMap[static_cast<unsigned int>(_plantsPosition.y)][static_cast<unsigned int>(_plantsPosition.x)] =
				static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId);

			buttonLayerInformation->plantsCards[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].progressTimer->setPercentage(0);
			buttonLayerInformation->plantsCards[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].plantsCards->setColor(Color3B::WHITE);

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
			}
		}
	}
	if (buttonLayerInformation->mouseSelectImage->isSelectShovel) /* ������в��� */
	{
		if (judgeMousePositionIsInMap() && judgeMousePositionHavePlant())    /* ����ڵ�ͼ��Χ�� && ����ֲ�� */
		{
			PlayMusic::playMusic("plant2");
			animationLayerInformation->deletePlants();/* ����ֲ�� */
			removeShovel();
		}
		else
		{
			if (!buttonLayerInformation->getChildByName("ShovelBank")->getBoundingBox().containsPoint(_cur))
			{
				removeShovel();
			}
			PlayMusic::playMusic("shovel");
		}
		recoveryPlantsColor();
	}
}

void TGControlLayer::mouseRightButtonDownControl()
{
	if (buttonLayerInformation->mouseSelectImage->isSelectPlants)/* �������ֲ�� */
	{
		if (_cur.x > CARD_BAR_RIGHT)
		{
			PlayMusic::playMusic("tap2");
			buttonLayerInformation->plantsCards[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].progressTimer->setPercentage(0);
			buttonLayerInformation->plantsCards[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].plantsCards->setColor(Color3B::WHITE);

			removePreviewPlant();
		}
	}

	if (buttonLayerInformation->mouseSelectImage->isSelectShovel) /* ������в��� */
	{
		removeShovel();
		recoveryPlantsColor();
	}
}

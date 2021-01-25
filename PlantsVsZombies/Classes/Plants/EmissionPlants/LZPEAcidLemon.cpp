/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.25
 *Email: 2117610943@qq.com
 */

#include "LZPEAcidLemon.h"
#include "Bullet/LZPEBAcidLemonBullet.h"
#include "Zombies/LZZZombies.h"
#include "Scenes/GameScene/LZSGData.h"
#include "Based/LZBPlayMusic.h"

AcidLemonShooter::AcidLemonShooter(Node* node) :
	_isCreateAcidLemon(false)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_combatEffecttiveness = 0;
	_plantsType = PlantsType::AcidLemonShooter;
}

AcidLemonShooter::~AcidLemonShooter()
{
}

AcidLemonShooter* AcidLemonShooter::create(Node* node)
{
	AcidLemonShooter* acidLemonShooter = new (std::nothrow) AcidLemonShooter(node);
	if (acidLemonShooter && acidLemonShooter->init())
	{
		acidLemonShooter->autorelease();
		return acidLemonShooter;
	}
	CC_SAFE_DELETE(acidLemonShooter);
	return nullptr;
}

Sprite* AcidLemonShooter::createPlantImage()
{
	imageInit("LemonShooter", INIT);
	_plantImage->setScale(1.3f);
	return _plantImage;
}

void AcidLemonShooter::createPlantAnimation()
{
	_plantAnimation = plantInit("LemonShooter", "LemonNormal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.2f);
	_plantAnimation->setTimeScale(0.8f);
	_node->addChild(_plantAnimation);

	// Ӱ��
	setPlantShadow(1.5f);
	_plantAnimation->getChildByName("plantshadow")->setPosition(Vec2(0, 10));

	// �����ɽ�����
	setPlantSoilSplashAnimation(0.8f);
}

void AcidLemonShooter::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* ��ʬ��ֲ�� */

		plantAttack(zombie);         /* ֲ�﹥�� */

		zombieRecoveryMove(zombie);  /* ��ʬ�ָ��ƶ� */
	}

	plantRecovery("LemonNormal");
}

void AcidLemonShooter::plantAttack(Zombies* zombie)
{
	if (getPlantIsSurvive() && zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() &&     /* ֲ���� && ��ʬû������ && ��ʬ�����ͼ */
		getZombieIsTheFrontOfPlant(zombie) && getZombieIsSameLineWithPlant(zombie))                 /* ��ʬ��ֲ����ͬһ�� && ��ʬ��ֲ���ǰ�� */
	{
		plantEmission("LemonShooter");
	}
}

void AcidLemonShooter::plantEmission(const string& plantAnimation)
{
	_isCreateAcidLemon = true; /* ��ʾ�н�ʬ��ֲ����ͬһ�� */
	if (!_isChanged)     /* �ж϶����Ƿ��Ѿ��ı� */
	{
		_plantAnimation->addAnimation(0, plantAnimation, true);
		_isChanged = true;
	}

	_plantAnimation->setEventListener([&](spTrackEntry* entry, spEvent* event)
		{
			if (strcmp(event->data->name, "shoot") == 0)
			{
				rand() % 2 == 0 ? PlayMusic::playMusic("throw") : PlayMusic::playMusic("throw2");
				createAcidLemon();
			}
		});
}

void AcidLemonShooter::plantRecovery(const string& plantAnimation)
{
	if (!_isCreateAcidLemon)  /* �ж��Ƿ��н�ʬ��ǿ������������ͬһ�� */
	{
		if (_isChanged)
		{
			_plantAnimation->addAnimation(0, plantAnimation, true);
			_isChanged = false;
		}
	}

	_isCreateAcidLemon = false; /* ÿѭ��һ�ξͳ�ʼ�� */
}

void AcidLemonShooter::createAcidLemon()
{
	_bulletAnimation = new AcidLemonBullet(_node);
	_bulletAnimation->setBulletPosition(_position);
	_bulletAnimation->setBulletInRow(_rowAndColumn.y);
	_bulletAnimation->createBullet();

	BulletGroup.push_back(_bulletAnimation);
}

SkeletonAnimation* AcidLemonShooter::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("ACIDLEMON_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("LemonShooter", "LemonNormal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(2.2f);
	_plantAnimation->setPosition(Vec2(200, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("ACIDLEMON_1")->second->text, Vec2(190, 910), lta.find("ACIDLEMON_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("ACIDLEMON_2")->second->text, Vec2(360, 1000), lta.find("ACIDLEMON_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("ACIDLEMON_3")->second->text, Vec2(440, 1000), lta.find("ACIDLEMON_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, SPSSpriteLayer::selectRequirementText(lta, PlantsType::AcidLemonShooter, "ACIDLEMON_4", "ACIDLEMON_5"), Vec2(360, 870),
		lta.find("ACIDLEMON_4")->second->fontsize, SPSSpriteLayer::isPlantIsCanSelect[static_cast<unsigned int>(PlantsType::AcidLemonShooter)] ? Color3B::YELLOW : Color3B(255, 70, 0), false);
	
	return _plantAnimation;
}
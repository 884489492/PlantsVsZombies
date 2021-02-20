/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "LZPeaShooter.h"
#include "Bullet/LZPea.h"

#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"


PeaShooter::PeaShooter(Node* node)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_combatEffecttiveness = 0;
	_plantsType = PlantsType::PeaShooter;
}

PeaShooter::~PeaShooter()
{
}

PeaShooter* PeaShooter::create(Node* node)
{
	PeaShooter* peaShooter = new (std::nothrow) PeaShooter(node);
	if (peaShooter && peaShooter->init())
	{
		peaShooter->autorelease();
		return peaShooter;
	}
	CC_SAFE_DELETE(peaShooter);
	return nullptr;
}

Sprite* PeaShooter::createPlantImage()
{
	imageInit("PeaShooter", INIT);
	_plantImage->setScale(1.3f);
	_plantImage->setAnchorPoint(Vec2(0.35f, 0.6f));
	return _plantImage;
}

void PeaShooter::createPlantAnimation()
{
	_plantAnimation = plantInit("PeaShooter", "PeaShooter_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.0f);
	_node->addChild(_plantAnimation);

	// Ӱ��
	setPlantShadow(1.9f);
	_plantAnimation->getChildByName("plantshadow")->setPosition(Vec2(0, 15));

	// �����ɽ�����
	setPlantSoilSplashAnimation(0.8f);

	// ��������
	createListener("PeaShooter_Shoot");
}

void PeaShooter::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* ��ʬ��ֲ�� */

		zombieRecoveryMove(zombie);  /* ��ʬ�ָ��ƶ� */

		plantAttack(zombie);         /* ֲ�﹥�� */
	}

	plantRecovery("PeaShooter_Normal");
}

void PeaShooter::plantAttack(Zombies* zombie)
{
	if (getPlantIsSurvive() && zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() &&     /* ֲ���� && ��ʬû������ && ��ʬ�����ͼ */
		getZombieIsTheFrontOfPlant(zombie) && getZombieIsSameLineWithPlant(zombie))                 /* ��ʬ��ֲ����ͬһ�� && ��ʬ��ֲ���ǰ�� */
	{
		plantEmission("PeaShooter_Shoot");
	}
}

void PeaShooter::plantEmission(const string& plantAnimation)
{
	_isHaveZombies = true; /* ��ʾ�н�ʬ��ֲ����ͬһ�� */
	if (!_isChanged)     /* �ж϶����Ƿ��Ѿ��ı� */
	{
		_plantAnimation->addAnimation(0, plantAnimation, true);
		_isChanged = true;
	}
}

void PeaShooter::plantRecovery(const string& plantAnimation)
{
	if (!_isHaveZombies) /* �ж��Ƿ��н�ʬ���㶹������ͬһ�� */
	{
		if (_isChanged)
		{
			_plantAnimation->addAnimation(0, plantAnimation, true);
			_isChanged = false;
		}
	}

	_isHaveZombies = false; /* ÿѭ��һ�ξͳ�ʼ�� */
}

void PeaShooter::createBullet()
{
	_bulletAnimation= new Pea(_node);
	_bulletAnimation->setBulletPosition(_position);
	_bulletAnimation->setBulletInRow(_rowAndColumn.y);
	_bulletAnimation->createBullet();

	BulletGroup.push_back(_bulletAnimation);
}

SkeletonAnimation* PeaShooter::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("PEASHOOTER_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("PeaShooter", "PeaShooter_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.8f);
	_plantAnimation->setPosition(Vec2(200, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("PEASHOOTER_1")->second->text, Vec2(190, 910), lta.find("PEASHOOTER_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("PEASHOOTER_2")->second->text, Vec2(360, 1000), lta.find("PEASHOOTER_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("PEASHOOTER_3")->second->text, Vec2(440, 1000), lta.find("PEASHOOTER_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, lta.find("PEASHOOTER_4")->second->text, Vec2(360, 830), lta.find("PEASHOOTER_4")->second->fontsize, Color3B::ORANGE, false);

	return _plantAnimation;
}
/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "LZPEFirePeaShooter.h"
#include "Bullet/LZPEBFirePea.h"

#include "Zombies/LZZZombies.h"
#include "Scenes/GameScene/LZSGData.h"

FirePeaShooter::FirePeaShooter(Node* node)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_combatEffecttiveness = 0;
	_plantsType = PlantsType::FirePeaShooter;
}

FirePeaShooter::~FirePeaShooter()
{
}

FirePeaShooter* FirePeaShooter::create(Node* node)
{
	FirePeaShooter* firePeaShooter = new (std::nothrow) FirePeaShooter(node);
	if (firePeaShooter && firePeaShooter->init())
	{
		firePeaShooter->autorelease();
		return firePeaShooter;
	}
	CC_SAFE_DELETE(firePeaShooter);
	return nullptr;
}
Sprite* FirePeaShooter::createPlantImage()
{
	imageInit("FirePeaShooter", INIT);
	_plantImage->setScale(1.5f);
	_plantImage->setAnchorPoint(Vec2(0.4f, 0.45f));
	return _plantImage;
}

void FirePeaShooter::createPlantAnimation()
{
	_plantAnimation = plantInit("FirePeaShooter", "FirePeaShooter_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.12f);
	_node->addChild(_plantAnimation);

	// Ӱ��
	setPlantShadow(1.6f);
	_plantAnimation->getChildByName("plantshadow")->setPosition(Vec2(0, 10));

	// �����ɽ�����
	setPlantSoilSplashAnimation(0.8f);

	// ��������
	createListener("FirePeaShooter_Shoot");
}

void FirePeaShooter::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* ��ʬ��ֲ�� */

		zombieRecoveryMove(zombie);  /* ��ʬ�ָ��ƶ� */

		plantAttack(zombie);         /* ֲ�﹥�� */
	}

	plantRecovery("FirePeaShooter_Normal");
}

void FirePeaShooter::plantAttack(Zombies* zombie)
{
	if (getPlantIsSurvive() && zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() &&     /* ֲ���� && ��ʬû������ && ��ʬ�����ͼ */
		getZombieIsTheFrontOfPlant(zombie) && getZombieIsSameLineWithPlant(zombie))                 /* ��ʬ��ֲ����ͬһ�� && ��ʬ��ֲ���ǰ�� */
	{
		plantEmission("FirePeaShooter_Shoot");
	}
}

void FirePeaShooter::createBullet()
{
	_isCreateBullet = true;

	_bulletAnimation = new FirePea(_node);
	_bulletAnimation->setBulletPosition(_position);
	_bulletAnimation->setBulletInRow(_rowAndColumn.y);
	_bulletAnimation->createBullet();

	BulletGroup.push_back(_bulletAnimation);
}

SkeletonAnimation* FirePeaShooter::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("FIREPEASHOOTER_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("FirePeaShooter", "FirePeaShooter_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.8f);
	_plantAnimation->setPosition(Vec2(200, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("FIREPEASHOOTER_1")->second->text, Vec2(190, 910), lta.find("FIREPEASHOOTER_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("FIREPEASHOOTER_2")->second->text, Vec2(360, 1000), lta.find("FIREPEASHOOTER_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("FIREPEASHOOTER_3")->second->text, Vec2(440, 1000), lta.find("FIREPEASHOOTER_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, SPSSpriteLayer::selectRequirementText(lta, PlantsType::FirePeaShooter, "FIREPEASHOOTER_4", "FIREPEASHOOTER_5"), Vec2(360, 870),
		lta.find("FIREPEASHOOTER_4")->second->fontsize, SPSSpriteLayer::isPlantIsCanSelect[static_cast<unsigned int>(PlantsType::FirePeaShooter)] ? Color3B::ORANGE : Color3B(255, 70, 0), false);
	
	return _plantAnimation;
}

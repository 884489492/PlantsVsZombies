/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.1.17
 *Email: 2117610943@qq.com
 */

#include "LZPEIcePeaShooter.h"
#include "Bullet/LZPEBIcePea.h"
#include "Zombies/LZZZombies.h"
#include "Scenes/GameScene/LZSGData.h"
#include "Based/LZBPlayMusic.h"

IcePeaShooter::IcePeaShooter(Node* node)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_combatEffecttiveness = 0;
	_plantsType = PlantsType::IcePeaShooter;
}

IcePeaShooter::~IcePeaShooter()
{
}

IcePeaShooter* IcePeaShooter::create(Node* node)
{
	IcePeaShooter* icePeaShooter = new (std::nothrow) IcePeaShooter(node);
	if (icePeaShooter && icePeaShooter->init())
	{
		icePeaShooter->autorelease();
		return icePeaShooter;
	}
	CC_SAFE_DELETE(icePeaShooter);
	return nullptr;
}

Sprite* IcePeaShooter::createPlantImage()
{
	imageInit("IcePeaShooter", INIT);
	_plantImage->setScale(1.3f);
	_plantImage->setAnchorPoint(Vec2(0.4f, 0.6f));
	return _plantImage;
}

void IcePeaShooter::createPlantAnimation()
{
	_plantAnimation = plantInit("IcePeaShooter", "IcePeaShooter_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.1f);
	_node->addChild(_plantAnimation);

	// Ӱ��
	setPlantShadow(1.7f);
	_plantAnimation->getChildByName("plantshadow")->setPosition(Vec2(0, 10));

	// �����ɽ�����
	setPlantSoilSplashAnimation(0.8f);

	// ��������
	createListener("IcePeaShooter_Shoot");
}

void IcePeaShooter::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* ��ʬ��ֲ�� */

		zombieRecoveryMove(zombie);  /* ��ʬ�ָ��ƶ� */

		plantAttack(zombie);         /* ֲ�﹥�� */
	}

	plantRecovery("IcePeaShooter_Normal");
}

void IcePeaShooter::plantAttack(Zombies* zombie)
{
	if (getPlantIsSurvive() && zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() &&     /* ֲ���� && ��ʬû������ && ��ʬ�����ͼ */
		getZombieIsTheFrontOfPlant(zombie) && getZombieIsSameLineWithPlant(zombie))                 /* ��ʬ��ֲ����ͬһ�� && ��ʬ��ֲ���ǰ�� */
	{
		plantEmission("IcePeaShooter_Shoot");
	}
}

void IcePeaShooter::createBullet()
{
	PlayMusic::playMusic("snow_pea_sparkles");

	_bulletAnimation = new IcePea(_node);
	_bulletAnimation->setBulletPosition(_position);
	_bulletAnimation->setBulletInRow(_rowAndColumn.y);
	_bulletAnimation->createBullet();

	BulletGroup.push_back(_bulletAnimation);
}

SkeletonAnimation* IcePeaShooter::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("ICEPEASHOOTER_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("IcePeaShooter", "IcePeaShooter_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(2.0f);
	_plantAnimation->setPosition(Vec2(200, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("ICEPEASHOOTER_1")->second->text, Vec2(190, 910), lta.find("ICEPEASHOOTER_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("ICEPEASHOOTER_2")->second->text, Vec2(360, 1000), lta.find("ICEPEASHOOTER_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("ICEPEASHOOTER_3")->second->text, Vec2(440, 1000), lta.find("ICEPEASHOOTER_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, SPSSpriteLayer::selectRequirementText(lta, PlantsType::IcePeaShooter, "ICEPEASHOOTER_4", "ICEPEASHOOTER_5"), Vec2(360, 870),
		lta.find("ICEPEASHOOTER_4")->second->fontsize, SPSSpriteLayer::isPlantIsCanSelect[static_cast<unsigned int>(PlantsType::IcePeaShooter)] ? Color3B::ORANGE : Color3B(255, 70, 0), false);
	
	return _plantAnimation;
}

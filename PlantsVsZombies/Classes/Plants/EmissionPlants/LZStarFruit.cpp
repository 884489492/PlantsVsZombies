/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.1.26
 *Email: 2117610943@qq.com
 */

#include "LZStarFruit.h"
#include "Bullet/LZStarFruitBullet.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Zombies/LZZombies.h"

StarFruit::StarFruit(Node* node)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_combatEffecttiveness = 0;
	_plantsType = PlantsType::StarFruit;
}

StarFruit::~StarFruit()
{
}

StarFruit* StarFruit::create(Node* node)
{
	StarFruit* starFruit = new (std::nothrow) StarFruit(node);
	if (starFruit && starFruit->init())
	{
		starFruit->autorelease();
		return starFruit;
	}
	CC_SAFE_DELETE(starFruit);
	return nullptr;
}

Sprite* StarFruit::createPlantImage()
{
	imageInit("StarFruit", INIT);
	_plantImage->setScale(1.15f);
	_plantImage->setAnchorPoint(Vec2(0.5f, 0.7f));
	return _plantImage;
}

void StarFruit::createPlantAnimation()
{
	_plantAnimation = plantInit("StarFruit", "StarFruit_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(0.8f);
	_node->addChild(_plantAnimation);

	// Ӱ��
	setPlantShadow(2.7f);
	_plantAnimation->getChildByName("plantshadow")->setPosition(Vec2(-5, 50));
	_plantAnimation->getChildByName("plantshadow")->setScaleY(4.f);

	// �����ɽ�����
	setPlantSoilSplashAnimation(0.8f);

	// ��������
	createListener("StarFruit_Attack");
}

void StarFruit::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* ��ʬ��ֲ�� */

		zombieRecoveryMove(zombie);  /* ��ʬ�ָ��ƶ� */

		plantAttack(zombie);         /* ֲ�﹥�� */
	}

	plantRecovery("StarFruit_Normal");
}

void StarFruit::plantAttack(Zombies* zombie)
{
	if (getPlantIsSurvive() && zombie->getZombieIsSurvive() &&                 /* ֲ���� && ��ʬû������ */
		zombie->getZombieIsEnterMap() && zombieInPlantAttackRange(zombie))     /* ��ʬ�����ͼ && ��ʬ�ڹ�����Χ�� */
	{
		plantEmission("StarFruit_Attack");
	}
}

void StarFruit::plantEmission(const string& plantAnimation)
{
	_isHaveZombies = true; /* ��ʾ�н�ʬ��ֲ��Ĺ�����Χ */
	if (!_isChanged)        /* �ж϶����Ƿ��Ѿ��ı� */
	{
		_plantAnimation->setMix("StarFruit_Normal", plantAnimation, 0.5f);
		_plantAnimation->setAnimation(0, plantAnimation, true);
		_isChanged = true;
	}
}

void StarFruit::plantRecovery(const string& plantAnimation)
{
	if (!_isHaveZombies) /* �ж��Ƿ��н�ʬ�����ҵĹ�����Χ�� */
	{
		if (_isChanged)
		{
			_plantAnimation->addAnimation(0, plantAnimation, true);
			_isChanged = false;
		}
	}

	_isHaveZombies = false; /* ÿѭ��һ�ξͳ�ʼ�� */
}

bool StarFruit::zombieInPlantAttackRange(Zombies* zombie)
{
	auto disY = fabs((zombie->getZombieAnimation()->getPositionY() + 50) - (_plantAnimation->getPositionY() + 60));
	auto disX = fabs(zombie->getZombieAnimation()->getPositionX() - _plantAnimation->getPositionX());

	return(getZombieIsSameLineWithPlant(zombie) && !getZombieIsTheFrontOfPlant(zombie)) ||     /* ��ʬ��ֲ��ͬһ�� && ��ʬ��ֲ��� */
		getZombieIsSameColumnWithPlant(zombie) ||                                              /* ��ʬ��ֲ����ͬһ�� */
		(getZombieIsTheFrontOfPlant(zombie) && zombieInObliqueDirection(disX, disY))           /* ��ʬ��ֲ��ǰ�� && ��ʬ��ֲ�﷽����ʮ��λ�� */
		? true : false;
}

bool StarFruit::zombieInObliqueDirection(float& disX, float& disY)
{
	const auto obd = sqrt(pow(disY, 2) + pow(disX, 2));
	return (disY * 2 > obd && disY * 2 - obd < 20) || (obd > disY * 2 && obd - disY * 2 < 80);
}

void StarFruit::createBullet()
{
	for (int i = 0; i < 5; ++i)
	{
		_bulletAnimation = new StarFruitBullet(_node);
		_bulletAnimation->setBulletPosition(_position + Vec2(-10, 70));
		_bulletAnimation->setBulletInRow(_rowAndColumn.y);
		_bulletAnimation->createBullet();
		dynamic_cast<StarFruitBullet*>(_bulletAnimation)->setBulletAction(static_cast<StarFruitBullet::StarFruitBulletDirection>(i));

		BulletGroup.push_back(_bulletAnimation);
	}
}

SkeletonAnimation* StarFruit::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("STARFRUIT_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("StarFruit", "StarFruit_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.5f);
	_plantAnimation->setPosition(Vec2(200, 600));

	SPSSpriteLayer::createPlantsText(0, lta.find("STARFRUIT_1")->second->text, Vec2(190, 910), lta.find("STARFRUIT_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("STARFRUIT_2")->second->text, Vec2(360, 1000), lta.find("STARFRUIT_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("STARFRUIT_3")->second->text, Vec2(440, 1000), lta.find("STARFRUIT_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, SPSSpriteLayer::selectRequirementText(lta, PlantsType::StarFruit, "STARFRUIT_4", "STARFRUIT_5"), Vec2(360, 870),
		lta.find("STARFRUIT_4")->second->fontsize, SPSSpriteLayer::isPlantIsCanSelect[static_cast<unsigned int>(PlantsType::StarFruit)] ? Color3B::ORANGE : Color3B(255, 70, 0), false);

	return _plantAnimation;
}

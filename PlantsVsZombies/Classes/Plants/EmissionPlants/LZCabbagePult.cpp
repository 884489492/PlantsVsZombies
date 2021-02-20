/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Emal: 2117610943@qq.com
 */

#include "LZCabbagePult.h"
#include "LZPeaShooter.h"
#include "Bullet/LZCabbage.h"

#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"

CabbagePult::CabbagePult(Node* node):
    _zombiePostion(Vec2::ZERO)
,   _zombie(nullptr)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_combatEffecttiveness = 0; // !!!��ʱû��ʹ�� ʹ�õ���cabbage�����_attack
	_plantsType = PlantsType::CabbagePult;
}

CabbagePult::~CabbagePult()
{
}

CabbagePult* CabbagePult::create(Node* node)
{
	CabbagePult* cabbagePult = new (std::nothrow) CabbagePult(node);
	if (cabbagePult && cabbagePult->init())
	{
		cabbagePult->autorelease();
		return cabbagePult;
	}
	CC_SAFE_DELETE(cabbagePult);
	return nullptr;
}
Sprite* CabbagePult::createPlantImage()
{
	imageInit("Cabbage", INIT);
	_plantImage->setScale(1.5f);
	_plantImage->setAnchorPoint(Vec2(0.6f, 0.6f));
	return _plantImage;
}

void CabbagePult::createPlantAnimation()
{
	_plantAnimation = plantInit("Cabbage", "Cabbage_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setTimeScale(1.f);
	_plantAnimation->setScale(0.8f);
	_node->addChild(_plantAnimation);

	// Ӱ��
	setPlantShadow(2.6f);

	// �����ɽ�����
	setPlantSoilSplashAnimation(1.0f);

	// ��������
	createListener("Cabbage_Shoot", "Shoot");
}

void CabbagePult::determineRelativePositionPlantsAndZombies()
{
	determineRelativePositionPlantsAndZombies("Cabbage_Normal");
}

void CabbagePult::plantAttack(Zombies* zombie)
{
	plantAttack(zombie, "Cabbage_Shoot");
}

void CabbagePult::determineRelativePositionPlantsAndZombies(const string& animationName)
{
	_distance = _MAX_; /* ��ʼ��������� */

	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* ��ʬ��ֲ�� */

		plantAttack(zombie);         /* ֲ�﹥�� */

		zombieRecoveryMove(zombie);  /* ��ʬ�ָ��ƶ� */
	}

	if (_isHaveZombies)  /* ����н�ʬ����Ĳ�Ͷ����ͬһ�� */
	{
		_zombiePostion = _zombie->getZombieAnimation()->getPosition();
		_zombieSpeed = _zombie->getZombieCurrentSpeed();
		//_zombieHeight = _zombie->getZombieAnimation()->getScale() * _zombie->getZombieAnimation()->getBoundingBox().size.height;
	}

	if (!_isHaveZombies) /* ���û�н�ʬ����Ĳ�Ͷ����ͬһ�� */
	{
		if (_isChanged)
		{
			_plantAnimation->addAnimation(0, animationName, true);
			_isChanged = false;
		}
	}

	_isHaveZombies = false; /* ÿѭ��һ�ξͳ�ʼ�� */
}

void CabbagePult::plantAttack(Zombies* zombie, const string& animationName)
{
	if (zombie->getZombieIsSurvive() && getPlantIsSurvive() && zombie->getZombieIsEnterMap() && /* ��ʬ��� && ֲ���� && ��ʬ�����ͼ */
		getZombieIsSameLineWithPlant(zombie) && getZombieIsTheFrontOfPlant(zombie))             /* ��ֲ����ͬһ�� && ��ʬ��ֲ���ǰ�� */
	{
		_isHaveZombies = true; /* ��ʾ�н�ʬ��ֲ����ͬһ�� */
		if (!_isChanged)
		{
			_plantAnimation->addAnimation(0, animationName, true);
			_isChanged = true;
		}

		float distance = zombie->getZombieAnimation()->getPositionX() - _plantAnimation->getPositionX();
		if (_distance > distance) /* ��ʬ��ֲ��ľ���С����һ����ʬ��ֲ��ľ��� */
		{
			_distance = distance;
			_zombie = zombie;     /* ��¼�ý�ʬ */
		}
	}
}

void CabbagePult::createBullet()
{
	_bulletAnimation = new Cabbage(_node);
	_bulletAnimation->setBulletPosition(_position);
	_bulletAnimation->setBulletInRow(_rowAndColumn.y);
	dynamic_cast<Cabbage*>(_bulletAnimation)->setZombiePosition(_zombiePostion);
	dynamic_cast<Cabbage*>(_bulletAnimation)->setZombieSpeed(_zombieSpeed);
	dynamic_cast<Cabbage*>(_bulletAnimation)->setZombieHeight(_zombieHeight);
	_bulletAnimation->createBullet();

	BulletGroup.push_back(_bulletAnimation);
}

SkeletonAnimation* CabbagePult::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("CABBAGE_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("Cabbage", "Cabbage_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.3f);
	_plantAnimation->setPosition(Vec2(200, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("CABBAGE_1")->second->text, Vec2(190, 910), lta.find("CABBAGE_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("CABBAGE_2")->second->text, Vec2(360, 1000), lta.find("CABBAGE_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("CABBAGE_3")->second->text, Vec2(440, 1000), lta.find("CABBAGE_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, lta.find("CABBAGE_4")->second->text, Vec2(360, 870), lta.find("CABBAGE_4")->second->fontsize, Color3B::ORANGE, false);

	return _plantAnimation;
}
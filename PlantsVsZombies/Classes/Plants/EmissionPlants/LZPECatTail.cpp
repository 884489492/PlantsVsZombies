/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.7
 *Email: 2117610943@qq.com
 */

#include "LZPECatTail.h"
#include "Bullet/LZPEBCatTailBullet.h"
#include "Scenes/GameScene/LZSGData.h"
#include "Zombies/LZZZombies.h"

CatTail::CatTail(Node* node)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_combatEffecttiveness = 0;
	_plantsType = PlantsType::CatTail;
}

CatTail::~CatTail()
{
}

CatTail* CatTail::create(Node* node)
{
	CatTail* catTail = new (std::nothrow) CatTail(node);
	if (catTail && catTail->init())
	{
		catTail->autorelease();
		return catTail;
	}
	CC_SAFE_DELETE(catTail);
	return nullptr;
}

Sprite* CatTail::createPlantImage()
{
	imageInit("CatTail", INIT);
	_plantImage->setScale(1.7f);
	_plantImage->setAnchorPoint(Vec2(0.55f, 0.43f));
	return _plantImage;
}

void CatTail::createPlantAnimation()
{
	_plantAnimation = plantInit("CatTail", "CatTail_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setTimeScale(1.f);
	_plantAnimation->setScale(1.1f);
	_node->addChild(_plantAnimation);

	// Ӱ��
	setPlantShadow(1.9f);
	_plantAnimation->getChildByName("plantshadow")->setPosition(Vec2(0, 25));

	// �����ɽ�����
	setPlantSoilSplashAnimation(0.8f);

	// ��������
	createListener("CatTail_Shoot");
}

void CatTail::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* ��ʬ��ֲ�� */

		zombieRecoveryMove(zombie);  /* ��ʬ�ָ��ƶ� */

		plantAttack(zombie);         /* ֲ�﹥�� */
	}

	plantRecovery("CatTail_Normal");
}

void CatTail::plantAttack(Zombies* zombie)
{
	if (getPlantIsSurvive() && zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap())     /* ֲ���� && ��ʬû������ && ��ʬ�����ͼ */
	{
		plantEmission("CatTail_Shoot");
	}
}

void CatTail::plantEmission(const string& plantAnimation)
{
	_isHaveZombies = true; /* ��ʾ�н�ʬ��ֲ����ͬһ�� */
	if (!_isChanged)     /* �ж϶����Ƿ��Ѿ��ı� */
	{
		_plantAnimation->addAnimation(0, plantAnimation, true);
		_isChanged = true;
	}
}

void CatTail::plantRecovery(const string& plantAnimation)
{
	if (!_isHaveZombies) /* �ж��Ƿ��н�ʬ */
	{
		if (_isChanged)
		{
			_plantAnimation->addAnimation(0, plantAnimation, true);
			_isChanged = false;
		}
	}

	_isHaveZombies = false; /* ÿѭ��һ�ξͳ�ʼ�� */
}

void CatTail::createBullet()
{
	_isCreateBullet = true;

	_bulletAnimation = new CatTailBullet(_node);
	_bulletAnimation->setBulletPosition(_position + Vec2(-65, 65));
	_bulletAnimation->setBulletInRow(_rowAndColumn.y);
	_bulletAnimation->createBullet();

	BulletGroup.push_back(_bulletAnimation);
}

SkeletonAnimation* CatTail::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("CATTAIL_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("CatTail", "CatTail_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.8f);
	_plantAnimation->setPosition(Vec2(200, 590));

	SPSSpriteLayer::createPlantsText(0, lta.find("CATTAIL_1")->second->text, Vec2(190, 910), lta.find("CATTAIL_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("CATTAIL_2")->second->text, Vec2(360, 1000), lta.find("CATTAIL_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("CATTAIL_3")->second->text, Vec2(440, 1000), lta.find("CATTAIL_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, SPSSpriteLayer::selectRequirementText(lta, PlantsType::CatTail, "CATTAIL_4", "CATTAIL_5"), Vec2(360, 870),
		lta.find("CATTAIL_4")->second->fontsize, SPSSpriteLayer::isPlantIsCanSelect[static_cast<unsigned int>(PlantsType::CatTail)] ? Color3B::ORANGE : Color3B(255, 70, 0), false);
	return _plantAnimation;
}
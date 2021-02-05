/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "LZPBJalapeno.h"

#include "Zombies/LZZZombies.h"
#include "Scenes/GameScene/LZSGData.h"
#include "Scenes/GameScene/LZSGBackgroundLayer.h"
#include "Scenes/GameScene/LZSGDefine.h"
#include "Based/LZBPlayMusic.h"

Jalapeno::Jalapeno(Node* node)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_isLoop = false;
	_combatEffecttiveness = 1800;
	_plantsType = PlantsType::Jalapeno;
}

Jalapeno::~Jalapeno()
{
}

Jalapeno* Jalapeno::create(Node* node)
{
	Jalapeno* jalapeno = new (std::nothrow) Jalapeno(node);
	if (jalapeno && jalapeno->init())
	{
		jalapeno->autorelease();
		return jalapeno;
	}
	CC_SAFE_DELETE(jalapeno);
	return nullptr;
}
Sprite* Jalapeno::createPlantImage()
{
	imageInit("Jalapeno", INIT);
	_plantImage->setScale(1.4f);
	return _plantImage;
}

void Jalapeno::createPlantAnimation()
{
	_plantAnimation = plantInit("Jalapeno", "Jalapeno_Explode");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setTimeScale(1.5f);
	_node->addChild(_plantAnimation);

	// Ӱ��
	setPlantShadow(1.8f);

	// �����ɽ�����
	setPlantSoilSplashAnimation(0.8f);

	// �¼�����
	createListener();
}

void Jalapeno::plantExplode()
{
	if (!getPlantIsSurvive() || getPlantIsReadyExplode()) /* ֲ������ || ֲ��׼����ը */
	{
		if (!_isExplode) /* ���û�б�ը */
		{
			_isExplode = true;
			explodeHurtZombies();
			showExplodeAnimation();
		}
	}
}

void Jalapeno::showExplodeAnimation()
{
	PlayMusic::playMusic("cherrybomb");

	GSBackgroundLayer::backgroundRunAction();

	for (int i = 0; i < 9; i++)
	{
		auto jalapenoFire = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("Jalapeno_Fire")->second);
		jalapenoFire->setPosition(Vec2(GRASS_POSITION_LEFT + 122 * i + 60, _plantAnimation->getPositionY() - 10));
		jalapenoFire->setAnimation(0, "animation", false);
		jalapenoFire->setScaleY(3.0f);
		jalapenoFire->setLocalZOrder(_plantAnimation->getLocalZOrder() + 10); // ֲ�����˳���10���õ��ڽ�ʬ����˳�� ����ը�Ϳ��Ը��ǵ���ʬ����
		jalapenoFire->runAction(Sequence::create(DelayTime::create(2),
			CallFunc::create([jalapenoFire]()
				{
					jalapenoFire->removeFromParent();
				}), nullptr));
		_node->addChild(jalapenoFire);
	}
}

bool Jalapeno::getZombieIsInExplodeRange(Zombies* zombie)
{
	return getPlantRow() == zombie->getZombieInRow() && zombie->getZombieAnimation()->getPositionX() > 570 ? true : false;
}

void Jalapeno::setPlantNoramlAnimation()
{
	_plantAnimation->setAnimation(0, "Jalapeno_Normal", true);
}

SkeletonAnimation* Jalapeno::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("JALAPENO_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("Jalapeno", "Jalapeno_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.5f);
	_plantAnimation->setPosition(Vec2(200, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("JALAPENO_1")->second->text, Vec2(190, 910), lta.find("JALAPENO_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("JALAPENO_2")->second->text, Vec2(360, 1000), lta.find("JALAPENO_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("JALAPENO_3")->second->text, Vec2(440, 1000), lta.find("JALAPENO_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, SPSSpriteLayer::selectRequirementText(lta, PlantsType::Jalapeno, "JALAPENO_4", "JALAPENO_5"), Vec2(360, 870),
		lta.find("JALAPENO_4")->second->fontsize, SPSSpriteLayer::isPlantIsCanSelect[static_cast<unsigned int>(PlantsType::Jalapeno)] ? Color3B::ORANGE : Color3B(255, 70, 0), false);
	
	return _plantAnimation;
}

/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.1.16
 *Email: 2117610943@qq.com
 */

#include "LZChomper.h"

#include "Based/LZPlayMusic.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Zombies/LZZombies.h"

Chomper::Chomper(Node* node):
	_isCanEat(true)
,   _chewTime(0)
,   _attackZombie(nullptr)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_isLoop = true;
	_combatEffecttiveness = 3000;
	_plantsType = PlantsType::Chomper;
}

Chomper::~Chomper()
{
}

Chomper* Chomper::create(Node* node)
{
	Chomper* chomper = new (std::nothrow) Chomper(node);
	if (chomper && chomper->init())
	{
		chomper->autorelease();
		return chomper;
	}
	CC_SAFE_DELETE(chomper);
	return nullptr;
}

Sprite* Chomper::createPlantImage()
{
	imageInit("Chomper", INIT);
	_plantImage->setScale(1.7f);
	_plantImage->setAnchorPoint(Vec2(0.37f, 0.45f));
	return _plantImage;
}

void Chomper::createPlantAnimation()
{
	_plantAnimation = plantInit("Chomper", "Chomper_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.7f);
	_node->addChild(_plantAnimation);

	// Ӱ��
	setPlantShadow(1.4f);
	_plantAnimation->getChildByName("plantshadow")->setPosition(Vec2(0, 10));

	// �����ɽ�����
	setPlantSoilSplashAnimation(0.5f);

	// ��������(��ȡ�浵����)
	createListener();
}

void Chomper::createListener()
{
	if (!_isCanEat)
	{
		_plantAnimation->setAnimation(0, "Chomper_Eat_Chew", true);
		_plantAnimation->runAction(Sequence::create(DelayTime::create(_chewTime),
			CallFunc::create([=]()
				{
					_plantAnimation->addAnimation(0, "Chomper_Eat_Pharynx", false);
					_plantAnimation->addAnimation(0, "Chomper_Normal", true);
					_chewTime = 0;
				}), nullptr));
	}

	_plantAnimation->setEventListener([=](spTrackEntry* entry, spEvent* event)
		{
			if (!strcmp(event->data->name, "Attack_Begin") && getPlantIsSurvive())
			{
				if (_attackZombie) // �����ʬ����
				{
					PlayMusic::playMusic("bigchomp");

					if (!isCanKillZombieOnce(_attackZombie))
					{
						_combatEffecttiveness = 80;
						hurtZombies(_attackZombie);
					}
					else
					{
						_attackZombie->setZombieVisible(false);
						_attackZombie->getZombieAnimation()->setAnimation(0, "Zombies_Stand", true);
					}
				}
			}

			if (!strcmp(event->data->name, "Pharynx_Finished") && !_chewTime)
			{
				_isCanEat = true;
			}
		});
}

void Chomper::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* ��ʬ��ֲ�� */

		plantAttack(zombie);         /* ֲ�﹥�� */

		zombieRecoveryMove(zombie);  /* ��ʬ�ָ��ƶ� */
	}
}

void Chomper::plantAttack(Zombies* zombie)
{
	if (getPlantIsSurvive() && zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() &&  /* ֲ���� && ��ʬ��� && ��ʬ�����ͼ */
		getZombieIsSameLineWithPlant(zombie) && getZombieIsEncounterPlant(zombie))               /* ֲ���뽩ʬ��ͬһ�� && ��ʬ��ֲ�﹥����Χ */
	{
		if (_isCanEat) /* �����컨���ԳԽ�ʬ */
		{ 
			_isCanEat = false;
			_attackZombie = zombie;

			if (isCanKillZombieOnce(zombie) && !zombie->getZombieReserveKill()) /* �����ʬ���Ա�һ��ɱ�� && ��ʬû�б�Ԥ��ɱ�� */
			{
				_chewTime = 30;
				zombie->setZombieReserveKill(true); /* ��ʬ�ѱ�Ԥ��ɱ�� */
				zombieRelieveReserveKill(zombie);   /* ��ʬ��һ��ʱ����Զ������������ */
				chomperKillZombie();
			}
			else
			{
				chomperHurtZombie();
			}
		}
	}
}

bool Chomper::getZombieIsEncounterPlant(Zombies* zombie)
{
	return fabs(zombie->getZombiePositionX() - _plantAnimation->getPositionX()) <=
		((zombie->getZombieIsEat() || !isCanKillZombieOnce(zombie)) ? 230 : 150) ? true : false;
}

bool Chomper::isCanKillZombieOnce(Zombies* zombie)
{
	auto blood =
		zombie->getZombieCurrentBloodVolume() +
		zombie->getZombieCurrentBodyShieldVolume() +
		zombie->getZombieCurrentHeadShieldVolume();

	if (blood < _combatEffecttiveness)return true;
	return false;
}

void Chomper::chomperKillZombie()
{
	_plantAnimation->setAnimation(0, "Chomper_Eat", false);
	_plantAnimation->addAnimation(0, "Chomper_Eat_Chew", true);
	_plantAnimation->runAction(Repeat::create(Sequence::create(DelayTime::create(1.0f),
		CallFunc::create([=]()
			{
				if (--_chewTime < 0)_chewTime = 0;
			}), nullptr), 30));
	_plantAnimation->runAction(Sequence::create(DelayTime::create(_chewTime),
		CallFunc::create([=]()
			{
				_plantAnimation->addAnimation(0, "Chomper_Eat_Pharynx", false);
				_plantAnimation->addAnimation(0, "Chomper_Normal", true);
			}), nullptr));
}

void Chomper::chomperHurtZombie()
{
	_plantAnimation->setAnimation(0, "Chomper_Eat_Attack", false);
	_plantAnimation->addAnimation(0, "Chomper_Normal", true);
	_isCanEat = true;
}

void Chomper::zombieRelieveReserveKill(Zombies* zombie)
{
	zombie->getZombieAnimation()->runAction(Sequence::create(DelayTime::create(0.7f), 
		CallFunc::create([=]() 
			{
				if (zombie->getZombieIsSurvive()) 
				{
					zombie->setZombieReserveKill(false);
				}
			}), nullptr));
}

int Chomper::getChewTime() const
{
	return _chewTime;
}

bool Chomper::getIsCanEatZombie() const
{
	return _isCanEat;
}

void Chomper::setChewTime(const int time)
{
	_chewTime = time;
}

void Chomper::setIsCanEatZombie(const bool isCanEat)
{
	_isCanEat = isCanEat;
}

void Chomper::cavePlantInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator)
{
	object.AddMember("IsCanEat", _isCanEat, allocator);
	object.AddMember("ChewTime", _chewTime, allocator);
}

void Chomper::readPlantInforamtion(rapidjson::Document* levelDataDocument, char* key, int i)
{
	_isCanEat = (*levelDataDocument)[key]["Plants"][to_string(i).c_str()]["IsCanEat"].GetBool();
	if (!_isCanEat)_chewTime = (*levelDataDocument)[key]["Plants"][to_string(i).c_str()]["ChewTime"].GetInt();

	if ((*levelDataDocument)[key]["Plants"][to_string(i).c_str()]["AnimationNumber"].GetInt() > 1)
	{
		_isCanEat = true;
	}
}

SkeletonAnimation* Chomper::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("CHOMER_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("Chomper", "Chomper_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(2.3f);
	_plantAnimation->setPosition(Vec2(200, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("CHOMER_1")->second->text, Vec2(190, 910), lta.find("CHOMER_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("CHOMER_2")->second->text, Vec2(360, 1000), lta.find("CHOMER_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("CHOMER_3")->second->text, Vec2(440, 1000), lta.find("CHOMER_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, SPSSpriteLayer::selectRequirementText(lta, PlantsType::Chomper, "CHOMER_4", "CHOMER_5"), Vec2(360, 870),
		lta.find("CHOMER_4")->second->fontsize, SPSSpriteLayer::isPlantIsCanSelect[static_cast<unsigned int>(PlantsType::Chomper)] ? Color3B::ORANGE : Color3B(255, 70, 0), false);
	
	return _plantAnimation;
}
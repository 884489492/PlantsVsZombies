/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.27
 *Email: 2117610943@qq.com
 */

#include "LZPECitron.h"
#include "Bullet/LZPEBCitronBullet.h"

#include "Zombies/LZZZombies.h"
#include "Scenes/GameScene/LZSGData.h"
#include "Based/LZBPlayMusic.h"

Citron::Citron(Node* node):
	_attackInterval(0)
,   _animationId(0)
,   _readyFinished(false)
,   _isCreateBullet(false)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_combatEffecttiveness = 0;
	_plantsType = PlantsType::Citron;
	srand(time(nullptr));
}

Citron::~Citron()
{
}

Citron* Citron::create(Node* node)
{
	Citron* citron = new (std::nothrow) Citron(node);
	if (citron && citron->init())
	{
		citron->autorelease();
		return citron;
	}
	CC_SAFE_DELETE(citron);
	return nullptr;
}

Sprite* Citron::createPlantImage()
{
	imageInit("Citron", INIT);
	_plantImage->setScale(1.3f);
	_plantImage->setAnchorPoint(Vec2(0.4f, 0.7f));
	return _plantImage;
}

void Citron::createPlantAnimation()
{
	_plantAnimation = plantInit("Citron", "normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.1f);
	_node->addChild(_plantAnimation);

	// Ӱ��
	setPlantShadow(1.9f);
	_plantAnimation->getChildByName("plantshadow")->setPosition(Vec2(0, 15));

	// �����ɽ�����
	setPlantSoilSplashAnimation(0.8f);

	// ��������
	createListener();
}

void Citron::createListener()
{
	_plantAnimation->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.5f),
		CallFunc::create([&]()
			{
				++_attackInterval;
				if (_attackInterval >= 16 && !_readyFinished)
				{
					_readyFinished = true;
					_isCreateBullet = false;
				}
			}), nullptr)));

	_plantAnimation->setEventListener([&](spTrackEntry* entry, spEvent* event)
		{
			if (strcmp(event->data->name, "shoot") == 0)
			{
				rand() % 2 == 0 ? PlayMusic::playMusic("throw") : PlayMusic::playMusic("throw2");
				
				!_isCreateBullet ? createBullet() : nullptr;
				plantRecovery("normal");
			}
		});
}

void Citron::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* ��ʬ��ֲ�� */

		plantAttack(zombie);         /* ֲ�﹥�� */

		zombieRecoveryMove(zombie);  /* ��ʬ�ָ��ƶ� */
	}
}

void Citron::plantAttack(Zombies* zombie)
{
	if (getPlantIsSurvive() && zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() &&     /* ֲ���� && ��ʬû������ && ��ʬ�����ͼ */
		getZombieIsTheFrontOfPlant(zombie) && getZombieIsSameLineWithPlant(zombie))                 /* ��ʬ��ֲ����ͬһ�� && ��ʬ��ֲ���ǰ�� */
	{
		plantEmission(calculateGreatEvocationProbability() ? "shoot1" : "shoot");
	}
}

void Citron::plantEmission(const string& plantAnimation)
{
	if (!_isChanged && _readyFinished)     /* �ж϶����Ƿ��Ѿ��ı� */
	{
		_plantAnimation->addAnimation(0, plantAnimation, true);
		_isChanged = true;
	}
}

void Citron::plantRecovery(const string& plantAnimation)
{
	if (_isChanged)
	{
		_plantAnimation->addAnimation(0, plantAnimation, true);
		_readyFinished = false; /* ��ʼ׼�� */
		_attackInterval = 0;

		_isChanged = false;
	}
}

int Citron::calculateGreatEvocationProbability()
{
	int number = rand() % 100;
	if (number < 5)
		_animationId = 1;
	else
		_animationId = 0;
	return _animationId;
}

void Citron::createBullet()
{
	_isCreateBullet = true;

	_bulletAnimation = new CitronBullet(_node, _animationId);
	_bulletAnimation->setBulletPosition(_position);
	_bulletAnimation->setBulletInRow(_rowAndColumn.y);
	_bulletAnimation->createBullet();

	BulletGroup.push_back(_bulletAnimation);
}

void Citron::cavePlantInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator)
{
	object.AddMember("isChanged", _isChanged, allocator);
	object.AddMember("isCreateBullet", _isCreateBullet, allocator);
	object.AddMember("attackInterval", _attackInterval, allocator);
}

void Citron::readPlantInforamtion(rapidjson::Document* levelDataDocument, char* key, int i)
{
	_isChanged = (*levelDataDocument)[key]["Plants"][to_string(i).c_str()]["isChanged"].GetBool();
	_isCreateBullet = (*levelDataDocument)[key]["Plants"][to_string(i).c_str()]["isCreateBullet"].GetBool();
	_attackInterval = (*levelDataDocument)[key]["Plants"][to_string(i).c_str()]["attackInterval"].GetInt();
}

SkeletonAnimation* Citron::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("CITRON_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("Citron", "normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(2.0f);
	_plantAnimation->setPosition(Vec2(200, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("CITRON_1")->second->text, Vec2(190, 910), lta.find("CITRON_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("CITRON_2")->second->text, Vec2(360, 1000), lta.find("CITRON_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("CITRON_3")->second->text, Vec2(440, 1000), lta.find("CITRON_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, SPSSpriteLayer::selectRequirementText(lta, PlantsType::Citron, "CITRON_4", "CITRON_5"), Vec2(360, 870),
		lta.find("CITRON_4")->second->fontsize, SPSSpriteLayer::isPlantIsCanSelect[static_cast<unsigned int>(PlantsType::Citron)] ? Color3B::ORANGE : Color3B(255, 70, 0), false);

	return _plantAnimation;
}

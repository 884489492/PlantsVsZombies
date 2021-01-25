/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.5
 *Email: 2117610943@qq.com
 */

#include "LZPPlants.h"

#include "Zombies/LZZZombies.h"
#include "Scenes/GameScene/LZSGData.h"
#include "Based/LZBPlayMusic.h"

int Plants::plantNumber = -1;
GLProgram* Plants::_normalGLProgram = nullptr;
GLProgram* Plants::_highLightGLProgram = nullptr;

#ifdef _DEBUG
PlantsInformation::PlantsCardInformation plantsCardInformation[] =
{
	{"SunFlower",              "PlantsIcon4",     "B",    3,    0,          0,         0,      0, 0,     false,   PlantsType::SunFlower,           Color3B::YELLOW                         },  /* ���տ� */
	{"PeaShooter",             "PlantsIcon5",     "C",    3,    1,          0,         0,      0, 0,     false,   PlantsType::PeaShooter,          Color3B::GREEN                          },  /* �㶹���� */
	{"Wallnut_body",           "PlantsIcon6",     "C",    3,    2,          0,         0,      0, 0,     false,   PlantsType::WallNut,             Color3B(140,80,10)                      },  /* ���ǽ */
	{"CherryBomb",             "PlantsIcon7",     "A",    2,    3,          0,         0,      0, 0,     false,   PlantsType::CherryBomb,          Color3B::RED                            },  /* ӣ��ը�� */
	{"PotatoMine",             "PlantsIcon7",     "B",    3,    4,          0,         0,      0, 0,     false,   PlantsType::PotatoMine,          Color3B(250,75,10)                      },  /* ������ */
	{"Cabbage",                "PlantsIcon3",     "B",    3,    5,          0,         0,      0, 0,     false,   PlantsType::CabbagePult,         Color3B(64,0,0)                         },  /* ���Ĳ�Ͷ�� */
	{"Torchwood",              "PlantsIcon2",     "C",    3,    6,          0,         0,      0, 0,     false,   PlantsType::Torchwood,           Color3B::RED                            },  /* �����׮ */
	{"Spikeweed",              "PlantsIcon12",    "B",    3,    7,          0,         0,      0, 0,     false,   PlantsType::Spikeweed,           Color3B::BLACK                          },  /* �ش� */
	{"Garlic",                 "PlantsIcon10",    "B",    3,    8,          0,         0,      0, 0,     false,   PlantsType::Garlic,              Color3B(172,255,0)                      },  /* ���� */
	{"Chomper",                "PlantsIcon8",     "B",    3,    9,          0,         0,      0, 0,     false,   PlantsType::Chomper,             Color3B(163,73,164),  { 1000,  1000  }  },  /* ���컨 */
	{"IcePeaShooter",          "PlantsIcon15",    "A",    2,    10,         0,         0,      0, 0,     false,   PlantsType::IcePeaShooter,       Color3B(0,162,232),   { 1500,  2000  }  },  /* �����㶹���� */
	{"FirePeaShooter",         "PlantsIcon2",     "A",    2,    11,         0,         0,      0, 0,     false,   PlantsType::FirePeaShooter,      Color3B::RED,         { 2200,  3000  }  },  /* �����㶹���� */
	{"Jalapeno",               "PlantsIcon2",     "A",    2,    12,         0,         0,      0, 0,     false,   PlantsType::Jalapeno,            Color3B::RED,         { 3500,  5000  }  },  /* ������ */
	{"JalapenoVariation",      "PlantsIcon2",     "S",    1,    13,         0,         0,      0, 0,     true,    PlantsType::JalapenoVariation,   Color3B::RED,         { 4000,  6000  }  },  /* ���������� */
	{"LemonShooter",           "PlantsIcon10",    "A",    1,    14,         0,         0,      0, 0,     false,   PlantsType::AcidLemonShooter,    Color3B::ORANGE,      { 5000,  8000  }  },  /* �������� */
	{"Citron",                 "PlantsIcon",      "SS",   1,    15,         0,         0,      0, 0,     false,   PlantsType::Citron,              Color3B(0,255,255),   { 10000, 14000 }  },  /* ����Ե */
	{"GloomShroom",            "PlantsIcon9",     "SSS",  1,    16,         0,         0,      0, 0,     false,   PlantsType::GloomShroom,         Color3B(163,73,164),  { 15000, 20000 }  },  /* ������ */
	/* ֲ��ͼƬ���� */         /* ��־���� */    /*Ʒ��*/    /*���ȼ�*/  /*������*/  /*ʱ��*/           /*��ת*/       /*ֲ������*/                   /* ��ɫ */            /* Ҫ�� */
};
#else
PlantsInformation::PlantsCardInformation plantsCardInformation[] =
{
	{"SunFlower",             "PlantsIcon4",    "B",    3,   0,          50,          7.5f,      0, 0,     false,   PlantsType::SunFlower,           Color3B::YELLOW                         },  /* ���տ� */
	{"PeaShooter",            "PlantsIcon5",    "C",    3,   1,          100,         7.5f,      0, 0,     false,   PlantsType::PeaShooter,          Color3B::GREEN                          },  /* �㶹���� */
	{"Wallnut_body",          "PlantsIcon6",    "C",    3,   2,          50,          30,        0, 0,     false,   PlantsType::WallNut,             Color3B(140,80,10)                      },  /* ���ǽ */
	{"CherryBomb",            "PlantsIcon7",    "A",    2,   3,          150,         37.5f,     0, 0,     false,   PlantsType::CherryBomb,          Color3B::RED                            },  /* ӣ��ը�� */
	{"PotatoMine",            "PlantsIcon7",    "C",    3,   4,          25,          30,        0, 0,     false,   PlantsType::PotatoMine,          Color3B(250,75,10)                      },  /* ������ */
	{"Cabbage",               "PlantsIcon3",    "B",    3,   5,          100,         7.5f,      0, 0,     false,   PlantsType::CabbagePult,         Color3B(64,0,0)                         },  /* ���Ĳ�Ͷ�� */
	{"Torchwood",             "PlantsIcon2",    "C",    3,   6,          175,         7.5f,      0, 0,     false,   PlantsType::Torchwood,           Color3B::RED                            },  /* �����׮ */
	{"Spikeweed",             "PlantsIcon12",   "B",    3,   7,          100,         7.5f,      0, 0,     false,   PlantsType::Spikeweed,           Color3B::BLACK                          },  /* �ش� */
	{"Garlic",                "PlantsIcon10",   "B",    3,   8,          50,          10,        0, 0,     false,   PlantsType::Garlic,              Color3B(172,255,0)                      },  /* ���� */
	{"Chomper",               "PlantsIcon8",    "B",    3,   9,          150,         7.5f,      0, 0,     false,   PlantsType::Chomper,             Color3B(163,73,164),  { 1000,  1000  }  },  /* ���컨 */
	{"IcePeaShooter",         "PlantsIcon15",   "B",    2,   10,         175,         10,        0, 0,     false,   PlantsType::IcePeaShooter,       Color3B(0,162,232),   { 1500,  2000  }  },  /* �����㶹���� */
	{"FirePeaShooter",        "PlantsIcon2",    "B",    2,   11,         200,         10,        0, 0,     false,   PlantsType::FirePeaShooter,      Color3B::RED,         { 2200,  3000  }  },  /* �����㶹���� */
	{"Jalapeno",              "PlantsIcon2",    "A",    2,   12,         150,         40,        0, 0,     false,   PlantsType::Jalapeno,            Color3B::RED,         { 3500,  5000  }  },  /* ������ */
	{"JalapenoVariation",     "PlantsIcon2",    "S",    1,   13,         175,         40,        0, 0,     true,    PlantsType::JalapenoVariation,   Color3B::RED,         { 4000,  6000  }  },  /* ���������� */
	{"LemonShooter",          "PlantsIcon10",   "A",    1,   14,         125,         7.5f,      0, 0,     false,   PlantsType::AcidLemonShooter,    Color3B::ORANGE,      { 5000,  8000  }  },  /* �������� */
	{"Citron",                "PlantsIcon",     "SS",   1,   15,         350,         7.5f,      0, 0,     false,   PlantsType::Citron,              Color3B(0,255,255),   { 10000, 14000 }  },  /* ����Ե */
	{"GloomShroom",           "PlantsIcon9",    "SSS",  1,   16,         300,         40,        0, 0,     false,   PlantsType::GloomShroom,         Color3B(163,73,164),  { 15000, 20000 }  },  /* ������ */
	/* ֲ��ͼƬ���� */       /* ��־���� */    /*Ʒ��*/   /*���ȼ�*/  /*������*/   /*ʱ��*/              /*��ת*/       /*ֲ������*/                   /* ��ɫ */            /* Ҫ�� */
};
#endif // _DEBUG

/*���ֲ����Ҫ�޸ĵĺ���
 * GameScene->AnimationLayer:��Ҫ�޸ģ�
 * 1. createDifferentPlants(PlantsType plantsType)
 *
 * plants->plants��Ҫ�޸ģ�
 * 1. ���plantsType
 */

Plants::Plants(Node* node, const Vec2& position) :
	_node(node)
,	_position(position)
,	_plantShadow(nullptr)
,	_plantImage(nullptr)
,	_plantAnimation(nullptr)
,	_soilSplashAnimation(nullptr)
,   _highLightGLProgramState(nullptr)
,	_sunNeed(NOINITIALIZATION)
,	_coolDownTime(NOINITIALIZATION)
,	_costGold(NOINITIALIZATION)
,	_costMasonry(NOINITIALIZATION)
,	_healthPoint(NOINITIALIZATION)
,	_plantTag(NOINITIALIZATION)
,	_isLoop(true)
,   _isCanDelete{ false,false }
,   _zOrder(0)
,   _highLightIntensity(0.6f)
,   _plantNumber(++plantNumber)
,   _combatEffecttiveness(0)
,   _plantsType(PlantsType::None)
,	_global(Global::getInstance())
{
}

Plants::~Plants()
{
}

SkeletonAnimation* Plants::plantInit(const std::string& plantname, const std::string& animaionname)
{
	auto iter = _global->userInformation->getAnimationData().find(plantname);
	if (iter != _global->userInformation->getAnimationData().end())/* ��������ҵ� */
	{
		_plantAnimation = SkeletonAnimation::createWithData(iter->second);
		_plantAnimation->setPosition(_position);
		_plantAnimation->setAnimation(0, animaionname, _isLoop);
		_plantAnimation->setLocalZOrder(_zOrder);
		_plantAnimation->setTag(_plantTag);
		_plantAnimation->setTimeScale(0.7f + rand() % 11 / 100.f);
		_plantAnimation->update(0);
		setPlantGLProgram();
		return _plantAnimation;
	}
	return nullptr;
}

void Plants::imageInit(const std::string& name, const Vec2& position)
{
	_plantImage = Sprite::createWithSpriteFrameName(name + ".png");
	_plantImage->setPosition(position);
	_plantImage->setName("Preview");
	_plantImage->setOpacity(150);
	_node->addChild(_plantImage, 99);
}

void Plants::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* ��ʬ��ֲ�� */

		zombieRecoveryMove(zombie);  /* ��ʬ�ָ��ƶ� */
	}
}

void Plants::stopPlantsAllAction()
{
	for (auto& sunFlower : PlantsGroup)
	{
		sunFlower.second->getPlantAnimation()->stopAllActions();
	}
}

void Plants::setPlantSoilSplashAnimation(const float& scale)
{
	/* ������ֲ�����ɽ����� */
	auto iter = _global->userInformation->getAnimationData().find("SplashOfSoil");
	if (iter != _global->userInformation->getAnimationData().end())
	{
		_soilSplashAnimation = SkeletonAnimation::createWithData(iter->second);
		_soilSplashAnimation->setPosition(Vec2(0, -50));
		_soilSplashAnimation->setAnimation(0, "SplashOfSoil", false);
		_soilSplashAnimation->setScale(scale);
		_soilSplashAnimation->setName("SplashOfSoil");
		_soilSplashAnimation->setGlobalZOrder(_plantAnimation->getGlobalZOrder());
		_plantAnimation->addChild(_soilSplashAnimation);
	}
}

void Plants::setPlantShadow(const float& scale)
{
	/* ����ֲ��Ӱ�� */
	_plantShadow = Sprite::createWithSpriteFrameName("plantshadow.png");
	_plantShadow->setScale(scale);
	_plantShadow->setPosition(Vec2(-5, 15));
	_plantShadow->setName("plantshadow");
	_plantAnimation->addChild(_plantShadow, -1);
}

void Plants::setPlantHurtBlink()
{
	auto action = Repeat::create(Sequence::create(
		CallFunc::create([this]()
			{
				_highLightIntensity -= 0.01f;
				_highLightGLProgramState->setUniformFloat("intensity", _highLightIntensity);
			}), DelayTime::create(0.005f), nullptr), 50);

	_plantAnimation->runAction(Sequence::create(
		CallFunc::create([this]()
			{
				_plantAnimation->setGLProgram(_highLightGLProgram);
				_highLightGLProgramState = _plantAnimation->getGLProgramState();
				_highLightGLProgramState->setUniformFloat("intensity", 0.6f);
			}), DelayTime::create(0.15f),
		CallFunc::create([this]()
			{
				_plantAnimation->setGLProgram(_normalGLProgram);
				_highLightIntensity = 0.6f;
			}), nullptr));
}

void Plants::setPlantHurtBlink(PlantsType type) const
{
	_plantAnimation->runAction(Sequence::create(TintTo::create(0.15f, Color3B(70, 70, 70)), TintTo::create(0.15f, Color3B::WHITE), nullptr));
}

void Plants::setPlantVisible(const bool visible)
{
	_plantAnimation->setVisible(visible);
}

void Plants::setPlantScale()
{
	_plantAnimation->setScale(_plantAnimation->getScale() + (4 - _plantTag / 10) / 20.f);
}

bool Plants::getPlantIsSurvive() const
{
	if (_healthPoint > 0 && _plantAnimation->isVisible())
		return true;
	return false;
}

bool Plants::getPlantVisible()
{
	return _plantAnimation->isVisible();
}

bool* Plants::getPlantIsCanDelete()
{
	return _isCanDelete;
}

bool Plants::getZombieIsTheFrontOfPlant(Zombies* zombie)
{
	return zombie->getZombiePositionX() >= _plantAnimation->getPositionX() ? true : false;
}

bool Plants::getZombieIsSameLineWithPlant(Zombies* zombie)
{
	return zombie->getZombieInRow() == getPlantRow() ? true : false;
}

bool Plants::getZombieIsEncounterPlant(Zombies* zombie)
{
	return fabs(zombie->getZombiePositionX() - _plantAnimation->getPositionX()) <= 70 ? true : false;
}

void Plants::zombieEatPlant(Zombies* zombie)
{
	if (getPlantIsSurvive() && getZombieIsSameLineWithPlant(zombie) && Plants::getZombieIsEncounterPlant(zombie)&& /* ֲ���� && ֲ���뽩ʬ��ͬһ�� && ��ʬ����ֲ�� */
		zombie->getZombieType() != ZombiesType::SnowZombies) /* ��ʬ����ѩ�˽�ʬ */
	{
		if (zombie->getZombieIsSurvive() && !zombie->getZombieIsEat())
		{
			const string eateffect[3] = { "chomp","chomp2","chompsoft" };
			zombie->setZombieEatPlantNumber(_plantNumber);
			zombie->setZombieStop();
			zombie->setZombieIsEat(true);
			zombie->getZombieAnimation()->setAnimation(0,
				zombie->getZombieType() == ZombiesType::LmpZombies ? "Zombies_Eat" : rand() % 4 ? "Zombies_Eat" : "Zombies_Eat1", true);
			zombie->getZombieAnimation()->setEventListener([this, eateffect, zombie](spTrackEntry* entry, spEvent* event)
				{
					if (!strcmp(event->data->name, "eat") && zombie->getZombieIsSurvive())
					{
						if (event->intValue == 1)
						{
							reducePlantHealthPoint(100);
							PlayMusic::playMusic(eateffect[rand() % 3]);
							setPlantHurtBlink();
						}
					}
				});
		}
	}
}

void Plants::zombieRecoveryMove(Zombies* zombie)
{
	if (!getPlantIsSurvive() && zombie->getZombieEatPlantNumber() == _plantNumber &&  /* ֲ������ && ��ʬ�ǳԵĸ�ֲ�� */
		zombie->getZombieIsEat() && zombie->getZombieIsStop()) /* ��ʬ���ڳ�ֲ�� && ��ʬ����ֹͣ�ƶ� */
	{
		setPlantVisible(false);
		_plantAnimation->stopAllActions();
		_plantAnimation->setTimeScale(0);
		getPlantType() != PlantsType::PotatoMine ? PlayMusic::playMusic("gulp") : nullptr;

		zombie->setZombieIsEat(false);
		if (!zombie->getZombieIsPlayDieAnimation()) /* ��ʬû�в����������� */
		{
			zombie->getZombieAnimation()->setMix("Zombies_Eat", Zombies::getZombieAniamtionName(zombie->getZombieType()), 0.5f);
			zombie->getZombieAnimation()->setAnimation(0, Zombies::getZombieAniamtionName(zombie->getZombieType()), true);
			//zombie->update(0.35f + (rand() % 10) / 100.f);
			zombie->setZombieCurrentSpeed(zombie->getZombieSpeed());
		}
	}
}

void Plants::setPlantGLProgram()
{
	if (!_normalGLProgram || !_highLightGLProgram)
	{
		_normalGLProgram = _plantAnimation->getGLProgram();
		_highLightGLProgram = Zombies::getHighLight();
	}
}

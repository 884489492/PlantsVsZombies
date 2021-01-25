/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.18
 *Email: 2117610943@qq.com
 */

#include "LZPEBCabbage.h"
#include "../LZPEPeaShooter.h"

#include "Zombies/LZZZombies.h"
#include "Scenes/GameScene/LZSGData.h"

Cabbage::Cabbage(Node* node) :
  _zombiePosition(Vec2::ZERO)
, _zombieSpeed(0)
, _zombieHeight(0)
, _isFileData(false)
{
	_node = node;
	_attack = 60;
	_bulletType = BulletType::Cabbage;
}

void Cabbage::createBullet()
{
	float height = 300, time = 1.0f;
	Vec2 endPosition = calculateZombiePosition();
	Vec2 initPosition = _position + Vec2(70, 150);

	if (_isFileData)
	{
		_isFileData = false;
		auto position = _position + Vec2(70, 150); //���Ĳ�������ʼλ��

		auto distance = _zombiePosition.x - position.x; // ��ʬ����Ĳ˳�ʼ����
		if (_currentPosition.x <= _zombiePosition.x - distance / 2) // ��ǰλ�����������ϰ벿��
			height = 300 - (_currentPosition.y - position.y);
		else
			height = 0;
		time = (distance - (_currentPosition.x - _position.x)) / distance; //����ʣ���˶�ʱ��
		if (time < 0)time = 0;
		if (time > 1)time = 1;

		initPosition = _currentPosition;
		endPosition = _zombiePosition - Vec2(0, 20);
	}

	bulletInit("CabbageBullet", "Cabbage_Rotate");

	_bulletAnimation->setScale(0.8f);
	_bulletAnimation->setPosition(initPosition);
	_bulletAnimation->setAnchorPoint(Vec2(0, 0));

	auto jto = JumpTo::create(time, endPosition, height, 1);

	_bulletAnimation->runAction(Sequence::create(jto,
		CallFunc::create([this]()
			{
				if (_bulletAnimation->getOpacity()) /* ���û������˵��û�л��н�ʬ */
				{
					playSoundEffect(SoundEffectType::kernelpult);
				}
				_bulletAnimation->setAnimation(0, "Cabbage_Crush", false);
			}), DelayTime::create(1.4f),
				CallFunc::create([this]()
					{
						_bulletAnimation->runAction(Sequence::create(FadeOut::create(0.2f),
							CallFunc::create([this]()
								{
									_bulletAnimation->setVisible(false);
								}), nullptr));
					}), nullptr));

	createShadow();
}

void Cabbage::bulletAndZombiesCollision()
{
	for (auto zombie : ZombiesGroup)
	{
		if (!_isUsed && zombie->getZombieIsSurvive() &&                                           /* ���Ĳ�û��ʹ�� && ��ʬû������ */
			getBulletIsSameLineWithZombie(zombie) && getBulletIsEncounterWithZombie(zombie))      /* �ӵ��뽩ʬͬһ�� && �ӵ��뽩ʬ��ײ */
		{
			selectSoundEffect(zombie->getZombieHeadAttackSoundEffect());
			_bulletAnimation->setOpacity(0);
			bulletAttackHurtZombies(zombie);   /* ��ʬ��������ֵ */

			zombie->setZombieHurtBlink();

			createCabbageExplode();

			_isUsed = true;
			break; /* һ��ֻ�ܻ���һ����ʬ */
		}
	}
}

Vec2 Cabbage::calculateZombiePosition()
{
	auto data = OpenLevelData::getInstance()->readLevelData(OpenLevelData::getInstance()->getLevelNumber());
	if (data->getZombiesIsSmall())
	{
		if (fabs(_position.x + 70 - _zombiePosition.x) >= 662)
			return _zombiePosition + Vec2(0, 20);
		else if (fabs(_position.x + 70 - _zombiePosition.x) >= 300)
			return _zombiePosition + Vec2(-_zombieSpeed / 2.f, 20);
		else
			return _zombiePosition + Vec2(-_zombieSpeed, 20);
	}
	else
	{
		if (fabs(_position.x + 70 - _zombiePosition.x) >= 662)
			return _zombiePosition + Vec2(_zombieSpeed * 2, 20);
		else if (fabs(_position.x + 70 - _zombiePosition.x) >= 300)
			return _zombiePosition + Vec2(_zombieSpeed / 2.f, 20);
		else
			return _zombiePosition + Vec2(-_zombieSpeed, 20);
	}
}

void Cabbage::createShadow()
{
	/* ����Ӱ�� */
	auto shadow = Sprite::createWithSpriteFrameName("plantshadow.png");
	shadow->setPosition(_position + Vec2(70, 10));
	shadow->setLocalZOrder(getZOrder());
	shadow->setOpacity(200);
	_node->addChild(shadow);
	shadow->runAction(RepeatForever::create(Sequence::create(
		CallFunc::create([=]()
			{
				shadow->setPositionX(_bulletAnimation->getPositionX());
			}), DelayTime::create(0.01f),
		CallFunc::create([=]() 
			{
				if (!_bulletAnimation->getOpacity())
				{
					shadow->removeFromParent();
				}
			}), nullptr)));
	shadow->runAction(Sequence::create(DelayTime::create(1.0f),
		CallFunc::create([shadow]()
			{
				shadow->removeFromParent();
			}), nullptr));
}

void Cabbage::createCabbageExplode()
{
	auto cabbageExplode = SkeletonAnimation::createWithData(
		_global->userInformation->getAnimationData().find("CabbageBullet")->second);
	cabbageExplode->setAnimation(0, "Cabbage_Crush", false);
	cabbageExplode->setScale(0.6f);
	cabbageExplode->setLocalZOrder(_bulletAnimation->getLocalZOrder());
	cabbageExplode->setPosition(_bulletAnimation->getPosition());
	cabbageExplode->setAnchorPoint(Vec2(0, 0));
	cabbageExplode->runAction(Sequence::create(DelayTime::create(1.4f),
		CallFunc::create([cabbageExplode]()
			{
				cabbageExplode->runAction(Sequence::create(FadeOut::create(0.2f),
					CallFunc::create([cabbageExplode]()
						{
							cabbageExplode->removeFromParent();
						}), nullptr));
			}), nullptr));
	_node->addChild(cabbageExplode);
}

void Cabbage::setZombiePosition(const Vec2& position)
{
	_zombiePosition = position; /* ��¼��ʬλ��*/
}

void Cabbage::setZombieSpeed(const float speed)
{
	_zombieSpeed = speed;
}

void Cabbage::setZombieHeight(const float height)
{
	_zombieHeight = height;
}

void Cabbage::setIsFileData(const bool isFileData)
{
	_isFileData = isFileData;
}

float Cabbage::getZombieSpeed() const
{
	return _zombieSpeed;
}

void Cabbage::setCabbageCurrentPosition(const Vec2& position)
{
	_currentPosition = position;
}

Vec2 Cabbage::getCabbageInitialPosition()
{
	return _position;
}

bool Cabbage::getBulletIsEncounterWithZombie(Zombies* zombie)
{
	auto& rect = zombie->getZombieAnimation()->getBoundingBox();
	return _bulletAnimation->getBoundingBox().intersectsRect(
		Rect(rect.origin.x + 70, rect.origin.y - 70, rect.size.width, rect.size.height));
}

void Cabbage::bulletAttackHurtZombies(Zombies* zombie)
{
	if (zombie->getZombieCurrentHeadShieldVolume() < _attack) /* �����ǰͷ������Ѫ��С�ڹ����˺� */
	{
		if (zombie->getZombieCurrentHeadShieldVolume() + 
			zombie->getZombieCurrentBloodVolume() <= _attack) /* �����ʬ����Ѫ��С���˺�����ʬ������ */
		{
			/* ��ʬ���� */
			zombie->setZombieCurrentBloodVolume(0);
			zombie->setZombieCurrentHeadShieldVolume(0);
		}
		else
		{
			/* ���㽩ʬ����Ѫ�� */
			zombie->setZombieCurrentBloodVolume(
				zombie->getZombieCurrentHeadShieldVolume() + 
				zombie->getZombieCurrentBloodVolume() - _attack);
			zombie->setZombieCurrentHeadShieldVolume(0);
		}
	}
	else
	{
		/* ���㽩ʬ����ʣ��Ѫ�� */
		zombie->setZombieCurrentHeadShieldVolume(
			zombie->getZombieCurrentHeadShieldVolume() - _attack);
	}
}

void Cabbage::caveBulletInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator)
{
	object.AddMember("cabbageInitialPositionX", _position.x, allocator);
	object.AddMember("cabbageInitialPositionY", _position.y, allocator);
	object.AddMember("ZombiePositionX", calculateZombiePosition().x, allocator);
	object.AddMember("ZombiePositionY", calculateZombiePosition().y, allocator);
	object.AddMember("ZombieSpeed", _zombieSpeed, allocator);
}

void Cabbage::readBulletInformation(rapidjson::Document* levelDataDocument, char* key, int i)
{
	_isFileData = true;
	_zombiePosition.x = (*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["ZombiePositionX"].GetFloat();
	_zombiePosition.y = (*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["ZombiePositionY"].GetFloat();
	_zombieSpeed = (*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["ZombiePositionY"].GetFloat();
	_currentPosition = Vec2(
		(*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionX"].GetFloat(),
		(*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionY"].GetFloat());
	setBulletPosition(Vec2(
		(*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["cabbageInitialPositionX"].GetFloat(),
		(*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["cabbageInitialPositionY"].GetFloat()));
}

/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.18
 *Email: 2117610943@qq.com
 */

#include "LZCabbage.h"
#include "../LZPeaShooter.h"

#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"

Cabbage::Cabbage(Node* node) :
  _zombiePosition(Vec2::ZERO)
, _initPosition(Vec2::ZERO)
, _endPosition(Vec2::ZERO)
, _currentPosition(Vec2::ZERO)
, _actionTime(1.f)
, _acxtionHeight(300)
, _zombieSpeed(0)
, _zombieHeight(0)
, _isFileData(false)
{
	_node = node;
	_attack = 40;
	_bulletType = BulletType::Cabbage;
}

Cabbage::~Cabbage()
{
}

void Cabbage::createBullet()
{
	calculateInformationForReadFile();

	bulletInit("CabbageBullet", "Cabbage_Rotate");

	_bulletAnimation->setScale(0.8f);
	_bulletAnimation->setPosition(_initPosition);
	_bulletAnimation->setAnchorPoint(Vec2(0, 0));

	createShadow(1.f);

	createListener("Cabbage_Crush");
}

void Cabbage::createListener(const string& actionName, float scale)
{
	auto jto = JumpTo::create(_actionTime, _endPosition, _acxtionHeight, 1);

	_bulletAnimation->runAction(Sequence::create(jto,
		CallFunc::create([=]()
			{
				if (_bulletAnimation->getOpacity()) /* ���û������˵��û�л��н�ʬ */
				{
					playSoundEffect(_bulletType == BulletType::Cabbage ?
						SoundEffectType::kernelpult : SoundEffectType::watermelon);
				}
				_bulletAnimation->setScale(scale);
				_bulletAnimation->setAnimation(0, actionName, false);
			}), DelayTime::create(1.4f),
		CallFunc::create([this]()
				{
					_bulletAnimation->runAction(Sequence::create(FadeOut::create(0.2f),
						CallFunc::create([this]()
							{
								_bulletAnimation->setVisible(false);
							}), nullptr));
				}), nullptr));
}

void Cabbage::calculateInformationForReadFile()
{
	_endPosition = calculateZombiePosition();
	_initPosition = _position + Vec2(70, 150);

	if (_isFileData)
	{
		_isFileData = false;
		auto position = _position + Vec2(70, 150); //���Ĳ�������ʼλ��

		auto distance = _zombiePosition.x - position.x; // ��ʬ����Ĳ˳�ʼ����
		if (_currentPosition.x <= _zombiePosition.x - distance / 2) // ��ǰλ�����������ϰ벿��
			_acxtionHeight = 300 - (_currentPosition.y - position.y);
		else
			_acxtionHeight = 0;
		_actionTime = (distance - (_currentPosition.x - _position.x)) / distance; //����ʣ���˶�ʱ��
		if (_actionTime < 0)_actionTime = 0;
		if (_actionTime > 1)_actionTime = 1;

		_initPosition = _currentPosition;
		_endPosition = _zombiePosition - Vec2(0, 20);
	}
}

void Cabbage::bulletAndZombiesCollision()
{
	for (auto zombie : ZombiesGroup)
	{
		if (!_isUsed && zombie->getZombieIsSurvive() &&                                           /* ���Ĳ�û��ʹ�� && ��ʬû������ */
			getBulletIsSameLineWithZombie(zombie) && getBulletIsEncounterWithZombie(zombie))      /* �ӵ��뽩ʬͬһ�� && �ӵ��뽩ʬ��ײ */
		{
			selectSoundEffect(zombie->getZombieHeadAttackSoundEffect());
			bulletAttackHurtZombies(zombie);   /* ��ʬ��������ֵ */

			_bulletAnimation->setOpacity(0);
			zombie->setZombieHurtBlink();

			createExplodeAnimation("CabbageBullet", "Cabbage_Crush");

			setBulletIsUsed(true);
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

void Cabbage::createShadow(float scale)
{
	/* ����Ӱ�� */
	auto bulletShadow = Sprite::createWithSpriteFrameName("plantshadow.png");
	bulletShadow->setPosition(Vec2(_initPosition.x, _position.y));
	bulletShadow->setLocalZOrder(getZOrder());
	bulletShadow->setOpacity(180);
	bulletShadow->setName("shadow");
	bulletShadow->setScale(scale);
	_node->addChild(bulletShadow);
	bulletShadow->runAction(RepeatForever::create(Sequence::create(
		CallFunc::create([=]()
			{
				bulletShadow->setPositionX(_bulletAnimation->getPositionX());
			}), DelayTime::create(0.02f),nullptr)));

	bulletShadow->runAction(Sequence::create(DelayTime::create(_actionTime), 
		CallFunc::create([=]() 
			{
				bulletShadow->removeFromParent();
			}), nullptr));
}

void Cabbage::createExplodeAnimation(const string& animationName, const string& actionName,const float scale)
{
	auto cabbageExplode = SkeletonAnimation::createWithData(
		_global->userInformation->getAnimationData().find(animationName)->second);
	cabbageExplode->setAnimation(0, actionName, false);
	cabbageExplode->setScale(scale);
	cabbageExplode->setLocalZOrder(_bulletAnimation->getLocalZOrder());
	cabbageExplode->setPosition(_bulletAnimation->getPosition());
	cabbageExplode->setAnchorPoint(Vec2(0, 0));
	cabbageExplode->update(0);
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

void Cabbage::setBulletCurrentPosition(const Vec2& position)
{
	_currentPosition = position;
}

Vec2 Cabbage::getBulletInitialPosition()
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
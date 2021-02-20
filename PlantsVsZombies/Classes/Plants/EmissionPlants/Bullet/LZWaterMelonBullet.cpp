/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.04
 *Email: 2117610943@qq.com
 */

#include "LZWaterMelonBullet.h"
#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"

WaterMelonBullet::WaterMelonBullet(Node* node)
{
	_node = node;
	_attack = 80;
	_bulletType = BulletType::WaterMelonBullet;
}

WaterMelonBullet::~WaterMelonBullet()
{
}

void WaterMelonBullet::createBullet()
{
	calculateInformationForReadFile();

	bulletInit("WaterMelon", "WaterMelon_Normal");

	_bulletAnimation->setScale(0.8f);
	_bulletAnimation->setPosition(_initPosition);

	createShadow(1.5f);

	createListener("WaterMelon_Explode");
}

void WaterMelonBullet::bulletAndZombiesCollision()
{
	for (auto zombie : ZombiesGroup)
	{
		if (!_isUsed && zombie->getZombieIsSurvive() &&                                           /* û��ʹ�� && ��ʬû������ */
			getBulletIsSameLineWithZombie(zombie) && getBulletIsEncounterWithZombie(zombie))      /* �ӵ��뽩ʬͬһ�� && �ӵ��뽩ʬ��ײ */
		{
			playSoundEffect(SoundEffectType::watermelon);

			bulletAttackHurtZombies(zombie);   /* ��ʬ��������ֵ */
			_bulletAnimation->setOpacity(0);
			
			createExplodeAnimation("WaterMelon","WaterMelon_Explode");

			setBulletIsUsed(true);
			break; /* һ��ֻ�ܻ���һ����ʬ */
		}
	}
}

void WaterMelonBullet::bulletAttackHurtZombies(Zombies* zombie)
{
	Cabbage::bulletAttackHurtZombies(zombie);
	zombie->setZombieHurtBlink();

	splashDamageZombies(zombie);  /* �Խ�ʬ��ɽ���*/
}

void WaterMelonBullet::splashDamageZombies(Zombies* exceptZombie)
{
	/* ���㽦���˺���ʬ�� */
	for (auto zombie : ZombiesGroup)
	{
		if (zombie->getZombieIsEnterMap() && zombie->getZombieIsSurvive() && getZombieInExplodeRange(zombie))
		{
			++_zombieInExplodeRangeNumbers;
		}
	}

	for (auto zombie : ZombiesGroup)
	{
		if (exceptZombie != zombie && zombie->getZombieIsEnterMap() &&
			zombie->getZombieIsSurvive() && getZombieInExplodeRange(zombie))
		{
			/* �����˺����� */
			if (int(_attack / 3) * _zombieInExplodeRangeNumbers > _attack * 7)
			{
				_attack = max(int(7 * pow(_attack, 2) / (int(_attack / 3) * 3 * _zombieInExplodeRangeNumbers)), 1);
			}
			else
			{
				_attack = int(_attack / 3);
			}

			Cabbage::bulletAttackHurtZombies(zombie);
			zombie->setZombieHurtBlink();
		}
	}

	_zombieInExplodeRangeNumbers = 0;
}

bool WaterMelonBullet::getZombieInExplodeRange(Zombies* zombie)
{
	return sqrt(pow(zombie->getZombieAnimation()->getPositionX() - _bulletAnimation->getPositionX(), 2) +
		pow((zombie->getZombieAnimation()->getPositionY() + 50) - (_bulletAnimation->getPositionY() - 40), 2)) <= 200 ? true : false;
}

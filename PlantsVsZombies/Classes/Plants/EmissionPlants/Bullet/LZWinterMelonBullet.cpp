/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.04
 *Email: 2117610943@qq.com
 */

#include "LZWinterMelonBullet.h"
#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Based/LZPlayMusic.h"

WinterMelonBullet::WinterMelonBullet(Node* node)
{
	_node = node;
	_attack = 80;
	_bulletType = BulletType::WinterMelonBullet;
}

WinterMelonBullet::~WinterMelonBullet()
{
}

void WinterMelonBullet::createBullet()
{
	calculateInformationForReadFile();

	bulletInit("WinterMelon", "WinterMelon_Normal");

	_bulletAnimation->setScale(0.8f);
	_bulletAnimation->setPosition(_initPosition);

	createShadow(1.5f);

	createListener("WinterMelon_Explode", 1.5f);
}

void WinterMelonBullet::bulletAndZombiesCollision()
{
	for (auto zombie : ZombiesGroup)
	{
		if (!_isUsed && zombie->getZombieIsSurvive() &&                                           /* û��ʹ�� && ��ʬû������ */
			getBulletIsSameLineWithZombie(zombie) && getBulletIsEncounterWithZombie(zombie))      /* �ӵ��뽩ʬͬһ�� && �ӵ��뽩ʬ��ײ */
		{
			playSoundEffect(SoundEffectType::watermelon);

			bulletAttackHurtZombies(zombie);   /* ��ʬ��������ֵ */
			_bulletAnimation->setOpacity(0);
			
			createExplodeAnimation("WinterMelon", "WinterMelon_Explode", 1.5f);

			setBulletIsUsed(true);
			break; /* һ��ֻ�ܻ���һ����ʬ */
		}
	}
}

void WinterMelonBullet::bulletAttackHurtZombies(Zombies* zombie)
{
	Cabbage::bulletAttackHurtZombies(zombie);
	zombie->setZombieHurtBlink();

	splashDamageZombies(zombie);  /* �Խ�ʬ��ɽ���*/
	winterMelonExplodeEffect(zombie);
}

void WinterMelonBullet::splashDamageZombies(Zombies* exceptZombie)
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
			winterMelonExplodeEffect(zombie);
		}
	}

	_zombieInExplodeRangeNumbers = 0;
}

void WinterMelonBullet::winterMelonExplodeEffect(Zombies* zombie)
{
	zombie->setZombieTimerTime(10, true);

	if (!zombie->getZombieIsFrozen())//�ж��Ƿ��Ѿ�������
	{
		PlayMusic::playMusic("frozen");
		zombie->setZombieActionSlow();
		zombie->createZombieTimer();
	}
}

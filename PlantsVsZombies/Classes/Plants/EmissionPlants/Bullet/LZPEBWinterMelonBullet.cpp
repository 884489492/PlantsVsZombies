/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.04
 *Email: 2117610943@qq.com
 */

#include "LZPEBWinterMelonBullet.h"
#include "Zombies/LZZZombies.h"
#include "Scenes/GameScene/LZSGData.h"

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
			selectSoundEffect(zombie->getZombieHeadAttackSoundEffect());

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
	winterMelonExplodeEffect(zombie);
	splashDamageZombies(zombie);  /* �Խ�ʬ��ɽ���*/
}

void WinterMelonBullet::winterMelonExplodeEffect(Zombies* zombie)
{
	zombie->setZombieTimerTime(10);

	if (!zombie->getZombieIsFrozen())//�ж��Ƿ��Ѿ�������
	{
		zombie->setZombieActionSlow();
		zombie->createZombieTimer();
	}
	else
	{
		zombie->setZombieHurtBlink();
	}
}

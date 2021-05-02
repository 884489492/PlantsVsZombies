/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.21
 *Email: 2117610943@qq.com
 */

#include "LZDoorZombies5.h"

DoorZombies5::DoorZombies5(Node* node)
{
	_node = node;
	_attackHeadSoundEffectType = 0;
	_attackBodySoundEffectType = 1;
	_bloodVolume = 200;
	_headShieldVolume = 0;
	_bodyShieldVolume = 2000;
	_currentBloodVolume = 200;
	_currentHeadShieldVolume = 0;
	_currentBodyShieldVolume = 2000;
	_speed = 40;
	_currentSpeed = 40;
	_isHaveShield = true;
	_bodyShieldType = ShieldType::StoneBodyShield;
	_zombiesType = ZombiesType::DoorZombies5;
}

DoorZombies5* DoorZombies5::create(Node* node)
{
	DoorZombies5* doorZombies2 = new (std::nothrow) DoorZombies5(node);
	if (doorZombies2 && doorZombies2->init())
	{
		doorZombies2->autorelease();
		return doorZombies2;
	}
	CC_SAFE_DELETE(doorZombies2);
	return nullptr;
}

void DoorZombies5::createZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 3 + 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAnimation(0, "Zombies_Door_Walk", true);
	_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
	_zombiesAnimation->setAttachment("Zombie_screendoor", "51");
}

void DoorZombies5::createPreviewZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAnimation(0, "Zombies_Door_Stand", true);
	_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
	_zombiesAnimation->setAttachment("Zombie_screendoor", "51");
}

void DoorZombies5::zombieInjuredEventUpdate()
{
	if (_isHaveShield)/* ��������廤�� */
	{
		if (_currentBodyShieldVolume <= _bodyShieldVolume * 2.0f / 3.0f) /* ���廤��һ������ */
		{
			setZombieBodyShieldPrimaryInjure("Zombie_screendoor", "52");
		}
		if (_currentBodyShieldVolume <= _bodyShieldVolume / 3.0f)        /* ���廤�ܶ������� */
		{
			setZombieBodyShieldSecondaryInjure("Zombie_screendoor", "53");
		}
		if (_currentBodyShieldVolume <= 0) /* ���廤����ʧ */
		{
			setZombieBodyShieldThirdInjure("Zombie_screendoor", "tt_innerleg_foot3");
		}
	}
	if (_currentBloodVolume <= _bloodVolume / 2.0f)
	{
		setZombiePrimaryInjure();
	}
	if (_currentBloodVolume <= 0)
	{
		setZombieSecondaryInjure();
	}
}
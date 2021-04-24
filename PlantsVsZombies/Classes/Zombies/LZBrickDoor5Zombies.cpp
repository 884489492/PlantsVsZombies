/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.23
 *Email: 2117610943@qq.com
 */

#include "LZBrickDoor5Zombies.h"
#include "Based/LZPlayMusic.h"

BrickDoor5Zombies::BrickDoor5Zombies(Node* node)
{
	_node = node;
	_attackHeadSoundEffectType = 0;
	_attackBodySoundEffectType = 0;
	_bloodVolume = 200;
	_headShieldVolume = 2000;
	_bodyShieldVolume = 2000;
	_currentBloodVolume = 200;
	_currentHeadShieldVolume = 2000;
	_currentBodyShieldVolume = 2000;
	_speed = 30;
	_currentSpeed = 30;
	_isHaveShield = true;
	_bodyShieldType = ShieldType::BrickHeadShield;
	_headShieldType = ShieldType::StoneBodyShield;
	_zombiesType = ZombiesType::BrickDoor5Zombies;
}

BrickDoor5Zombies* BrickDoor5Zombies::create(Node* node)
{
	BrickDoor5Zombies* brickDoor5Zombies = new (std::nothrow) BrickDoor5Zombies(node);
	if (brickDoor5Zombies && brickDoor5Zombies->init())
	{
		brickDoor5Zombies->autorelease();
		return brickDoor5Zombies;
	}
	CC_SAFE_DELETE(brickDoor5Zombies);
	return nullptr;
}

void BrickDoor5Zombies::createZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 3 + 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAnimation(0, "Zombies_Door_Walk", true);
	_zombiesAnimation->setAttachment("Zombie_bucket", "0");
	_zombiesAnimation->setAttachment("Zombie_screendoor", "51");
}

void BrickDoor5Zombies::createPreviewZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAnimation(0, "Zombies_Door_Stand", true);
	_zombiesAnimation->setAttachment("Zombie_bucket", "0");
	_zombiesAnimation->setAttachment("Zombie_screendoor", "51");
}

void BrickDoor5Zombies::zombieInjuredEventUpdate()
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

		if (_currentHeadShieldVolume <= _headShieldVolume * 2.0f / 3) /* ͷ������һ������ */
		{
			setZombieHeadShieldPrimaryInjure("Zombie_bucket", "1");
		}
		if (_currentHeadShieldVolume <= _headShieldVolume * 1.0f / 3) /* ͷ�����ܶ������� */
		{
			setZombieHeadShieldSecondaryInjure("Zombie_bucket", "2");
		}
		if (_currentHeadShieldVolume <= 0)   /* ͷ��������ʧ */
		{
			setZombieHeadShieldThirdInjure("Zombie_bucket", "tt_innerleg_foot3");
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

void BrickDoor5Zombies::setZombieBodyShieldThirdInjure(const string& oldName, const string& newName)
{
	if (_bodyShieldAnimationId == 3) /* ���������˺� */
	{
		_zombiesAnimation->setAttachment("Zombie_screendoor", "tt_innerleg_foot3");
		_bodyShieldAnimationId = 4;

		_attackBodySoundEffectType = 0;
		_bodyShieldType = ShieldType::none;
	}
}

void BrickDoor5Zombies::setZombieHeadShieldThirdInjure(const string& oldName, const string& newName)
{
	if (_headShieldAnimationId == 3) /* ͷ�����������˺� */
	{
		_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
		_headShieldAnimationId = 4;
		_isHaveShield = false;
		_attackHeadSoundEffectType = 0;
		_headShieldType = ShieldType::none;

		/* ��ʬ��ͷ������ */
		zombieLoseShieldAnimation("Zombie_Brick", 1.f);
	}
}

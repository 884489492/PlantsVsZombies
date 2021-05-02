/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.26
 *Email: 2117610943@qq.com
 */

#include "LZBonesZombies2.h"

BonesZombies2::BonesZombies2(Node* node)
{
	_node = node;
	_attackHeadSoundEffectType = 0;
	_attackBodySoundEffectType = 0;
	_bloodVolume = 200;
	_headShieldVolume = 1400;
	_bodyShieldVolume = 0;
	_currentBloodVolume = 200;
	_currentHeadShieldVolume = 1400;
	_currentBodyShieldVolume = 0;
	_speed = 40;
	_currentSpeed = 40;
	_isHaveShield = true;
	_headShieldType = ShieldType::BonesHeadShield;
	_zombiesType = ZombiesType::BonesZombies2;
}

BonesZombies2* BonesZombies2::create(Node* node)
{
	BonesZombies2* bonesZombies2 = new (std::nothrow) BonesZombies2(node);
	if (bonesZombies2 && bonesZombies2->init())
	{
		bonesZombies2->autorelease();
		return bonesZombies2;
	}
	CC_SAFE_DELETE(bonesZombies2);
	return nullptr;
}

void BonesZombies2::createZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 3 + 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAttachment("Zombie_bucket", "42");
	_zombiesAnimation->setAttachment("tt_head", "58");
	_zombiesAnimation->setAttachment("tt_jaw", "57");
}

void BonesZombies2::createPreviewZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAttachment("Zombie_bucket", "42");
	_zombiesAnimation->setAttachment("tt_head", "58");
	_zombiesAnimation->setAttachment("tt_jaw", "57");
}

void BonesZombies2::zombieInjuredEventUpdate()
{
	if (_isHaveShield)/* ���ͷ���л��� */
	{
		if (_currentHeadShieldVolume <= _headShieldVolume * 2.0f / 3.0f) /* ͷ������һ������ */
		{
			setZombieHeadShieldPrimaryInjure("Zombie_bucket", "43");
		}
		if (_currentHeadShieldVolume <= _headShieldVolume / 3.0f)       /* ͷ�����ܶ������� */
		{
			setZombieHeadShieldSecondaryInjure("Zombie_bucket", "44");
		}
		if (_currentHeadShieldVolume <= 0)                              /* ͷ��������ʧ */
		{
			setZombieHeadShieldThirdInjure("Zombie_bucket", "tt_innerleg_foot3");
		}
	}
	if (_currentBloodVolume <= _bloodVolume / 2.0f)   /* ��ʬѪ��С��һ��ֵ����ʬ���첲 */
	{
		setZombiePrimaryInjure();
	}
	if (_currentBloodVolume <= 0)                     /* ��ʬѪ��С���㣬��ʬ���� */
	{
		setZombieSecondaryInjure();
	}
}

void BonesZombies2::setZombieHeadShieldThirdInjure(const string& oldName, const string& newName)
{
	if (_headShieldAnimationId == 3) /* ���������˺� */
	{
		_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
		_headShieldAnimationId = 4;
		_isHaveShield = false;
		_attackHeadSoundEffectType = 0;
		_headShieldType = ShieldType::none;

		/* ��ʬ������ */
		zombieLoseShieldAnimation("Zombie_Spine2", 1.f);
	}
}
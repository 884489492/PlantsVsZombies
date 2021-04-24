/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.21
 *Email: 2117610943@qq.com
 */

#include "LZWoodZombies3.h"

WoodZombies3::WoodZombies3(Node* node)
{
	_node = node;
	_attackHeadSoundEffectType = 0;
	_attackBodySoundEffectType = 0;
	_bloodVolume = 200;
	_headShieldVolume = 800;
	_bodyShieldVolume = 0;
	_currentBloodVolume = 200;
	_currentHeadShieldVolume = 800;
	_currentBodyShieldVolume = 0;
	_speed = 30;
	_currentSpeed = 30;
	_isHaveShield = true;
	_headShieldType = ShieldType::WoodHeadShiled;
	_zombiesType = ZombiesType::WoodZombies3;
}

WoodZombies3* WoodZombies3::create(Node* node)
{
	WoodZombies3* woodZombies3 = new (std::nothrow) WoodZombies3(node);
	if (woodZombies3 && woodZombies3->init())
	{
		woodZombies3->autorelease();
		return woodZombies3;
	}
	CC_SAFE_DELETE(woodZombies3);
	return nullptr;
}

void WoodZombies3::createZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 3 + 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAttachment("Zombie_bucket", "39");
	_zombiesAnimation->setAttachment("tt_head", "60");
}

void WoodZombies3::createPreviewZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAttachment("Zombie_bucket", "39");
	_zombiesAnimation->setAttachment("tt_head", "60");
}

void WoodZombies3::zombieInjuredEventUpdate()
{
	if (_isHaveShield)/* ���ͷ���л��� */
	{
		if (_currentHeadShieldVolume <= _headShieldVolume * 2.0f / 3.0f) /* ͷ������һ������ */
		{
			setZombieHeadShieldPrimaryInjure("Zombie_bucket", "40");
		}
		if (_currentHeadShieldVolume <= _headShieldVolume / 3.0f)       /* ͷ�����ܶ������� */
		{
			setZombieHeadShieldSecondaryInjure("Zombie_bucket", "41");
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

void WoodZombies3::setZombieHeadShieldThirdInjure(const string& oldName, const string& newName)
{
	if (_headShieldAnimationId == 3) /* ���������˺� */
	{
		_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
		_headShieldAnimationId = 4;
		_isHaveShield = false;
		_attackHeadSoundEffectType = 0;
		_headShieldType = ShieldType::none;

		/* ��ʬ������ */
		zombieLoseShieldAnimation("Zombie_Wood3", 1.f);
	}
}

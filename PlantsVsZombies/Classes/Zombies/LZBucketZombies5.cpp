/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.21
 *Email: 2117610943@qq.com
 */

#include "LZBucketZombies5.h"

BucketZombies5::BucketZombies5(Node* node)
{
	_node = node;
	_attackHeadSoundEffectType = 1;
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
	_headShieldType = ShieldType::IronHeadShield;
	_zombiesType = ZombiesType::BucketZombies5;
}

BucketZombies5* BucketZombies5::create(Node* node)
{
	BucketZombies5* bucketZombies5 = new (std::nothrow) BucketZombies5(node);
	if (bucketZombies5 && bucketZombies5->init())
	{
		bucketZombies5->autorelease();
		return bucketZombies5;
	}
	CC_SAFE_DELETE(bucketZombies5);
	return nullptr;
}

void BucketZombies5::createZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 3 + 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAttachment("Zombie_bucket", "18");
}

void BucketZombies5::createPreviewZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAttachment("Zombie_bucket", "18");
}

void BucketZombies5::zombieInjuredEventUpdate()
{
	if (_isHaveShield)/* �����ͷ������ */
	{
		if (_currentHeadShieldVolume <= _headShieldVolume * 2.0f / 3.0f) /* ͷ������һ������ */
		{
			setZombieHeadShieldPrimaryInjure("Zombie_bucket", "19");
		}
		if (_currentHeadShieldVolume <= _headShieldVolume / 3.0f)       /* ͷ�����ܶ������� */
		{
			setZombieHeadShieldSecondaryInjure("Zombie_bucket", "20");
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

void BucketZombies5::setZombieHeadShieldThirdInjure(const string& oldName, const string& newName)
{
	if (_headShieldAnimationId == 3) /* ���������˺� */
	{
		_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
		_headShieldAnimationId = 4;
		_isHaveShield = false;
		_attackHeadSoundEffectType = 0;
		_headShieldType = ShieldType::none;

		/* ��ʬ������ */
		zombieLoseShieldAnimation("Zombie_bucket5", 1.f);
	}
}
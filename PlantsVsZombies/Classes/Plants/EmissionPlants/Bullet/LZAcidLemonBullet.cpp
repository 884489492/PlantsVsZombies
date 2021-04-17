/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.25
 *Email: 2117610943@qq.com
 */

#include "LZAcidLemonBullet.h"

#include "../LZPeaShooter.h"
#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"

AcidLemonBullet::AcidLemonBullet(Node* node)
{
    _node = node;
    _attack = 15;
    _bulletType = BulletType::AcidLemonBullet;
}

AcidLemonBullet::~AcidLemonBullet()
{
}

void AcidLemonBullet::createBullet()
{
    bulletInit("LemonJuice", rand() % 2 ? "shoot" : "shoot1");

	_bulletAnimation->setPosition(_position + Vec2(70, 90));
	_bulletAnimation->setScale(0.9f);
	_bulletAnimation->runAction(Sequence::create(MoveBy::create(8.0f, Vec2(8000, rand() % 31 - 15)),
		CallFunc::create([this]()
			{
				_bulletAnimation->setVisible(false);
			}), nullptr));

    createShadow();
}

void AcidLemonBullet::bulletAndZombiesCollision()
{
	for (auto zombie : ZombiesGroup)
	{
		if (!_isUsed && getBulletIsSameLineWithZombie(zombie) &&                         /* û�б�ʹ�� && �������뽩ʬ��ͬһ�� */
			zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() &&             /* ��ʬû������ && ��ʬ�����ͼ�� */
			getBulletIsEncounterWithZombie(zombie) && zombie->getZombieIsCanBeAttack())  /* �������뽩ʬ��ײ && ��ʬ���Ա�������*/
		{
			selectSoundEffect(zombie->getZombieBodyAttackSoundEffect(), zombie->getZombieHeadAttackSoundEffect());  /* ����ָ������ */

			setBulletOpacity();                /* �ӵ���ʧ */
			setAttackForShield(zombie);        /* �Խ�ʬ���ʻ�����ɶ����˺� */
			bulletAttackHurtZombies(zombie);   /* ��ʬ��������ֵ */

			zombie->setZombieHurtBlink();

			createAcidLemonBulletExplode();   /* ������ը���� */
			setBulletAttack(0);
			setBulletIsUsed(true);

			break; /* һ���㶹ֻ�ܻ���һ����ʬ */
		}
	}
}

void AcidLemonBullet::createShadow()
{
	/* ����Ӱ�� */
	auto shadow = Sprite::createWithSpriteFrameName("plantshadow.png");
	shadow->setScaleX(1.7f);
	shadow->setName("shadow");
	shadow->setPosition(Vec2(0, -92));
	shadow->setOpacity(200);
	shadow->setLocalZOrder(_bulletAnimation->getLocalZOrder());
	_bulletAnimation->addChild(shadow, -1);
}

void AcidLemonBullet::createAcidLemonBulletExplode()
{
	auto explode = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("PeaExplode")->second);
	explode->setPosition(getBulletPosition());
	explode->setAnimation(0, "LemonBullet_Explode", false);
	explode->setScale(0.8f);
	explode->update(0);
	explode->setLocalZOrder(_bulletAnimation->getLocalZOrder());
	_node->addChild(explode);

	explode->runAction(Sequence::create(DelayTime::create(0.8f), CallFunc::create([explode]()
		{
			explode->removeFromParent();
		}), nullptr));
}

void AcidLemonBullet::setAttackForShield(Zombies* zombie)
{
	if (zombie->getZombieIsHaveShield()) /* ��ʬ�л��� */
	{
		if (zombie->getZombieHeadShieldType() == ShieldType::IronHeadShield ||
			zombie->getZombieBodyShieldType() == ShieldType::IronBodyShield)     /* ��ʬ�����ʻ��� */
		{
			_attack = 80;
		}

		if (zombie->getZombieHeadShieldType() == ShieldType::PlasticsHeadShield) /* ��ʬ�����ϻ��� */
		{
			_attack = 15;
		}
	}
	else
	{
		_attack = 20;
	}
}

void AcidLemonBullet::readBulletAnimationInformation(rapidjson::Document* levelDataDocument, char* key, int i)
{
	_bulletAnimation->setPosition(Vec2(
		(*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionX"].GetFloat(),
		(*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionY"].GetFloat()));
}

/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.19
 *Email: 2117610943@qq.com
 */

#include "LZPEBFirePea.h"
#include "LZPEBIcePea.h"
#include "Zombies/LZZZombies.h"
#include "Scenes/GameScene/LZSGData.h"

FirePea::FirePea(Node* node)
{
    _node = node;
    _attack = 40;
    _bulletType = BulletType::FirePea;

    _isFire = true;
    _fireNumbers = 1;
    _peaAnimationName = "fire";
}

FirePea::~FirePea()
{
}

void FirePea::createBullet()
{
    bulletInit("FirePea", _peaAnimationName);

    _bulletAnimation->setPosition(_position + Vec2(70, 85));
    _bulletAnimation->setScale(1.5f);
    _bulletAnimation->runAction(Sequence::create(MoveBy::create(3.0f, Vec2(2000, rand() % 31 - 15)),
        CallFunc::create([=]()
            {
                _bulletAnimation->setVisible(false);
            }), nullptr));

    createShadow();
}

void FirePea::createShadow()
{
    /* ����Ӱ�� */
    auto shadow = Sprite::createWithSpriteFrameName("plantshadow.png");
    shadow->setScaleY(0.7f);
    shadow->setName("shadow");
    shadow->setOpacity(200);
    shadow->setPosition(Vec2(0, -52));
    shadow->setLocalZOrder(_bulletAnimation->getLocalZOrder());
    _bulletAnimation->addChild(shadow, -1);
}

void FirePea::bulletAndZombiesCollision()
{
    for (auto zombie : ZombiesGroup)
    {
        if (!_isUsed && getBulletIsSameLineWithZombie(zombie) &&             /* �㶹û�б�ʹ�� && �㶹�뽩ʬ��ͬһ�� */
            zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() && /* ��ʬû������ && ��ʬ�����ͼ�� */
            getBulletIsEncounterWithZombie(zombie))                          /* �㶹�뽩ʬ��ײ */
        {
            playSoundEffect(SoundEffectType::firepea); /* ����ָ������ */

            attackZombies(zombie);                     /* �����˺��˽�ʬ */
            splashDamageZombies(zombie);               /* �����˺���ʬ*/
            setBulletOpacity();                        /* �ӵ���ʧ */
            createPeaExplode();                        /* �����㶹��ը���� */
            setBulletAttack(0);
            setBulletIsUsed(true);

            break;
        }
    }
}

void FirePea::attackZombies(Zombies* zombie)
{
    bulletAttackHurtZombies(zombie);
    recoveryFrozenZombie(zombie);
    zombie->setZombieHurtBlink();
}

void FirePea::recoveryFrozenZombie(Zombies* zombie)
{
    if (zombie->getZombieTimerTime() != -1)
    {
        zombie->setZombieTimerTime(-1); /* ��ʬ���������ּ���ʱ������ */
        IcePea::setZombieActionRecovery(zombie);
    }
}

void FirePea::readBulletInformation(rapidjson::Document* levelDataDocument, char* key, int i)
{
    _isFire = (*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["IsFire"].GetBool();
    _fireNumbers = (*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PeaFireNumbers"].GetInt();
}

void FirePea::readBulletAnimationInformation(rapidjson::Document* levelDataDocument, char* key, int i)
{
    _bulletAnimation->setPosition(Vec2(
        (*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionX"].GetFloat(),
        (*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionY"].GetFloat()));
    Bullet::setBulletOpacity((*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["Opacity"].GetInt());
}
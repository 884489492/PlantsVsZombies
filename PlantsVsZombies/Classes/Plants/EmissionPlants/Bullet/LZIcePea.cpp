/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.1.17
 *Email: 2117610943@qq.com
 */

#include "LZIcePea.h"

#include "Based/LZPlayMusic.h"
#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"

IcePea::IcePea(Node* node)
{
    _node = node;
    _attack = 20;
    _bulletType = BulletType::IcePea;

    _peaAnimationName = "ice";

    srand(time(nullptr));
}

IcePea::~IcePea()
{
}

void IcePea::createShadow()
{ 
    /* ����Ӱ�� */
    auto shadow = Sprite::createWithSpriteFrameName("plantshadow.png");
    shadow->setName("plantshadow");
    shadow->setPosition(Vec2(0, -80));
    shadow->setOpacity(200);
    shadow->setLocalZOrder(_bulletAnimation->getLocalZOrder());
    _bulletAnimation->addChild(shadow, -1);
}

void IcePea::bulletAndZombiesCollision()
{
    for (auto zombie : ZombiesGroup)
    {
        if (!_isUsed && getBulletIsSameLineWithZombie(zombie) &&             /* �㶹û�б�ʹ�� && �㶹�뽩ʬ��ͬһ�� */
            zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() && /* ��ʬû������ && ��ʬ�����ͼ�� */
            getBulletIsEncounterWithZombie(zombie))                          /* �㶹�뽩ʬ��ײ */
        {
            selectSoundEffect(zombie->getZombieBodyAttackSoundEffect(),
                zombie->getZombieHeadAttackSoundEffect());  /* ����ָ������ */

            setBulletOpacity();                /* �ӵ���ʧ */
            attackZombies(zombie);             /* ������ʬ */
            setBulletAttack(0);
            setBulletIsUsed(true);

            break; /* һ���㶹ֻ�ܻ���һ����ʬ */
        }
    }
}

void IcePea::attackZombies(Zombies* zombie)
{
    icePeaExplodeEffect(zombie);       /* �����㶹ʹ��ʬ���� */
    bulletAttackHurtZombies(zombie);   /* ��ʬ��������ֵ */
    createPeaExplode();
}

void IcePea::createPeaExplode()
{
    static string animation[] = { {"IcePea_Explode_1"},{"IcePea_Explode_2"} };

    auto peaExplode = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("PeaExplode")->second);
    peaExplode->setPosition(getBulletPosition() + Vec2(20, 5));
    peaExplode->setAnimation(0, animation[rand() % 2], false);
    peaExplode->update(0);
    peaExplode->setScale(1.7f);
    peaExplode->setScaleX(-peaExplode->getScaleX());
    peaExplode->setLocalZOrder(_bulletAnimation->getLocalZOrder());
    _node->addChild(peaExplode);

    peaExplode->runAction(Sequence::create(DelayTime::create(0.8f), CallFunc::create([peaExplode]()
        {
            peaExplode->removeFromParent();
        }), nullptr));

    if (!(rand() % 3)) createExplodeEffect();
}

void IcePea::icePeaExplodeEffect(Zombies* zombie)
{
    zombie->setZombieTimerTime(10, true);
    
    if (!zombie->getZombieIsFrozen())//�ж��Ƿ��Ѿ�������
    {
        PlayMusic::playMusic("frozen");
        zombie->setZombieActionSlow();
        zombie->createZombieTimer();
    }
    else
    {
        zombie->setZombieHurtBlink();
    }
}

void IcePea::readBulletAnimationInformation(rapidjson::Document* levelDataDocument, char* key, int i)
{
    _bulletAnimation->setPosition(Vec2(
        (*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionX"].GetFloat(),
        (*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionY"].GetFloat()));
}

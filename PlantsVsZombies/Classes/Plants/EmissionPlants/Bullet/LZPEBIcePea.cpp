/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.1.17
 *Email: 2117610943@qq.com
 */

#include "LZPEBIcePea.h"

#include "Based/LZBPlayMusic.h"
#include "Zombies/LZZZombies.h"
#include "Scenes/GameScene/LZSGData.h"

IcePea::IcePea(Node* node)
{
    _node = node;
    _attack = 20;
    _bulletType = BulletType::IcePea;

    _isIce = true;
    _fireNumbers = -1;
    _peaAnimationName = "ice";
}

IcePea::~IcePea()
{
}

void IcePea::createShadow()
{ 
    /* ����Ӱ�� */
    auto shadow = Sprite::createWithSpriteFrameName("plantshadow.png");
    shadow->setName("shadow");
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

            judgeAttackMethod(zombie);         /* �жϹ�����ʽ*/
            setBulletOpacity();                /* �ӵ���ʧ */
            createPeaExplode(zombie);          /* ���������㶹��ը���� */
            setBulletAttack(0);
            setBulletIsUsed(true);

            break; /* һ���㶹ֻ�ܻ���һ����ʬ */
        }
    }
}

void IcePea::createPeaExplode()
{
    static string animation[] = { {"IcePea_Explode_1"},{"IcePea_Explode_2"} };

    auto peaExplode = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("PeaExplode")->second);
    peaExplode->setPosition(getBulletPosition() - Vec2(5, -5));
    peaExplode->setAnimation(0, animation[rand() % 2], false);
    peaExplode->update(0);
    peaExplode->setScale(1.7f);
    peaExplode->setLocalZOrder(_bulletAnimation->getLocalZOrder());
    _node->addChild(peaExplode);

    peaExplode->runAction(Sequence::create(DelayTime::create(0.8f), CallFunc::create([peaExplode]()
        {
            peaExplode->removeFromParent();
        }), nullptr));
}

void IcePea::createPeaExplode(Zombies* zombie)
{
    if (_isIce) /* ����Ǳ����㶹 */
    {
        icePeaExplodeEffect(zombie);       /* �����㶹ʹ��ʬ���� */
        createPeaExplode();
    }
    else
    {
        zombie->setZombieHurtBlink();
        Pea::createPeaExplode();
    }

}

void IcePea::icePeaExplodeEffect(Zombies* zombie)
{
    zombie->setZombieTimerTime(10);
    
    if (!zombie->getZombieIsFrozen())//�ж��Ƿ��Ѿ�������
    {
        PlayMusic::playMusic("frozen");
        setZombieActionSlow(zombie);

        if (!zombie->getZombieIsCreateTimer())
        {
            zombie->setZombieIsCreateTimer(true);
            zombie->getZombieAnimation()->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1.f),
                CallFunc::create([=]()
                    {
                        if (zombie->getZombieTimerTime() > 0)--zombie->getZombieTimerTime();
                        else if (zombie->getZombieTimerTime() == 0)
                        {
                            --zombie->getZombieTimerTime();
                            setZombieActionRecovery(zombie);
                        }
                    }), nullptr)));
        }
    }
    else
    {
        zombie->setZombieHurtBlink();
    }
}

void IcePea::setZombieActionSlow(Zombies* zombie)
{
    zombie->setZombieIsFrozen(true);
    zombie->getZombieAnimation()->setColor(Color3B(0, 162, 232));
    zombie->getZombieAnimation()->setTimeScale(zombie->getZombieAnimation()->getTimeScale() / 2.0f);   /* �˶��ٶȼ��� */
    zombie->setZombieCurrentSpeed(zombie->getZombieCurrentSpeed() / 2.0f);                             /* �ƶ��ٶȼ��� */
}

void IcePea::setZombieActionRecovery(Zombies* zombie)
{
    zombie->setZombieIsFrozen(false);
    zombie->getZombieAnimation()->setColor(Color3B::WHITE);
    zombie->getZombieAnimation()->setTimeScale(zombie->getZombieAnimation()->getTimeScale() * 2.0f);   /* �˶��ٶȻָ� */
    zombie->setZombieCurrentSpeed(zombie->getZombieCurrentSpeed() * 2.0f);                             /* �ƶ��ٶȻָ� */
}

void IcePea::readBulletInformation(rapidjson::Document* levelDataDocument, char* key, int i)
{
    _isIce = (*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["IsIce"].GetBool();
    _isFire = (*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["IsFire"].GetBool();
    _fireNumbers = (*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PeaFireNumbers"].GetInt();
}

void IcePea::readBulletAnimationInformation(rapidjson::Document* levelDataDocument, char* key, int i)
{
    _bulletAnimation->setPosition(Vec2(
        (*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionX"].GetFloat(),
        (*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionY"].GetFloat()));
    Bullet::setBulletOpacity((*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["Opacity"].GetInt());

    if (_fireNumbers == 0)
    {
        _bulletAnimation->setAnimation(0, "normal", true);
    }
    if (_fireNumbers == 1)
    {
        _bulletAnimation->setAnimation(0, "fire", true);
        _bulletAnimation->getChildByName("shadow")->setPosition(Vec2(0, -52));
        _bulletAnimation->getChildByName("shadow")->setScaleY(0.7f);
        _bulletAnimation->getChildByName("shadow")->setScaleX(1.0f);
        _bulletAnimation->setScale(1.5f);
    }
}

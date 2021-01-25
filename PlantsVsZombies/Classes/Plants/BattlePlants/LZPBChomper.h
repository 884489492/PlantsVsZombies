/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.1.16
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZPBBattlePlants.h"

class Chomper :public BattlePlants
{
public:
    static Chomper* create(Node* node = nullptr);
    Sprite* createPlantImage() override;
    void createPlantAnimation() override;
    void determineRelativePositionPlantsAndZombies() override;

    int getChewTime() const;
    bool getIsCanEatZombie() const;
    void setChewTime(const int time);
    void setIsCanEatZombie(const bool isCanEat);

CC_CONSTRUCTOR_ACCESS:
    Chomper(Node* node = nullptr);
    ~Chomper();

protected:
    virtual void plantAttack(Zombies* zombie);
    virtual bool getZombieIsEncounterPlant(Zombies* zombie) override;
    virtual SkeletonAnimation* showPlantAnimationAndText() override;
    virtual void createListener();

private:
    void cavePlantInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator) override;
    void readPlantInforamtion(rapidjson::Document* levelDataDocument, char* key, int i) override;
    bool isCanKillZombieOnce(Zombies* zombie);
    void chomperKillZombie(Zombies* zombie);
    void chomperHurtZombie(Zombies* zombie);
    void zombieRelieveReserveKill(Zombies* zombie);

private:
    bool _isCanEat;      // �Ƿ���ԳԽ�ʬ
    int _chewTime;       // ���컨�׽�ʱ��
};

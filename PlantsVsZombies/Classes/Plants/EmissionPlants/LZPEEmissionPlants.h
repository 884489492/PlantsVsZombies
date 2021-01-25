/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#pragma once
#include "../LZPPlants.h"
#include "Scenes/SelectPlantsScene/LZSSSpriteLayer.h"

class Zombies;
class Bullet;

class EmissionPlants :public Plants
{
CC_CONSTRUCTOR_ACCESS:
    EmissionPlants();
    ~EmissionPlants();

protected:
    virtual void plantAttack(Zombies* zombie) = 0;
	virtual void cavePlantInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator);
	virtual void readPlantInforamtion(rapidjson::Document* levelDataDocument, char* key, int i);

protected:
    bool _isChanged;           // �Ƿ�ı�Ϊ������̬
    Bullet* _bulletAnimation;  // �ӵ�����
};
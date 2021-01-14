/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.11
 *Email: 2117610943@qq.com
 */

#include "LZSGData.h"
#include "LZSGBackgroundLayer.h"
#include "LZSGInformationLayer.h"
#include "LZSGButtonLayer.h"
#include "LZSGControlLayer.h"
#include "LZSGAnimationLayer.h"

#include "Zombies/LZZZombies.h"

GSBackgroundLayer* backgroundLayerInformation;   // ������ʵ����
GSInformationLayer* informationLayerInformation; // ��Ϣ��ʵ����
GSButtonLayer* buttonLayerInformation;           // ��ť��ʵ����
GSControlLayer* controlLayerInformation;         // ���Ʋ�ʵ����
GSAnimationLayer* animationLayerInformation;     // ������ʾ����

multimap<int, Plants*> PlantsGroup;
list<Zombies*> ZombiesGroup;
list<Bullet*> BulletGroup;
list<Sun*> SunsGroup;
list<Car*> CarsGroup;
list<Coin*> CoinsGroup;

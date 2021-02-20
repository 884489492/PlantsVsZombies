/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.11
 *Email: 2117610943@qq.com
 */

#include "LZAGSData.h"
#include "LZAGSBackgroundLayer.h"
#include "LZAGSInformationLayer.h"
#include "LZAGSButtonLayer.h"
#include "LZAGSControlLayer.h"
#include "LZAGSAnimationLayer.h"

#include "Zombies/LZZombies.h"

GSBackgroundLayer* backgroundLayerInformation;   // ������ʵ����
GSInformationLayer* informationLayerInformation; // ��Ϣ��ʵ����
GSButtonLayer* buttonLayerInformation;           // ��ť��ʵ����
GSControlLayer* controlLayerInformation;         // ���Ʋ�ʵ����
GSAnimationLayer* animationLayerInformation;     // ������ʾ����
Layer* goodsLayerInformation;                    // ��Ʒ��ʵ����

multimap<int, Plants*> PlantsGroup;
list<Zombies*> ZombiesGroup;
list<Bullet*> BulletGroup;
list<Sun*> SunsGroup;
list<Car*> CarsGroup;
list<Coin*> CoinsGroup;
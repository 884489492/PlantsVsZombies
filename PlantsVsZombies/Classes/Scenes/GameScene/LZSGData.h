/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.11
 *Email: 2117610943@qq.com
 */

#pragma once
#include <map>
#include <list>
#include <vector>

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;

class GSBackgroundLayer;
class GSInformationLayer;
class GSButtonLayer;
class GSControlLayer;
class GSAnimationLayer;

class Plants;
class Zombies;
class Bullet;
class Sun;
class Coin;
class Car;

extern GSBackgroundLayer* backgroundLayerInformation;   // ������ʵ����
extern GSInformationLayer* informationLayerInformation; // ��Ϣ��ʵ����
extern GSButtonLayer* buttonLayerInformation;           // ��ť��ʵ����
extern GSControlLayer* controlLayerInformation;         // ���Ʋ�ʵ����
extern GSAnimationLayer* animationLayerInformation;     // ������ʾ����
extern Layer* goodsLayerInformation;                    // ��Ʒ��ʵ����

/**ʹ��multimap��ʹ���������ɾ��
 *�洢����ֲ��ļ���
 */
extern multimap<int, Plants*> PlantsGroup;

/**ʹ��list��������������ɾ��
 *�洢���н�ʬ�ļ���
 */
extern list<Zombies*> ZombiesGroup;

/**ʹ��list��������������ɾ��
 *�洢����ֲ�﷢�����ӵ��ļ���
 */
extern list<Bullet*> BulletGroup;

/**ʹ��list��������������ɾ��
 *�洢����ļ���
 */
extern list<Sun*> SunsGroup;

/**ʹ��list��������������ɾ��
 *�洢�����ļ���
 */
extern list<Car*> CarsGroup;

/**ʹ��list��������������ɾ��
 *�洢��ҵļ���
 */
extern list<Coin*> CoinsGroup;
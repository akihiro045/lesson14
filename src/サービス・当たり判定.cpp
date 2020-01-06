#include "サービス・当たり判定.h"
#include "DxLib.h"

namespace engine {

	void colision::Initialize()
	{
	}

	void colision::CleanUp()
	{
	}

	void colision::Reset()
	{
	}

	void colision::ChackHitCircle(entity* obj1, entity* obj2)
	{
		float2 pos[2];
		pos[0] = obj1->getPosition();
		pos[1] = obj2->getPosition();
		float distance = (pos[0].x - pos[1].x) * (pos[0].x - pos[1].x) + (pos[0].y - pos[1].y) * (pos[0].y - pos[1].y);
		float range = (obj1->GetRange() + obj2->GetRange()) * (obj1->GetRange() + obj2->GetRange());

		if (distance < range)
		{
			obj1->HitAction();
			obj2->HitAction();
		}
	}

	void colision::Update()
	{
	}



	colisionService::colisionService()
	{

	}

	colisionService::~colisionService()
	{

	}
}
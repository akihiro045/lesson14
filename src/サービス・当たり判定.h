#pragma once
#include <vector>
#include "����.h"
#include "�G���e�B�e�B.h"

namespace engine {
	class bulletService;
	class entity;


	class colision {
	//public:
	//	friend colisionService;
	private:
		int tmp;
	public:
		colision() {}
		 ~colision() {}

		void Initialize();
		void CleanUp();
		void Reset();

		void ChackHitCircle(entity* obj1, entity* obj2);

		void Update();
	};

	class colisionService {

	public:
		colisionService();
		~colisionService();

	};
}
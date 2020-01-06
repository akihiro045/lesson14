#pragma once
#include <vector>
#include "共通.h"

namespace engine {

	class bulletService;
	class renderingServices;

	struct bulletData{
		float2 position;
		float2 speed;
		bool death;
	};

	// 種類ごと確保
	class bullet {
	public:
		friend bulletService;
	private:
		unsigned int resourceID_ = 0;
		float2 halfSize_ = {0.0f, 0.0f};// 幅と高さの半分
		unsigned int maximum_ = 0;
		unsigned int bulletNum_ = 0;

		bulletData* dataArray_ = nullptr;

		static bool offScreen(float2 position, float2 サイズ, float2 screenSize);
		bool flag;
	public:
		bullet() {}
		~bullet() {}

		void Initialize(unsigned int maximum, unsigned int resourceID, renderingServices& renderingServices);
		void Cleanup();
		void Reset();// スタート時など弾を消す際の処理

		int Add(float2 position, float2 speed);

		void Update(float elapsedTime, renderingServices& renderingServices);
		void PostUpdateProcexxing();
		void Draw(renderingServices& renderingServices);

		//弾の生存フラグを取得
		bool GetFlag() { return flag; }
		void HItAction() { flag = false; }
	};


	class bulletService {
	public:// 定数宣言
		enum type {
			myBullet = 0,
			enemyBullet,

			maximum,
		};

	private:
		bullet bullet_[(int)(type::maximum)];
		renderingServices &renderingServices_;

	public:
		bulletService(renderingServices&);
		~bulletService();

		int Add(bulletService::type type, float2 position, float2 speed);
		void Update(float elapsedTime);
		void Draw();

	};

}// namespace engine
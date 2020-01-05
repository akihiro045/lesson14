#include <windows.h>
#include "DxLib.h"
#include "共通.h"
#include "サービス・レンダリング.h"

/// 設定情報 
#define ルートパス L""

namespace engine
{

	/// 静的データ
	TCHAR* resourceLoader::imgPass_[画像_枚数] = {
		ルートパス"img/mat04-01.png",
		ルートパス"img/mat04-02.png",
		ルートパス"img/mat04-03.png",
		ルートパス"img/mat04-04.png",
	};

	UVtable resourceLoader::UVtable_[RID_個数] = {// エラーチェックのため要素数を明示的に指定
		 ///  RID_IMG_MAT04_01
		 // RID_MY_SHIP: 自機
		{IMG_MAT04_01, 0 + 32 * 0, 0, 32,32},
		{IMG_MAT04_01, 0 + 32 * 1, 0, 32,32},
		{IMG_MAT04_01, 0 + 32 * 2, 0, 32,32},// まっすぐ
		{IMG_MAT04_01, 0 + 32 * 3, 0, 32,32},
		{IMG_MAT04_01, 0 + 32 * 4, 0, 32,32},
		// RID_INJECTION: 噴射
		{IMG_MAT04_01, 72,32, 16,16},
		{IMG_MAT04_01, 72,48, 16,16},
		{IMG_MAT04_01, 04,32, 16,16},
		{IMG_MAT04_01, 104,48, 16,16},
		// RID_SHOT: 自機の弾
		{IMG_MAT04_01,  0,48, 16,16},

		// RID_BULLET: 大きめの弾
		{IMG_MAT04_01, 32,32, 32,32},

		// RID_FORMING: 16方向弾(下)
		{IMG_MAT04_01, 192 + 16 * 0, 0,16,16},
		{IMG_MAT04_01, 192 + 16 * 1, 0,16,16},
		{IMG_MAT04_01, 192 + 16 * 2, 0,16,16},
		{IMG_MAT04_01, 192 + 16 * 3, 0,16,16},
		{IMG_MAT04_01, 192 + 16 * 4, 0,16,16},// 右
		{IMG_MAT04_01, 192 + 16 * 5, 0,16,16},
		{IMG_MAT04_01, 192 + 16 * 6, 0,16,16},
		{IMG_MAT04_01, 192 + 16 * 7, 0,16,16},
		{IMG_MAT04_01, 192 + 16 * 0,16,16,16},// 上
		{IMG_MAT04_01, 192 + 16 * 1,16,16,16},
		{IMG_MAT04_01, 192 + 16 * 2,16,16,16},
		{IMG_MAT04_01, 192 + 16 * 3,16,16,16},
		{IMG_MAT04_01, 192 + 16 * 4,16,16,16},// 左
		{IMG_MAT04_01, 192 + 16 * 5,16,16,16},
		{IMG_MAT04_01, 192 + 16 * 6,16,16,16},
		{IMG_MAT04_01, 192 + 16 * 7,16,16,16},

		// RID_BULLET_ANIM: アニメ弾
		{IMG_MAT04_01, 256 + 8 * 0, 32, 8,8},
		{IMG_MAT04_01, 256 + 8 * 1, 32, 8,8},
		{IMG_MAT04_01, 256 + 8 * 2, 32, 8,8},
		{IMG_MAT04_01, 256 + 8 * 3, 32, 8,8},
		{IMG_MAT04_01, 256 + 8 * 4, 32, 8,8},
		{IMG_MAT04_01, 256 + 8 * 5, 32, 8,8},
		{IMG_MAT04_01, 256 + 8 * 6, 32, 8,8},
		{IMG_MAT04_01, 256 + 8 * 7, 32, 8,8},

		// RID_EXPLOSION_L: 爆発(大)
		{IMG_MAT04_01, 0 + 28 * 0, 64, 48,48},
		{IMG_MAT04_01, 0 + 28 * 1, 64, 48,48},
		{IMG_MAT04_01, 0 + 28 * 2, 64, 48,48},
		{IMG_MAT04_01, 0 + 28 * 3, 64, 48,48},
		{IMG_MAT04_01, 0 + 28 * 4, 64, 48,48},
		{IMG_MAT04_01, 0 + 28 * 5, 64, 48,48},

		// RID_EXPLOSION_M: 爆発(中)
		{IMG_MAT04_01, 0 + 32 * 0,160, 32,32},
		{IMG_MAT04_01, 0 + 32 * 1,160, 32,32},
		{IMG_MAT04_01, 0 + 32 * 2,160, 32,32},
		{IMG_MAT04_01, 0 + 32 * 3,160, 32,32},
		{IMG_MAT04_01, 0 + 32 * 4,160, 32,32},
		{IMG_MAT04_01, 0 + 32 * 5,160, 32,32},

		// RID_EXPLOSION_SR: 爆発(小・赤)
		{IMG_MAT04_01, 0 + 24 * 0,112, 24,24},
		{IMG_MAT04_01, 0 + 24 * 1,112, 24,24},
		{IMG_MAT04_01, 0 + 24 * 2,112, 24,24},
		{IMG_MAT04_01, 0 + 24 * 3,112, 24,24},
		{IMG_MAT04_01, 0 + 24 * 4,112, 24,24},
		{IMG_MAT04_01, 0 + 24 * 5,112, 24,24},

		// RID_EXPLOSION_SB: 爆発(小・青)
		{IMG_MAT04_01, 0 + 24 * 0,136, 24,24},
		{IMG_MAT04_01, 0 + 24 * 1,136, 24,24},
		{IMG_MAT04_01, 0 + 24 * 2,136, 24,24},
		{IMG_MAT04_01, 0 + 24 * 3,136, 24,24},
		{IMG_MAT04_01, 0 + 24 * 4,136, 24,24},
		{IMG_MAT04_01, 0 + 24 * 5,136, 24,24},

		///  RID_IMG_MAT04_02

		///  RID_IMG_MAT04_03
		// RID_ENEMY_S0: 敵(小・その0)
		{IMG_MAT04_03, 0 + 32 * 0, 0, 32,32},
		{IMG_MAT04_03, 0 + 32 * 1, 0, 32,32},
		// RID_ENEMY_S1: 敵(小・その2)
		{IMG_MAT04_03, 0 + 32 * 2, 0, 32,32},
		{IMG_MAT04_03, 0 + 32 * 3, 0, 32,32},
		// RID_ENEMY_S2: 敵(小・その3)
		{IMG_MAT04_03, 0 + 32 * 4, 0, 32,32},
		{IMG_MAT04_03, 0 + 32 * 5, 0, 32,32},
		// RID_ENEMY_S3: 敵(小・その4)
		{IMG_MAT04_03, 0 + 32 * 6, 0, 32,32},
		{IMG_MAT04_03, 0 + 32 * 7, 0, 32,32},
		// RID_ENEMY_S4: 敵(小・その5)
		{IMG_MAT04_03, 0 + 32 * 8, 0, 32,32},
		{IMG_MAT04_03, 0 + 32 * 9, 0, 32,32},

		// RID_ENEMY_M0: 敵(中・その0)
		{IMG_MAT04_03, 0 + 64 * 0, 32, 64,48},
		{IMG_MAT04_03, 0 + 64 * 1, 32, 64,48},
		// RID_ENEMY_M1: 敵(中・その1)
		{ IMG_MAT04_03, 0 + 64 * 2, 32, 64,48},
		{ IMG_MAT04_03, 0 + 64 * 3, 32, 64,48},

		// RID_ENEMY_L0: 敵(中・その0)
		{ IMG_MAT04_03, 0 + 48 * 0, 80, 48,80},
		{ IMG_MAT04_03, 0 + 48 * 2, 80, 48,80},
		// RID_ENEMY_L1: 敵(中・その1)
		{ IMG_MAT04_03, 96 + 64 * 0, 80, 64,80},
		{ IMG_MAT04_03, 96 + 64 * 1, 80, 64,80},

		// RID_ENEMY_BOSS: 敵(ボス)
		{ IMG_MAT04_03, 0 + 128 * 0, 160, 128,64},
		{ IMG_MAT04_03, 0 + 128 * 1, 160, 128,64},

		// RID_MISSILE: 16方向ミサイル
		{ IMG_MAT04_03, 0 + 16 * 0, 224, 16,16},
		{ IMG_MAT04_03, 0 + 16 * 1, 224, 16,16},
		{ IMG_MAT04_03, 0 + 16 * 2, 224, 16,16},
		{ IMG_MAT04_03, 0 + 16 * 3, 224, 16,16},
		{ IMG_MAT04_03, 0 + 16 * 4, 224, 16,16},
		{ IMG_MAT04_03, 0 + 16 * 5, 224, 16,16},
		{ IMG_MAT04_03, 0 + 16 * 6, 224, 16,16},
		{ IMG_MAT04_03, 0 + 16 * 7, 224, 16,16},
		{ IMG_MAT04_03, 0 + 16 * 8, 224, 16,16},
		{ IMG_MAT04_03, 0 + 16 * 9, 224, 16,16},
		{ IMG_MAT04_03, 0 + 16 * 10, 224, 16,16},
		{ IMG_MAT04_03, 0 + 16 * 11, 224, 16,16},
		{ IMG_MAT04_03, 0 + 16 * 12, 224, 16,16},
		{ IMG_MAT04_03, 0 + 16 * 13, 224, 16,16},
		{ IMG_MAT04_03, 0 + 16 * 14, 224, 16,16},
		{ IMG_MAT04_03, 0 + 16 * 15, 224, 16,16},
		// RID_MISSILE_BREAK: ミサイル破壊
		{ IMG_MAT04_03, 0 + 16 * 16, 224, 16,16},

	};


	/// リソース_ローダー

	resourceLoader::resourceLoader()
	{
		handloeInitialize();
	}

	resourceLoader::~resourceLoader()
	{
	}

	void resourceLoader::handloeInitialize()
	{
		for (int i = 0; i < 画像_枚数; i++) {
			handleArray_[i] = -1;
		}
	}

	int resourceLoader::Initialize()
	{
		for (int i = 0; i < 画像_枚数; i++) {
			handleArray_[i] = LoadGraph(imgPass_[i]);
			if (handleArray_[i] == -1) return -1;
		}

		return 0;
	}

	int resourceLoader::Cleanup()
	{
		// この後描画を呼ばれても変なメモリを使われないようにする
		handloeInitialize();

		// 読みこんだグラフィックデータをすべて削除
		int ret = InitGraph();

		return ret;
	}


	/// renderingServices

	renderingServices::renderingServices()
	{
		loader = new resourceLoader();
	}

	renderingServices::~renderingServices()
	{
		安全DELETE(loader);
	}

	int renderingServices::Initialize()
	{
		int returnValue = 0;

		returnValue = loader->Initialize();
		if (returnValue != 0) return returnValue;

		// 情報の取得
		RECT shortForm;
		GetClientRect(GetMainWindowHandle(), &shortForm);
		infomation_.screenSize[0] = shortForm.right;
		infomation_.screenSize[1] = shortForm.bottom;

		return returnValue;
	}

	int renderingServices::Cleanup()
	{
		int returnValue = 0;

		returnValue = loader->Cleanup();
		if (returnValue != 0) return returnValue;

		return 0;
	}

	int renderingServices::GetSize(unsigned int resourceID, int(&output)[2]) const
	{
		if (RID_個数 <= resourceID) return -1; // 範囲外確認
		
		const UVtable& uv = loader->UVtable(resourceID);
		output[0] = uv.w;
		output[1] = uv.h;

		return 0;
	}


	void renderingServices::DrawUpperLeft(unsigned int resourceID, int x, int y)
	{
		const UVtable& uv = resourceLoader::UVtable(resourceID);

		//描画(x,y, u,v, w,h, handle,半透明,反転)
		DrawRectGraph(x, y, uv.u, uv.v, uv.w, uv.h,
			loader->GetHandle(uv.img), TRUE, FALSE);
	}

	void renderingServices::Draw(unsigned int resourceID, int x, int y)
	{
		const UVtable& uv = resourceLoader::UVtable(resourceID);

		//描画(x,y, u,v, w,h, handle,半透明,反転)
		DrawRectGraph(x - uv.w / 2, y - uv.h / 2, uv.u, uv.v, uv.w, uv.h,
			loader->GetHandle(uv.img), TRUE, FALSE);
	}

}

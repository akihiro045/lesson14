﻿#include "エンジン.h"
#include "エンティティ.h"
#include "サービス・レンダリング.h"
#include "サービス・入力.h"
#include "サービス・弾丸.h"
#include "DxLib.h"

using namespace エンジン;

ゲームエンジン::ゲームエンジン()
{
	エンティティサービス_ = new エンティティサービス();
	レンダリングサービス_ = new レンダリングサービス();
	入力サービス_ = new 入力サービス();
	弾丸サービス_ = new 弾丸サービス(*レンダリングサービス_);
	システムサービス_ = new システムサービス(*エンティティサービス_, *レンダリングサービス_, *入力サービス_, *弾丸サービス_);
}

ゲームエンジン::~ゲームエンジン()
{
	安全DELETE(システムサービス_);
	安全DELETE(弾丸サービス_);
	安全DELETE(入力サービス_);
	安全DELETE(レンダリングサービス_);
	安全DELETE(エンティティサービス_);
}

int ゲームエンジン::初期化()
{
	ChangeWindowMode(TRUE);//非全画面にセット
	SetGraphMode(540, 960, 32);
	if (DxLib_Init() == -1) { return -1; }	// DXライブラリ初期化(エラーで終了)

	レンダリングサービス_->初期化();
	エンティティサービス_->初期化(システムサービス_);

	//////// ゲームシステム的な初期化
	エンティティサービス_->追加(エンティティサービス::種類::プレイヤー); // 自機追加
	エンティティサービス_->追加(エンティティサービス::種類::ステージ１); // ステージ1の流れを追加

	return 0; 
}

int ゲームエンジン::片付け()
{
	エンティティサービス_->片付け();
	レンダリングサービス_->片付け();

	DxLib_End(); // DXライブラリの終了処理

	return 0;
}

int ゲームエンジン::更新()
{
	int 終わり？ = 0;

	// 時間更新処理
	LONGLONG μ時間 = GetNowHiPerformanceCount();
	LONGLONG 経過μ時間 = μ時間 - 前フレームのμ時刻_;
	if (1000000 < 経過μ時間) 経過μ時間 = 1000000;// 一秒の上限を入れる
	float 経過時間 =  0.000001f * static_cast<float>(経過μ時間);
	前フレームのμ時刻_ = μ時間;

	// いろいろなオブジェクトの更新処理
	入力サービス_->更新();
	エンティティサービス_->更新(経過時間);
	弾丸サービス_->更新(経過時間);

	// 描画
	ClearDrawScreen();//裏画面消す
	SetDrawScreen(DX_SCREEN_BACK);//描画先を裏画面に
	エンティティサービス_->描画();
	弾丸サービス_->描画();
	ScreenFlip();//裏画面を表画面にコピー

	return 終わり？;
}

int ゲームエンジン::実行() 
{
	int 返り値 = 0;

	返り値 = 初期化();
	if (返り値 != 0) return 返り値;

	// メインループ
	while (ProcessMessage() == 0)
	{
		if (更新() != 0) break;// 返り値の値で抜ける
	}

	返り値 = 片付け();

	return 返り値;
}

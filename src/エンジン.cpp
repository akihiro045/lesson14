#include "エンジン.h"
#include "エンティティ.h"
#include "サービス・レンダリング.h"
#include "サービス・入力.h"
#include "サービス・弾丸.h"
#include "DxLib.h"

using namespace engine;

gameEngine::gameEngine()
{
	EntityService_ = new EntityService();
	renderingServices_ = new renderingServices();
	inputService_ = new inputService();
	bulletService_ = new bulletService(*renderingServices_);
	systemService_ = new systemService(*EntityService_, *renderingServices_, *inputService_, *bulletService_);
	//colisionService_ = new colisionService();
}

gameEngine::~gameEngine()
{
	安全DELETE(systemService_);
	安全DELETE(bulletService_);
	安全DELETE(inputService_);
	安全DELETE(renderingServices_);
	安全DELETE(EntityService_);
}

int gameEngine::Initialize()
{
	ChangeWindowMode(TRUE);//非全画面にセット
	SetGraphMode(540, 540, 32);
	if (DxLib_Init() == -1) { return -1; }	// DXライブラリ初期化(エラーで終了)

	renderingServices_->Initialize();
	EntityService_->Initialize(systemService_);

	//////// ゲームシステム的なinitialize
	EntityService_->Add(EntityService::type::player); // 自機追加
	EntityService_->Add(EntityService::type::stage1); // ステージ1の流れを追加

	return 0; 
}

int gameEngine::Cleanup()
{
	EntityService_->Cleanup();
	renderingServices_->Cleanup();

	DxLib_End(); // DXライブラリの終了処理

	return 0;
}

int gameEngine::Update()
{
	int finish = 0;

	// 時間更新処理
	LONGLONG μtime = GetNowHiPerformanceCount();
	LONGLONG elapsedμTime = μtime - μtimeOfOldFrame_;
	if (1000000 < elapsedμTime) elapsedμTime = 1000000;// 一秒の上限を入れる
	float elapsedTime =  0.000001f * static_cast<float>(elapsedμTime);
	μtimeOfOldFrame_ = μtime;

	// いろいろなオブジェクトの更新処理
	inputService_->Update();
	EntityService_->Update(elapsedTime);
	bulletService_->Update(elapsedTime);

	//当たり判定
	for (int i = 0; i < bulletService_->myBullet; i++)
	{
		if (bulletService_->myBullet != NULL)
		{
			for (int j = 0; j < bulletService_->myBullet; j++)
			{
				
			}
		}
	}
	// 描画
	ClearDrawScreen();//裏画面消す
	SetDrawScreen(DX_SCREEN_BACK);//描画先を裏画面に
	EntityService_->Draw();
	bulletService_->Draw();
	ScreenFlip();//裏画面を表画面にコピー

	if (CheckHitKey(KEY_INPUT_ESCAPE)) { finish++; }

	return finish;
}

int gameEngine::Run() 
{
	int returnValue = 0;

	returnValue = Initialize();
	if (returnValue != 0) return returnValue;

	// メインループ
	while (ProcessMessage() == 0)
	{
		if (Update() != 0) break;// 返り値の値で抜ける
	}

	returnValue = Cleanup();

	return returnValue;
}

#include "サービス・弾丸.h"
#include "サービス・レンダリング.h"
#include "DxLib.h"

namespace engine {

	/////////////////////////////////////////////////////
	// 弾丸
	/////////////////////////////////////////////////////

	

	void bullet::Initialize(unsigned int maximum, unsigned int resourceID, renderingServices& renderingServices)
	{
		resourceID_ = resourceID;

		int size[2];
		renderingServices.GetSize(resourceID_, size);
		halfSize_.x = static_cast<float>(size[0] >> 1);
		halfSize_.y = static_cast<float>(size[1] >> 1);

		maximum_ = maximum;
		dataArray_ = new bulletData[maximum];

		Reset();
	}
	void bullet::Cleanup()
	{
		安全DELETE_ARRAY(dataArray_);
		maximum_ = 0;

		Reset();
	}
	void bullet::Reset()
	{
		bulletNum_ = 0;
	}

	int bullet::Add(float2 position, float2 speed)
	{
		if (maximum_ <= bulletNum_) return -1; // プールが余っていない

		bulletData &data = dataArray_[bulletNum_++];
		data.position = position;
		data.speed = speed;
		data.death = false;

		return 0;
	}

	bool bullet::offScreen(float2 position, float2 サイズ, float2 screenSize) {
		return
			position.x < -サイズ.x ||
			position.y < -サイズ.y ||
			screenSize.x + サイズ.x < position.x ||
			screenSize.y + サイズ.y < position.y;
	}

	void bullet::Update(float elapsedTime, renderingServices& renderingServices)
	{
		const renderingServices::infomation& screenInfo = renderingServices.getInfo();

		float2 screenSize(screenInfo.screenSize[0], screenInfo.screenSize[1]);

		for (int i = 0; i < bulletNum_; i++) {
			bulletData& r = dataArray_[i];

			// 移動
			r.position += r.speed * elapsedTime;

			// カリング
			if (offScreen(r.position, halfSize_, screenSize)) {
				r.death = true;
			}
		}
	}

	void bullet::PostUpdateProcexxing()
	{
		// 死んだ子に対して、後ろからデータをコピーしてくる
		// (コピーが起きるので重いが、リスト管理だとキャッシュに載りにくくなるので、
		// あえてデータを丸ごとコピー)
		for (int i = 0; i < bulletNum_; i++) 
		{
			bulletData& r = dataArray_[i];
			if (!r.death) continue;// 死んでないなら対象外

			bulletData* replacementTarget;
			do {
				replacementTarget = &dataArray_[--bulletNum_];// 後ろから持ってくる
			}while(replacementTarget->death && 0 < bulletNum_);// 生きてる最初の物を探す
			if (i < bulletNum_) {// 個数が追いつく場合は入れ替えられるものがなかったという意味
				r = *replacementTarget;
				r.death = false;// この処理の後には、動いている物の死んだフラグは全て落ちている
			}
		}
	}

	void bullet::Draw(renderingServices& renderingServices)
	{
		for (int i = 0; i < bulletNum_; i++) {
			bulletData& r = dataArray_[i];
			renderingServices.Draw(resourceID_, 
				static_cast<int>(r.position.x + 0.5f), 
				static_cast<int>(r.position.y + 0.5f));
		}
	}

	/////////////////////////////////////////////////////
	// bulletService
	/////////////////////////////////////////////////////


	bulletService::bulletService(renderingServices& renderingServices)
		: renderingServices_(renderingServices)
	{
		bullet_[type::myBullet].Initialize(3, RID_SHOT, renderingServices);
		bullet_[type::enemyBullet].Initialize(1024, RID_BULLET_ANIM, renderingServices);

	}

	bulletService::~bulletService()
	{
		for (auto& bullet : bullet_) {
			bullet.Cleanup();
		}
	}

	int bulletService::Add(bulletService::type type, float2 position, float2 speed)
	{
		switch (type) {
		case type::myBullet:
			speed = float2(0.0f, -500.f);
			break;
		case type::enemyBullet:
			break;
		default:
			return -1;// おかしな引数が来た
			break;
		}

		bullet_[type].Add(position, speed);

		return 0;
	}

	void bulletService::Update(float elapsedTime)
	{
		for (auto& bullet : bullet_) {
			bullet.Update(elapsedTime, renderingServices_);
		}
	}

	void bulletService::Draw()
	{
		for (auto& bullet : bullet_) {
			bullet.PostUpdateProcexxing();// 消えた弾のメモリをきれいにしてから描画
			bullet.Draw(renderingServices_);
		}
	}
}// namespace engine

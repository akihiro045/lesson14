#include "共通.h"
#include "サービス・入力.h"
#include "エンティティ.h"

namespace engine
{

	/////////////////////////////////////////////////////
	// スプライトコンポーネント
	/////////////////////////////////////////////////////

	void spriteComponent::Draw()
	{
		float2 position = parent_.getPosition();
		int x = static_cast<int>(position.x + 0.5);
		int y = static_cast<int>(position.y + 0.5);

		if (isThePositionCenter_) {
			systemService_->getRenderer().Draw(resourceID_, x, y);
		}
		else {
			systemService_->getRenderer().DrawUpperLeft(resourceID_, x, y);
		}
	}

	/////////////////////////////////////////////////////
	// プレイヤー・エンティティ
	/////////////////////////////////////////////////////

	playerEntity::playerEntity()
	{
		const renderingServices::infomation& renderingInfo = systemService_->getRenderer().getInfo();
		positionSetting(float2(
			renderingInfo.screenSize[0] / 2,
			renderingInfo.screenSize[1] * 4 / 5));

		sprite_ = dynamic_cast<spriteComponent*>
			(component::componentGeneration(L"スプライトコンポーネント", *this));
		sprite_->resourceConf(RID_IMG_MAT04_01 + 2);
		Add(sprite_);

		input_ = dynamic_cast<inputComponent*>(component::componentGeneration(L"入力コンポーネント", *this));
		Add(input_);

		bullet_ = dynamic_cast<bulletComponent*>(component::componentGeneration(L"弾丸コンポーネント", *this));
		Add(bullet_);
	}

	playerEntity::~playerEntity()
	{

	}

	void playerEntity::Update(float elapsedTime)
	{
		inputData input = input_->GetData();

		// 移動
		float2 position = getPosition();
		float speed = 100.0 * elapsedTime;

		if (input.now & inputService::upMask) {
			position.y -= speed;
		}
		if (input.now & inputService::downMask) {
			position.y += speed;
		}
		if (input.now & inputService::leftMask) {
			position.x -= speed;
		}
		if (input.now & inputService::rightMask) {
			position.x += speed;
		}
		positionSetting(position);

		// 撃つ
		if (input.pushDown & inputService::shootMask) {
			bullet_->Add(bulletService::type::myBullet, this->position_, this->position_);// 速度にダミーで位置を入れた
		}
	}

	void playerEntity::Draw()
	{
		sprite_->Draw();
	}

	/////////////////////////////////////////////////////
	// ザコ１・エンティティ
	/////////////////////////////////////////////////////

	mob1Entity::mob1Entity()
		:survivalTime_(0.0f), shootTimeBullet_(5.0f), ammo_(3)
	{
		sprite_ = dynamic_cast<spriteComponent*>
			(component::componentGeneration(L"スプライトコンポーネント", *this));
		sprite_->resourceConf(RID_ENEMY_S0);
		Add(sprite_);

		Add(dynamic_cast<bulletComponent*>(component::componentGeneration(L"弾丸コンポーネント", *this)));
	}

	void mob1Entity::Update(float elapsedTime)
	{
		survivalTime_ += elapsedTime;
		shootTimeBullet_ -= elapsedTime;

		float2 position = getPosition();
		// 
		// ここをAIで動かすのだ！！！！
		// 
		position.y += 50.0f * elapsedTime;// 全体スクロール
		positionSetting(position);

		// 撃つ
		if (shootTimeBullet_ < 0.0f && 0 < ammo_) {
			ammo_--;
			shootTimeBullet_ = 3.0;

			playerEntity* player = dynamic_cast<playerEntity*>
				(systemService_->getEntity().searchOfFirstEntity(EntityService::type::player));
			float2 speed = player->getPosition() - position_;
			speed = speed.normalaize() * 100.0f;
			bulletComponent* bullet = dynamic_cast<bulletComponent*>(this->searchComponent(L"弾丸コンポーネント"));
			bullet->Add(bulletService::type::enemyBullet, this->position_, speed);// 速度にダミーで位置を入れた
		}
	}

	void mob1Entity::Draw()
	{
		sprite_->Draw();
	}

	/////////////////////////////////////////////////////
	// ステージ１・エンティティ
	/////////////////////////////////////////////////////

	stage1Entity::stage1Entity()
	{
		status_ = 0;
		time_ = 0.0f;
		timeInStatus_ = 0.0f;
	}

	void stage1Entity::Update(float elapsedTime)
	{
		timeInStatus_ += elapsedTime;
		time_ += elapsedTime;
		if (60.0f * 60.0f * 24.0f < time_) time_ = 60.0f * 60.0f * 24.0f;// 24時間で時間を止める(安全のため)


		switch (status_) {
		case 0:// 最初は何も起きない
			if (3.0f < timeInStatus_) {
				advanceStatus();
			}
			break;
		case 1:// 定期的に敵グループを出す
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
			if (1.0f < timeInStatus_) {
				// 敵を出す
				EntityService& service = systemService_->getEntity();
				int enemyHandle = service.Add(EntityService::type::mob1);
				mob1Entity *敵 = dynamic_cast<mob1Entity*>(service.getEntity(enemyHandle));

				// 中央上空から出る
				const renderingServices::infomation& renderingInfo = systemService_->getRenderer().getInfo();
				敵->positionSetting(float2(renderingInfo.screenSize[0] / 2, -100.0f));

				advanceStatus();
			}
			break;
		default:
			// 何も起きない....
			break;
		}
	}

	
}// namespace engine

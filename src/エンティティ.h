#pragma once
#include <vector>
#include <unordered_map>
#include <typeinfo>
#include "共通.h"
#include "サービス・レンダリング.h"
#include "サービス・入力.h"
#include "サービス・弾丸.h"
#include "サービス・当たり判定.h"

namespace engine
{
	// 前方宣言
	class entity;
	class component;
	class EntityService;
	class renderingServices;
	class inputService;
	struct inputData;
	class bulletService;

	/////////////////////////////////////////////////////
	// コンポーネントから使うシステムサービスの集約
	/////////////////////////////////////////////////////
	class systemService
	{
	private:
		EntityService& EntityService_;
		renderingServices& renderingServices_;
		inputService& input_;
		bulletService& bullet_;

	public:
		systemService(EntityService& EntityService, renderingServices& レンダラー, inputService& input, bulletService& bullet) :
			EntityService_(EntityService),
			renderingServices_(レンダラー),
			input_(input),
			bullet_(bullet) {}
		~systemService() {}

		// エンティティから呼び出し
		EntityService& getEntity() { return EntityService_; }
		renderingServices& getRenderer() { return renderingServices_; }
		inputService& getInput() { return input_; }
		bulletService& getBullet() { return bullet_; }
	};


	/////////////////////////////////////////////////////
	// コンポーネント
	/////////////////////////////////////////////////////
	// コンポーネントを追加したら、次も修正すること
	// + それぞれの名前
	// + コンポーネント::キャスト可能？ の中身
	// + コンポーネント::コンポーネント生成 の中身

	class component
	{
	public:// friend 宣言
		friend EntityService;

	private:
		TCHAR* name_ = L"コンポーネント";
	protected:
		static systemService* systemService_;
		entity& parent_;

	public:
		component(entity& parent);
		virtual ~component();

		static bool castPossible(const component* インスタンス, const TCHAR* name);

		static const std::type_info& getTypeInfo(const TCHAR* name);
		TCHAR* getName() { return name_; }
		virtual void Update(float elapsedTime) = 0;

	public:// 静的関数
		static component* componentGeneration(const TCHAR* name, entity& parent);
	};


	class spriteComponent final : public component
	{
	private:
		TCHAR* name_ = L"スプライトコンポーネント";

		int resourceID_ = RID_EXPLOSION_L;		// スプライトの指定
		bool isThePositionCenter_ = true;
	public:
		spriteComponent(entity& parent) :component(parent) {}
		~spriteComponent() {}

		void resourceConf(int resourceID) { resourceID_ = resourceID; }
		void Update(float elapsedTime) {};
		void Draw();
	};

	class inputComponent final : public component
	{
	private:
		TCHAR* name_ = L"入力コンポーネント";
		inputService& input_;
	public:
		inputComponent(entity& parent) : component(parent)
			, input_(systemService_->getInput()) {}
		~inputComponent() {}

		const inputData& GetData() { return input_.GetData(); };

		void Update(float elapsedTime) {};
	};

	class bulletComponent final : public component
	{
	private:
		TCHAR* name_ = L"弾丸コンポーネント";
		bulletService& bullet_;
	public:
		bulletComponent(entity& parent) : component(parent)
			, bullet_(systemService_->getBullet()) {}
		~bulletComponent() {}

		int Add(bulletService::type type, float2 position, float2 speed) { return bullet_.Add(type, position, speed); }

		void Update(float elapsedTime) {};
	};


	/////////////////////////////////////////////////////
	// エンティティ
	/////////////////////////////////////////////////////
	// エンティティを追加したら、次も修正すること
	// + エンティティ・システム::種類
	// + エンティティ・システム::型情報取得
	// + エンティティ・システム::追加

	class entity
	{
	public:// friend 宣言
		friend EntityService;

	protected:
		static systemService* systemService_;

		float2 position_;
		std::vector<component*> componentArray_;

		//当たり判定
		float range;

		//敵の生存フラグ（true = 生存）
		bool flag;
	private:
		void deleteAllComponents();
		void UpdateProcess(float elapsedTime);

	public:
		entity();
		virtual ~entity();

		void Add(component* コンポ) { componentArray_.push_back(コンポ); }
		component* searchComponent(const TCHAR* name);

		const float2& getPosition() const { return position_; }
		void positionSetting(float2 x) { position_ = x; }
		float GetRange() { return range; }
		//当たった時の処理
		virtual void HitAction() {};
		virtual void Update(float elapsedTime) = 0; // 作らなきゃだめ
		virtual void Draw() {};  // 作らなくていい

		//敵の生存フラグを取得
		bool GetFlag() { return flag; }
	};


	/////////////////////////////////////////////////////
	// 具象エンティティ
	/////////////////////////////////////////////////////

	class playerEntity final : public entity
	{
	private:
		spriteComponent* sprite_;
		inputComponent* input_;
		bulletComponent* bullet_;
	public:
		playerEntity();
		~playerEntity();

		void Update(float elapsedTime) override;
		void Draw() override;
	};


	class mob1Entity final : public entity
	{
	private:
		spriteComponent* sprite_;
		float survivalTime_;
		float shootTimeBullet_;
		int ammo_;
	public:
		mob1Entity();
		~mob1Entity() {}

		void Update(float elapsedTime) override;
		void Draw() override;
	};


	class stage1Entity final : public entity
	{
	private:
		int status_;
		float time_;
		float timeInStatus_;

		void advanceStatus() { status_++;  timeInStatus_ = 0.0f; }
	public:
		stage1Entity();
		~stage1Entity() {}

		void Update(float elapsedTime) override;
	};


	/////////////////////////////////////////////////////
	// エンティティの管理
	/////////////////////////////////////////////////////

	class EntityService
	{
	public:// 定数
		enum class type {
			player,
			mob1,
			stage1,
		};

	private:
		int entityID_ = 0;
		std::unordered_map<int, entity*> entityMap_;

		static bool castPossible(entity* インスタンス, type type);
		int allDelete();
	public:
		EntityService();
		~EntityService();

		int Initialize(systemService* service);
		int Cleanup() { return 0; }

		int Add(type type);// ポインタを直接渡さずハンドル(int)を渡すことで、内部管理のコンテナの自由度を増す
		entity* getEntity(int handle);
		entity* searchOfFirstEntity(type type);
		int Delete(int handle);


		void Update(float elapsedTime);
		void Draw();
	};
}
#include <algorithm>
#include "エンティティ.h"
#include "サービス・レンダリング.h"
#include "DxLib.h"

namespace engine
{
	/// static メンバー
	systemService* component::systemService_ = nullptr;
	systemService* entity::systemService_ = nullptr;

	/////////////////////////////////////////////////////
	/// コンポーネント
	/////////////////////////////////////////////////////

	component::component(entity& parent) :parent_(parent)
	{
	}

	component::~component()
	{
	}

	bool component::castPossible(const component* インスタンス, const TCHAR* name)
	{
		if (_tcscmp(L"スプライトコンポーネント", name) == 0) { return nullptr != dynamic_cast<const spriteComponent*>(インスタンス); }
		if (_tcscmp(L"入力コンポーネント", name) == 0) { return nullptr != dynamic_cast<const inputComponent*>(インスタンス); }
		if (_tcscmp(L"弾丸コンポーネント", name) == 0) { return nullptr != dynamic_cast<const bulletComponent*>(インスタンス); }

		return false;
	}

	component* component::componentGeneration(const TCHAR* name, entity& parent)
	{
		struct 文字列分岐 { TCHAR* 文字列; component* (*function)(entity& parent); };

		static 文字列分岐 対応表[] =
		{
			// 具象クラスのコンポーネントが追加されたらここにデータを追加
			{ L"スプライトコンポーネント",   [](entity & parent) { return (component*)(new spriteComponent(parent)); } },
			{ L"入力コンポーネント",         [](entity & parent) { return (component*)(new inputComponent(parent)); } },
			{ L"弾丸コンポーネント",         [](entity & parent) { return (component*)(new bulletComponent(parent)); } },
		};

		for (const auto& c : 対応表)
		{
			int diff = _tcscmp(c.文字列, name);
			if (diff == 0) { return (*c.function)(parent); }
		}

		return nullptr;
	}


	/////////////////////////////////////////////////////
	// エンティティ
	/////////////////////////////////////////////////////

	entity::entity()
	{
		position_.x = position_.y = 0.0f;
	}

	entity::~entity()
	{
		deleteAllComponents();
	}

	component* entity::searchComponent(const TCHAR* name)
	{
		for (auto& component : componentArray_) {
			if (component::castPossible(component, name)) return component;
		}

		return nullptr;
	}


	void entity::deleteAllComponents()
	{
		auto component = componentArray_.begin();
		while (component != componentArray_.end()) {
			安全DELETE(*component);
			component = componentArray_.erase(component);
		}
	}

	void entity::UpdateProcess(float elapsedTime)
	{
		// コンポーネントを最初に処理
		for (auto& component : componentArray_) {
			component->Update(elapsedTime);
		}

		// コンポーネントの後に自分の更新の処理
		Update(elapsedTime);
	}


	/////////////////////////////////////////////////////
	// エンティティ・システム
	/////////////////////////////////////////////////////

	EntityService::EntityService()
	{
	}

	EntityService::~EntityService()
	{
		allDelete();
	}

	int EntityService::Initialize(systemService* service)
	{
		component::systemService_ = service;
		entity::systemService_ = service;

		return 0;
	}


	bool EntityService::castPossible(entity* インスタンス, type type)
	{
		switch (type) {
		case type::player: return nullptr != dynamic_cast<const playerEntity*>(インスタンス);
		case type::stage1: return nullptr != dynamic_cast<const stage1Entity*>(インスタンス);
		case type::mob1:     return nullptr != dynamic_cast<const mob1Entity*>(インスタンス);
		default:return false; // おかしな種類が指定された
		}
		return false;
	}

	int EntityService::Add(type type)
	{
		entity* p = nullptr;

		switch (type) {
		case type::player:
			p = new playerEntity();
			break;
		case type::stage1:
			p = new stage1Entity();
			break;
		case type::mob1:
			p = new mob1Entity();
			break;
		default:
			return -1; // おかしな種類が指定された
		}

		entityMap_[entityID_++] = p;

		return entityID_ - 1;
	}

	entity* EntityService::getEntity(int handle)
	{
#ifdef _DEBUG
		// 存在確認
		auto itr = entityMap_.find(handle);
		if (itr == entityMap_.end()) return nullptr;// 登録されていない
#endif // _DEBUG

		return entityMap_[handle];
	}


	entity* EntityService::searchOfFirstEntity(type type)
	{
		auto it = entityMap_.begin();
		while (it != entityMap_.end()) {
			if (castPossible(it->second, type)) {
				return it->second;
			}
			it++;
		}

		return nullptr;
	}


	int EntityService::allDelete()
	{
		auto it = entityMap_.begin();
		while (it != entityMap_.end()) {
			安全DELETE(it->second);
			entityMap_.erase(it++);
		}

		return 0;
	}

	int EntityService::Delete(int handle)
	{
		auto itr = entityMap_.find(handle);
		if (itr == entityMap_.end()) return -1;// 既に登録されていない

		delete entityMap_[handle];
		entityMap_.erase(handle);

		return 0;
	}

	void EntityService::Update(float elapsedTime)
	{
		for (auto it = entityMap_.begin(); it != entityMap_.end(); it++)
		{
			it->second->UpdateProcess(elapsedTime);
		}
	}

	void EntityService::Draw()
	{
		for (auto it = entityMap_.begin(); it != entityMap_.end(); it++)
		{
			it->second->Draw();
		}
	}
}
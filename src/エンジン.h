#pragma once
#include "DxLib.h"

// 前方宣言
namespace engine {
	class renderingServices;
	class EntityService;
	class inputService;
	class bulletService;
	class systemService;
	class colisionService;
}

// クラス宣言
class gameEngine
{
private:
	engine::EntityService* EntityService_;
	engine::renderingServices* renderingServices_;
	engine::inputService* inputService_;
	engine::bulletService* bulletService_;
	engine::systemService* systemService_;
	engine::colisionService* colisionService_;

	LONGLONG μtimeOfOldFrame_ = 0;

	int Initialize();
	int Update();
	int Cleanup();

public:
	gameEngine();
	~gameEngine();

	int Run();
};

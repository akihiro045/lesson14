#include "サービス・入力.h"
#include "DxLib.h"

namespace engine {

	void inputService::Update()
	{
		static const unsigned char table[][2] = {
			{KEY_INPUT_SPACE,	startMask},
			{KEY_INPUT_ESCAPE,	cancelMask},
			{KEY_INPUT_Z,		shootMask},
			{KEY_INPUT_X,		specialMask},
			{KEY_INPUT_UP,		upMask},
			{KEY_INPUT_DOWN,	downMask},
			{KEY_INPUT_LEFT,	leftMask},
			{KEY_INPUT_RIGHT,	rightMask},
			{0xff, }
		};

		// 前の値を保存
		oldFrame_ = inputData_.now;

		// 全キーの状態を補足
		char buf[256];
		GetHitKeyStateAll(buf);

		// 8bitに収める
		unsigned char now = 0;
		for (int i = 0; table[i][0] != 0xff; i++) {
			if (buf[table[i][0]])
			{
				now |= table[i][1];
			}
		}
		inputData_.now = now;

		// トリガ情報を生成
		unsigned char difference = now ^ oldFrame_;
		inputData_.pushDown = difference & now;
		inputData_.pullUp = difference & oldFrame_;

		//		printfDx(L"%x %x %x\n", 生_, 押し下げ_, 引き上げ_);
	}

}// namespace engine
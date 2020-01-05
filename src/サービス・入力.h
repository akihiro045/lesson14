#pragma once

namespace engine {

	struct inputData{
		unsigned char now = 0;
		unsigned char pushDown = 0;
		unsigned char pullUp = 0;
	};

	class inputService {
	public:// 定数宣言
		enum type {
			スタート = 0,
			キャンセル,
			ショット,
			スペシャル,
			上,
			下,
			左,
			右,
		};

		static constexpr unsigned int startMask	= (1 << スタート);
		static constexpr unsigned int cancelMask = (1 << キャンセル);
		static constexpr unsigned int shootMask	= (1 << ショット);
		static constexpr unsigned int specialMask = (1 << スペシャル);
		static constexpr unsigned int upMask			= (1 << 上);
		static constexpr unsigned int downMask			= (1 << 下);
		static constexpr unsigned int leftMask			= (1 << 左);
		static constexpr unsigned int rightMask			= (1 << 右);

	private:
		unsigned char oldFrame_ = 0;
		inputData inputData_;
	public:
		inputService() {}
		~inputService() {}

		void Update();

		inline const inputData &GetData() { return inputData_; }
	};

}// namespace engine
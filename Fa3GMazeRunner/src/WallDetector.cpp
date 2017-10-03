#include <thread>
#include "WallDetector.h"

// コンストラクタ
WallDetector::WallDetector()
{
	// 初期化処理

	// INIファイルの読み込み

	// 壁接近のキャリブレーション(前左右を壁に)

}


//// 暫定
//std::this_thread::sleep_for(std::chrono::seconds(time));

//return MotorOutputParam();

// 現在の壁情報を返す
Wall WallDetector::chkWall()
{
	return this->Wall();
}

// 距離を求める
void WallDetector::calcDistances()
{

}

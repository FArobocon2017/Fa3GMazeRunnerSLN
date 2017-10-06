#include <iostream>
#include <fstream>
#include <thread>
//#include "WallDetector.h"
#include "..\include\WallDetector.h"

// コンストラクタ
WallDetector::WallDetector()
{
	// 初期ファイル読込
	// 
	// 壁検知のキャリブレーション
}

// 延々とセンサ計測と壁情報の更新を行うメソッド
void WallDetector::startLightSensor()
{
	this->continueSenseFlag = true;

	// フラグが立ち続ける限り計測＆壁情報更新
	while (this->continueSenseFlag)
	{
		// 距離計測
		this->calcDistances();

		// 壁情報の更新
		this->updateWallInfo();

		// スリープ
		std::this_thread::sleep_for(std::chrono::microseconds(this->chkInterval));
	}
}

// 現在の壁情報を返す
WallDetector::Wall WallDetector::chkWall()
{
	return this->wall;
}

// 光センサの計測を中止する関数（中身はフラグの変更のみ）
int WallDetector::stopLightSensor()
{
	try
	{
		this->continueSenseFlag = false;
	}
	catch (const std::exception&)
	{
		return -1;
	}

	return 0;
}

// 距離を求める
void WallDetector::calcDistances()
{
	// 変数の初期化
	int lumidata[4] = { 0 }; // 輝度値[4]
	
	// 各方向の距離を求める
	this->distdata[WallDirectionenum::Left] = this->calcOneDistance(SensorAccessenum::Left, lumidata[SensorAccessenum::Left]);
	this->distdata[WallDirectionenum::Right] = this->calcOneDistance(SensorAccessenum::Right, lumidata[SensorAccessenum::Right]);
	this->distdata[WallDirectionenum::Forward] = this->calcOneDistance(SensorAccessenum::Forward1, lumidata[SensorAccessenum::Forward1]);
	this->distdata[WallDirectionenum::Forward] += this->calcOneDistance(SensorAccessenum::Forward2, lumidata[SensorAccessenum::Forward2]);

	// 前方は2回あるので、平均を求める
	this->distdata[WallDirectionenum::Forward] /= 2;
}

// センサの取得値を返す
void WallDetector::getlumidata(int lumidata[4]) {
	
	// 取得データをそのまま返す
	lumidata[SensorAccessenum::Left] = this->lumihistory[0].lumidata[SensorAccessenum::Left];
	lumidata[SensorAccessenum::Right] = this->lumihistory[0].lumidata[SensorAccessenum::Right];
	lumidata[SensorAccessenum::Forward1] = this->lumihistory[0].lumidata[SensorAccessenum::Forward1];
	lumidata[SensorAccessenum::Forward2] = this->lumihistory[0].lumidata[SensorAccessenum::Forward2];
}


// 取得データをそのまま返す
void get_sensor_sts(int* lumidata) {
	std::ifstream ifs("/dev/rtlightsensor0");
	ifs >> lumidata[0] >> lumidata[1] >> lumidata[2] >> lumidata[3];
}

// 取得履歴からデータを返す
void get_sensor_sts(int* lumidata) {
	std::ifstream ifs("/dev/rtlightsensor0");
	ifs >> lumidata[0] >> lumidata[1] >> lumidata[2] >> lumidata[3];
}

// 距離計算(1つ)
double WallDetector::calcOneDistance(SensorAccessenum direction, int lumi)
{
	double dist = 0;
	double coeffSub = this->coeffDistanceSubtraction[direction];
	double coeffDiv = this->coeffDistanceDivision[direction];

	dist = sqrt((lumi - coeffSub) / coeffDiv);

	return dist;
}

// 壁情報の更新
void WallDetector::updateWallInfo()
{
	for (int i = 0; i < WallDirectionenum::Max; i++)
	{
		if (this->distdata[i] < this->wallDetectThreshold)
		{
			this->wall.data[i] = 1;
		}
		else
		{
			this->wall.data[i] = 0;
		}
	}
}
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <unistd.h>

//#include "WallDetector.h"
#include "..\include\WallDetector.h"

using namespace std;

// コンストラクタ
//WallDetector::WallDetector()
//{
	// 初期ファイル読込
	// 
	// 壁検知のキャリブレーション
//}

// iniファイルの読み込み
void WallDetector::initWallDetector()
{
	try
	{
		ifstream ifs(".\\WallDetector.ini");

		// 分割パラメタ取得
		for (size_t i = 0; i < LightSensor::Sensor::Max; i++)
		{
			string str;
			int col[LightSensor::Max] = { 0 };
			cout << str << ":" << col[0] << " " << col[1] << " " << col[2] << " " << col[3] << endl;
			this->coeffDistanceDivision[LightSensor::Sensor::Left] = col[LightSensor::Sensor::Left];
			this->coeffDistanceDivision[LightSensor::Sensor::Forward1] = col[LightSensor::Sensor::Forward1];
			this->coeffDistanceDivision[LightSensor::Sensor::Forward2] = col[LightSensor::Sensor::Forward2];
			this->coeffDistanceDivision[LightSensor::Sensor::Right] = col[LightSensor::Sensor::Right];
		}

		// 差分パラメタ取得
		for (size_t i = 0; i < LightSensor::Sensor::Max; i++)
		{
			string str;
			int col[LightSensor::Max] = { 0 };
			cout << str << ":" << col[0] << " " << col[1] << " " << col[2] << " " << col[3] << endl;
			this->coeffDistanceSubtraction[LightSensor::Sensor::Left] = col[LightSensor::Sensor::Left];
			this->coeffDistanceSubtraction[LightSensor::Sensor::Forward1] = col[LightSensor::Sensor::Forward1];
			this->coeffDistanceSubtraction[LightSensor::Sensor::Forward2] = col[LightSensor::Sensor::Forward2];
			this->coeffDistanceSubtraction[LightSensor::Sensor::Right] = col[LightSensor::Sensor::Right];
		}
		ifs.close();

	}
	catch (const std::exception&)
	{
		// 実装しない
	}
}

// 現在の壁情報を返す(Minimum構造)
WallDetector::Wall WallDetector::chkWall()
{
	// 距離計測
	this->calcDistances();

	// 壁情報の更新
	this->updateWallInfo();

	return this->wall;
}

// 延々とセンサ計測と壁情報の更新を行うメソッド
void WallDetector::startLightSensor()
{
	this->isContinueSense = true;
	int i = 0;
	// フラグが立ち続ける限り計測＆壁情報更新
	while (this->isContinueSense)
	{
		// 距離計測
		this->calcDistances();

		// 壁情報の更新
		this->updateWallInfo();

		// スリープ
		usleep(100000);
		
	}
}

// 光センサの計測を中止する関数（中身はフラグの変更のみ）
int WallDetector::stopLightSensor()
{
	try
	{
		this->isContinueSense = false;
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
	int lumidata[LightSensor::Max] = { 0 }; // 輝度値[4]
	
	// センサを1回作動
	this->getlumidataOnce(lumidata);

	// 各方向の距離を求める
	this->distdata[WallDetection::Direction::Left] = this->calcOneDistance(LightSensor::Sensor::Left, lumidata[LightSensor::Sensor::Left]);
	this->distdata[WallDetection::Direction::Right] = this->calcOneDistance(LightSensor::Sensor::Right, lumidata[LightSensor::Sensor::Right]);
	this->distdata[WallDetection::Direction::Forward] = this->calcOneDistance(LightSensor::Sensor::Forward1, lumidata[LightSensor::Sensor::Forward1]);
	this->distdata[WallDetection::Direction::Forward] += this->calcOneDistance(LightSensor::Sensor::Forward2, lumidata[LightSensor::Sensor::Forward2]);

	// 前方は2回あるので、平均を求める
	this->distdata[WallDetection::Direction::Forward] /= 2;

	if (this->isDebug)
	{
		this->writeDistancefromEachSensor();
	}
}

// センサの取得値を返す(1回計測を返す)
void WallDetector::getlumidataOnce(int lumidata[LightSensor::Max]) {
	
	// センサ作動(取得データをそのまま返す)
	get_sensor_sts(lumidata);
}

// センサの取得値を返す(過去historyMax回の計測の平均を返す)
void WallDetector::getlumidataHist(int outputlumidata[LightSensor::Max]) {

	// 一時用
	int templumidata[LightSensor::Max];
	// センサ作動
	get_sensor_sts(templumidata);

	// 保有数の更新
	if (historyHolderNum < historyMax) historyHolderNum++;

	// 取得値の格納と平均値の格納
	for (int i = 0; i < LightSensor::Max; i++)
	{
		// 格納
		this->lumihistory[historyNextLocation].lumidata[i] = templumidata[i];

		// i番目のセンサの平均値の計算
		int ave = 0;
		for (int j = 0; j < historyHolderNum; j++)
		{
			ave += this->lumihistory[j].lumidata[i];
		}
		// 返す配列に平均値を代入
		outputlumidata[i] = ave / historyHolderNum;
	}

	// 次の格納先の番地を更新
	historyNextLocation = (historyNextLocation + 1) % historyMax;
}

// 取得データをそのまま返す
void WallDetector::get_sensor_sts(int* lumidata) {
	std::ifstream ifs("/dev/rtlightsensor0");
	ifs >> lumidata[0] >> lumidata[1] >> lumidata[2] >> lumidata[3];
}

// 距離計算(1つ)
double WallDetector::calcOneDistance(LightSensor::Sensor direction, int lumi)
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
	for (int i = 0; i < WallDetection::Direction::Max; i++)
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

// 距離をファイル出力(dist_history)
void WallDetector::writeDistancefromEachSensor()
{
	ofstream ofs("dist_history.txt", ios::app);
	ofs << this->distdata[LightSensor::Sensor::Left] << " ";
	ofs << this->distdata[LightSensor::Sensor::Forward1] << " ";
	ofs << this->distdata[LightSensor::Sensor::Forward2] << " ";
	ofs << this->distdata[LightSensor::Sensor::Right] << endl;
	ofs.close();
}

// 光センサの取得値をファイル出力(light_history.txt)
void WallDetector::writeLightfromEachSensor()
{
	ofstream ofs("light_history.txt", ios::app);
	ofs << this->distdata[LightSensor::Sensor::Left] << " ";
	ofs << this->distdata[LightSensor::Sensor::Forward1] << " ";
	ofs << this->distdata[LightSensor::Sensor::Forward2] << " ";
	ofs << this->distdata[LightSensor::Sensor::Right] << endl;
	ofs.close();
}



// 光センサのキャリブレーション→距離計算の係数を求める
int WallDetector::setCoefficient()
{
	// 係数計算の入力になる配列
	int aveDistantLight[LightSensor::Max] = { 0 };
	int aveCloseantLight[LightSensor::Max] = { 0 };

	// センサ値の受け取り用の配列
	int tempDistantLight[LightSensor::Max] = { 0 };
	int tempCloseantLight[LightSensor::Max] = { 0 };

	// ブザー鳴らす（1秒）
	// 3秒スリープ
	// forループ5回でセンサ平均値取得（壁と判定する距離）
	// 1秒スリープ


	// ブザー鳴らす（1秒）
	// 3秒スリープ
	// forループ5回でセンサ平均値取得（壁と判定しない距離（2cm以上離す?）
	// 1秒スリープ

	// 係数計算
	this->calcCoefficient(aveDistantLight, aveCloseantLight);

	// iniファイルへ書き出し

	// ブザー鳴らす（1秒。終了の意味）


}

// 距離計算の係数を求める
void WallDetector::calcCoefficient(int distantLight[LightSensor::Max], int closeantLight[LightSensor::Max])
{
	// 係数を求めるための前準備
	double diffDist = pow(this->wallDetectThreshold,2) - pow(this->updateWallInfo, 2);
	double distSquare = pow(this->wallDetectThreshold, 2);

	// 係数の計算と代入
	for (int i = 0; i < LightSensor::Max; i++)
	{
		this->coeffDistanceDivision[i] = (closeantLight[i] - distantLight[i]) / diffDist;
		this->coeffDistanceSubtraction[i] = closeantLight[i] - (this->coeffDistanceDivision[i] / distSquare);
	}
}
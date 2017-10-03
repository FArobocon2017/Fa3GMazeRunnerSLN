#pragma once

class WallDetector
{
public:

	// 方向ごとの壁情報の参照方法
	enum WallDirectionenum
	{
		Left = 0,
		Forward,
		Right,
	};

	// センサの参照方法
	enum SensorAccessenum
	{
		Left = 0,
		Forward1,
		Forward2,
		Right
	};

	// 壁検知結果を返す構造体
	// 0：未検知、1:検知
    struct Wall
    {
        int data[3]; 
        Wall():data(){}
    };

	////// 以下、関数 //////
	// コンストラクタ
	WallDetector() {};
	// デストラクタ
	~WallDetector() {};
	
	// 壁情報を返す
	Wall chkWall();// const { return Wall(); }

	// 光センサの計測を延々と続ける関数
	void startLightSensor();

	// 光センサの計測を中止する関数（中身はフラグの変更）
	int stopLightSensor();



private:
	////// 以下、属性 //////
	// センサ実行の時間間隔(INIファイルに移行する？)
	int chkInterval = 100;

	// 検知判定の距離(cm)(INIファイルに移行する？)
	const double wallDetectThreshold = 4.0;

	// 履歴の最大保持数(INIファイルに移行する？)
	static const int historyMax = 10;

	// 履歴の保持数(MAX=historyMax)
	int historyHolderNum = 0;

	// 最新の結果が入っている番地
	int historyLatestLocation;

	// 過去の検知結果の履歴(保持数は検知間隔によって要検討)
	struct LumiHistory
	{
		int lumidata[historyMax][4];
		LumiHistory() :lumidata() {}
	};
	//	int chkWallHistory[historyMax];

	// 逆2乗の法則で、光量のルートを取る前にを割る値
	double calcDistanceCoefficient[4];

	// センサ実行を続けるためのフラグ
	bool continueSenseFlag;

	////// 以下、関数 //////
	// 壁情報の更新
	void updateWallInfo();

	// 距離計算（4センサまとめて）
	void calcDistances();

	// 距離計算
	void calcOneDistance(int lumi);

	// 光センサのキャリブレーション→距離計算の係数を求める
	int calcCoefficient();
};
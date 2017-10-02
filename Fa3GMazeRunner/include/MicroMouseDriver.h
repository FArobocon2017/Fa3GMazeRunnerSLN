#pragma once
#include <memory>
#include "MazeData.h"
#include "MazeData.h"

class WallDetector;
class Motor;


class MicroMouseDriver
{   
	 #define	STEP_ANG				0.9		    //モータが1ステップ辺りに回転する角度
	 #define	STEP_SLOW				500			//1秒あたりにモーターに与えるステップ数
	 #define	STEP_MIDDLE				1000		//1秒あたりにモーターに与えるステップ数
	 #define	STEP_FAST				1500		//1秒あたりにモーターに与えるステップ数
	 #define	PI						3.14159		//π
	 #define	WHEEL_RADIUS			47		    //車輪の直径(mm)
	 #define	DEGREE					360		    //360°
	 #define	BLOCK					180		    //1ブロックの大きさ(mm)
	 #define	WHEEL_AXLE				95		    //車軸の長さ(mm)




     std::unique_ptr<WallDetector> m_pWallDetector;
     std::unique_ptr<Motor> m_pMotor;
	 double MicroMouseDriver::calculateSleepTime(double stepNum);
	 double MicroMouseDriver::calculateTurnSleepTime(double turnDegree, double stepNum);

public:
    MicroMouseDriver();
    ~MicroMouseDriver();
    MotorOutputParam drive(const MotorInputParam& motorParam);


	void MicroMouseDriver::driveNBlock(int N);
	void MicroMouseDriver::riverseNBlock(int N);
	void MicroMouseDriver::stop();
	void MicroMouseDriver::turnLeft();
	void MicroMouseDriver::turnRight();
};
#include <thread>
#include "..\include\WallDetector.h"
#include "..\include\MicroMouseDriver.h"
#include "..\include\Motor.h"


MicroMouseDriver::MicroMouseDriver()
    : m_pWallDetector(new WallDetector())
    , m_pMotor(new Motor())
{}

MicroMouseDriver::~MicroMouseDriver()
{}

MotorOutputParam MicroMouseDriver::drive(const MotorInputParam& motorParam)
{
    // モーターに指令
    auto output = m_pMotor->setMotorparam(motorParam.lVal, motorParam.rVal, motorParam.time);
    // 壁との距離を計測
    auto wall = m_pWallDetector->chkWall();
    output.wallDistance[0] = wall.data[0];
    output.wallDistance[1] = wall.data[1];
    output.wallDistance[2] = wall.data[2];
    return output;
}

//ロボットを指定ブロック数直進させる関数
 void MicroMouseDriver::driveNBlock(int N)
{
	Motor motor;

	motor.ctr_motor_hz(STEP_MIDDLE, STEP_MIDDLE);

	_sleep(calculateSleepTime(STEP_MIDDLE));

	stop();
}


 //ロボットを指定ブロック数後退させる関数
 void MicroMouseDriver::riverseNBlock(int N)
 {
	 Motor motor;

	 motor.ctr_motor_hz(-STEP_MIDDLE, -STEP_MIDDLE);

	 _sleep(calculateSleepTime(STEP_MIDDLE));

	 stop();
 }


 //ロボットを停止させる関数
 void MicroMouseDriver::stop()
 {
	 Motor motor;
	 motor.ctr_motor_stop();

 }



 //ロボットを左旋回させる関数
 void MicroMouseDriver::turnLeft()
 {
	 Motor motor;
	 motor.ctr_motor_hz(-STEP_MIDDLE, STEP_MIDDLE);
	 calculateTurnSleepTime(45, STEP_MIDDLE);
	 motor.ctr_motor_stop();
 }


 //ロボットを右旋回させる関数
 void MicroMouseDriver::turnRight()
 {

	 Motor motor;
	 motor.ctr_motor_hz(STEP_MIDDLE, -STEP_MIDDLE);
	 calculateTurnSleepTime(45, STEP_MIDDLE);
	 motor.ctr_motor_stop();
 }

 //ロボットを直進させるときのスリープ時間を計算
 //double stepNum = ステップ数
 //return スリープさせる時間
 double MicroMouseDriver::calculateSleepTime(double stepNum)
 {
	 //式：STEP_ANG*ステップ数/360*直径*π*秒=進む距離

	 if (stepNum < 0) stepNum = stepNum*-1;

	 //1秒あたりの回転角度
	 double rotationAng = STEP_ANG * stepNum;
	 //1秒あたりの回転数
	 double rotatioNum = rotationAng / DEGREE;
	 //車輪の周
	 double wheelCircumference = WHEEL_RADIUS * PI;

	 return BLOCK / rotatioNum / wheelCircumference;

 }


 //ロボットを旋回させるときのスリープ時間を計算
 //double turnDegree = 旋回させる角度
 //double stepNum = ステップ数
 //return スリープさせる時間
 double MicroMouseDriver::calculateTurnSleepTime(double turnDegree, double stepNum)
 {

	 if (stepNum < 0) stepNum = stepNum*-1;

	 //1秒あたりの回転角度
	 double rotationAng = STEP_ANG * stepNum;

	 //1秒あたりの回転数
	 double rotatioNum = rotationAng / DEGREE;

	 //車輪の周
	 double wheelCircumference = WHEEL_RADIUS * PI;

	 //車軸の周
	 double wheelAxleCircumference = WHEEL_AXLE * PI;

	 //車輪の回転する距離
	 double turnDistance = wheelAxleCircumference * turnDegree / DEGREE;

	 return turnDistance / rotatioNum / wheelCircumference;
 }
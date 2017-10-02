#pragma once
#include <memory>
#include "MazeData.h"
#include "MazeData.h"

class WallDetector;
class Motor;


class MicroMouseDriver
{   
	 #define	STEP_ANG				0.9		    //���[�^��1�X�e�b�v�ӂ�ɉ�]����p�x
	 #define	STEP_SLOW				500			//1�b������Ƀ��[�^�[�ɗ^����X�e�b�v��
	 #define	STEP_MIDDLE				1000		//1�b������Ƀ��[�^�[�ɗ^����X�e�b�v��
	 #define	STEP_FAST				1500		//1�b������Ƀ��[�^�[�ɗ^����X�e�b�v��
	 #define	PI						3.14159		//��
	 #define	WHEEL_RADIUS			47		    //�ԗւ̒��a(mm)
	 #define	DEGREE					360		    //360��
	 #define	BLOCK					180		    //1�u���b�N�̑傫��(mm)
	 #define	WHEEL_AXLE				95		    //�Ԏ��̒���(mm)




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
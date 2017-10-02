#pragma once

#include "MazeData.h"

class Motor
{



public:
    Motor();

	void motor_sw_on(void);
	void motor_sw_off(void);
	void ctr_motor_hz(int l_val, int r_val);
	void ctr_motor_stop(void);
    MotorOutputParam setMotorparam(int lVal, int rVal, int time);
};
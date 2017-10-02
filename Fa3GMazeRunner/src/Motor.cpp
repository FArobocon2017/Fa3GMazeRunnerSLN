#include <thread>
#include "..\include\Motor.h"

Motor::Motor()
{}

using namespace std;

void Motor::motor_sw_on(void)
{
	ofstream motor_sw("/dev/rtmotoren0");
	motor_sw << 1;
	motor_sw.close();
}

void Motor::motor_sw_off(void)
{
	ofstream motor_sw("/dev/rtmotoren0");
	motor_sw << 0;
	motor_sw.close();
}

void Motor::ctr_motor_hz(int l_val, int r_val)
{
	ofstream l_motor("/dev/rtmotor_raw_l0");
	ofstream r_motor("/dev/rtmotor_raw_r0");
	l_motor << l_val;
	r_motor << r_val;
	l_motor.close();
	r_motor.close();
}

void Motor::ctr_motor_stop(void)
{
	ofstream l_motor("/dev/rtmotor_raw_l0");
	ofstream r_motor("/dev/rtmotor_raw_r0");
	l_motor << 0;
	r_motor << 0;
	l_motor.close();
	r_motor.close();
}
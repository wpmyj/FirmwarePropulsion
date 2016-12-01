/**
 * @file	BrushlessMotorDriver.cpp
 * @author	Kevin
 * @date	1 d�c. 2016
 * @brief	L6235 Brushless Motor Driver class
 */

#include "BrushlessMotorDriver.hpp"

using namespace Utils;
using namespace HAL;

/*----------------------------------------------------------------------------*/
/* Definitions                                                                */
/*----------------------------------------------------------------------------*/

#define L_MOT_BRAKE_GPIO_ID		(GPIO::GPIO2)
#define L_MOT_DIR_GPIO_ID		(GPIO::GPIO1)
#define L_MOT_DIAG_GPIO_ID		(GPIO::GPIO0)
#define L_MOT_ENABLE_PWM_ID		(PWM::PWM0)

#define R_MOT_BRAKE_GPIO_ID		(GPIO::GPIO5)
#define R_MOT_DIR_GPIO_ID		(GPIO::GPIO4)
#define R_MOT_DIAG_GPIO_ID		(GPIO::GPIO3)
#define R_MOT_ENABLE_PWM_ID		(PWM::PWM1)

#define ENABLE_PWM_FREQ			(10000u)

/*----------------------------------------------------------------------------*/
/* Private Members                                                            */
/*----------------------------------------------------------------------------*/

static BrushlessMotorDriver* _drivers[BrushlessMotorDriver::MOTOR_MAX] = {NULL};

/*----------------------------------------------------------------------------*/
/* Private Functions                                                          */
/*----------------------------------------------------------------------------*/

static BLMOTDRV_DEF _getDefStructure (BrushlessMotorDriver::ID id)
{
	BLMOTDRV_DEF def;

	assert(id < BrushlessMotorDriver::MOTOR_MAX);

	switch(id)
	{
	case BrushlessMotorDriver::LEFT_MOTOR:
		def.BrakePinID	=	L_MOT_BRAKE_GPIO_ID;
		def.DirPinID	=	L_MOT_DIR_GPIO_ID;
		def.DiagPinID	=	L_MOT_DIAG_GPIO_ID;
		def.EnablePinID	=	L_MOT_ENABLE_PWM_ID;
		break;

	case BrushlessMotorDriver::RIGHT_MOTOR:
		def.BrakePinID	=	R_MOT_BRAKE_GPIO_ID;
		def.DirPinID	=	R_MOT_DIR_GPIO_ID;
		def.DiagPinID	=	R_MOT_DIAG_GPIO_ID;
		def.EnablePinID	=	R_MOT_ENABLE_PWM_ID;
		break;

	default:
		break;
	}

	return def;
}

static void _diagStateChanged (void * obj)
{

}

/*----------------------------------------------------------------------------*/
/* Class Implementation	                                                      */
/*----------------------------------------------------------------------------*/

namespace HAL
{
	BrushlessMotorDriver* BrushlessMotorDriver::GetInstance(BrushlessMotorDriver::ID id)
	{
		assert(id < BrushlessMotorDriver::MOTOR_MAX);

		// if instance already exists
		if(_drivers[id] != NULL)
		{
			return _drivers[id];
		}
		else
		{
			// Create instance
			_drivers[id] = new BrushlessMotorDriver(id);

			return _drivers[id];
		}
	}

	BrushlessMotorDriver::BrushlessMotorDriver(BrushlessMotorDriver::ID id)
	{
		this->speed = 0.0f;
		this->direction = BrushlessMotorDriver::FORWARD;
		this->def = _getDefStructure(id);

		// Get brake pin instance
		this->brakePin = GPIO::GetInstance(this->def.BrakePinID);
		this->brakePin->Set(GPIO::Low);

		// Get dir pin instance
		this->dirPin = GPIO::GetInstance(this->def.DirPinID);
		this->dirPin->Set(GPIO::Low);

		// Get enable pin instance
		this->enablePin = PWM::GetInstance(this->def.EnablePinID);
		this->enablePin->SetFrequency(ENABLE_PWM_FREQ);
		this->enablePin->SetDutyCycle(0.0f);

		// Get diag pin instance
		this->diagPin = GPIO::GetInstance(this->def.DiagPinID);
		this->diagPin->StateChanged += _diagStateChanged;
	}

	void BrushlessMotorDriver::SetSpeed(float32_t percent)
	{
		this->speed = speed;

		this->enablePin->SetDutyCycle(speed);
	}

	void BrushlessMotorDriver::SetDirection(enum BrushlessMotorDriver::Direction direction)
	{
		if(direction == BrushlessMotorDriver::FORWARD)
		{
			this->dirPin->Set(GPIO::High);
		}
		else
		{
			this->dirPin->Set(GPIO::Low);
		}
	}

	void BrushlessMotorDriver::Brake()
	{
		this->enablePin->SetDutyCycle(0.0f);
		this->brakePin->Set(GPIO::Low);
	}

	void BrushlessMotorDriver::Move()
	{
		this->brakePin->Set(GPIO::High);
		this->enablePin->SetDutyCycle(this->speed);
	}

	void BrushlessMotorDriver::Freewheel()
	{
		this->brakePin->Set(GPIO::High);
		this->enablePin->SetDutyCycle(0.0f);
	}
}

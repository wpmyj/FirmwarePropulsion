/**
 * @file	PID.hpp
 * @author	Kevin WYSOCKI
 * @date	18 nov. 2016
 * @brief	PID Controller class
 *
 * HOWTO :
 * - Create a new PID with PID()
 * - Call Reset() to reset errors
 * - Call SetSetpoint() to set controller setpoint
 * - Call Get() to get the new output from a feedback value
 */

#ifndef INC_PID_HPP_
#define INC_PID_HPP_

#include "common.h"

/*----------------------------------------------------------------------------*/
/* Definitions                                                                */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Class declaration	                                                      */
/*----------------------------------------------------------------------------*/

/**
 * @namespace Utils
 */
namespace Utils
{
	/**
	 * @class PID
	 * HOWTO :
	 * - Create a new PID with PID()
	 * - Call Reset() to reset errors
	 * - Call SetSetpoint() to set controller setpoint
	 * - Call Get() to get the new output from a feedback value
	 */
	class PID
	{
	public:

		/**
		 * @brief PID default constructor
		 */
		PID();

		/**
		 * @brief PID constructor
		 * @param kp : Kp term
		 * @param ki : Ki term
		 * @param kd : Kd term
		 * @param dt : Loop period in seconds (Integration and derivation time)
		 */
		PID (float32_t kp, float32_t ki, float32_t kd, float32_t dt);

		/**
		 * @brief Reset all errors
		 */
		void Reset();

		/**
		 * @brief Update setpoint
		 */
		void SetSetpoint (float32_t setpoint)
		{
			this->setpoint = setpoint;
		}

		/**
		 * @brief Compute PID
		 * @param feedback : System feedback value
		 * @return PID filter output
		 *
		 * The filter output is normalized to the setpoint :
		 * - 0.0 means 0% of the setpoint
		 * - 1.0 means 100% of the sepoint
		 *
		 * This is usefull to controll processes in duty cycle
		 *
		 */
		float32_t Get (float32_t feedback);

	private:

		/**
		 * @private
		 * @brief Kp term
		 */
		float32_t kp;

		/**
		 * @private
		 * @brief Ki term
		 */
		float32_t ki;

		/**
		 * @private
		 * @brief Kd term
		 */
		float32_t kd;

		/**
		 * @private
		 * @brief Loop period in seconds
		 */
		float32_t dt;

		/**
		 * @private
		 * @brief Current error
		 */
		float32_t err;

		/**
		 * @private
		 * @brief Integral error
		 */
		float32_t intErr;

		/**
		 * @private
		 * @brief Differential error
		 */
		float32_t diffErr;

		/**
		 * @private
		 * @brief Current setpoint
		 */
		float32_t setpoint;

		/**
		 * @private
		 * @brief Current output
		 */
		float32_t output;
	};
}

#endif /* INC_PID_HPP_ */

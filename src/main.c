/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include "device.h"

#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include <stdlib.h>
#include <string.h>

#include "system.h"        /* System funct/params, like osc/peripheral config */

#include "connectivity.h"
#include "motors.h"

#include <bus.h>
#include <offsets.h>


void incoming_event(struct bus_descriptor* bus, char* data, size_t len)
{
	__builtin_nop();
    struct bus_event_hdr* evhdr = get_bus_event_header(data);
	struct bus_set_motor_driver* setmotor = get_bus_set_motor_driver(data);
	

	uint16_t testint;
	memcpy(&testint, &device.motor[1].throttle, 2);
	__builtin_nop();
    switch(evhdr->type) 
	{
		case EV_SET_THROTTLES:
		{	
			device.motor[0].throttle = abs(setmotor->motors[0]);
			if(setmotor->motors[0] > 0)
				device.motor[0].flags = MOT_FORWARD;
			else if(setmotor->motors[0] < 0)
				device.motor[0].flags = MOT_BACKWARD;
			else
				device.motor[0].flags = MOT_BRAKE;

			motor_set_power(1,setmotor->motors[0]/10);

				device.motor[1].throttle = abs(setmotor->motors[1]);
			if(setmotor->motors[1] > 0)
				device.motor[1].flags = MOT_FORWARD;
			else if(setmotor->motors[1] < 0)
				device.motor[1].flags = MOT_BACKWARD;
			else
				device.motor[1].flags = MOT_BRAKE;


			motor_set_power(2,setmotor->motors[1]/10);
		}
	}	
}

int16_t main(void)
{
        /* Configure the oscillator for the device */
        ConfigureOscillator();

		clock_start();
        
        bus_init(1, DT_DUAL_MOTOR);

        init_motors();


        while(1)
        {
		
		//	while(U1STAbits.UTXBF);
		//	U1TXREG = 'U';
              update_motors();
              read_sensors();      
              bus_do_work();
        }
}

//
//  TemperatureDecoupler.h
//  SmartCitizen
//
//  Created by Oriol Ferrer Mesià on 03/06/13.
//
//

#ifndef SmartCitizen_TemperatureDecoupler_h
#define SmartCitizen_TemperatureDecoupler_h

#define BATTERY_HEATUP_MAX	11

#include <Arduino.h>
#include "GhettoFilter.h"


class TemperatureDecoupler{

  public:
  
	void setup(){
		_prevBattery = 0;
		filter.setup(0.3);
	}

	void update( uint16_t battery ){

		Serial.println( "# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #" );

		bool charging = false;
		bool doNothing = false;
		if (battery == _prevBattery){
			//doNothing;
			if ( battery > 980 ){
				charging = true;
				Serial.println( "Battery same val > 980! charging!");
			}else{
				charging = false;
				Serial.println( "Battery same val < 980! NOT charging!");
			}
		}else{
			if ( battery > _prevBattery || battery > 980 ){ //battery is charging!
				Serial.println( "Battery charging!");
				charging = true;
			}else{ //battery is being drained
				Serial.println( "Battery dis-charging!");
				charging = false;
			}
		}

		if (!doNothing){
			if(charging)
				filter.goUp();
			else
				filter.goDown();
		}

		Serial.print(F("battery: ")); Serial.println( battery );
		Serial.print(F("_prevBattery: ")); Serial.println( _prevBattery );
		Serial.print(F("filter: ")); Serial.println( filter.getVal() );

		Serial.println( "# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #" );
		//store last bat reading for future comparing
		_prevBattery = battery;
		lastChargingState = charging;
	}

	short int getCompensation(){
		return (short int) (filter.getVal() * BATTERY_HEATUP_MAX);
	}

	short int _prevBattery;
	GhettoFilter filter;
	bool lastChargingState; //true == up, false == down

};


#endif

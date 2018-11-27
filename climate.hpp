// Developer note stack size needs to be increased to allow the allocation of memory for this double = 8 bytes
// 8 * 24 * 60 * 60 * 2 arrays of doubles = 1382400 bytes base allocation is 1MB 
// In VS2017 go to project > properties > System > and set Stack Reserve Size and Stack Commit Size to 2000000
// This prevents a stack overflow

#define maximum_readings 24 * 60 * 60
#define invalid_reading  -1000.0


#define fakedata 1	 // NOTE:  Set to 1 use fake sensor values instead of the online values 0

#include <chrono>  // System time
#include <stdexcept>

#if fakedata == 1

#include "fakesensor.hpp"

#else

#include "http.h"

#endif


using namespace std;
using namespace chrono;


class Climate {

private:

#if fakedata == 1	
	FakeSensor sensorDevice;
#else
	Http sensorDevice;
#endif

protected:
		int temp, hum;


public:

	// Constructors
	Climate(double temp_, double hum_);


	// Utility
	void	clearSamples();

	// Sensor related
	long	readSensor();

};

// Constructor
Climate::Climate(double temp_, double hum_) :temp(temp_), hum(hum_) {
	
	vector <Climate> v;
	
	v.push_back({ 2,3 });
	//cout << v[0] << " ";
	
	
	

}


void	Climate::clearSamples() {
}


long	Climate::readSensor() {

	int currentSecond = 0;

	sensorDevice.read_data();

	// This line is purely for your debugging and can be removes/commented out in the final code.
	cout << endl << "Debugging information : "  << "Temperature is " << sensorDevice.get_temperature_in_c() << " in degrees C " << sensorDevice.get_humidity() << "% humidity" << endl;
	
	return currentSecond;

}

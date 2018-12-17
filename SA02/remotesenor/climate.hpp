// Developer note stack size needs to be increased to allow the allocation of memory for this double = 8 bytes
// 8 * 24 * 60 * 60 * 2 arrays of doubles = 1382400 bytes base allocation is 1MB 
// In VS2017 go to project > properties > System > and set Stack Reserve Size and Stack Commit Size to 2000000
// This prevents a stack overflow

#define maximum_readings 24 * 60 * 60
#define invalid_reading  -1000.0



#define fakedata 1	 // NOTE:  Set to 1 use fake sensor values instead of the online values 0

#include <chrono>  // System time
#include <stdexcept>
#include <vector>;
#include <iostream>

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

	//sampleCount method
	int samples, totalSamples;

	//declare temperature and humidity arrays
	float temp[maximum_readings];
	float hum[maximum_readings];

public:

	

	// Constructors
	Climate();

	// Utility
	void	clearSamples();

	// Sensor related
	long	readSensor();
	long sampleCount(long secs);
	long sampleTotal();

	//calculations
	double getHumidity(long sec);
	double minimumHumidity(long lookBack);
	double maximumHumidity(long lookBack);
	double averageHumidity(long lookBack);
	double getTemperature(long sec);
	double minimumTemperature(long lookBack);
	double maximumTemperature(long lookBack);
	double averageTemperature(long lookBack);

	system_clock::time_point StartTime;
};

// Constructor
Climate::Climate() {

	//set start time to the current
	StartTime = std::chrono::system_clock::now();


}

long Climate::sampleCount(long secs) {

	for (int v = (int)secs; v >= 1; v--) {
		if (temp[v] != NULL) {
			samples++;
		}
		if (hum[v] != NULL) {
			samples++;
		}
	}

	return samples;
}



long Climate::sampleTotal() {
	for (auto it = begin(temp); it != end(temp); ++it) {
		totalSamples++;
	}
	return totalSamples;
}


double Climate::getHumidity(long sec)
{
	float value = hum[sec];
	return value;
}


//minimum
double Climate::minimumHumidity(long lookBack)
{
	double minimum = hum[lookBack];

	for (int i = lookBack; i >= 1; i--)
	{
		if (hum[i] == NULL)
		{
			throw invalid_argument("hum vector Null");

		}

		if (hum[i] < minimum)
		{
			minimum = hum[i];

		}


	}

	return minimum;
}

//maximum

double Climate::maximumHumidity(long lookBack)
{
	double maximum = hum[lookBack];

	for (int i = lookBack; i >= 1; i--)
	{
		if (hum[i] == NULL)
		{
			throw invalid_argument("hum vector Null");

		}

		if (hum[i] > maximum)
		{
			maximum = hum[i];

		}
		

	}

	return maximum;
}



//mean of values



double Climate::averageHumidity(long lookBack)
{
	double sum = 0;
	double avg;


	for (int i = lookBack; i >=1; i--)
	{
		if (hum[i] == NULL)
		{
			throw invalid_argument("hum vector Null");

		}

		sum += hum[i];

	}

	avg = sum / lookBack;
	return avg;
}


double Climate::getTemperature(long sec)
{
	float value = temp[sec];
	return value;
}

//minimum

double Climate::minimumTemperature(long lookBack)
{
	double minimum =  temp[lookBack];

	for (int i = lookBack; i >= 1; i--)
	{
		if (temp[i] == NULL)
		{
			throw invalid_argument("temp vector Null");

		}

		if (temp[i] < minimum)
		{
			minimum = temp[i];

		}


	}

	return minimum;
}

//maximum

double Climate::maximumTemperature(long lookBack)
{
	double maximum = temp[lookBack];

	for (int i = lookBack; i >= 1; i--)
	{
		if (temp[i] == NULL)
		{
			throw invalid_argument("temp vector Null");

		}

		if (temp[i] > maximum)
		{
			maximum = temp[i];

		}


	}

	return maximum;
}



//mean of values



double Climate::averageTemperature(long lookBack)
{
	double sum = 0;
	double avg;


	for (int i = lookBack; i >= 1; i--)
	{
		if (temp[i] == NULL)
		{
			throw invalid_argument("temp vector Null");

		}

		sum += temp[i];

	}

	avg = sum / lookBack;
	return avg;
}


void	Climate::clearSamples() {
	for (int i = 0; i < maximum_readings; i++)
	{
		temp[i] = 0;
		hum[i] = 0;
	}

}


long	Climate::readSensor() {

	system_clock::time_point EndTime = std::chrono::system_clock::now();
	sensorDevice.read_data();

	//calculate time passed since launch of app
	std::chrono::duration<double> currentSecond = EndTime - StartTime;

	
	
	sensorDevice.read_data();

	//store data in vectors
	hum[(long)currentSecond.count()] = sensorDevice.get_humidity();
	temp[(long)currentSecond.count()] = sensorDevice.get_temperature_in_c();

	

	// This line is purely for your debugging and can be removes/commented out in the final code.
	//cout << endl << "Debugging information : "  << "Temperature is " << sensorDevice.get_temperature_in_c() << " in degrees C " << sensorDevice.get_humidity() << "% humidity" << endl;
	
	cout << " Element at index: " << (long)currentSecond.count() << " Temp: "<<temp[(long)currentSecond.count()]<< endl;
	cout << " Element at index: " << (long)currentSecond.count() << " Hum: " << hum[(long)currentSecond.count()] << endl;
	
	return (long)currentSecond.count();

}
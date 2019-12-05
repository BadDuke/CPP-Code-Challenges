#define _CRT_SECURE_NO_WARNINGS // VS no deprecation

#include "DataTypes.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <map>

using namespace std;

// Because pieces of string are not null terminated, we need to figure out how to slice null terminated substring and convert to int
int extractIntFromAscii(char * asciiPtr, int numCharsToExtract)
{
	char * buffer = new char[numCharsToExtract + 1]; // Add one for null termination

	strncpy(buffer, asciiPtr, numCharsToExtract);
	buffer[numCharsToExtract] = 0; // null terminate

	int returnValue = atoi(buffer); // ASCII to int
	delete[] buffer;
	return returnValue;
}

bool isTmax(char * asciiPtr) // Checks whether the current line we're reading is TMAX
{
	char buffer[5] = {};
	strncpy(buffer, asciiPtr, sizeof(buffer));
	buffer[sizeof(buffer) - 1] = 0; // null terminate

	return (strcmp(buffer, "TMAX") == 0); // '0' in this case represents that they're equal, thus true
}

bool isTmin(char * asciiPtr)
{
	char buffer[5] = {};
	strncpy(buffer, asciiPtr, sizeof(buffer));
	buffer[sizeof(buffer) - 1] = 0; // null terminate

	return (strcmp(buffer, "TMIN") == 0); // '0' in this case represents that they're equal, thus true
}

int extractTempValue(char * asciiPtr)
{
	char buffer[6] = {}; // maximum size of temperature data is 5 and we add one for the null terminator
	strncpy(buffer, asciiPtr, sizeof(buffer));
	buffer[sizeof(buffer) - 1] = 0; // null terminate

	return atoi(buffer); // ASCII to int
}

int main()
{
	std::map<int, YearData> years; // use map because years are unique and map is a unique container
	std::string line;
	std::ifstream myfile("state11_IL.txt");

	if (myfile.is_open())
	{
		int lineNum = 0;
		std::cout << "Processing raw data & populating local data containers" << endl;

		while (std::getline(myfile, line)) // reading until end of file
		{
			char charString[1024] = {};
			strncpy(charString, line.c_str(), sizeof(charString)); // copy the string into a non-const buffer (line.c_str() is const)
			charString[sizeof(charString) - 1] = 0; // null terminate

			int year = extractIntFromAscii(charString + 6, 4);

			YearData * yearData = &years[year];
			yearData->initialize(year);	// Initialize the year (if not initialized already)

			int month = extractIntFromAscii(charString + 10, 2);
			MonthData * monthData = yearData->getMonth(month - 1);	// month is 1-indexed, so offset monthby 1

			if (isTmax(charString + 12))
			{
				int day = 0;
				for (int dayIndex = 16; dayIndex <= 256; dayIndex += 8) // based on the data format, extract temperature values
				{
					int tmax = extractTempValue(charString + dayIndex);

					monthData->getDay(day)->addTmax(tmax);
					day++;
				}
			}
			else if (isTmin(charString + 12))
			{
				int day = 0;
				for (int dayIndex = 16; dayIndex <= 256; dayIndex += 8) // based on the data format, extract temperature values
				{
					int tmin = extractTempValue(charString + dayIndex);

					monthData->getDay(day)->addTmin(tmin);
					day++;
				}
			}
		}
		myfile.close();

		cout << "Raw data processing done. Now analyzing..." << endl;

		int daysAbove100 = 0;
		int daysBelow0 = 0;
		int recordHigh = 0;
		int recordLow = 0;

		// Go through each year and month and calculate the average temp
		map<int, YearData>::iterator it = years.begin();
		for (it; it != years.end(); it++)
		{
			YearData * yearData = &(it->second);	// it->second accesses the yearData

			if (!yearData || !yearData->isInitialized())
			{
				continue;
			}

			for (int monthIndex = 0; monthIndex < MONTHS_IN_YEAR; monthIndex++)
			{
				MonthData * monthData = yearData->getMonth(monthIndex);
				
				int validDaysInMonth = 0;
				double monthlyTemperatureSum = 0.0;	// Since we are getting averages, we will cumulate all maxes and mins here.

				for (int dayIndex = 0; dayIndex < MAX_DAYS_IN_MONTH; dayIndex++)
				{
					DayData * dayData = monthData->getDay(dayIndex);
					if (dayData->dayHasData())
					{
						validDaysInMonth++;
						if (dayData->isDayAbove100()) {
							daysAbove100++;
						}
						if (dayData->getMaxTemp() > recordHigh) {
							recordHigh = dayData->getMaxTemp();
						}
						if (dayData->isDayBelow0()) {
							daysBelow0++;
						}
						if (dayData->getMinTemp() < recordLow) {
							recordLow = dayData->getMinTemp();
						}
						// Add the avg temp to the sum
						 monthlyTemperatureSum += dayData->getAvgTemp();
					}
				}
			
				if (validDaysInMonth != 0) {
					cout << "Average temperature for year " << it->first			// it->first is the key, the actual year
						<< " month " << monthIndex + 1 << " is: " << monthlyTemperatureSum / validDaysInMonth << endl;
				}
			}
		}
		std::cout << "Days Above 100: " << daysAbove100 << endl << "Days Below 0: " << "\t" << daysBelow0 << endl;
		std::cout << "Record High: " << "\t" << recordHigh << endl << "Record Low: " << "\t" << recordLow << endl;
	}
	else
	{
		std::cout << "Unable to open file";
	}

	cin.get();

	return 0;
}
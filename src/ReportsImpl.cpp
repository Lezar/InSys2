#include "ReportsImpl.h"

// converts a String of the form YYYY-MM-DD to time_t
// precondition: string is always of the form YYYY-MM-DD
// parameter[in]: date is a string of the form YYYY-MM-DD
// returns: a time_t object representing the date of the converted dateString
time_t ReportsImpl::convertStringToDate(string dateString) { 

	int delimiter1, delimiter2; // positions of the '-' in dateString

	// strings to store year mont and day of the dateString
	string year, month, day;

	// struct tm to hold year, month, and day
	struct tm date_tm = {0};

	// find positions of '-'
	delimiter1 = dateString.find('-');
	delimiter2 = dateString.find('-', delimiter1 + 1);

	// get year, month, and day
	year = dateString.substr(0, delimiter1);
	month = dateString.substr(delimiter1 + 1, delimiter2 - delimiter1 - 1);
	day = dateString.substr(delimiter2 + 1);

	// store year, month, and day into struct tm date_tm
	date_tm.tm_year = atoi(year.c_str()) - 1900; // tm_year counts from 1900
	date_tm.tm_mon = atoi(month.c_str()) - 1; // tm_mon is 0 to 11 for Jan to Dec
	date_tm.tm_mday = atoi(day.c_str()); // day of the month, ie. 1 to 31

	// convert date_tm to time_t and returned
	return mktime(&date_tm); 
}

string ReportsImpl::totalRevenueReport(string startDate, string endDate) { return NULL; }

ReportsImpl::~ReportsImpl() {};
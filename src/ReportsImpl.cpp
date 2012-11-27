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
// Finds all the sales/invoice/return id's between and including two dates
// precondition: The table is set in a way that the first column is the ID (primary key), 
//               the date is in the form YYYY-MM-DD, all other entries are integers    
// parameter[in]: table is a Table of the type to search in
// parameter[in]: startDate is a string of the form YYYY-MM-DD representing the start of the period
// parameter[in]: endDate is  string of the form YYYY-MM-DD representing the end of the period
// returns: a string vector where each element an ID whose date is within the range
vector<string> ReportsImpl::findIDsBetweenDates (Table table, string startDate, string endDate)
	throw (DoesNotExistException) { 

		vector<string> returnVector; // vector of strings to store the vector to be returned

		string searchResult; // string to find return of search
		string currentRow; // stores current row of searchResult
		string ID; // stores ID of the current row
		string date; // stores date of the current row

		time_t dateTime; // time_t conversion of date
		time_t startDateTime = convertStringToDate(startDate); // time_t conversion of startDate string
		time_t endDateTime = convertStringToDate(endDate); // time_t conversion of endDate string

		int delimiter; // store position of delimiters

		searchResult = table->search("all", "all"); // get all entries in the table

		// get the first row of the search result
		delimiter = searchResult.find('\n');
		currentRow = searchResult.substr(0, delimiter +1 );

		// break down searchResult line by line
		// storing each removed line into current row, until searchResult is empty
		// each iteration find the date and add ID to returnVector if date between start and end dates
		while (!searchResult.empty()){

			// find the date of the current row and convert to a time_t
			delimiter = currentRow.find('-');
			date = currentRow.substr(delimiter - 4, 10);
			dateTime = convertStringToDate(date);

			// if the date is between start and end dates, find the ID and add it to returnVector
			if (dateTime >= startDateTime && dateTime <= endDateTime)
			{
				delimiter = currentRow.find('|');
				ID = currentRow.substr(0, delimiter);
				returnVector.push_back(ID);
			}

			// break down the searchResult and find currentRow
			delimiter = searchResult.find('\n');
			searchResult = searchResult.substr(delimiter + 1);
			currentRow = searchResult.substr(0, delimiter + 1);
		}

		// throw DoesNotExistException if there are no entries within date range
		if (returnVector.size() == 0)
			throw DoesNotExistException("Nothing found between " + startDate + " and " + endDate);

		return returnVector; 
}

string ReportsImpl::totalRevenueReport(string startDate, string endDate) { return NULL; }

ReportsImpl::~ReportsImpl() {};
#ifndef REPORTSIMPL_H
#define REPORTSIMPL_H

#include "ReportsInterface.h"

/// \brief Implementation of ReportsInterface
///
/// Responsibilities include:
/// - Convert string to time_t object
/// - Reporting the total revenue and revenue earned by each catagory in the database
class ReportsImpl : public ReportsInterface {

public:

	/// \brief converts a String of the form YYYY-MM-DD to time_t
	///
	/// \pre string is always of the form YYYY-MM-DD
	/// \param[in] date is a string of the form YYYY-MM-DD
	/// \return a time_t object representing the date of the converted date string
	time_t convertStringToDate(string dateString);

	/// \brief finds all the sales/invoice/return id's between and including two dates
	///
	/// \pre The table is set in a way that the first column is the ID (primary key), 
	///      the date is in the form YYYY-MM-DD, and all other entries are integers  
	/// \param[in] table is a Table of the type to search in
	/// \param[in] startDate is a string of the form YYYY-MM-DD representing the start of the period
	/// \param[in] endDate is  string of the form YYYY-MM-DD representing the end of the period
	/// \throw DoesNotExistException if there are entries between those dates
	/// \return a string vector where each element an ID whose date is within the range
	vector<string> findIDsBetweenDates (Table table, string startDate, string endDate) throw (DoesNotExistException);

	string totalRevenueReport(string startDate, string endDate);

	string topSellersReport(string selectedCategory, string startDate, string endDate);

	~ReportsImpl();
};

#endif
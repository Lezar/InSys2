#ifndef REPORTSIMPL_H
#define REPORTSIMPL_H

#include "ReportsInterface.h"

/// \brief Implementation of ReportsInterface
///
/// Responsibilities include:
/// - Reporting the total revenue and revenue earned by each catagory in the database
class ReportsImpl : public ReportsInterface {

public:

	/// \brief converts a String of the form YYYY-MM-DD to time_t
	///
	/// \pre string is always of the form YYYY-MM-DD
	/// \param[in] date is a string of the form YYYY-MM-DD
	/// \return a time_t object representing the date of the converted date string
	time_t convertStringToDate(string dateString);


	string totalRevenueReport(string startDate, string endDate);

	~ReportsImpl();
};

#endif
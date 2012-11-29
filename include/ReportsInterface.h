#ifndef REPORTSINTERFACE_H
#define REPORTSINTERFACE_H

#include "TableInterface.h"
#include "Category.h"
#include "Invoice.h"
#include "InvoiceItem.h"
#include "Orders.h"
#include "Product.h"
#include "Receipt.h"
#include "Returns.h"
#include "Sales.h"
#include "SalesSummary.h"
#include "Summary.h"

#include <ctime>

/// \brief A class to create various reports on the Inventory Management database
///
/// Responsibilities include:
/// - Reporting the total revenue and revenue earned by each catagory in the database
class ReportsInterface {
public:

	/// \brief Reports the total revenue and revenues per category as between two dates
    ///
	/// Calculates the total revenue and the revenue generated by each category between and including 
	/// two dates, startDate and endDate
	/// \param[in] startDate is a string of form "YYYY-MM-DD" representing the beginning of the period
	/// \param[in] endDate is a string of form "YYYY-MM-DD" representing the end of the period
	/// \return A string representing the whole report. It will display all categories, names, and revenues,
	///         as well as total revenue.
	virtual string totalRevenueReport(string startDate, string endDate) = 0;

	/// \brief ReportsInterface destructor
	virtual ~ReportsInterface() {};
};

typedef ReportsInterface* Reports;

#endif
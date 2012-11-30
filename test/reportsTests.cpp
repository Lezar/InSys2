#include "stdafx.h"
#include "CppUnitTest.h"

#include "../InventoryManagement/ReportsImpl.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestInventory
{
	/// \brief Object of ReportsImpl to test implemented and helper functions
	ReportsImpl reports;

	/// \brief test class to test ReportsImpl implemented and helper functions
	TEST_CLASS(reportsTests)
	{
	public:

		/// \brief sets up each test method
		///
		/// - Initializes all necessary tables and fills with approriate data
		TEST_METHOD_INITIALIZE(SetUp)
		{
			// prepare invoice table file with testing date
			ofstream ofstr;
			ofstr.open("textfiles\\invoice.txt", ios_base::trunc); // clear Invoice table

			// Data for testing
			ofstr << "1|2011-08-01\n";
			ofstr << "2|2012-09-08\n";
			ofstr << "3|2012-09-15\n";
			ofstr << "4|2012-07-22\n";
			ofstr << "5|2012-09-15\n";
			ofstr << "6|2012-10-15\n";

			ofstr.close();

			// prepare returns table file with testing date
			ofstr.open("textfiles\\returns.txt", ios_base::trunc); // clear Returns table

			// Data for testing
			ofstr << "1|1|11|2012-08-30\n";
			ofstr << "2|2|12|2012-07-21\n";
			ofstr << "3|4|3|2012-10-31\n";
			ofstr << "5|7|10|2012-10-31\n";
			ofstr << "6|12|10|2012-11-15\n";
			ofstr << "9|32|10|2012-11-26\n";

			ofstr.close();

			// prepare receipt table file with testing date
			ofstr.open("textfiles\\receipt.txt", ios_base::trunc); // clear Receipt table

			// Data for testing
			ofstr << "1|2011-07-11\n";
			ofstr << "2|2012-08-15\n";
			ofstr << "3|2012-09-15\n";
			ofstr << "4|2012-07-22\n";
			ofstr << "5|2012-09-22\n";
			ofstr << "6|2012-11-23\n";

			ofstr.close();
		}

		/// \brief Test if reports can convert the string "2012-12-25" to appropriate time_t
		TEST_METHOD(TestReportsCanConvert2012_12_25ToDate)
		{
			Logger::WriteMessage("TestReportsCanConvert2012_12_25ToDate");

			string dateString = "2012-12-25"; // string of form YYYY-MM-DD

			// convert dateString to time_t using convertStringToDate
			time_t dateReturned = reports.convertStringToDate(dateString);

			// convert dateReturned to char* of form YYYY-MM-DD
			char dateReturnedString[15];
			strftime(dateReturnedString, 15, "%Y-%m-%d", localtime(&dateReturned));

			Logger::WriteMessage(dateReturnedString);

			// see if converted time_t object has proper year, month, and day
			// by comparing to string
			Assert::AreEqual("2012-12-25", dateReturnedString);
		}

		/// \brief Test if reports can convert the string "2012-12-25" to appropriate time_t
		TEST_METHOD(TestReportsCanConvert2012_07_22ToDate)
		{
			Logger::WriteMessage("TestReportsCanConvert2012_07_22ToDate");

			string dateString = "2012-07-22"; // string of form YYYY-MM-DD

			// convert dateString to time_t using convertStringToDate
			time_t dateReturned = reports.convertStringToDate(dateString);

			// convert dateReturned to char* of form YYYY-MM-DD
			char dateReturnedString[15];
			strftime(dateReturnedString, 15, "%Y-%m-%d", localtime(&dateReturned));

			Logger::WriteMessage(dateReturnedString);

			// see if converted time_t object has proper year, month, and day
			// by comparing to string
			Assert::AreEqual("2012-07-22", dateReturnedString);
		}

		/// \brief Test if reports can find single the invoice ID of a within a range of the same date
		TEST_METHOD(TestReportsFindSingleInvoiceIDSingleDate)
		{
			Logger::WriteMessage("TestReportsFindSingleInvoiceIDSingleDate");

			// find the invoice ID between and including 2012-07-22 and 2012-07-22
			vector<string> returnedVector = reports.findIDsBetweenDates(new Invoice(), "2012-07-22", "2012-07-22");

			vector<string>::iterator it; // iterator for returnedVector

			// Log all vector elements
			for (it = returnedVector.begin() ; it < returnedVector.end(); it++ )
				Logger::WriteMessage((*it + ',').c_str());

			// Check if all elements are returned and vector is proper size
			Assert::AreEqual(1, (int)returnedVector.size()); // check size
			Assert::AreEqual("4", returnedVector[0].c_str());
		}

		/// \brief Test if reports can find a single ReturnsID within a range between different dates
		TEST_METHOD(TestReportsFindSingleReturnsIDBetweenDates)
		{
			Logger::WriteMessage("TestReportsFindSingleReturnsIDBetweenDates");

			// find the single Returns ID between and including 2012-11-01 and 2012-11-20
			vector<string> returnedVector = reports.findIDsBetweenDates(new Returns(), "2012-11-01", "2012-11-20");

			vector<string>::iterator it; // iterator for returnedVector

			// Log all vector elements
			for (it = returnedVector.begin() ; it < returnedVector.end(); it++ )
				Logger::WriteMessage((*it + ',').c_str());

			// Check if all elements are returned and vector is proper size
			Assert::AreEqual(1, (int)returnedVector.size()); // check size
			Assert::AreEqual("6", returnedVector[0].c_str());
		}

		/// \brief Test if reports can find multiple Receipt IDs between Dates
		TEST_METHOD(TestReportsFindMultipleReceiptIDBetweenDates)
		{
			Logger::WriteMessage("TestReportsFindMultipleReceiptIDBetweenDates");

			// find all Receipt ID's between and including 2009-01-01 and 2015-12-12
			vector<string> returnedVector = reports.findIDsBetweenDates(new Receipt(), "2009-01-01", "2015-12-12");

			vector<string>::iterator it; // iterator for returnedVector

			// Log all vector elements
			for ( it = returnedVector.begin() ; it < returnedVector.end(); it++ )
				Logger::WriteMessage((*it + ',').c_str());

			// Check if all elements are returned and vector is proper size
			Assert::AreEqual(6, (int)returnedVector.size()); // check size
			Assert::AreEqual("1", returnedVector[0].c_str());
			Assert::AreEqual("2", returnedVector[1].c_str());
			Assert::AreEqual("3", returnedVector[2].c_str());
			Assert::AreEqual("4", returnedVector[3].c_str());
			Assert::AreEqual("5", returnedVector[4].c_str());
			Assert::AreEqual("6", returnedVector[5].c_str());
		}

		/// \brief Test if reports throws a DoesNotExistException if nothing exists in the date range
		TEST_METHOD(TestReportsThrowsDoesNotExistException)
		{
			Logger::WriteMessage("TestReportsThrowsDoesNotExistException");

			// try finding within date range with no existing entries
			try {
				vector<string> returnedVector = 
					reports.findIDsBetweenDates(new Invoice(), "2040-01-01", "2040-12-12");
				Logger::WriteMessage("No exception thrown");
				Assert::Fail(); // fail if nothing is thrown
			}
			catch (DoesNotExistException e) { // pass test and log error if DoesNotExistException is thrown
				Logger::WriteMessage(e.what()); 
			}
			catch (...) { // fail test if other type of exception is thrown
				Logger::WriteMessage("Wrong type of exception");
				Assert::Fail();
			}
		}
	};
}
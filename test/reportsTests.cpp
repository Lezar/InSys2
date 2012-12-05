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
			ofstr << "2|2012-07-22\n";
			ofstr << "3|2012-09-15\n";
			ofstr << "4|2012-07-22\n";
			ofstr << "6|2012-09-22\n";
			ofstr << "7|2012-11-23\n";

			ofstr.close();

			// prepare salessummary table file with testing date
			ofstr.open("textfiles\\SalesSummary.txt", ios_base::trunc); // clear SalesSummary table

			// Data for testing
			ofstr << "1|1\n";
			ofstr << "1|2\n";
			ofstr << "1|3\n";
			ofstr << "2|5\n";
			ofstr << "2|6\n";
			ofstr << "3|8\n";
			ofstr << "4|9\n";
			ofstr << "6|10\n";
			ofstr << "6|11\n";
			ofstr << "7|12\n";
			ofstr << "2|13\n";

			ofstr.close();

			// prepare Sales table file with testing date
			ofstr.open("textfiles\\Sales.txt", ios_base::trunc); // clear Sales table

			// Data for testing
			ofstr << "1|3|11100|10\n";
			ofstr << "2|4|22200|0\n";
			ofstr << "3|5|33300|0\n";
			ofstr << "5|1|22200|5\n";
			ofstr << "6|20|33300|33\n";
			ofstr << "8|10|44400|0\n";
			ofstr << "9|1|55500|0\n";
			ofstr << "10|1|22200|0\n";
			ofstr << "11|27|44400|15\n";
			ofstr << "12|10|33300|10\n";
			ofstr << "13|10|11100|30\n";

			ofstr.close();

			// prepare Product table file with testing date
			ofstr.open("textfiles\\product.txt", ios_base::trunc); // clear Product table

			// Data for testing
			ofstr << "11100|1|10 Pack Red Delicious|RedDel10Pk|4.00\n";
			ofstr << "22200|1|20 Pack Navel Orange|Orange20Pk|5.99\n";
			ofstr << "33300|3|Clover Tree 100g Flaked Tuna|ClTuna100g|1.29\n";
			ofstr << "44400|4|Delicioso Frozen Pepperoni Pizza|DelPeppPizz|7.99\n";
			ofstr << "55500|4|McCairn Frozen Fries 1kg|McFries1k|6.99\n";

			ofstr.close();

			// prepare Category table file with testing date
			ofstr.open("textfiles\\category.txt", ios_base::trunc); // clear Category table

			// Data for testing
			ofstr << "1|Sweet and tangy|Fruits\n";
			ofstr << "3|Preserved in a metal container|Canned food\n";
			ofstr << "4|Cold and hard|Frozen food\n";
		}

		/// \brief Test if convertStringToDate can convert the string "2012-12-25" to appropriate time_t
		TEST_METHOD(TestReportsCanConvertString2012_12_25ToDate)
		{
			Logger::WriteMessage("TestReportsCanConvertString2012_12_25ToDate");

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
		TEST_METHOD(TestReportsCanConvertString2012_07_22ToDate)
		{
			Logger::WriteMessage("TestReportsCanConvertString2012_07_22ToDate");

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

		/// \brief Test if findIDsBetweenDates can find single the invoice ID of a within a range of the same date
		TEST_METHOD(TestReportsFindSingleInvoiceIDSingleDate)
		{
			Logger::WriteMessage("TestReportsFindSingleInvoiceIDSingleDate");

			// find the invoice ID between and including 2012-07-22 and 2012-07-22
			Table invoice = new Invoice();
			vector<string> returnedVector = reports.findIDsBetweenDates(invoice, "2012-07-22", "2012-07-22");
			delete invoice;

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
			Table returns = new Returns();
			vector<string> returnedVector = reports.findIDsBetweenDates(returns, "2012-11-01", "2012-11-20");
			delete returns;

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
			Table receipt = new Receipt();
			vector<string> returnedVector = reports.findIDsBetweenDates(receipt, "2009-01-01", "2015-12-12");
			delete receipt;

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
			Assert::AreEqual("6", returnedVector[4].c_str());
			Assert::AreEqual("7", returnedVector[5].c_str());
		}

		/// \brief Test if findIDsBetweenDates throws a DoesNotExistException if nothing exists in the date range
		TEST_METHOD(TestReportsFindIDsBtwDatesThrowsDoesNotExistException)
		{
			Logger::WriteMessage("TestReportsFindIDsBtwDatesThrowsDoesNotExistException");

			Table invoice = new Invoice();

			// try finding within date range with no existing entries
			try {
				
				vector<string> returnedVector = 
					reports.findIDsBetweenDates(invoice, "2040-01-01", "2040-12-12");
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

			delete invoice;
		}

		/// \brief Test if totalRevenueReport can report total revenue of only one sale with no discount
		TEST_METHOD(TestReportsTotalRevenueOneSaleNoDiscount)
		{
			Logger::WriteMessage("TestReportsTotalRevenueOneSaleNoDiscount");

			// receipt on 2012-09-15 has only one sale with no discount
			string totalRevenueReturned = reports.totalRevenueReport("2012-09-15", "2012-09-15");

			Logger::WriteMessage(totalRevenueReturned.c_str());

			// check if revenue report is properly calculated and formatted
			Assert::AreEqual("Fruits (ID:1) ----- $0.00\n"
							"Canned food (ID:3) ----- $0.00\n"
							"Frozen food (ID:4) ----- $79.90\n"
							"TOTAL REVENUE ----- $79.90\n", totalRevenueReturned.c_str());
		}

		/// \brief Test if totalRevenueReport can report total revenue of only one Sale with discount
		TEST_METHOD(TestReportsTotalRevenueOneSaleWithDiscount)
		{
			Logger::WriteMessage("TestReportsTotalRevenueOneSaleWithDiscount");

			// receipt on 2012-11-23 has only one sale but with a discount
			string totalRevenueReturned = reports.totalRevenueReport("2012-11-23", "2012-11-23");

			Logger::WriteMessage(totalRevenueReturned.c_str());

			// check if revenue report is properly calculated and formatted
			Assert::AreEqual("Fruits (ID:1) ----- $0.00\n"
							"Canned food (ID:3) ----- $11.61\n"
							"Frozen food (ID:4) ----- $0.00\n"
							"TOTAL REVENUE ----- $11.61\n", totalRevenueReturned.c_str());
		}

		/// \brief Test if totalRevenueReport can report total revenue of multiple sales on one receipt
		TEST_METHOD(TestReportsTotalRevenueOneReceiptMultipleSales)
		{

		Logger::WriteMessage("TestReportsTotalRevenueOneReceiptMultiSales");

			// receipt only on 2012-09-22 has multiple sales on one receipt
			string totalRevenueReturned = reports.totalRevenueReport("2012-09-21", "2012-09-23");

			Logger::WriteMessage(totalRevenueReturned.c_str());

			// check if revenue report is properly calculated and formatted
			Assert::AreEqual("Fruits (ID:1) ----- $5.99\n"
							"Canned food (ID:3) ----- $0.00\n"
							"Frozen food (ID:4) ----- $183.37\n"
							"TOTAL REVENUE ----- $189.36\n", totalRevenueReturned.c_str());
		}

		/// \brief Test if totalRevenueReport can report total revenue of multiple sales on multiple receipts
		TEST_METHOD(TestReportsTotalRevenueMultiReceiptsMultiSales)
		{

		Logger::WriteMessage("TestReportsTotalRevenueMultiReceiptsMultiales");

			// 2012-07-22 to 2012-09-22 contains multiple receipts
			string totalRevenueReturned = reports.totalRevenueReport("2012-07-22", "2012-09-22");

			Logger::WriteMessage(totalRevenueReturned.c_str());

			// check if revenue report is properly calculated and formatted
			Assert::AreEqual("Fruits (ID:1) ----- $39.68\n"
							"Canned food (ID:3) ----- $17.29\n"
							"Frozen food (ID:4) ----- $270.26\n"
							"TOTAL REVENUE ----- $327.23\n", totalRevenueReturned.c_str());
		}

		/// \brief Test if totalRevenuereport throws a DoesNotExistException if nothing exists in the date range 
		TEST_METHOD(TestReportsTotalRevenueThrowsDoesNotExistException)
		{
			Logger::WriteMessage("TestReportsTotalRevenueThrowsDoesNotExistException");

			// try finding within date range with no existing entries
			try {
				string returnedString = reports.totalRevenueReport("2040-01-01", "2040-12-12");
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
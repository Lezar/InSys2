#include "stdafx.h"
#include "CppUnitTest.h"

#include "../InventoryManagement/ReportsImpl.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestInventory
{
	ReportsImpl reports;

	/// \brief test class to test Reports
	TEST_CLASS(reportsTests)
	{
	public:

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
	

	};
}
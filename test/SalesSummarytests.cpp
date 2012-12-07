#include "stdafx.h"
#include "CppUnitTest.h"
#include "SalesSummary.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace TestInventory
{		
	/// \brief Pointer to TableInterface 
	Table ss;

	/// \brief Unit test class for testing Sales Summary class
	TEST_CLASS(SalesSummarytests)
	{

	public:

		/// \brief Setup before the Test class
		TEST_METHOD_INITIALIZE(setup)
		{
			Logger::WriteMessage("TEST_CLASS setup");

			ss = new SalesSummary();

			ofstream ofstr;
			ofstr.open("textFiles/SalesSummary.txt", ios_base::trunc); // clear Category table

			// Data for testing
			ofstr << "1|001\n";
			ofstr << "2|002\n";
			ofstr << "3|003\n";

			ofstr.close();
		}

		/// \brief Clean up function after test class is run
		TEST_METHOD_CLEANUP(teardown)
		{
			Logger::WriteMessage("TEST_CLASS cleanup");

			delete ss;
		}

		/// \brief Test if the SalesSummary class can search by receipt ID 
		TEST_METHOD(TestSalesSummarySearchByReceiptID)
		{
			Logger::WriteMessage("TestSalesSummarySearchByReceiptID");

			string ssReturned;

			ssReturned = ss->search("receiptID", "2");

			const char *p;
			p = ssReturned.c_str();
			Logger::WriteMessage(p);

			Assert::AreEqual("2|002\n", ssReturned.c_str());
		}

		
		/// \brief Test if the SalesSummary class can search by sales ID 
		TEST_METHOD(TestSalesSummarySearchBySalesID)
		{
			Logger::WriteMessage("TestSalesSummarySearchBySalesID");

			string ssReturned;

			ssReturned = ss->search("salesID", "003");

			const char *p;
			p = ssReturned.c_str();
			Logger::WriteMessage(p);

			Assert::AreEqual("3|003\n", ssReturned.c_str());

		}

		
		/// \brief Test if the SalesSummary class can add new sales id and receipt id
		TEST_METHOD(TestSalesSummaryAdd)
		{
			Logger::WriteMessage("TestSalesSummaryAdd");

			string ssReturned;

			string ssExpected = "4|004\n";

			vector<string> ssVector;
			
			ssVector.push_back("4");
			ssVector.push_back("004");

			ss->add(ssVector);

			ssReturned = ss->search("receiptID", "4");

			const char *p;
			p = ssReturned.c_str();
			Logger::WriteMessage(p);

			Assert::AreEqual(ssExpected, ssReturned);
		}

		/// \brief tests if the SalesSummary class can search and return the whole table
		TEST_METHOD(TestSalesSummarySearchAll)
		{
			Logger::WriteMessage("TestSalesSummarySearchAll");

			string returned = ss->search("all", "all"); // search all

			Logger::WriteMessage(returned.c_str());

			// see if whole table is returned
			Assert::AreEqual("1|001\n"
							"2|002\n"
							"3|003\n", returned.c_str());
		}
};
}
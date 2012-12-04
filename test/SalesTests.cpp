#include "stdafx.h"
#include "CppUnitTest.h"
#include "../InventoryManagement/Sales.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace TestInventory
{	

	/// \brief Pointer to TableInterface class to be used for interacting with the Sales table	
	Table sale;

	
	/// \brief Unit test class for testing sales class
	TEST_CLASS(salesTests)
	{

	public:
		
		/// \brief sets up each test method
		///
		/// - Initializes sales variable to Sales class
		/// - Clears the sales.txt table and inputs data to test
		TEST_METHOD_INITIALIZE(setup)
		{
			Logger::WriteMessage("TEST_CLASS setup");

			sale = new Sales();

			ofstream ofstr;
			ofstr.open("textFiles/sales.txt", ios_base::trunc); // clear Sales table

			// Data for testing
			ofstr << "1|10|5|1\n";
			ofstr << "2|5|2|2\n";
			ofstr << "3|80|20|3\n";
			ofstr << "4|2|2|4\n";

			ofstr.close();
		}

		
		/// \brief clean up after every test method
		TEST_METHOD_CLEANUP(teardown)
		{
			Logger::WriteMessage("TEST_CLASS cleanup");

			delete sale;
		}

		/// \brief tests if the sales class can search a table by sales id
		
		TEST_METHOD(TestSalesSearchBySalesID)
		{
			// Log test name
			Logger::WriteMessage("TestSalesSearchBySalesID");

			string salesReturned;
			
			// String to store value returned by Sales::search
			salesReturned = sale->search("sales_id", "1");

			const char *p;
			// Logs returned values
			p = salesReturned.c_str();

			Logger::WriteMessage(p);
			
			// Asserts what is expected with what is actually returned
			Assert::AreEqual("1|10|5|1\n", salesReturned.c_str());
		}

		
		/// \brief tests if the Sales class can search a table by quantity sold
		TEST_METHOD(TestSalesSearchByQuantitySold)
		{
			Logger::WriteMessage("TestSalesSearchByQuantitySold");

			string salesReturned;

			salesReturned = sale->search("quantity_sold", "10");

			const char *p;
			p = salesReturned.c_str();
			Logger::WriteMessage(p);

			Assert::AreEqual("1|10|5|1\n", salesReturned.c_str());

		}

		/// \brief tests if the Sales class can search a table by discount
		
		TEST_METHOD(TestSalesSearchByDiscount)
		{
			Logger::WriteMessage("TestSalesSearchByDiscount");

			string salesReturned;

			salesReturned = sale->search("discount", "2");

			const char *p;
			p = salesReturned.c_str();
			Logger::WriteMessage(p);
			
			Assert::AreEqual("2|5|2|2\n", salesReturned.c_str());

		}

		
		/// \brief tests if the Sales class can search a table by product id
		TEST_METHOD(TestSalesSearchByProductID)
		{
			Logger::WriteMessage("TestSalesSearchByProductID");

			string salesReturned;

			salesReturned = sale->search("productID", "2");

			const char *p;
			p = salesReturned.c_str();
			Logger::WriteMessage(p);

			Assert::AreEqual("2|5|2|2\n" "4|2|2|4\n", salesReturned.c_str());

		}

		/// \brief tests if the sales class can add a row to the sales table
		
		TEST_METHOD(TestSalesAdd)
		{
			Logger::WriteMessage("TestSalesAdd");

			string salesReturned;

			string salesExpected = "5|2|5|80\n";

			vector<string> saleVector;
			saleVector.push_back("2");
			saleVector.push_back("5");
			saleVector.push_back("80");
			sale->add(saleVector);

			salesReturned = sale->search("sales_id", "5");

			const char *p;
			p = salesReturned.c_str();
			Logger::WriteMessage(p);

			Assert::AreEqual(salesExpected, salesReturned);
		}

		/// \brief tests if the sales class can modify a row in the sales table
		
		TEST_METHOD(TestSalesModify)
		{
			Logger::WriteMessage("TestSalesModify");

			string returnedString;

			string expectedString = "1|10|6|1\n";

			sale->modifyRow("1","productID","6");

			returnedString = sale->search("sales_id", "1");

			const char *p;
			p = returnedString.c_str();
			Logger::WriteMessage(p);

			Assert::AreEqual(expectedString,returnedString);
		}


		/// \brief tests if the sales class can delete a row in the sales table
		
		TEST_METHOD(TestSalesDelete)
		{
			Logger::WriteMessage("TestSalesDelete");

			string returnedString;

			string expectedString = "Sales Does Not Exist";

			sale->deleteRow("3");

			try{
				// call the sales search function to ensure that the sales was successfully deleted
				returnedString = sale->search("sales_id", "3");
				Assert::Fail(L"No exception for input",LINE_INFO());
			} catch(DoesNotExistException e) {
				
			} catch(...)
			{
				Assert::Fail(L"Wrong Exception Caught", LINE_INFO());
			}
					
		}
		
		/// \brief tests if the Sales class can search and return the whole table
		TEST_METHOD(TestSalesSearchAll)
		{
			Logger::WriteMessage("TestSalesSearchAll");

			string returned = sale->search("all", "all"); // search all

			Logger::WriteMessage(returned.c_str());

			// see if whole table is returned
			Assert::AreEqual("1|10|5|1\n"
							"2|5|2|2\n"
							"3|80|20|3\n"
							"4|2|2|4\n", returned.c_str());
		}
	};
}
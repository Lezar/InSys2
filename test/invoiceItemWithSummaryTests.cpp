#include "stdafx.h"
#include "CppUnitTest.h"

#include "InvoiceItem.h"
#include "Summary.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestInventory
{
	/// \brief TableInterface pointers to interact Summary and InvoiceItem tables
	Table invoiceItem2, summary2;

	/// \brief test class for the integration of InvoiceItem with Summary
	TEST_CLASS(invoiceItemWithSummaryTests)
	{
	public:

		/// \brief Setup before each Test Method
		///
		/// - Initializes invoiceItem and summary variables
		/// - Clears and sets up invoiceItem.txt and summary.txt tables for testing
		TEST_METHOD_INITIALIZE(SetUp)
		{
			invoiceItem2 = new InvoiceItem();
			summary2 = new Summary();

			ofstream ofstr;
			ofstr.open("textfiles\\invoiceItem.txt", ios_base::trunc); // clear InvoiceItem table

			// Data for testing invoice item
			ofstr << "1010|1425|20\n";
			ofstr << "1011|2543|10\n";
			ofstr << "1012|1002|50\n";
			ofstr << "1013|1003|10\n";

			ofstr.close();

			ofstr.open("textfiles\\summary.txt", ios_base::trunc); // clear Summary table

			// Set up summary data for testing
			ofstr << "1425|100\n";
			ofstr << "2543|200\n";
			ofstr << "1002|100\n";
			ofstr << "1003|160\n";

			ofstr.close();
		}

		/// \brief clean up after each test method
		TEST_METHOD_CLEANUP(TearDown)
		{
			delete invoiceItem2;
			delete summary2;
		}

		/// \brief Test that adding a new invoice_item with a non-existent product adds it to summary
		TEST_METHOD(TestInvoiceItemAddNewProductToSummary)
		{
			// Log test name
			Logger::WriteMessage("TestInvoiceItemAddNewProductToSummary");

			// vector of strings to store values to add to row
			vector<string> addVector;
			addVector.push_back("2720"); // new product_id
			addVector.push_back("30");

			invoiceItem2->add(addVector);

			// string to store invoiceItem's search for product_id 2720
			string invoiceItemReturned = invoiceItem2->search("product_id", "2720");
			// string to store summary's search for product_id 2720
			string summaryReturned = summary2->search("product_id", "2720");

			// Log results
			Logger::WriteMessage(invoiceItemReturned.c_str());
			Logger::WriteMessage(summaryReturned.c_str());

			// Check that new invoice item was added to InvoiceItem table
			Assert::AreEqual("1014|2720|30\n", invoiceItemReturned.c_str());
			// Check that summary is updated with new product and right quantity
			Assert::AreEqual("2720|30\n", summaryReturned.c_str());
		}


		/// \brief Test adding new invoice item with existing product updates product's quantity in Summary
		TEST_METHOD(TestInvoiceItemAddUpdateProductQuantityInSummary)
		{
			// Log test name
			Logger::WriteMessage("TestInvoiceItemAddUpdateProductQuantityInSummary");

			// vector of strings to store values to add to row
			vector<string> addVector;
			addVector.push_back("1002"); // existing product_id
			addVector.push_back("50");

			invoiceItem2->add(addVector);

			// string to store invoiceItem's search for invoice_item_id 1014
			string invoiceItemReturned = invoiceItem2->search("invoice_item_id", "1014");
			// string to store summary's search for product_id 1002
			string summaryReturned = summary2->search("product_id", "1002");

			// Log results
			Logger::WriteMessage(invoiceItemReturned.c_str());
			Logger::WriteMessage(summaryReturned.c_str());

			// Check that new invoice item was added to InvoiceItem table
			Assert::AreEqual("1014|1002|50\n", invoiceItemReturned.c_str());
			// Check that summary is updated with right quantity
			Assert::AreEqual("1002|150\n", summaryReturned.c_str());
		}

		/// \brief Test modifying invoice_item's quantity updates product's quantity in Summary
		TEST_METHOD(TestInvoiceItemModifyQuantityUpdatesQuantityInSummary)
		{
			// Log test name
			Logger::WriteMessage("TestInvoiceItemModifyQuantityUpdatesQuantityInSummary");

			invoiceItem2->modifyRow("1011", "quantity", "30"); //increase by 20

			// string to store invoiceItem's search for invoice_item_id 1014
			string invoiceItemReturned = invoiceItem2->search("invoice_item_id", "1011");
			// string to store summary's search for product_id 1002
			string summaryReturned = summary2->search("product_id", "2543");

			// Log results
			Logger::WriteMessage(invoiceItemReturned.c_str());
			Logger::WriteMessage(summaryReturned.c_str());

			// Check that invoice item's quantity was modified correctly
			Assert::AreEqual("1011|2543|30\n", invoiceItemReturned.c_str());
			// Check that summary is updated with right quantity
			Assert::AreEqual("2543|220\n", summaryReturned.c_str()); //200 + 20
		}

		/// \brief Test modifying invoice_item's product_id updates product's quantity in Summary
		TEST_METHOD(TestInvoiceItemModifyProductIDUpdatesQuantityInSummary)
		{
			// Log test name
			Logger::WriteMessage("TestInvoiceItemModifyProductIDUpdatesQuantityInSummary");

			invoiceItem2->modifyRow("1011", "product_id", "1002"); //chage 2543 to 1002

			// string to store invoiceItem's search for invoice_item_id 1014
			string invoiceItemReturned = invoiceItem2->search("invoice_item_id", "1011");
			// string to store summary's search for product_id 2543
			string summaryReturnedFor2543 = summary2->search("product_id", "2543");
			// string to store summary's search for product_id 2543
			string summaryReturnedFor1002 = summary2->search("product_id", "1002");


			// Log results
			Logger::WriteMessage(invoiceItemReturned.c_str());
			Logger::WriteMessage(summaryReturnedFor2543.c_str());
			Logger::WriteMessage(summaryReturnedFor1002.c_str());

			// Check that invoice item's quantity was modified correctly
			Assert::AreEqual("1011|1002|10\n", invoiceItemReturned.c_str());
			// Check that product 2543's quantity is decreased by 10 because it is no longer in the invoice
			Assert::AreEqual("2543|190\n", summaryReturnedFor2543.c_str());
			// Check that product 1002's quantity is incread by 10 because it is given 2543's quantity
			Assert::AreEqual("1002|110\n", summaryReturnedFor1002.c_str());
		}

		/// \brief Test deleting a row in InvoiceItem, substracts that product's quantity from Summary
		TEST_METHOD(TestInvoiceItemDeleteUpdatesQuantityInSummary)
		{
			// Log test name
			Logger::WriteMessage("TestInvoiceItemDeleteyUpdatesQuantityInSummary");

			invoiceItem2->deleteRow("1011"); //increase by 20

			// tests if DoesNotExistException is thrown, which means invoice_item_id no longer exists
			try {
				string returned = invoiceItem2->search("invoice_item_id", "1011");
				Assert::Fail(); // fail test if no exception is thrown
			}
			catch (DoesNotExistException e) { // continue if DoesNotExistException was thrown
				Logger::WriteMessage(e.what());
			} 
			catch (...) { // Fail if something else is thrown
				Logger::WriteMessage("Other exception");
				Assert::Fail(); 
			}

			// string to store summary's search for product_id 1002
			string summaryReturned = summary2->search("product_id", "2543");

			// Log results
			Logger::WriteMessage(summaryReturned.c_str());

			// Check that summary is updated with right quantity
			Assert::AreEqual("2543|190\n", summaryReturned.c_str()); //200 - 1
		}
	};
}
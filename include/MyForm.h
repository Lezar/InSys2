#pragma once
#include "TableInterface.h"
#include "Category.h"
#include "Product.h"
#include "Returns.h"
#include "Invoice.h"
#include "InvoiceItem.h"
#include "Summary.h"
#include "Orders.h"
#include "Sales.h"
#include <sstream>
#include <iterator>
#include "Receipt.h"
#include "SalesSummary.h"
#include "ReportForm.h"
#include "ReportsImpl.h"
#include <msclr/marshal.h>
#include <msclr/marshal_cppstd.h>
#include <regex>

namespace InventoryManagement {

	using namespace msclr::interop;
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;
	using namespace std::tr1;

	/// \brief GUI class that uses TableInterface to allow the user to interact with the database
	/// 
	/// GUI for InventoryManagement project that uses managed code for Win32
	/// Responsibilties include:
	/// - Using TableInterface concrete classes to manage and interact with the database
	/// - Creating a GUI for the user to interact with
	/// - Error checking to prevent the user from inputting invalid information
	/// - 
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:

		/// \brief Clean up any resources being used.
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

		/// \brief Function to return the entire contents of the specified file
	private: vector<string> returnFile(string fileName){
				 // vector to store each row of the file
				 vector<string> fileVect;

				 ifstream inFile;

				 // opens the connection to the textFile
				 inFile.open(fileName);

				 // string to contain current row
				 string rowReceive;

				 // ensures that categoryInFile is open
				 if(inFile.is_open())
				 {
					 // while loop continues as long as there is another line in the text file
					 while(inFile.good())
					 {
						 // retrieves the next line in categoryInFile and assigns it to the string rowReceive
						 getline(inFile, rowReceive);

						 // ensures that the row contains data
						 if(!rowReceive.empty())
						 {
							 // adds the row to the vector
							 fileVect.push_back(rowReceive);
						 }
					 }
				 }

				 // close connection to file
				 inFile.close();

				 return fileVect;
			 }

			 /// \brief populates lstInvoiceProductList with the selected invoice's items
			 ///
			 /// \pre valid Invoice is currently selected
	private: void populateInvoiceProductList() {

				 // clear Product List
				 lstInvoiceProductList->Items->Clear();

				 // TablesInterfaces to interact with tables
				 Table orders = new Orders();
				 Table invoiceItem = new InvoiceItem();
				 Table product = new Product();

				 // String^ to hold the current selected invoice
				 System::String^ invoiceSystemStr = cmbInvoiceSelect->SelectedItem->ToString();
				 // convert to std::string
				 std::string invoiceSTDStr = marshal_as<std::string>(invoiceSystemStr);

				 // Integers to store delimiter positions
				 int delimiter1, delimiter2, delimiter3, delimiter4;

				 // strings to store returns of searches
				 std::string ordersReturned, invoiceItemReturned, productReturned;

				 // string to store current row in of search result of Orders
				 std::string currentRow;

				 // find delimiter for invoice_Id
				 delimiter1 = invoiceSTDStr.find(":");
				 delimiter2 = invoiceSTDStr.find(":", delimiter1 + 1);

				 // strings for invoice_id, invoice_item_id, product_id, name, and quantity
				 std::string invoice_id, invoice_item_id, product_id, name, quantity;

				 // search Orders for all invoice_items with the invoice_id of selected invoice
				 invoice_id = invoiceSTDStr.substr(delimiter1 + 2, delimiter2 - delimiter1 - 9); 
				 ordersReturned = orders->search("invoice_id", invoice_id);

				 // get the first row of the search result
				 delimiter1 = ordersReturned.find("\n");
				 currentRow = ordersReturned.substr(0, delimiter1+1);

				 // if the current row is not equal to the ordersReturned
				 // keep breaking down ordersReturned line by line until they do
				 // ie, currentRow is the last line of the search result
				 // find each invoice item id of each currentRow
				 // and then find corresponding product_id and quantity
				 // then search for product name and add all three to listbox
				 while (!ordersReturned.empty()){

					 // Remove first line from orders Returned and store to currentRow
					 delimiter1 = ordersReturned.find("\n");
					 ordersReturned = ordersReturned.substr(delimiter1+1);

					 // find invoice_item_id
					 delimiter1 = currentRow.find("|");
					 invoice_item_id = currentRow.substr(0, delimiter1);

					 // get new currentRow
					 delimiter1 = ordersReturned.find("\n");
					 currentRow = ordersReturned.substr(0, delimiter1 + 1);

					 // search for that invoiceItem row
					 invoiceItemReturned = invoiceItem->search("invoice_item_id", invoice_item_id);

					 // find product_id and quantity
					 delimiter1 = invoiceItemReturned.find("|");
					 delimiter2 = invoiceItemReturned.find("|", delimiter1+1);
					 product_id = invoiceItemReturned.substr(delimiter1 + 1, delimiter2 - delimiter1 - 1);
					 quantity = invoiceItemReturned.substr(delimiter2 + 1);

					 // search for the product
					 productReturned = product->search("product_id", product_id);

					 // find product name
					 delimiter1 = productReturned.find("|");
					 delimiter2 = productReturned.find("|", delimiter1+1);
					 delimiter3 = productReturned.find("|", delimiter2+1);
					 delimiter4 = productReturned.find("|", delimiter3+1);

					 name = productReturned.substr(delimiter3 + 1, delimiter4 - delimiter3 - 1);

					 // add invoice_item_id, product_id, name, and quantity
					 lstInvoiceProductList->Items->Add(
						 gcnew String(invoice_item_id.c_str()) + " | " +
						 gcnew String(product_id.c_str()) + " | " +
						 gcnew String(name.c_str()) + " | " +
						 gcnew String(quantity.c_str()));

				 } // end while for add to listbox

				 delete invoiceItem;
				 delete orders;
				 delete product;
			 }

	private: void populateSalesProductList() {

				 // clear Product List
				 lstSalesProductList->Items->Clear();

				 // TablesInterfaces to interact with tables
				 Table salesSummary = new SalesSummary();
				 Table sales = new Sales();
				 Table product = new Product();

				 // String^ to hold the current selected receipt
				 System::String^ receiptSystemStr = cmbSalesReceiptSelect->SelectedItem->ToString();
				 // convert to std::string
				 std::string receiptSTDStr = marshal_as<std::string>(receiptSystemStr);

				 // Integers to store delimiter positions
				 int delimiter1, delimiter2, delimiter3, delimiter4;

				 // strings to store returns of searches
				 std::string salesSummaryReturned, salesReturned, productReturned;

				 // string to store current row in of search result of Orders
				 std::string currentRow;

				 // find delimiter for invoice_Id
				 delimiter1 = receiptSTDStr.find(":");
				 delimiter2 = receiptSTDStr.find(":", delimiter1 + 1);

				 // strings for receipt_id, sales_id, product_id, name, discount and quantity
				 std::string receipt_id, salesID, product_id, product_name, qSold, discount;

				 // search Sales summary for all sales with the receipt_id of selected receipt
				 if (cmbSalesFunction->SelectedIndex == 2) 
					 receipt_id = receiptSTDStr.substr(delimiter1 + 2, delimiter2 - delimiter1-9); 
				 if (cmbSalesFunction->SelectedIndex == 1) 					  
					 receipt_id = receiptSTDStr.substr(delimiter1 + 2, delimiter2 - delimiter1-17); 

				 salesSummaryReturned = salesSummary->search("receiptID", receipt_id);

				 // get the first row of the search result
				 delimiter1 = salesSummaryReturned.find("\n");
				 currentRow = salesSummaryReturned.substr(0, delimiter1+1);

				 // if the current row is not equal to the sales summary returned
				 // keep breaking down sales summary returned line by line until they do
				 // ie, currentRow is the last line of the search result
				 // find each sales id of each currentRow
				 // and then find corresponding product_id, discount and quantity
				 // then search for product name and add all three to listbox
				 while (!salesSummaryReturned.empty()){

					 // Remove first line from sales summary Returned and store to currentRow
					 delimiter1 = salesSummaryReturned.find("\n");
					 salesSummaryReturned = salesSummaryReturned.substr(delimiter1+1);

					 // find sales 
					 delimiter1 = currentRow.find("|");
					 salesID = currentRow.substr(delimiter1+1);

					 // get new currentRow
					 delimiter1 = salesSummaryReturned.find("\n");
					 currentRow = salesSummaryReturned.substr(0, delimiter1 + 1);

					 // search for that sales row
					 salesReturned = sales->search("sales_id", salesID);

					 // find product_id, discount and quantity
					 delimiter1 = salesReturned.find("|");
					 delimiter2 = salesReturned.find("|", delimiter1+1);
					 delimiter3 = salesReturned.find("|", delimiter2+1);
					 product_id  = salesReturned.substr(delimiter2+1, delimiter3-delimiter2-1);
					 qSold  = salesReturned.substr(delimiter1+1, delimiter2-delimiter1-1);
					 discount = salesReturned.substr(delimiter3+1);
					 // search for the product
					 productReturned = product->search("product_id", product_id);

					 // find product name
					 delimiter1 = productReturned.find("|");
					 delimiter2 = productReturned.find("|", delimiter1+1);
					 delimiter3 = productReturned.find("|", delimiter2+1);
					 delimiter4 = productReturned.find("|", delimiter3+1);

					 product_name = productReturned.substr(delimiter3 + 1, delimiter4 - delimiter3 - 1);

					 // add sales, product_id, name, discount and quantity
					 lstSalesProductList->Items->Add(
						 gcnew String(salesID.c_str()) + " | " +
						 gcnew String(product_id.c_str()) + " | " +
						 gcnew String(product_name.c_str()) + " | " +
						 gcnew String(qSold.c_str())+ "|" +
						 gcnew String(discount.c_str()));

				 } // end while for add to listbox

				 delete sales;
				 delete salesSummary;
				 delete product;
			 }


#pragma region Controls declared
	private: System::Windows::Forms::TabControl^  tbInventorySystem;
	protected: 
	private: System::Windows::Forms::TabPage^  tpCategory;
	private: System::Windows::Forms::Button^  btnCategoryAdd;

	private: System::Windows::Forms::Label^  lblCategoryDescription;
	private: System::Windows::Forms::TextBox^  txtCategoryDescription;


	private: System::Windows::Forms::Label^  lblCategoryName;
	private: System::Windows::Forms::TextBox^  txtCategoryName;
	private: System::Windows::Forms::ComboBox^  cmbCategorySelect;



	private: System::Windows::Forms::ComboBox^  cmbCategoryFunction;
	private: System::Windows::Forms::TabPage^  tpProduct;
	private: System::Windows::Forms::TabPage^  tpSales;
	private: System::Windows::Forms::TabPage^  tpInvoices;
	private: System::Windows::Forms::MenuStrip^  msMenu;

	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  aboutToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  programHelpToolStripMenuItem;
	private: System::Windows::Forms::Label^  lblCategoryFunction;
	private: System::Windows::Forms::Label^  lblCategorySelect;
	private: System::Windows::Forms::Label^  lblProductFunction;
	private: System::Windows::Forms::ComboBox^  cmbProductFunction;
	private: System::Windows::Forms::Label^  lblProductSelect;

	private: System::Windows::Forms::Label^  lblProductPrice;

	private: System::Windows::Forms::Label^  lblProductDescription;
	private: System::Windows::Forms::TextBox^  txtProductDescription;

	private: System::Windows::Forms::Label^  lblProductName;
	private: System::Windows::Forms::TextBox^  txtProductName;

	private: System::Windows::Forms::Button^  btnCategoryModify;
	private: System::Windows::Forms::Button^  btnProductModify;
	private: System::Windows::Forms::Button^  btnProductAdd;
	private: System::Windows::Forms::TextBox^  txtProductPrice;
	private: System::Windows::Forms::Button^  btnCategoryDelete;
	private: System::Windows::Forms::Label^  lblProductCategory;
	private: System::Windows::Forms::ComboBox^  cmbProductCategorySelect;
	private: System::Windows::Forms::TextBox^  txtProdID;
	private: System::Windows::Forms::Label^  lblProductID;
	private: System::Windows::Forms::Button^  btnProductDelete;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ComboBox^  cmbSalesFunction;
	private: System::Windows::Forms::Label^  lblSalesReceiptSelect;
	private: System::Windows::Forms::ComboBox^  cmbSalesReceiptSelect;
	private: System::Windows::Forms::Button^  btnSalesRemoveProduct;
	private: System::Windows::Forms::ListBox^  lstSalesProductList;

	private: System::Windows::Forms::Button^  btnSalesAddProduct;
	private: System::Windows::Forms::TextBox^  txtSalesProductQuantity;
	private: System::Windows::Forms::Label^  lblSaleProductQuantity;
	private: System::Windows::Forms::Label^  lblSalesProductSelect;
	private: System::Windows::Forms::ComboBox^  cmbSalesProductSelect;


	private: System::Windows::Forms::Button^  btnSalesSearch;
	private: System::Windows::Forms::TextBox^  txtSalesProductDiscount;
	private: System::Windows::Forms::Label^  lblSalesProductDiscount;
	private: System::Windows::Forms::Button^  btnSalesCreateReceipt;
	private: System::Windows::Forms::Button^  btnSalesModify;
	private: System::Windows::Forms::DateTimePicker^  dtSalesReceiptDate;
	private: System::Windows::Forms::DateTimePicker^  dtInvoiceDate;
	private: System::Windows::Forms::Button^  btnInvoiceModify;
	private: System::Windows::Forms::Button^  btnInvoiceCreateInvoice;
	private: System::Windows::Forms::Label^  lblInvoiceSelect;
	private: System::Windows::Forms::ComboBox^  cmbInvoiceSelect;
	private: System::Windows::Forms::Button^  btnInvoiceRemoveProduct;
	private: System::Windows::Forms::ListBox^  lstInvoiceProductList;
	private: System::Windows::Forms::Button^  btnInvoiceAddProduct;
	private: System::Windows::Forms::TextBox^  txtInvoiceProductQuantity;
	private: System::Windows::Forms::Label^  lblInvoiceProductQuantity;
	private: System::Windows::Forms::Label^  lblInvoiceProductSelect;
	private: System::Windows::Forms::ComboBox^  cmbInvoiceProductSelect;
	private: System::Windows::Forms::Label^  lblInvoiceFunction;
	private: System::Windows::Forms::ComboBox^  cmbInvoiceFunction;
	private: System::Windows::Forms::TextBox^  txtCategorySearch;
	private: System::Windows::Forms::ComboBox^  cmbCategorySearchBy;
	private: System::Windows::Forms::Label^  lblCategorySearchBy;
	private: System::Windows::Forms::Button^  btnCategorySearch;
	private: System::Windows::Forms::Label^  lblCategorySearchInfo;
	private: System::Windows::Forms::TextBox^  txtCategorySearchResults;
	private: System::Windows::Forms::Button^  btnProductSearch;
	private: System::Windows::Forms::Label^  lblInvoiceProductList;
	private: System::Windows::Forms::Button^  btnInvoiceSearch;


	private: System::Windows::Forms::Label^  lblsalesProductList;

	private: System::Windows::Forms::TextBox^  txtProductSearch;
	private: System::Windows::Forms::ComboBox^  cmbProductSearch;
	private: System::Windows::Forms::Label^  lblProductSearchBy;
	private: System::Windows::Forms::TextBox^  txtProductSearchResults;
	private: System::Windows::Forms::TabPage^  tpReturns;
	private: System::Windows::Forms::Label^  lblSearchValue;
	private: System::Windows::Forms::ComboBox^  cmbSearchBy;
	private: System::Windows::Forms::Label^  lblSearchBy;
	private: System::Windows::Forms::TextBox^  txtSearchValue;
	private: System::Windows::Forms::DateTimePicker^  dateReturned;
	private: System::Windows::Forms::Button^  btnFunction;
	private: System::Windows::Forms::Label^  lblReturns;
	private: System::Windows::Forms::ComboBox^  cmbReturns;
	private: System::Windows::Forms::TextBox^  txtQuantityReturned;
	private: System::Windows::Forms::Label^  lblQuantityReturned;
	private: System::Windows::Forms::Label^  lblSale;
	private: System::Windows::Forms::ComboBox^  cmbSale;
	private: System::Windows::Forms::Label^  lblFunction;
	private: System::Windows::Forms::ComboBox^  cmbReturnFunction;
	private: System::Windows::Forms::TextBox^  txtSearchReturns;
	private: System::Windows::Forms::Label^  lblProductSearchInfo;
	private: System::Windows::Forms::ComboBox^  cmbProductSelect;

	private: System::Windows::Forms::TabPage^  tpReports;
	private: System::Windows::Forms::ComboBox^  cmbReportSelect;
	private: System::Windows::Forms::Label^  lblReportSelect;
	private: System::Windows::Forms::Label^  lblReportDescription;

	private: System::Windows::Forms::TextBox^  txtReportDescription;
	private: System::Windows::Forms::Label^  lblReportEndDate;
	private: System::Windows::Forms::DateTimePicker^  dtReportEndDate;

	private: System::Windows::Forms::Label^  lblReportStartDate;
	private: System::Windows::Forms::DateTimePicker^  dtReportStartDate;
	private: System::Windows::Forms::ComboBox^  cmbReportCategorySelect;


	private: System::Windows::Forms::Label^  lblReportCategorySelect;
	private: System::Windows::Forms::Button^  btnReportGenerate;

	private: System::Windows::Forms::Button^  btnInvoiceModifyInvoiceItem;
	private: System::Windows::Forms::Button^  btnSalesModifyReceipt;


#pragma endregion

	private:

		/// \brief Required designer variable.
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->tbInventorySystem = (gcnew System::Windows::Forms::TabControl());
			this->tpCategory = (gcnew System::Windows::Forms::TabPage());
			this->txtCategorySearchResults = (gcnew System::Windows::Forms::TextBox());
			this->btnCategorySearch = (gcnew System::Windows::Forms::Button());
			this->lblCategorySearchInfo = (gcnew System::Windows::Forms::Label());
			this->txtCategorySearch = (gcnew System::Windows::Forms::TextBox());
			this->cmbCategorySearchBy = (gcnew System::Windows::Forms::ComboBox());
			this->lblCategorySearchBy = (gcnew System::Windows::Forms::Label());
			this->btnCategoryDelete = (gcnew System::Windows::Forms::Button());
			this->btnCategoryModify = (gcnew System::Windows::Forms::Button());
			this->lblCategorySelect = (gcnew System::Windows::Forms::Label());
			this->lblCategoryFunction = (gcnew System::Windows::Forms::Label());
			this->btnCategoryAdd = (gcnew System::Windows::Forms::Button());
			this->lblCategoryDescription = (gcnew System::Windows::Forms::Label());
			this->txtCategoryDescription = (gcnew System::Windows::Forms::TextBox());
			this->lblCategoryName = (gcnew System::Windows::Forms::Label());
			this->txtCategoryName = (gcnew System::Windows::Forms::TextBox());
			this->cmbCategorySelect = (gcnew System::Windows::Forms::ComboBox());
			this->cmbCategoryFunction = (gcnew System::Windows::Forms::ComboBox());
			this->tpProduct = (gcnew System::Windows::Forms::TabPage());
			this->txtProductSearchResults = (gcnew System::Windows::Forms::TextBox());
			this->btnProductSearch = (gcnew System::Windows::Forms::Button());
			this->lblProductSearchInfo = (gcnew System::Windows::Forms::Label());
			this->txtProductSearch = (gcnew System::Windows::Forms::TextBox());
			this->cmbProductSearch = (gcnew System::Windows::Forms::ComboBox());
			this->lblProductSearchBy = (gcnew System::Windows::Forms::Label());
			this->btnProductDelete = (gcnew System::Windows::Forms::Button());
			this->txtProdID = (gcnew System::Windows::Forms::TextBox());
			this->lblProductID = (gcnew System::Windows::Forms::Label());
			this->lblProductCategory = (gcnew System::Windows::Forms::Label());
			this->cmbProductCategorySelect = (gcnew System::Windows::Forms::ComboBox());
			this->btnProductModify = (gcnew System::Windows::Forms::Button());
			this->btnProductAdd = (gcnew System::Windows::Forms::Button());
			this->txtProductPrice = (gcnew System::Windows::Forms::TextBox());
			this->lblProductPrice = (gcnew System::Windows::Forms::Label());
			this->lblProductDescription = (gcnew System::Windows::Forms::Label());
			this->txtProductDescription = (gcnew System::Windows::Forms::TextBox());
			this->lblProductName = (gcnew System::Windows::Forms::Label());
			this->txtProductName = (gcnew System::Windows::Forms::TextBox());
			this->lblProductSelect = (gcnew System::Windows::Forms::Label());
			this->cmbProductSelect = (gcnew System::Windows::Forms::ComboBox());
			this->lblProductFunction = (gcnew System::Windows::Forms::Label());
			this->cmbProductFunction = (gcnew System::Windows::Forms::ComboBox());
			this->tpSales = (gcnew System::Windows::Forms::TabPage());
			this->btnSalesModifyReceipt = (gcnew System::Windows::Forms::Button());
			this->lblsalesProductList = (gcnew System::Windows::Forms::Label());
			this->btnSalesSearch = (gcnew System::Windows::Forms::Button());
			this->dtSalesReceiptDate = (gcnew System::Windows::Forms::DateTimePicker());
			this->btnSalesModify = (gcnew System::Windows::Forms::Button());
			this->btnSalesCreateReceipt = (gcnew System::Windows::Forms::Button());
			this->txtSalesProductDiscount = (gcnew System::Windows::Forms::TextBox());
			this->lblSalesProductDiscount = (gcnew System::Windows::Forms::Label());
			this->lblSalesReceiptSelect = (gcnew System::Windows::Forms::Label());
			this->cmbSalesReceiptSelect = (gcnew System::Windows::Forms::ComboBox());
			this->btnSalesRemoveProduct = (gcnew System::Windows::Forms::Button());
			this->lstSalesProductList = (gcnew System::Windows::Forms::ListBox());
			this->btnSalesAddProduct = (gcnew System::Windows::Forms::Button());
			this->txtSalesProductQuantity = (gcnew System::Windows::Forms::TextBox());
			this->lblSaleProductQuantity = (gcnew System::Windows::Forms::Label());
			this->lblSalesProductSelect = (gcnew System::Windows::Forms::Label());
			this->cmbSalesProductSelect = (gcnew System::Windows::Forms::ComboBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->cmbSalesFunction = (gcnew System::Windows::Forms::ComboBox());
			this->tpInvoices = (gcnew System::Windows::Forms::TabPage());
			this->btnInvoiceModifyInvoiceItem = (gcnew System::Windows::Forms::Button());
			this->btnInvoiceSearch = (gcnew System::Windows::Forms::Button());
			this->lblInvoiceProductList = (gcnew System::Windows::Forms::Label());
			this->dtInvoiceDate = (gcnew System::Windows::Forms::DateTimePicker());
			this->btnInvoiceModify = (gcnew System::Windows::Forms::Button());
			this->btnInvoiceCreateInvoice = (gcnew System::Windows::Forms::Button());
			this->lblInvoiceSelect = (gcnew System::Windows::Forms::Label());
			this->cmbInvoiceSelect = (gcnew System::Windows::Forms::ComboBox());
			this->btnInvoiceRemoveProduct = (gcnew System::Windows::Forms::Button());
			this->lstInvoiceProductList = (gcnew System::Windows::Forms::ListBox());
			this->btnInvoiceAddProduct = (gcnew System::Windows::Forms::Button());
			this->txtInvoiceProductQuantity = (gcnew System::Windows::Forms::TextBox());
			this->lblInvoiceProductQuantity = (gcnew System::Windows::Forms::Label());
			this->lblInvoiceProductSelect = (gcnew System::Windows::Forms::Label());
			this->cmbInvoiceProductSelect = (gcnew System::Windows::Forms::ComboBox());
			this->lblInvoiceFunction = (gcnew System::Windows::Forms::Label());
			this->cmbInvoiceFunction = (gcnew System::Windows::Forms::ComboBox());
			this->tpReturns = (gcnew System::Windows::Forms::TabPage());
			this->txtSearchReturns = (gcnew System::Windows::Forms::TextBox());
			this->lblSearchValue = (gcnew System::Windows::Forms::Label());
			this->cmbSearchBy = (gcnew System::Windows::Forms::ComboBox());
			this->lblSearchBy = (gcnew System::Windows::Forms::Label());
			this->txtSearchValue = (gcnew System::Windows::Forms::TextBox());
			this->dateReturned = (gcnew System::Windows::Forms::DateTimePicker());
			this->btnFunction = (gcnew System::Windows::Forms::Button());
			this->lblReturns = (gcnew System::Windows::Forms::Label());
			this->cmbReturns = (gcnew System::Windows::Forms::ComboBox());
			this->txtQuantityReturned = (gcnew System::Windows::Forms::TextBox());
			this->lblQuantityReturned = (gcnew System::Windows::Forms::Label());
			this->lblSale = (gcnew System::Windows::Forms::Label());
			this->cmbSale = (gcnew System::Windows::Forms::ComboBox());
			this->lblFunction = (gcnew System::Windows::Forms::Label());
			this->cmbReturnFunction = (gcnew System::Windows::Forms::ComboBox());
			this->tpReports = (gcnew System::Windows::Forms::TabPage());
			this->btnReportGenerate = (gcnew System::Windows::Forms::Button());
			this->cmbReportCategorySelect = (gcnew System::Windows::Forms::ComboBox());
			this->lblReportCategorySelect = (gcnew System::Windows::Forms::Label());
			this->lblReportEndDate = (gcnew System::Windows::Forms::Label());
			this->dtReportEndDate = (gcnew System::Windows::Forms::DateTimePicker());
			this->lblReportStartDate = (gcnew System::Windows::Forms::Label());
			this->dtReportStartDate = (gcnew System::Windows::Forms::DateTimePicker());
			this->lblReportDescription = (gcnew System::Windows::Forms::Label());
			this->txtReportDescription = (gcnew System::Windows::Forms::TextBox());
			this->cmbReportSelect = (gcnew System::Windows::Forms::ComboBox());
			this->lblReportSelect = (gcnew System::Windows::Forms::Label());
			this->msMenu = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->programHelpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tbInventorySystem->SuspendLayout();
			this->tpCategory->SuspendLayout();
			this->tpProduct->SuspendLayout();
			this->tpSales->SuspendLayout();
			this->tpInvoices->SuspendLayout();
			this->tpReturns->SuspendLayout();
			this->tpReports->SuspendLayout();
			this->msMenu->SuspendLayout();
			this->SuspendLayout();
			// 
			// tbInventorySystem
			// 
			this->tbInventorySystem->Controls->Add(this->tpCategory);
			this->tbInventorySystem->Controls->Add(this->tpProduct);
			this->tbInventorySystem->Controls->Add(this->tpSales);
			this->tbInventorySystem->Controls->Add(this->tpInvoices);
			this->tbInventorySystem->Controls->Add(this->tpReturns);
			this->tbInventorySystem->Controls->Add(this->tpReports);
			this->tbInventorySystem->Location = System::Drawing::Point(12, 28);
			this->tbInventorySystem->Name = L"tbInventorySystem";
			this->tbInventorySystem->SelectedIndex = 0;
			this->tbInventorySystem->Size = System::Drawing::Size(511, 497);
			this->tbInventorySystem->TabIndex = 0;
			// 
			// tpCategory
			// 
			this->tpCategory->Controls->Add(this->txtCategorySearchResults);
			this->tpCategory->Controls->Add(this->btnCategorySearch);
			this->tpCategory->Controls->Add(this->lblCategorySearchInfo);
			this->tpCategory->Controls->Add(this->txtCategorySearch);
			this->tpCategory->Controls->Add(this->cmbCategorySearchBy);
			this->tpCategory->Controls->Add(this->lblCategorySearchBy);
			this->tpCategory->Controls->Add(this->btnCategoryDelete);
			this->tpCategory->Controls->Add(this->btnCategoryModify);
			this->tpCategory->Controls->Add(this->lblCategorySelect);
			this->tpCategory->Controls->Add(this->lblCategoryFunction);
			this->tpCategory->Controls->Add(this->btnCategoryAdd);
			this->tpCategory->Controls->Add(this->lblCategoryDescription);
			this->tpCategory->Controls->Add(this->txtCategoryDescription);
			this->tpCategory->Controls->Add(this->lblCategoryName);
			this->tpCategory->Controls->Add(this->txtCategoryName);
			this->tpCategory->Controls->Add(this->cmbCategorySelect);
			this->tpCategory->Controls->Add(this->cmbCategoryFunction);
			this->tpCategory->Location = System::Drawing::Point(4, 22);
			this->tpCategory->Name = L"tpCategory";
			this->tpCategory->Padding = System::Windows::Forms::Padding(3);
			this->tpCategory->Size = System::Drawing::Size(503, 471);
			this->tpCategory->TabIndex = 0;
			this->tpCategory->Text = L"Category";
			this->tpCategory->UseVisualStyleBackColor = true;
			// 
			// txtCategorySearchResults
			// 
			this->txtCategorySearchResults->Location = System::Drawing::Point(7, 219);
			this->txtCategorySearchResults->Multiline = true;
			this->txtCategorySearchResults->Name = L"txtCategorySearchResults";
			this->txtCategorySearchResults->ReadOnly = true;
			this->txtCategorySearchResults->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->txtCategorySearchResults->Size = System::Drawing::Size(490, 141);
			this->txtCategorySearchResults->TabIndex = 16;
			// 
			// btnCategorySearch
			// 
			this->btnCategorySearch->Location = System::Drawing::Point(6, 168);
			this->btnCategorySearch->Name = L"btnCategorySearch";
			this->btnCategorySearch->Size = System::Drawing::Size(75, 23);
			this->btnCategorySearch->TabIndex = 15;
			this->btnCategorySearch->Text = L"Search";
			this->btnCategorySearch->UseVisualStyleBackColor = true;
			this->btnCategorySearch->Click += gcnew System::EventHandler(this, &MyForm::btnCategorySearch_Click);
			// 
			// lblCategorySearchInfo
			// 
			this->lblCategorySearchInfo->AutoSize = true;
			this->lblCategorySearchInfo->Location = System::Drawing::Point(7, 113);
			this->lblCategorySearchInfo->Name = L"lblCategorySearchInfo";
			this->lblCategorySearchInfo->Size = System::Drawing::Size(142, 13);
			this->lblCategorySearchInfo->TabIndex = 14;
			this->lblCategorySearchInfo->Text = L"Enter Information for Search:";
			// 
			// txtCategorySearch
			// 
			this->txtCategorySearch->Location = System::Drawing::Point(7, 137);
			this->txtCategorySearch->Name = L"txtCategorySearch";
			this->txtCategorySearch->Size = System::Drawing::Size(490, 20);
			this->txtCategorySearch->TabIndex = 13;
			// 
			// cmbCategorySearchBy
			// 
			this->cmbCategorySearchBy->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbCategorySearchBy->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"ID", L"Name", L"Description"});
			this->cmbCategorySearchBy->Location = System::Drawing::Point(7, 75);
			this->cmbCategorySearchBy->Name = L"cmbCategorySearchBy";
			this->cmbCategorySearchBy->Size = System::Drawing::Size(114, 21);
			this->cmbCategorySearchBy->TabIndex = 12;
			this->cmbCategorySearchBy->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::cmbCategorySearchBy_SelectedIndexChanged);
			// 
			// lblCategorySearchBy
			// 
			this->lblCategorySearchBy->AutoSize = true;
			this->lblCategorySearchBy->Location = System::Drawing::Point(7, 55);
			this->lblCategorySearchBy->Name = L"lblCategorySearchBy";
			this->lblCategorySearchBy->Size = System::Drawing::Size(59, 13);
			this->lblCategorySearchBy->TabIndex = 11;
			this->lblCategorySearchBy->Text = L"Search By:";
			// 
			// btnCategoryDelete
			// 
			this->btnCategoryDelete->Location = System::Drawing::Point(6, 108);
			this->btnCategoryDelete->Name = L"btnCategoryDelete";
			this->btnCategoryDelete->Size = System::Drawing::Size(75, 23);
			this->btnCategoryDelete->TabIndex = 10;
			this->btnCategoryDelete->Text = L"Delete";
			this->btnCategoryDelete->UseVisualStyleBackColor = true;
			this->btnCategoryDelete->Click += gcnew System::EventHandler(this, &MyForm::btnCategoryDelete_Click);
			// 
			// btnCategoryModify
			// 
			this->btnCategoryModify->Location = System::Drawing::Point(7, 246);
			this->btnCategoryModify->Name = L"btnCategoryModify";
			this->btnCategoryModify->Size = System::Drawing::Size(75, 23);
			this->btnCategoryModify->TabIndex = 9;
			this->btnCategoryModify->Text = L"Modify";
			this->btnCategoryModify->UseVisualStyleBackColor = true;
			this->btnCategoryModify->Click += gcnew System::EventHandler(this, &MyForm::btnCategoryModify_Click);
			// 
			// lblCategorySelect
			// 
			this->lblCategorySelect->AutoSize = true;
			this->lblCategorySelect->Location = System::Drawing::Point(7, 55);
			this->lblCategorySelect->Name = L"lblCategorySelect";
			this->lblCategorySelect->Size = System::Drawing::Size(52, 13);
			this->lblCategorySelect->TabIndex = 8;
			this->lblCategorySelect->Text = L"Category:";
			// 
			// lblCategoryFunction
			// 
			this->lblCategoryFunction->AutoSize = true;
			this->lblCategoryFunction->Location = System::Drawing::Point(7, 7);
			this->lblCategoryFunction->Name = L"lblCategoryFunction";
			this->lblCategoryFunction->Size = System::Drawing::Size(51, 13);
			this->lblCategoryFunction->TabIndex = 7;
			this->lblCategoryFunction->Text = L"Function:";
			// 
			// btnCategoryAdd
			// 
			this->btnCategoryAdd->Location = System::Drawing::Point(7, 246);
			this->btnCategoryAdd->Name = L"btnCategoryAdd";
			this->btnCategoryAdd->Size = System::Drawing::Size(43, 23);
			this->btnCategoryAdd->TabIndex = 6;
			this->btnCategoryAdd->Text = L"Add";
			this->btnCategoryAdd->UseVisualStyleBackColor = true;
			this->btnCategoryAdd->Click += gcnew System::EventHandler(this, &MyForm::btnCategoryAdd_Click);
			// 
			// lblCategoryDescription
			// 
			this->lblCategoryDescription->AutoSize = true;
			this->lblCategoryDescription->Location = System::Drawing::Point(7, 173);
			this->lblCategoryDescription->Name = L"lblCategoryDescription";
			this->lblCategoryDescription->Size = System::Drawing::Size(63, 13);
			this->lblCategoryDescription->TabIndex = 5;
			this->lblCategoryDescription->Text = L"Description:";
			// 
			// txtCategoryDescription
			// 
			this->txtCategoryDescription->Location = System::Drawing::Point(7, 192);
			this->txtCategoryDescription->MaxLength = 100;
			this->txtCategoryDescription->Multiline = true;
			this->txtCategoryDescription->Name = L"txtCategoryDescription";
			this->txtCategoryDescription->Size = System::Drawing::Size(490, 48);
			this->txtCategoryDescription->TabIndex = 4;
			// 
			// lblCategoryName
			// 
			this->lblCategoryName->AutoSize = true;
			this->lblCategoryName->Location = System::Drawing::Point(7, 118);
			this->lblCategoryName->Name = L"lblCategoryName";
			this->lblCategoryName->Size = System::Drawing::Size(38, 13);
			this->lblCategoryName->TabIndex = 3;
			this->lblCategoryName->Text = L"Name:";
			// 
			// txtCategoryName
			// 
			this->txtCategoryName->Location = System::Drawing::Point(7, 137);
			this->txtCategoryName->Name = L"txtCategoryName";
			this->txtCategoryName->Size = System::Drawing::Size(490, 20);
			this->txtCategoryName->TabIndex = 2;
			// 
			// cmbCategorySelect
			// 
			this->cmbCategorySelect->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbCategorySelect->FormattingEnabled = true;
			this->cmbCategorySelect->Location = System::Drawing::Point(6, 75);
			this->cmbCategorySelect->Name = L"cmbCategorySelect";
			this->cmbCategorySelect->Size = System::Drawing::Size(491, 21);
			this->cmbCategorySelect->TabIndex = 1;
			this->cmbCategorySelect->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::cmbCategorySelect_SelectedIndexChanged);
			// 
			// cmbCategoryFunction
			// 
			this->cmbCategoryFunction->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbCategoryFunction->Items->AddRange(gcnew cli::array< System::Object^  >(4) {L"Add", L"Modify", L"Delete", L"Search"});
			this->cmbCategoryFunction->Location = System::Drawing::Point(7, 27);
			this->cmbCategoryFunction->Name = L"cmbCategoryFunction";
			this->cmbCategoryFunction->Size = System::Drawing::Size(114, 21);
			this->cmbCategoryFunction->TabIndex = 0;
			this->cmbCategoryFunction->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::cmbCategoryFunction_SelectedIndexChanged);
			// 
			// tpProduct
			// 
			this->tpProduct->Controls->Add(this->txtProductSearchResults);
			this->tpProduct->Controls->Add(this->btnProductSearch);
			this->tpProduct->Controls->Add(this->lblProductSearchInfo);
			this->tpProduct->Controls->Add(this->txtProductSearch);
			this->tpProduct->Controls->Add(this->cmbProductSearch);
			this->tpProduct->Controls->Add(this->lblProductSearchBy);
			this->tpProduct->Controls->Add(this->btnProductDelete);
			this->tpProduct->Controls->Add(this->txtProdID);
			this->tpProduct->Controls->Add(this->lblProductID);
			this->tpProduct->Controls->Add(this->lblProductCategory);
			this->tpProduct->Controls->Add(this->cmbProductCategorySelect);
			this->tpProduct->Controls->Add(this->btnProductModify);
			this->tpProduct->Controls->Add(this->btnProductAdd);
			this->tpProduct->Controls->Add(this->txtProductPrice);
			this->tpProduct->Controls->Add(this->lblProductPrice);
			this->tpProduct->Controls->Add(this->lblProductDescription);
			this->tpProduct->Controls->Add(this->txtProductDescription);
			this->tpProduct->Controls->Add(this->lblProductName);
			this->tpProduct->Controls->Add(this->txtProductName);
			this->tpProduct->Controls->Add(this->lblProductSelect);
			this->tpProduct->Controls->Add(this->cmbProductSelect);
			this->tpProduct->Controls->Add(this->lblProductFunction);
			this->tpProduct->Controls->Add(this->cmbProductFunction);
			this->tpProduct->Location = System::Drawing::Point(4, 22);
			this->tpProduct->Name = L"tpProduct";
			this->tpProduct->Padding = System::Windows::Forms::Padding(3);
			this->tpProduct->Size = System::Drawing::Size(503, 471);
			this->tpProduct->TabIndex = 1;
			this->tpProduct->Text = L"Product";
			this->tpProduct->UseVisualStyleBackColor = true;
			// 
			// txtProductSearchResults
			// 
			this->txtProductSearchResults->Location = System::Drawing::Point(6, 224);
			this->txtProductSearchResults->Multiline = true;
			this->txtProductSearchResults->Name = L"txtProductSearchResults";
			this->txtProductSearchResults->ReadOnly = true;
			this->txtProductSearchResults->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->txtProductSearchResults->Size = System::Drawing::Size(490, 225);
			this->txtProductSearchResults->TabIndex = 30;
			// 
			// btnProductSearch
			// 
			this->btnProductSearch->Location = System::Drawing::Point(6, 186);
			this->btnProductSearch->Name = L"btnProductSearch";
			this->btnProductSearch->Size = System::Drawing::Size(75, 23);
			this->btnProductSearch->TabIndex = 29;
			this->btnProductSearch->Text = L"Search";
			this->btnProductSearch->UseVisualStyleBackColor = true;
			this->btnProductSearch->Click += gcnew System::EventHandler(this, &MyForm::btnProductSearch_Click);
			// 
			// lblProductSearchInfo
			// 
			this->lblProductSearchInfo->AutoSize = true;
			this->lblProductSearchInfo->Location = System::Drawing::Point(7, 117);
			this->lblProductSearchInfo->Name = L"lblProductSearchInfo";
			this->lblProductSearchInfo->Size = System::Drawing::Size(142, 13);
			this->lblProductSearchInfo->TabIndex = 28;
			this->lblProductSearchInfo->Text = L"Enter Information for Search:";
			// 
			// txtProductSearch
			// 
			this->txtProductSearch->Location = System::Drawing::Point(7, 141);
			this->txtProductSearch->Name = L"txtProductSearch";
			this->txtProductSearch->Size = System::Drawing::Size(490, 20);
			this->txtProductSearch->TabIndex = 27;
			// 
			// cmbProductSearch
			// 
			this->cmbProductSearch->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbProductSearch->Items->AddRange(gcnew cli::array< System::Object^  >(5) {L"Product ID", L"Category ID", L"Name", 
				L"Description", L"Price"});
			this->cmbProductSearch->Location = System::Drawing::Point(7, 75);
			this->cmbProductSearch->Name = L"cmbProductSearch";
			this->cmbProductSearch->Size = System::Drawing::Size(114, 21);
			this->cmbProductSearch->TabIndex = 26;
			this->cmbProductSearch->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::cmbProductSearch_SelectedIndexChanged);
			// 
			// lblProductSearchBy
			// 
			this->lblProductSearchBy->AutoSize = true;
			this->lblProductSearchBy->Location = System::Drawing::Point(7, 55);
			this->lblProductSearchBy->Name = L"lblProductSearchBy";
			this->lblProductSearchBy->Size = System::Drawing::Size(59, 13);
			this->lblProductSearchBy->TabIndex = 25;
			this->lblProductSearchBy->Text = L"Search By:";
			// 
			// btnProductDelete
			// 
			this->btnProductDelete->Location = System::Drawing::Point(7, 112);
			this->btnProductDelete->Name = L"btnProductDelete";
			this->btnProductDelete->Size = System::Drawing::Size(75, 23);
			this->btnProductDelete->TabIndex = 24;
			this->btnProductDelete->Text = L"Delete";
			this->btnProductDelete->UseVisualStyleBackColor = true;
			this->btnProductDelete->Click += gcnew System::EventHandler(this, &MyForm::btnProductDelete_Click);
			// 
			// txtProdID
			// 
			this->txtProdID->Location = System::Drawing::Point(6, 126);
			this->txtProdID->Name = L"txtProdID";
			this->txtProdID->Size = System::Drawing::Size(100, 20);
			this->txtProdID->TabIndex = 23;
			// 
			// lblProductID
			// 
			this->lblProductID->AutoSize = true;
			this->lblProductID->Location = System::Drawing::Point(7, 106);
			this->lblProductID->Name = L"lblProductID";
			this->lblProductID->Size = System::Drawing::Size(61, 13);
			this->lblProductID->TabIndex = 22;
			this->lblProductID->Text = L"Product ID:";
			// 
			// lblProductCategory
			// 
			this->lblProductCategory->AutoSize = true;
			this->lblProductCategory->Location = System::Drawing::Point(7, 157);
			this->lblProductCategory->Name = L"lblProductCategory";
			this->lblProductCategory->Size = System::Drawing::Size(52, 13);
			this->lblProductCategory->TabIndex = 21;
			this->lblProductCategory->Text = L"Category:";
			// 
			// cmbProductCategorySelect
			// 
			this->cmbProductCategorySelect->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbProductCategorySelect->FormattingEnabled = true;
			this->cmbProductCategorySelect->Location = System::Drawing::Point(6, 177);
			this->cmbProductCategorySelect->Name = L"cmbProductCategorySelect";
			this->cmbProductCategorySelect->Size = System::Drawing::Size(491, 21);
			this->cmbProductCategorySelect->TabIndex = 20;
			this->cmbProductCategorySelect->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::cmbProductCategorySelect_SelectedIndexChanged);
			// 
			// btnProductModify
			// 
			this->btnProductModify->Location = System::Drawing::Point(7, 414);
			this->btnProductModify->Name = L"btnProductModify";
			this->btnProductModify->Size = System::Drawing::Size(75, 23);
			this->btnProductModify->TabIndex = 19;
			this->btnProductModify->Text = L"Modify";
			this->btnProductModify->UseVisualStyleBackColor = true;
			this->btnProductModify->Click += gcnew System::EventHandler(this, &MyForm::btnProductModify_Click);
			// 
			// btnProductAdd
			// 
			this->btnProductAdd->Location = System::Drawing::Point(6, 414);
			this->btnProductAdd->Name = L"btnProductAdd";
			this->btnProductAdd->Size = System::Drawing::Size(43, 23);
			this->btnProductAdd->TabIndex = 18;
			this->btnProductAdd->Text = L"Add";
			this->btnProductAdd->UseVisualStyleBackColor = true;
			this->btnProductAdd->Click += gcnew System::EventHandler(this, &MyForm::btnProductAdd_Click);
			// 
			// txtProductPrice
			// 
			this->txtProductPrice->Location = System::Drawing::Point(7, 372);
			this->txtProductPrice->MaxLength = 8;
			this->txtProductPrice->Name = L"txtProductPrice";
			this->txtProductPrice->Size = System::Drawing::Size(100, 20);
			this->txtProductPrice->TabIndex = 17;
			// 
			// lblProductPrice
			// 
			this->lblProductPrice->AutoSize = true;
			this->lblProductPrice->Location = System::Drawing::Point(7, 355);
			this->lblProductPrice->Name = L"lblProductPrice";
			this->lblProductPrice->Size = System::Drawing::Size(34, 13);
			this->lblProductPrice->TabIndex = 16;
			this->lblProductPrice->Text = L"Price:";
			// 
			// lblProductDescription
			// 
			this->lblProductDescription->AutoSize = true;
			this->lblProductDescription->Location = System::Drawing::Point(6, 267);
			this->lblProductDescription->Name = L"lblProductDescription";
			this->lblProductDescription->Size = System::Drawing::Size(63, 13);
			this->lblProductDescription->TabIndex = 15;
			this->lblProductDescription->Text = L"Description:";
			// 
			// txtProductDescription
			// 
			this->txtProductDescription->Location = System::Drawing::Point(6, 286);
			this->txtProductDescription->MaxLength = 100;
			this->txtProductDescription->Multiline = true;
			this->txtProductDescription->Name = L"txtProductDescription";
			this->txtProductDescription->Size = System::Drawing::Size(490, 48);
			this->txtProductDescription->TabIndex = 14;
			// 
			// lblProductName
			// 
			this->lblProductName->AutoSize = true;
			this->lblProductName->Location = System::Drawing::Point(6, 212);
			this->lblProductName->Name = L"lblProductName";
			this->lblProductName->Size = System::Drawing::Size(38, 13);
			this->lblProductName->TabIndex = 13;
			this->lblProductName->Text = L"Name:";
			// 
			// txtProductName
			// 
			this->txtProductName->Location = System::Drawing::Point(6, 231);
			this->txtProductName->Name = L"txtProductName";
			this->txtProductName->Size = System::Drawing::Size(490, 20);
			this->txtProductName->TabIndex = 12;
			// 
			// lblProductSelect
			// 
			this->lblProductSelect->AutoSize = true;
			this->lblProductSelect->Location = System::Drawing::Point(7, 55);
			this->lblProductSelect->Name = L"lblProductSelect";
			this->lblProductSelect->Size = System::Drawing::Size(47, 13);
			this->lblProductSelect->TabIndex = 11;
			this->lblProductSelect->Text = L"Product:";
			// 
			// cmbProductSelect
			// 
			this->cmbProductSelect->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbProductSelect->FormattingEnabled = true;
			this->cmbProductSelect->Location = System::Drawing::Point(6, 75);
			this->cmbProductSelect->Name = L"cmbProductSelect";
			this->cmbProductSelect->Size = System::Drawing::Size(491, 21);
			this->cmbProductSelect->TabIndex = 10;
			this->cmbProductSelect->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::cmbProductSelect_SelectedIndexChanged);
			// 
			// lblProductFunction
			// 
			this->lblProductFunction->AutoSize = true;
			this->lblProductFunction->Location = System::Drawing::Point(7, 7);
			this->lblProductFunction->Name = L"lblProductFunction";
			this->lblProductFunction->Size = System::Drawing::Size(51, 13);
			this->lblProductFunction->TabIndex = 9;
			this->lblProductFunction->Text = L"Function:";
			// 
			// cmbProductFunction
			// 
			this->cmbProductFunction->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbProductFunction->Items->AddRange(gcnew cli::array< System::Object^  >(4) {L"Add", L"Modify", L"Delete", L"Search"});
			this->cmbProductFunction->Location = System::Drawing::Point(7, 27);
			this->cmbProductFunction->Name = L"cmbProductFunction";
			this->cmbProductFunction->Size = System::Drawing::Size(114, 21);
			this->cmbProductFunction->TabIndex = 8;
			this->cmbProductFunction->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::cmbProductFunction_SelectedIndexChanged);
			// 
			// tpSales
			// 
			this->tpSales->Controls->Add(this->btnSalesModifyReceipt);
			this->tpSales->Controls->Add(this->lblsalesProductList);
			this->tpSales->Controls->Add(this->btnSalesSearch);
			this->tpSales->Controls->Add(this->dtSalesReceiptDate);
			this->tpSales->Controls->Add(this->btnSalesModify);
			this->tpSales->Controls->Add(this->btnSalesCreateReceipt);
			this->tpSales->Controls->Add(this->txtSalesProductDiscount);
			this->tpSales->Controls->Add(this->lblSalesProductDiscount);
			this->tpSales->Controls->Add(this->lblSalesReceiptSelect);
			this->tpSales->Controls->Add(this->cmbSalesReceiptSelect);
			this->tpSales->Controls->Add(this->btnSalesRemoveProduct);
			this->tpSales->Controls->Add(this->lstSalesProductList);
			this->tpSales->Controls->Add(this->btnSalesAddProduct);
			this->tpSales->Controls->Add(this->txtSalesProductQuantity);
			this->tpSales->Controls->Add(this->lblSaleProductQuantity);
			this->tpSales->Controls->Add(this->lblSalesProductSelect);
			this->tpSales->Controls->Add(this->cmbSalesProductSelect);
			this->tpSales->Controls->Add(this->label1);
			this->tpSales->Controls->Add(this->cmbSalesFunction);
			this->tpSales->Location = System::Drawing::Point(4, 22);
			this->tpSales->Name = L"tpSales";
			this->tpSales->Padding = System::Windows::Forms::Padding(3);
			this->tpSales->Size = System::Drawing::Size(503, 471);
			this->tpSales->TabIndex = 2;
			this->tpSales->Text = L"Sales";
			this->tpSales->UseVisualStyleBackColor = true;
			// 
			// btnSalesModifyReceipt
			// 
			this->btnSalesModifyReceipt->Location = System::Drawing::Point(6, 363);
			this->btnSalesModifyReceipt->Name = L"btnSalesModifyReceipt";
			this->btnSalesModifyReceipt->Size = System::Drawing::Size(75, 23);
			this->btnSalesModifyReceipt->TabIndex = 54;
			this->btnSalesModifyReceipt->Text = L"Modify Date";
			this->btnSalesModifyReceipt->UseVisualStyleBackColor = true;

			this->btnSalesModifyReceipt->Click += gcnew System::EventHandler(this, &MyForm::btnSalesModifyReceipt_Click);
			// 
			// lblsalesProductList
			// 
			this->lblsalesProductList->AutoSize = true;
			this->lblsalesProductList->Location = System::Drawing::Point(150, 159);
			this->lblsalesProductList->Name = L"lblsalesProductList";
			this->lblsalesProductList->Size = System::Drawing::Size(209, 13);
			this->lblsalesProductList->TabIndex = 52;
			this->lblsalesProductList->Text = L"productID | quantity_sold | name | discount ";
			// 
			// btnSalesSearch
			// 
			this->btnSalesSearch->Location = System::Drawing::Point(7, 333);
			this->btnSalesSearch->Name = L"btnSalesSearch";
			this->btnSalesSearch->Size = System::Drawing::Size(74, 23);
			this->btnSalesSearch->TabIndex = 53;
			this->btnSalesSearch->Text = L"Search";
			this->btnSalesSearch->UseVisualStyleBackColor = true;
			this->btnSalesSearch->Click += gcnew System::EventHandler(this, &MyForm::btnSalesSearch_Click);
			// 
			// dtSalesReceiptDate
			// 
			this->dtSalesReceiptDate->Location = System::Drawing::Point(7, 306);
			this->dtSalesReceiptDate->MaxDate = System::DateTime(2999, 12, 31, 0, 0, 0, 0);
			this->dtSalesReceiptDate->MinDate = System::DateTime(1901, 1, 1, 0, 0, 0, 0);
			this->dtSalesReceiptDate->Name = L"dtSalesReceiptDate";
			this->dtSalesReceiptDate->Size = System::Drawing::Size(200, 20);
			this->dtSalesReceiptDate->TabIndex = 35;
			// 
			// btnSalesModify
			// 
			this->btnSalesModify->Location = System::Drawing::Point(87, 361);
			this->btnSalesModify->Name = L"btnSalesModify";
			this->btnSalesModify->Size = System::Drawing::Size(75, 23);
			this->btnSalesModify->TabIndex = 34;
			this->btnSalesModify->Text = L"Modify";
			this->btnSalesModify->UseVisualStyleBackColor = true;
			this->btnSalesModify->Click += gcnew System::EventHandler(this, &MyForm::btnSalesModify_Click);
			// 
			// btnSalesCreateReceipt
			// 
			this->btnSalesCreateReceipt->Location = System::Drawing::Point(87, 332);
			this->btnSalesCreateReceipt->Name = L"btnSalesCreateReceipt";
			this->btnSalesCreateReceipt->Size = System::Drawing::Size(99, 23);
			this->btnSalesCreateReceipt->TabIndex = 33;
			this->btnSalesCreateReceipt->Text = L"Create Receipt";
			this->btnSalesCreateReceipt->UseVisualStyleBackColor = true;
			this->btnSalesCreateReceipt->Click += gcnew System::EventHandler(this, &MyForm::btnSalesCreateReceipt_Click);
			// 
			// txtSalesProductDiscount
			// 
			this->txtSalesProductDiscount->Location = System::Drawing::Point(6, 231);
			this->txtSalesProductDiscount->Name = L"txtSalesProductDiscount";
			this->txtSalesProductDiscount->Size = System::Drawing::Size(100, 20);
			this->txtSalesProductDiscount->TabIndex = 32;

			this->txtSalesProductDiscount->TextChanged += gcnew System::EventHandler(this, &MyForm::txtSalesProductDiscount_TextChanged);
			// 
			// lblSalesProductDiscount
			// 
			this->lblSalesProductDiscount->AutoSize = true;
			this->lblSalesProductDiscount->Location = System::Drawing::Point(7, 211);
			this->lblSalesProductDiscount->Name = L"lblSalesProductDiscount";
			this->lblSalesProductDiscount->Size = System::Drawing::Size(92, 13);
			this->lblSalesProductDiscount->TabIndex = 31;
			this->lblSalesProductDiscount->Text = L"Product Discount:";
			// 
			// lblSalesReceiptSelect
			// 
			this->lblSalesReceiptSelect->AutoSize = true;
			this->lblSalesReceiptSelect->Location = System::Drawing::Point(7, 57);
			this->lblSalesReceiptSelect->Name = L"lblSalesReceiptSelect";
			this->lblSalesReceiptSelect->Size = System::Drawing::Size(47, 13);
			this->lblSalesReceiptSelect->TabIndex = 30;
			this->lblSalesReceiptSelect->Text = L"Receipt:";
			// 
			// cmbSalesReceiptSelect
			// 
			this->cmbSalesReceiptSelect->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbSalesReceiptSelect->FormattingEnabled = true;
			this->cmbSalesReceiptSelect->Location = System::Drawing::Point(6, 77);
			this->cmbSalesReceiptSelect->Name = L"cmbSalesReceiptSelect";
			this->cmbSalesReceiptSelect->Size = System::Drawing::Size(491, 21);
			this->cmbSalesReceiptSelect->TabIndex = 29;
			this->cmbSalesReceiptSelect->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::cmbSalesReceiptSelect_SelectedIndexChanged);

			// 
			// btnSalesRemoveProduct
			// 
			this->btnSalesRemoveProduct->Location = System::Drawing::Point(399, 280);
			this->btnSalesRemoveProduct->Name = L"btnSalesRemoveProduct";
			this->btnSalesRemoveProduct->Size = System::Drawing::Size(98, 23);
			this->btnSalesRemoveProduct->TabIndex = 28;
			this->btnSalesRemoveProduct->Text = L"Remove Product";
			this->btnSalesRemoveProduct->UseVisualStyleBackColor = true;
			this->btnSalesRemoveProduct->Click += gcnew System::EventHandler(this, &MyForm::btnSalesRemoveProduct_Click);
			// 
			// lstSalesProductList
			// 
			this->lstSalesProductList->FormattingEnabled = true;
			this->lstSalesProductList->Location = System::Drawing::Point(153, 179);
			this->lstSalesProductList->Name = L"lstSalesProductList";
			this->lstSalesProductList->Size = System::Drawing::Size(344, 95);
			this->lstSalesProductList->TabIndex = 27;
			this->lstSalesProductList->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::lstSalesProductList_SelectedIndexChanged);this->lstSalesProductList->TabIndex = 27;

			// 
			// btnSalesAddProduct
			// 
			this->btnSalesAddProduct->Location = System::Drawing::Point(7, 261);
			this->btnSalesAddProduct->Name = L"btnSalesAddProduct";
			this->btnSalesAddProduct->Size = System::Drawing::Size(75, 23);
			this->btnSalesAddProduct->TabIndex = 26;
			this->btnSalesAddProduct->Text = L"Add To List";
			this->btnSalesAddProduct->UseVisualStyleBackColor = true;
			this->btnSalesAddProduct->Click += gcnew System::EventHandler(this, &MyForm::btnSalesAddProduct_Click);
			// 
			// txtSalesProductQuantity
			// 
			this->txtSalesProductQuantity->Location = System::Drawing::Point(6, 179);
			this->txtSalesProductQuantity->Name = L"txtSalesProductQuantity";
			this->txtSalesProductQuantity->Size = System::Drawing::Size(100, 20);
			this->txtSalesProductQuantity->TabIndex = 25;

			this->txtSalesProductQuantity->TextChanged += gcnew System::EventHandler(this, &MyForm::txtSalesProductQuantity_TextChanged);
			// 
			// lblSaleProductQuantity
			// 
			this->lblSaleProductQuantity->AutoSize = true;
			this->lblSaleProductQuantity->Location = System::Drawing::Point(7, 159);
			this->lblSaleProductQuantity->Name = L"lblSaleProductQuantity";
			this->lblSaleProductQuantity->Size = System::Drawing::Size(89, 13);
			this->lblSaleProductQuantity->TabIndex = 24;
			this->lblSaleProductQuantity->Text = L"Product Quantity:";
			// 
			// lblSalesProductSelect
			// 
			this->lblSalesProductSelect->AutoSize = true;
			this->lblSalesProductSelect->Location = System::Drawing::Point(7, 104);
			this->lblSalesProductSelect->Name = L"lblSalesProductSelect";
			this->lblSalesProductSelect->Size = System::Drawing::Size(47, 13);
			this->lblSalesProductSelect->TabIndex = 13;
			this->lblSalesProductSelect->Text = L"Product:";
			// 
			// cmbSalesProductSelect
			// 
			this->cmbSalesProductSelect->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbSalesProductSelect->FormattingEnabled = true;
			this->cmbSalesProductSelect->Location = System::Drawing::Point(6, 124);
			this->cmbSalesProductSelect->Name = L"cmbSalesProductSelect";
			this->cmbSalesProductSelect->Size = System::Drawing::Size(491, 21);
			this->cmbSalesProductSelect->TabIndex = 12;
			this->cmbSalesProductSelect->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::cmbSalesProductSelect_SelectedIndexChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(7, 7);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(51, 13);
			this->label1->TabIndex = 11;
			this->label1->Text = L"Function:";
			// 
			// cmbSalesFunction
			// 
			this->cmbSalesFunction->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbSalesFunction->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"Add", L"Modify", L"Search"});
			this->cmbSalesFunction->Location = System::Drawing::Point(7, 27);
			this->cmbSalesFunction->Name = L"cmbSalesFunction";
			this->cmbSalesFunction->Size = System::Drawing::Size(114, 21);
			this->cmbSalesFunction->TabIndex = 10;
			this->cmbSalesFunction->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::cmbSalesFunction_SelectedIndexChanged);
			// 
			// tpInvoices
			// 
			this->tpInvoices->Controls->Add(this->btnInvoiceModifyInvoiceItem);
			this->tpInvoices->Controls->Add(this->btnInvoiceSearch);
			this->tpInvoices->Controls->Add(this->lblInvoiceProductList);
			this->tpInvoices->Controls->Add(this->dtInvoiceDate);
			this->tpInvoices->Controls->Add(this->btnInvoiceModify);
			this->tpInvoices->Controls->Add(this->btnInvoiceCreateInvoice);
			this->tpInvoices->Controls->Add(this->lblInvoiceSelect);
			this->tpInvoices->Controls->Add(this->cmbInvoiceSelect);
			this->tpInvoices->Controls->Add(this->btnInvoiceRemoveProduct);
			this->tpInvoices->Controls->Add(this->lstInvoiceProductList);
			this->tpInvoices->Controls->Add(this->btnInvoiceAddProduct);
			this->tpInvoices->Controls->Add(this->txtInvoiceProductQuantity);
			this->tpInvoices->Controls->Add(this->lblInvoiceProductQuantity);
			this->tpInvoices->Controls->Add(this->lblInvoiceProductSelect);
			this->tpInvoices->Controls->Add(this->cmbInvoiceProductSelect);
			this->tpInvoices->Controls->Add(this->lblInvoiceFunction);
			this->tpInvoices->Controls->Add(this->cmbInvoiceFunction);
			this->tpInvoices->Location = System::Drawing::Point(4, 22);
			this->tpInvoices->Name = L"tpInvoices";
			this->tpInvoices->Padding = System::Windows::Forms::Padding(3);
			this->tpInvoices->Size = System::Drawing::Size(503, 471);
			this->tpInvoices->TabIndex = 3;
			this->tpInvoices->Text = L"Invoices";
			this->tpInvoices->UseVisualStyleBackColor = true;
			// 
			// btnInvoiceModifyInvoiceItem
			// 
			this->btnInvoiceModifyInvoiceItem->Location = System::Drawing::Point(7, 234);
			this->btnInvoiceModifyInvoiceItem->Name = L"btnInvoiceModifyInvoiceItem";
			this->btnInvoiceModifyInvoiceItem->Size = System::Drawing::Size(89, 23);
			this->btnInvoiceModifyInvoiceItem->TabIndex = 54;
			this->btnInvoiceModifyInvoiceItem->Text = L"Modify Item";
			this->btnInvoiceModifyInvoiceItem->UseVisualStyleBackColor = true;
			this->btnInvoiceModifyInvoiceItem->Click += gcnew System::EventHandler(this, &MyForm::btnInvoiceModifyInvoiceItem_Click);
			// 
			// btnInvoiceSearch
			// 
			this->btnInvoiceSearch->Location = System::Drawing::Point(7, 333);
			this->btnInvoiceSearch->Name = L"btnInvoiceSearch";
			this->btnInvoiceSearch->Size = System::Drawing::Size(74, 23);
			this->btnInvoiceSearch->TabIndex = 53;
			this->btnInvoiceSearch->Text = L"Search";
			this->btnInvoiceSearch->UseVisualStyleBackColor = true;
			this->btnInvoiceSearch->Click += gcnew System::EventHandler(this, &MyForm::btnInvoiceSearch_Click);
			// 
			// lblInvoiceProductList
			// 
			this->lblInvoiceProductList->AutoSize = true;
			this->lblInvoiceProductList->Location = System::Drawing::Point(150, 159);
			this->lblInvoiceProductList->Name = L"lblInvoiceProductList";
			this->lblInvoiceProductList->Size = System::Drawing::Size(144, 13);
			this->lblInvoiceProductList->TabIndex = 52;
			this->lblInvoiceProductList->Text = L"Product ID | Name | Quantity ";
			// 
			// dtInvoiceDate
			// 
			this->dtInvoiceDate->Location = System::Drawing::Point(7, 298);
			this->dtInvoiceDate->MaxDate = System::DateTime(2999, 12, 31, 0, 0, 0, 0);
			this->dtInvoiceDate->MinDate = System::DateTime(1901, 1, 1, 0, 0, 0, 0);
			this->dtInvoiceDate->Name = L"dtInvoiceDate";
			this->dtInvoiceDate->Size = System::Drawing::Size(200, 20);
			this->dtInvoiceDate->TabIndex = 51;
			// 
			// btnInvoiceModify
			// 
			this->btnInvoiceModify->Location = System::Drawing::Point(7, 333);
			this->btnInvoiceModify->Name = L"btnInvoiceModify";
			this->btnInvoiceModify->Size = System::Drawing::Size(98, 23);
			this->btnInvoiceModify->TabIndex = 50;
			this->btnInvoiceModify->Text = L"Modify Date";
			this->btnInvoiceModify->UseVisualStyleBackColor = true;
			this->btnInvoiceModify->Click += gcnew System::EventHandler(this, &MyForm::btnInvoiceModify_Click_1);
			// 
			// btnInvoiceCreateInvoice
			// 
			this->btnInvoiceCreateInvoice->Location = System::Drawing::Point(6, 333);
			this->btnInvoiceCreateInvoice->Name = L"btnInvoiceCreateInvoice";
			this->btnInvoiceCreateInvoice->Size = System::Drawing::Size(99, 23);
			this->btnInvoiceCreateInvoice->TabIndex = 49;
			this->btnInvoiceCreateInvoice->Text = L"Create Invoice";
			this->btnInvoiceCreateInvoice->UseVisualStyleBackColor = true;
			this->btnInvoiceCreateInvoice->Click += gcnew System::EventHandler(this, &MyForm::btnInvoiceCreateInvoice_Click);
			// 
			// lblInvoiceSelect
			// 
			this->lblInvoiceSelect->AutoSize = true;
			this->lblInvoiceSelect->Location = System::Drawing::Point(7, 57);
			this->lblInvoiceSelect->Name = L"lblInvoiceSelect";
			this->lblInvoiceSelect->Size = System::Drawing::Size(45, 13);
			this->lblInvoiceSelect->TabIndex = 46;
			this->lblInvoiceSelect->Text = L"Invoice:";
			// 
			// cmbInvoiceSelect
			// 
			this->cmbInvoiceSelect->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbInvoiceSelect->FormattingEnabled = true;
			this->cmbInvoiceSelect->Location = System::Drawing::Point(6, 77);
			this->cmbInvoiceSelect->Name = L"cmbInvoiceSelect";
			this->cmbInvoiceSelect->Size = System::Drawing::Size(491, 21);
			this->cmbInvoiceSelect->TabIndex = 45;
			this->cmbInvoiceSelect->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::cmbInvoiceSelect_SelectedIndexChanged);
			// 
			// btnInvoiceRemoveProduct
			// 
			this->btnInvoiceRemoveProduct->Location = System::Drawing::Point(399, 295);
			this->btnInvoiceRemoveProduct->Name = L"btnInvoiceRemoveProduct";
			this->btnInvoiceRemoveProduct->Size = System::Drawing::Size(98, 23);
			this->btnInvoiceRemoveProduct->TabIndex = 44;
			this->btnInvoiceRemoveProduct->Text = L"Remove Product";
			this->btnInvoiceRemoveProduct->UseVisualStyleBackColor = true;
			this->btnInvoiceRemoveProduct->Click += gcnew System::EventHandler(this, &MyForm::btnInvoiceRemoveProduct_Click);
			// 
			// lstInvoiceProductList
			// 
			this->lstInvoiceProductList->FormattingEnabled = true;
			this->lstInvoiceProductList->Location = System::Drawing::Point(153, 179);
			this->lstInvoiceProductList->Name = L"lstInvoiceProductList";
			this->lstInvoiceProductList->Size = System::Drawing::Size(344, 95);
			this->lstInvoiceProductList->TabIndex = 43;
			this->lstInvoiceProductList->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::lstInvoiceProductList_SelectedIndexChanged);
			// 
			// btnInvoiceAddProduct
			// 
			this->btnInvoiceAddProduct->Location = System::Drawing::Point(6, 205);
			this->btnInvoiceAddProduct->Name = L"btnInvoiceAddProduct";
			this->btnInvoiceAddProduct->Size = System::Drawing::Size(75, 23);
			this->btnInvoiceAddProduct->TabIndex = 42;
			this->btnInvoiceAddProduct->Text = L"Add To List";
			this->btnInvoiceAddProduct->UseVisualStyleBackColor = true;
			this->btnInvoiceAddProduct->Click += gcnew System::EventHandler(this, &MyForm::btnInvoiceAddProduct_Click);
			// 
			// txtInvoiceProductQuantity
			// 
			this->txtInvoiceProductQuantity->Location = System::Drawing::Point(6, 179);
			this->txtInvoiceProductQuantity->Name = L"txtInvoiceProductQuantity";
			this->txtInvoiceProductQuantity->Size = System::Drawing::Size(100, 20);
			this->txtInvoiceProductQuantity->TabIndex = 41;
			this->txtInvoiceProductQuantity->TextChanged += gcnew System::EventHandler(this, &MyForm::txtInvoiceProductQuantity_TextChanged);
			// 
			// lblInvoiceProductQuantity
			// 
			this->lblInvoiceProductQuantity->AutoSize = true;
			this->lblInvoiceProductQuantity->Location = System::Drawing::Point(7, 159);
			this->lblInvoiceProductQuantity->Name = L"lblInvoiceProductQuantity";
			this->lblInvoiceProductQuantity->Size = System::Drawing::Size(89, 13);
			this->lblInvoiceProductQuantity->TabIndex = 40;
			this->lblInvoiceProductQuantity->Text = L"Product Quantity:";
			// 
			// lblInvoiceProductSelect
			// 
			this->lblInvoiceProductSelect->AutoSize = true;
			this->lblInvoiceProductSelect->Location = System::Drawing::Point(7, 104);
			this->lblInvoiceProductSelect->Name = L"lblInvoiceProductSelect";
			this->lblInvoiceProductSelect->Size = System::Drawing::Size(47, 13);
			this->lblInvoiceProductSelect->TabIndex = 39;
			this->lblInvoiceProductSelect->Text = L"Product:";
			// 
			// cmbInvoiceProductSelect
			// 
			this->cmbInvoiceProductSelect->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbInvoiceProductSelect->FormattingEnabled = true;
			this->cmbInvoiceProductSelect->Location = System::Drawing::Point(6, 124);
			this->cmbInvoiceProductSelect->Name = L"cmbInvoiceProductSelect";
			this->cmbInvoiceProductSelect->Size = System::Drawing::Size(491, 21);
			this->cmbInvoiceProductSelect->TabIndex = 38;
			this->cmbInvoiceProductSelect->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::cmbInvoiceProductSelect_SelectedIndexChanged);
			// 
			// lblInvoiceFunction
			// 
			this->lblInvoiceFunction->AutoSize = true;
			this->lblInvoiceFunction->Location = System::Drawing::Point(7, 7);
			this->lblInvoiceFunction->Name = L"lblInvoiceFunction";
			this->lblInvoiceFunction->Size = System::Drawing::Size(51, 13);
			this->lblInvoiceFunction->TabIndex = 37;
			this->lblInvoiceFunction->Text = L"Function:";
			// 
			// cmbInvoiceFunction
			// 
			this->cmbInvoiceFunction->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbInvoiceFunction->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"Add", L"Modify", L"Search"});
			this->cmbInvoiceFunction->Location = System::Drawing::Point(7, 27);
			this->cmbInvoiceFunction->Name = L"cmbInvoiceFunction";
			this->cmbInvoiceFunction->Size = System::Drawing::Size(114, 21);
			this->cmbInvoiceFunction->TabIndex = 36;
			this->cmbInvoiceFunction->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::cmbInvoiceFunction_SelectedIndexChanged);
			// 
			// tpReturns
			// 
			this->tpReturns->Controls->Add(this->txtSearchReturns);
			this->tpReturns->Controls->Add(this->lblSearchValue);
			this->tpReturns->Controls->Add(this->cmbSearchBy);
			this->tpReturns->Controls->Add(this->lblSearchBy);
			this->tpReturns->Controls->Add(this->txtSearchValue);
			this->tpReturns->Controls->Add(this->dateReturned);
			this->tpReturns->Controls->Add(this->btnFunction);
			this->tpReturns->Controls->Add(this->lblReturns);
			this->tpReturns->Controls->Add(this->cmbReturns);
			this->tpReturns->Controls->Add(this->txtQuantityReturned);
			this->tpReturns->Controls->Add(this->lblQuantityReturned);
			this->tpReturns->Controls->Add(this->lblSale);
			this->tpReturns->Controls->Add(this->cmbSale);
			this->tpReturns->Controls->Add(this->lblFunction);
			this->tpReturns->Controls->Add(this->cmbReturnFunction);
			this->tpReturns->Location = System::Drawing::Point(4, 22);
			this->tpReturns->Name = L"tpReturns";
			this->tpReturns->Padding = System::Windows::Forms::Padding(3);
			this->tpReturns->Size = System::Drawing::Size(503, 471);
			this->tpReturns->TabIndex = 4;
			this->tpReturns->Text = L"Returns";
			this->tpReturns->UseVisualStyleBackColor = true;
			// 
			// txtSearchReturns
			// 
			this->txtSearchReturns->Location = System::Drawing::Point(6, 73);
			this->txtSearchReturns->Multiline = true;
			this->txtSearchReturns->Name = L"txtSearchReturns";
			this->txtSearchReturns->ReadOnly = true;
			this->txtSearchReturns->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->txtSearchReturns->Size = System::Drawing::Size(490, 141);
			this->txtSearchReturns->TabIndex = 85;
			this->txtSearchReturns->Visible = false;
			// 
			// lblSearchValue
			// 
			this->lblSearchValue->AutoSize = true;
			this->lblSearchValue->Location = System::Drawing::Point(323, 7);
			this->lblSearchValue->Name = L"lblSearchValue";
			this->lblSearchValue->Size = System::Drawing::Size(74, 13);
			this->lblSearchValue->TabIndex = 84;
			this->lblSearchValue->Text = L"Search Value:";
			this->lblSearchValue->Visible = false;
			// 
			// cmbSearchBy
			// 
			this->cmbSearchBy->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbSearchBy->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"sales_id", L"quantity_returned", L"date_returned"});
			this->cmbSearchBy->Location = System::Drawing::Point(147, 27);
			this->cmbSearchBy->Name = L"cmbSearchBy";
			this->cmbSearchBy->Size = System::Drawing::Size(141, 21);
			this->cmbSearchBy->TabIndex = 83;
			this->cmbSearchBy->Visible = false;
			this->cmbSearchBy->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::cmbSearchBy_SelectedIndexChanged);
			// 
			// lblSearchBy
			// 
			this->lblSearchBy->AutoSize = true;
			this->lblSearchBy->Location = System::Drawing::Point(144, 7);
			this->lblSearchBy->Name = L"lblSearchBy";
			this->lblSearchBy->Size = System::Drawing::Size(59, 13);
			this->lblSearchBy->TabIndex = 82;
			this->lblSearchBy->Text = L"Search By:";
			this->lblSearchBy->Visible = false;
			// 
			// txtSearchValue
			// 
			this->txtSearchValue->Location = System::Drawing::Point(326, 27);
			this->txtSearchValue->Name = L"txtSearchValue";
			this->txtSearchValue->Size = System::Drawing::Size(148, 20);
			this->txtSearchValue->TabIndex = 81;
			this->txtSearchValue->Visible = false;
			// 
			// dateReturned
			// 
			this->dateReturned->Location = System::Drawing::Point(10, 230);
			this->dateReturned->MaxDate = System::DateTime(2999, 12, 31, 0, 0, 0, 0);
			this->dateReturned->MinDate = System::DateTime(1901, 1, 1, 0, 0, 0, 0);
			this->dateReturned->Name = L"dateReturned";
			this->dateReturned->Size = System::Drawing::Size(200, 20);
			this->dateReturned->TabIndex = 80;
			this->dateReturned->Visible = false;
			// 
			// btnFunction
			// 
			this->btnFunction->Location = System::Drawing::Point(6, 266);
			this->btnFunction->Name = L"btnFunction";
			this->btnFunction->Size = System::Drawing::Size(99, 23);
			this->btnFunction->TabIndex = 79;
			this->btnFunction->Text = L"Add Return";
			this->btnFunction->UseVisualStyleBackColor = true;
			this->btnFunction->Visible = false;
			this->btnFunction->Click += gcnew System::EventHandler(this, &MyForm::btnFunction_Click);
			// 
			// lblReturns
			// 
			this->lblReturns->AutoSize = true;
			this->lblReturns->Location = System::Drawing::Point(7, 57);
			this->lblReturns->Name = L"lblReturns";
			this->lblReturns->Size = System::Drawing::Size(47, 13);
			this->lblReturns->TabIndex = 78;
			this->lblReturns->Text = L"Returns:";
			this->lblReturns->Visible = false;
			// 
			// cmbReturns
			// 
			this->cmbReturns->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbReturns->FormattingEnabled = true;
			this->cmbReturns->Location = System::Drawing::Point(6, 77);
			this->cmbReturns->Name = L"cmbReturns";
			this->cmbReturns->Size = System::Drawing::Size(491, 21);
			this->cmbReturns->TabIndex = 77;
			this->cmbReturns->Visible = false;
			this->cmbReturns->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::cmbReturns_SelectedIndexChanged);
			// 
			// txtQuantityReturned
			// 
			this->txtQuantityReturned->Location = System::Drawing::Point(6, 179);
			this->txtQuantityReturned->Name = L"txtQuantityReturned";
			this->txtQuantityReturned->Size = System::Drawing::Size(100, 20);
			this->txtQuantityReturned->TabIndex = 76;
			this->txtQuantityReturned->Visible = false;
			// 
			// lblQuantityReturned
			// 
			this->lblQuantityReturned->AutoSize = true;
			this->lblQuantityReturned->Location = System::Drawing::Point(7, 159);
			this->lblQuantityReturned->Name = L"lblQuantityReturned";
			this->lblQuantityReturned->Size = System::Drawing::Size(96, 13);
			this->lblQuantityReturned->TabIndex = 75;
			this->lblQuantityReturned->Text = L"Quantity Returned:";
			this->lblQuantityReturned->Visible = false;
			// 
			// lblSale
			// 
			this->lblSale->AutoSize = true;
			this->lblSale->Location = System::Drawing::Point(7, 104);
			this->lblSale->Name = L"lblSale";
			this->lblSale->Size = System::Drawing::Size(31, 13);
			this->lblSale->TabIndex = 74;
			this->lblSale->Text = L"Sale:";
			this->lblSale->Visible = false;
			// 
			// cmbSale
			// 
			this->cmbSale->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbSale->FormattingEnabled = true;
			this->cmbSale->Location = System::Drawing::Point(6, 124);
			this->cmbSale->Name = L"cmbSale";
			this->cmbSale->Size = System::Drawing::Size(491, 21);
			this->cmbSale->TabIndex = 73;
			this->cmbSale->Visible = false;
			// 
			// lblFunction
			// 
			this->lblFunction->AutoSize = true;
			this->lblFunction->Location = System::Drawing::Point(7, 7);
			this->lblFunction->Name = L"lblFunction";
			this->lblFunction->Size = System::Drawing::Size(51, 13);
			this->lblFunction->TabIndex = 72;
			this->lblFunction->Text = L"Function:";
			// 
			// cmbReturnFunction
			// 
			this->cmbReturnFunction->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbReturnFunction->Items->AddRange(gcnew cli::array< System::Object^  >(4) {L"Add", L"Modify", L"Delete", L"Search"});
			this->cmbReturnFunction->Location = System::Drawing::Point(7, 27);
			this->cmbReturnFunction->Name = L"cmbReturnFunction";
			this->cmbReturnFunction->Size = System::Drawing::Size(114, 21);
			this->cmbReturnFunction->TabIndex = 71;
			this->cmbReturnFunction->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::cmbReturnFunction_SelectedIndexChanged);
			// 
			// tpReports
			// 
			this->tpReports->Controls->Add(this->btnReportGenerate);
			this->tpReports->Controls->Add(this->cmbReportCategorySelect);
			this->tpReports->Controls->Add(this->lblReportCategorySelect);
			this->tpReports->Controls->Add(this->lblReportEndDate);
			this->tpReports->Controls->Add(this->dtReportEndDate);
			this->tpReports->Controls->Add(this->lblReportStartDate);
			this->tpReports->Controls->Add(this->dtReportStartDate);
			this->tpReports->Controls->Add(this->lblReportDescription);
			this->tpReports->Controls->Add(this->txtReportDescription);
			this->tpReports->Controls->Add(this->cmbReportSelect);
			this->tpReports->Controls->Add(this->lblReportSelect);
			this->tpReports->Location = System::Drawing::Point(4, 22);
			this->tpReports->Name = L"tpReports";
			this->tpReports->Padding = System::Windows::Forms::Padding(3);
			this->tpReports->Size = System::Drawing::Size(503, 471);
			this->tpReports->TabIndex = 5;
			this->tpReports->Text = L"Reports";
			this->tpReports->UseVisualStyleBackColor = true;
			// 
			// btnReportGenerate
			// 
			this->btnReportGenerate->Location = System::Drawing::Point(8, 230);
			this->btnReportGenerate->Name = L"btnReportGenerate";
			this->btnReportGenerate->Size = System::Drawing::Size(111, 23);
			this->btnReportGenerate->TabIndex = 22;
			this->btnReportGenerate->Text = L"Generate Report";
			this->btnReportGenerate->UseVisualStyleBackColor = true;
			this->btnReportGenerate->Click += gcnew System::EventHandler(this, &MyForm::btnReportGenerate_Click);
			// 
			// cmbReportCategorySelect
			// 
			this->cmbReportCategorySelect->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbReportCategorySelect->FormattingEnabled = true;
			this->cmbReportCategorySelect->Location = System::Drawing::Point(6, 190);
			this->cmbReportCategorySelect->Name = L"cmbReportCategorySelect";
			this->cmbReportCategorySelect->Size = System::Drawing::Size(491, 21);
			this->cmbReportCategorySelect->TabIndex = 21;
			// 
			// lblReportCategorySelect
			// 
			this->lblReportCategorySelect->AutoSize = true;
			this->lblReportCategorySelect->Location = System::Drawing::Point(6, 169);
			this->lblReportCategorySelect->Name = L"lblReportCategorySelect";
			this->lblReportCategorySelect->Size = System::Drawing::Size(52, 13);
			this->lblReportCategorySelect->TabIndex = 8;
			this->lblReportCategorySelect->Text = L"Category:";
			// 
			// lblReportEndDate
			// 
			this->lblReportEndDate->AutoSize = true;
			this->lblReportEndDate->Location = System::Drawing::Point(5, 114);
			this->lblReportEndDate->Name = L"lblReportEndDate";
			this->lblReportEndDate->Size = System::Drawing::Size(55, 13);
			this->lblReportEndDate->TabIndex = 7;
			this->lblReportEndDate->Text = L"End Date:";
			// 
			// dtReportEndDate
			// 
			this->dtReportEndDate->Location = System::Drawing::Point(8, 135);
			this->dtReportEndDate->Name = L"dtReportEndDate";
			this->dtReportEndDate->Size = System::Drawing::Size(154, 20);
			this->dtReportEndDate->TabIndex = 6;
			this->dtReportEndDate->ValueChanged += gcnew System::EventHandler(this, &MyForm::dtReportEndDate_ValueChanged);

			// 
			// lblReportStartDate
			// 
			this->lblReportStartDate->AutoSize = true;
			this->lblReportStartDate->Location = System::Drawing::Point(6, 61);
			this->lblReportStartDate->Name = L"lblReportStartDate";
			this->lblReportStartDate->Size = System::Drawing::Size(58, 13);
			this->lblReportStartDate->TabIndex = 5;
			this->lblReportStartDate->Text = L"Start Date:";
			// 
			// dtReportStartDate
			// 
			this->dtReportStartDate->Location = System::Drawing::Point(8, 82);
			this->dtReportStartDate->Name = L"dtReportStartDate";
			this->dtReportStartDate->Size = System::Drawing::Size(154, 20);
			this->dtReportStartDate->TabIndex = 4;
			// 
			// lblReportDescription
			// 
			this->lblReportDescription->AutoSize = true;
			this->lblReportDescription->Location = System::Drawing::Point(203, 7);
			this->lblReportDescription->Name = L"lblReportDescription";
			this->lblReportDescription->Size = System::Drawing::Size(98, 13);
			this->lblReportDescription->TabIndex = 3;
			this->lblReportDescription->Text = L"Report Description:";
			// 
			// txtReportDescription
			// 
			this->txtReportDescription->Location = System::Drawing::Point(206, 28);
			this->txtReportDescription->Multiline = true;
			this->txtReportDescription->Name = L"txtReportDescription";
			this->txtReportDescription->ReadOnly = true;
			this->txtReportDescription->Size = System::Drawing::Size(276, 73);
			this->txtReportDescription->TabIndex = 2;
			// 
			// cmbReportSelect
			// 
			this->cmbReportSelect->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbReportSelect->FormattingEnabled = true;
			this->cmbReportSelect->Items->AddRange(gcnew cli::array< System::Object^  >(7) {L"Out of Stock", L"Current Stock", L"Sales by Date", 
				L"Returns by Date", L"Invoices by Date", L"Top Selling Products", L"Revenue"});
			this->cmbReportSelect->Location = System::Drawing::Point(8, 28);
			this->cmbReportSelect->Name = L"cmbReportSelect";
			this->cmbReportSelect->Size = System::Drawing::Size(131, 21);
			this->cmbReportSelect->TabIndex = 1;
			this->cmbReportSelect->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::cmbReportSelect_SelectedIndexChanged);
			// 
			// lblReportSelect
			// 
			this->lblReportSelect->AutoSize = true;
			this->lblReportSelect->Location = System::Drawing::Point(6, 7);
			this->lblReportSelect->Name = L"lblReportSelect";
			this->lblReportSelect->Size = System::Drawing::Size(134, 13);
			this->lblReportSelect->TabIndex = 0;
			this->lblReportSelect->Text = L"Select Report to Generate:";
			// 
			// msMenu
			// 
			this->msMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->fileToolStripMenuItem, 
				this->helpToolStripMenuItem});
			this->msMenu->Location = System::Drawing::Point(0, 0);
			this->msMenu->Name = L"msMenu";
			this->msMenu->Size = System::Drawing::Size(535, 24);
			this->msMenu->TabIndex = 1;
			this->msMenu->Text = L"MainMenuStrip";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->exitToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(92, 22);
			this->exitToolStripMenuItem->Text = L"Exit";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::exitToolStripMenuItem_Click);
			// 
			// helpToolStripMenuItem
			// 
			this->helpToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->aboutToolStripMenuItem, 
				this->programHelpToolStripMenuItem});
			this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
			this->helpToolStripMenuItem->Size = System::Drawing::Size(44, 20);
			this->helpToolStripMenuItem->Text = L"Help";
			// 
			// aboutToolStripMenuItem
			// 
			this->aboutToolStripMenuItem->Name = L"aboutToolStripMenuItem";
			this->aboutToolStripMenuItem->Size = System::Drawing::Size(148, 22);
			this->aboutToolStripMenuItem->Text = L"About";
			// 
			// programHelpToolStripMenuItem
			// 
			this->programHelpToolStripMenuItem->Name = L"programHelpToolStripMenuItem";
			this->programHelpToolStripMenuItem->Size = System::Drawing::Size(148, 22);
			this->programHelpToolStripMenuItem->Text = L"Program Help";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(535, 537);
			this->Controls->Add(this->tbInventorySystem);
			this->Controls->Add(this->msMenu);
			this->MainMenuStrip = this->msMenu;
			this->MinimumSize = System::Drawing::Size(370, 415);
			this->Name = L"MyForm";
			this->Text = L"Inventory Management System";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->tbInventorySystem->ResumeLayout(false);
			this->tpCategory->ResumeLayout(false);
			this->tpCategory->PerformLayout();
			this->tpProduct->ResumeLayout(false);
			this->tpProduct->PerformLayout();
			this->tpSales->ResumeLayout(false);
			this->tpSales->PerformLayout();
			this->tpInvoices->ResumeLayout(false);
			this->tpInvoices->PerformLayout();
			this->tpReturns->ResumeLayout(false);
			this->tpReturns->PerformLayout();
			this->tpReports->ResumeLayout(false);
			this->tpReports->PerformLayout();
			this->msMenu->ResumeLayout(false);
			this->msMenu->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		/// \brief On loading the form, components are set to visible=false
	private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
				 // Category: all components (except Function label and combobox selector) set to invisible
				 lblCategoryName->Visible = false;
				 txtCategoryName->Visible = false;
				 lblCategoryDescription->Visible = false;
				 txtCategoryDescription->Visible = false;
				 btnCategoryAdd->Visible = false;
				 btnCategoryModify->Visible = false;
				 btnCategoryDelete->Visible = false;
				 lblCategorySelect->Visible = false;
				 cmbCategorySelect->Visible = false;
				 lblCategorySearchBy->Visible = false;
				 cmbCategorySearchBy->Visible = false;
				 lblCategorySearchInfo->Visible = false;
				 txtCategorySearch->Visible = false;
				 btnCategorySearch->Visible = false;
				 txtCategorySearchResults->Visible = false;

				 // Product: all components (except Function label and combobox selector) set to invisible
				 lblProductSelect->Visible = false;
				 cmbProductSelect->Visible = false;
				 lblProductID->Visible = false;
				 txtProdID->Visible = false;
				 lblProductCategory->Visible = false;
				 cmbProductCategorySelect->Visible = false;
				 lblProductName->Visible = false;
				 txtProductName->Visible = false;
				 lblProductDescription->Visible = false;
				 txtProductDescription->Visible = false;
				 lblProductPrice->Visible = false;
				 txtProductPrice->Visible = false;
				 btnProductAdd->Visible = false;
				 btnProductModify->Visible = false;
				 btnProductDelete->Visible = false;
				 lblProductSearchBy->Visible = false;
				 cmbProductSearch->Visible = false;
				 lblProductSearchInfo->Visible = false;
				 txtProductSearch->Visible = false;
				 btnProductSearch->Visible = false;
				 txtProductSearchResults->Visible = false;

				 // Sales: all components (except Function label and combobox selector) set to invisible
				 lblSalesReceiptSelect->Visible = false;
				 cmbSalesReceiptSelect->Visible = false;
				 lblSalesProductSelect->Visible = false;
				 cmbSalesProductSelect->Visible = false;
				 lblSaleProductQuantity->Visible = false;
				 txtSalesProductQuantity->Visible = false;
				 lblSalesProductDiscount->Visible = false;
				 txtSalesProductDiscount->Visible = false;
				 btnSalesAddProduct->Visible = false;
				 btnSalesModifyReceipt->Visible = false;

				 lblsalesProductList->Visible = false;

				 lstSalesProductList->Visible = false;
				 btnSalesRemoveProduct->Visible = false;
				 dtSalesReceiptDate->Visible = false;
				 btnSalesCreateReceipt->Visible = false;
				 btnSalesModify->Visible = false;
				 cmbSalesProductSelect->Visible= false;
				 btnSalesSearch->Visible = false;

				 // Invoice: all components (except Function label and combobox selector) set to invisible
				 lblInvoiceSelect->Visible = false;
				 cmbInvoiceSelect->Visible = false;
				 lblInvoiceProductSelect->Visible = false;
				 cmbInvoiceProductSelect->Visible = false;
				 lblInvoiceProductQuantity->Visible = false;
				 txtInvoiceProductQuantity->Visible = false;
				 btnInvoiceAddProduct->Visible = false;
				 lblInvoiceProductList->Visible = false;
				 lstInvoiceProductList->Visible = false;
				 btnInvoiceRemoveProduct->Visible = false;
				 dtInvoiceDate->Visible = false;
				 btnInvoiceCreateInvoice->Visible = false;
				 btnInvoiceModify->Visible = false;
				 btnInvoiceSearch->Visible = false;
				 btnInvoiceModifyInvoiceItem->Visible = false;
				 dtInvoiceDate->MaxDate = DateTime::Today;

				 // Reports: all components (Except Report Selection label and combobox selector) set to invisible
				 lblReportCategorySelect->Visible = false;
				 cmbReportCategorySelect->Visible = false;
				 lblReportDescription->Visible = false;
				 txtReportDescription->Visible = false;
				 lblReportStartDate->Visible = false;
				 dtReportStartDate->Visible = false;
				 lblReportEndDate->Visible = false;
				 dtReportEndDate->Visible = false;
				 btnReportGenerate->Visible = false;
				dtReportStartDate->MaxDate = DateTime::Today;
			 }

			 /// \brief Changes the visibility of certain components for the category tab based on the user selected function
			 ///
			 /// SelectedIndex == 0 : "Add"
			 /// SelectedIndex == 1 : "Modify"
			 /// SelectedIndex == 2 : "Delete"
			 /// SelectedIndex == 3 : "Search"
	private: System::Void cmbCategoryFunction_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {

				 if(cmbCategoryFunction->SelectedIndex == 0)
				 {
					 // Label "Name:" set to visible
					 lblCategoryName->Visible = true;
					 // Textbox for name set to visible
					 txtCategoryName->Visible = true;
					 // Label "Description:" set to visible
					 lblCategoryDescription->Visible = true;
					 // Textbox for description set to visible
					 txtCategoryDescription->Visible = true;
					 // "Add" button set to visible
					 btnCategoryAdd->Visible = true;
					 // "Modify" button set to invisible
					 btnCategoryModify->Visible = false;
					 // "Delete" button set to invisible
					 btnCategoryDelete->Visible = false;
					 // Label "Category:" set to visible
					 lblCategorySelect->Visible = false;
					 // Combobox for Category selection set to invisible
					 cmbCategorySelect->Visible = false;
					 // Label "Search By:" set to invisible
					 lblCategorySearchBy->Visible = false;
					 // Combobox for category "search by" set to invisible
					 cmbCategorySearchBy->Visible = false;
					 // Label "Enter Information for Search:" set to invisible
					 lblCategorySearchInfo->Visible = false;
					 // Text box for category search set to invisible
					 txtCategorySearch->Visible = false;
					 // Button for search set to invisible
					 btnCategorySearch->Visible = false;
					 // Category search results textbox set to invisible
					 txtCategorySearchResults->Visible = false;

					 // clears the textboxes
					 txtCategoryName->Text = "";
					 txtCategoryDescription->Text = "";

					 // enables textboxes
					 txtCategoryName->Enabled = true;
					 txtCategoryDescription->Enabled = true;

				 } else if(cmbCategoryFunction->SelectedIndex == 1)
				 {
					 lblCategoryName->Visible = true;
					 txtCategoryName->Visible = true;
					 lblCategoryDescription->Visible = true;
					 txtCategoryDescription->Visible = true;
					 btnCategoryAdd->Visible = false;
					 btnCategoryModify->Visible = true;
					 btnCategoryDelete->Visible = false;
					 lblCategorySelect->Visible = true;
					 cmbCategorySelect->Visible = true;
					 lblCategorySearchBy->Visible = false;
					 cmbCategorySearchBy->Visible = false;
					 lblCategorySearchInfo->Visible = false;
					 txtCategorySearch->Visible = false;
					 btnCategorySearch->Visible = false;
					 txtCategorySearchResults->Visible = false;

					 // disables textboxes and modify button
					 txtCategoryName->Enabled = false;
					 txtCategoryDescription->Enabled = false;
					 btnCategoryModify->Enabled = false;

					 // clears the textboxes
					 txtCategoryName->Text = "";
					 txtCategoryDescription->Text = "";

					 // clear combobox
					 cmbCategorySelect->Items->Clear();

					 // currentRow string
					 System::String ^ currentRow;

					 // vector to contain the category file contents
					 vector<string> categoriesFile;
					 // retrieve vector containing contents of category file
					 categoriesFile = returnFile("textFiles/category.txt");

					 // insert contents of category file into combobox
					 for(size_t i = 0; i < categoriesFile.size(); i++)
					 {
						 currentRow = gcnew String (categoriesFile[i].c_str());
						 cmbCategorySelect->Items->Add(currentRow);
					 }

				 } else if(cmbCategoryFunction->SelectedIndex == 2)
				 {
					 lblCategoryName->Visible = false;
					 txtCategoryName->Visible = false;
					 lblCategoryDescription->Visible = false;
					 txtCategoryDescription->Visible = false;
					 btnCategoryAdd->Visible = false;
					 btnCategoryModify->Visible = false;
					 btnCategoryDelete->Visible = true;
					 lblCategorySelect->Visible = true;
					 cmbCategorySelect->Visible = true;
					 lblCategorySearchBy->Visible = false;
					 cmbCategorySearchBy->Visible = false;
					 lblCategorySearchInfo->Visible = false;
					 txtCategorySearch->Visible = false;
					 btnCategorySearch->Visible = false;
					 txtCategorySearchResults->Visible = false;

					 btnCategoryDelete->Enabled = false;


					 // clear combobox
					 cmbCategorySelect->Items->Clear();

					 // currentRow string
					 System::String ^ currentRow;

					 // vector to contain the category file contents
					 vector<string> categoriesFile;
					 // retrieve vector containing contents of category file
					 categoriesFile = returnFile("textFiles/category.txt");

					 // insert contents of category file into combobox
					 for(size_t i = 0; i < categoriesFile.size(); i++)
					 {
						 currentRow = gcnew String (categoriesFile[i].c_str());
						 cmbCategorySelect->Items->Add(currentRow);
					 }
				 } else if(cmbCategoryFunction->SelectedIndex == 3)
				 {
					 lblCategoryName->Visible = false;
					 txtCategoryName->Visible = false;
					 lblCategoryDescription->Visible = false;
					 txtCategoryDescription->Visible = false;
					 btnCategoryAdd->Visible = false;
					 btnCategoryModify->Visible = false;
					 btnCategoryDelete->Visible = false;
					 lblCategorySelect->Visible = false;
					 cmbCategorySelect->Visible = false;
					 lblCategorySearchBy->Visible = true;
					 cmbCategorySearchBy->Visible = true;
					 lblCategorySearchInfo->Visible = true;
					 txtCategorySearch->Visible = true;
					 btnCategorySearch->Visible = true;
					 txtCategorySearchResults->Visible = true;
					 txtCategorySearchResults->Text = "";
					 txtCategorySearch->Text = "";

					 txtCategorySearch->Enabled = false;
					 btnCategorySearch->Enabled = false;
				 }

			 }

			 /// \brief Changes the visibility of certain components for the product tab based on the user selected function
			 ///
			 /// SelectedIndex == 0 : "Add"
			 /// SelectedIndex == 1 : "Modify"
			 /// SelectedIndex == 2 : "Delete"
			 /// SelectedIndex == 3 : "Search"
	private: System::Void cmbProductFunction_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
				 if(cmbProductFunction->SelectedIndex == 0)
				 {
					 lblProductSelect->Visible = false;
					 cmbProductSelect->Visible = false;
					 lblProductID->Visible = true;
					 txtProdID->Visible = true;
					 lblProductCategory->Visible = true;
					 cmbProductCategorySelect->Visible = true;
					 lblProductName->Visible = true;
					 txtProductName->Visible = true;
					 lblProductDescription->Visible = true;
					 txtProductDescription->Visible = true;
					 lblProductPrice->Visible = true;
					 txtProductPrice->Visible = true;
					 btnProductAdd->Visible = true;
					 btnProductAdd->Enabled = false;
					 btnProductModify->Visible = false;
					 btnProductDelete->Visible = false;
					 lblProductSearchBy->Visible = false;
					 cmbProductSearch->Visible = false;
					 lblProductSearchInfo->Visible = false;
					 txtProductSearch->Visible = false;
					 btnProductSearch->Visible = false;
					 txtProductSearchResults->Visible = false;

					 txtProdID->Enabled = true;
					 txtProductName->Enabled = true;
					 cmbProductCategorySelect->Enabled = true;
					 txtProductDescription->Enabled = true;
					 txtProductPrice->Enabled = true;

					 txtProductDescription->Text = "";
					 txtProductName->Text = "";
					 txtProductPrice->Text = "";
					 txtProdID->Text = "";

					 // clear combobox
					 cmbProductCategorySelect->Items->Clear();

					 // currentRow string
					 System::String ^ currentRow;

					 // vector to contain the category file contents
					 vector<string> categoriesFile;
					 // retrieve vector containing contents of category file
					 categoriesFile = returnFile("textFiles/category.txt");

					 // insert contents of category file into combobox
					 for(size_t i = 0; i < categoriesFile.size(); i++)
					 {
						 currentRow = gcnew String (categoriesFile[i].c_str());
						 cmbProductCategorySelect->Items->Add(currentRow);
					 }
				 } else if(cmbProductFunction->SelectedIndex == 1)
				 {
					 lblProductSelect->Visible = true;
					 cmbProductSelect->Visible = true;
					 lblProductID->Visible = true;
					 txtProdID->Visible = true;
					 lblProductCategory->Visible = true;
					 cmbProductCategorySelect->Visible = true;
					 lblProductName->Visible = true;
					 txtProductName->Visible = true;
					 lblProductDescription->Visible = true;
					 txtProductDescription->Visible = true;
					 lblProductPrice->Visible = true;
					 txtProductPrice->Visible = true;
					 btnProductAdd->Visible = false;
					 btnProductModify->Visible = true;
					 btnProductDelete->Visible = false;
					 lblProductSearchBy->Visible = false;
					 cmbProductSearch->Visible = false;
					 lblProductSearchInfo->Visible = false;
					 txtProductSearch->Visible = false;
					 btnProductSearch->Visible = false;
					 txtProductSearchResults->Visible = false;

					 // disable and clear controls
					 txtProdID->Enabled = false;
					 txtProdID->Text = "";
					 txtProductName->Enabled = false;
					 txtProductName->Text = "";
					 cmbProductCategorySelect->Enabled = false;
					 // clear combobox
					 cmbProductCategorySelect->Items->Clear();

					 // currentRow string
					 System::String ^ currentRow;

					 // vector to contain the category file contents
					 vector<string> categoriesFile;
					 // retrieve vector containing contents of category file
					 categoriesFile = returnFile("textFiles/category.txt");

					 // insert contents of category file into combobox
					 for(size_t i = 0; i < categoriesFile.size(); i++)
					 {
						 currentRow = gcnew String (categoriesFile[i].c_str());
						 cmbProductCategorySelect->Items->Add(currentRow);
					 }
					 txtProductDescription->Enabled = false;
					 txtProductDescription->Text = "";
					 txtProductPrice->Enabled = false;
					 txtProductPrice->Text = "";
					 btnProductModify->Enabled = false;

					 // populate product selection drop down list
					 cmbProductSelect->Items->Clear();

					 // vector to contain the category file contents
					 vector<string> productFile;
					 // retrieve vector containing contents of category file
					 productFile = returnFile("textFiles/product.txt");

					 // insert contents of category file into combobox
					 for(size_t i = 0; i < productFile.size(); i++)
					 {
						 currentRow = gcnew String (productFile[i].c_str());
						 cmbProductSelect->Items->Add(currentRow);
					 }
				 } else if(cmbProductFunction->SelectedIndex == 2)
				 {
					 lblProductSelect->Visible = true;
					 cmbProductSelect->Visible = true;
					 lblProductID->Visible = false;
					 txtProdID->Visible = false;
					 lblProductCategory->Visible = false;
					 cmbProductCategorySelect->Visible = false;
					 lblProductName->Visible = false;
					 txtProductName->Visible = false;
					 lblProductDescription->Visible = false;
					 txtProductDescription->Visible = false;
					 lblProductPrice->Visible = false;
					 txtProductPrice->Visible = false;
					 btnProductAdd->Visible = false;
					 btnProductModify->Visible = false;
					 btnProductDelete->Visible = true;
					 lblProductSearchBy->Visible = false;
					 cmbProductSearch->Visible = false;
					 lblProductSearchInfo->Visible = false;
					 txtProductSearch->Visible = false;
					 btnProductSearch->Visible = false;
					 txtProductSearchResults->Visible = false;

					 // disable delete button
					 btnProductDelete->Enabled = false;

					 // clear combobox
					 cmbProductSelect->Items->Clear();

					 // currentRow string
					 System::String ^ currentRow;

					 // vector to contain the category file contents
					 vector<string> productFile;
					 // retrieve vector containing contents of category file
					 productFile = returnFile("textFiles/product.txt");

					 // insert contents of category file into combobox
					 for(size_t i = 0; i < productFile.size(); i++)
					 {
						 currentRow = gcnew String (productFile[i].c_str());
						 cmbProductSelect->Items->Add(currentRow);
					 }
				 } else if(cmbProductFunction->SelectedIndex == 3)
				 {
					 lblProductSelect->Visible = false;
					 cmbProductSelect->Visible = false;
					 lblProductID->Visible = false;
					 txtProdID->Visible = false;
					 lblProductCategory->Visible = false;
					 cmbProductCategorySelect->Visible = false;
					 lblProductName->Visible = false;
					 txtProductName->Visible = false;
					 lblProductDescription->Visible = false;
					 txtProductDescription->Visible = false;
					 lblProductPrice->Visible = false;
					 txtProductPrice->Visible = false;
					 btnProductAdd->Visible = false;
					 btnProductModify->Visible = false;
					 btnProductDelete->Visible = false;
					 lblProductSearchBy->Visible = true;
					 cmbProductSearch->Visible = true;
					 lblProductSearchInfo->Visible = true;
					 txtProductSearch->Visible = true;
					 btnProductSearch->Visible = true;
					 txtProductSearchResults->Visible = true;
				 }
			 }

			 /// \brief Changes the visibility of certain components for the sales tab based on the user selected function
			 ///
			 /// SelectedIndex == 0 : "Add"
			 /// SelectedIndex == 1 : "Modify"
	private: System::Void cmbSalesFunction_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {

				 if(cmbSalesFunction->SelectedIndex == 0)
				 {

					 cmbSalesProductSelect->Items->Clear();
					 cmbSalesReceiptSelect->Items->Clear();
					 // lstSalesProducts->Items->Clear();
					 lstSalesProductList->Items->Clear();
					 // Label "quantity sold:" set to visible
					 lblSaleProductQuantity->Visible = true;
					 // Textbox for quantity sold set to visible
					 txtSalesProductQuantity->Visible = true;
					 // Label "discount:" set to visible
					 lblSalesProductDiscount->Visible = true;
					 // Textbox for discount set to visible
					 txtSalesProductDiscount->Visible = true;
					 lstSalesProductList->Visible = true;
					 // "Add" button set to visible
					 btnSalesAddProduct->Visible = true;
					 // "Modify" button set to invisible
					 btnSalesModify->Visible = false;

					 btnSalesModifyReceipt->Visible = false;
					 // Label "sales:" set to visible

					 lblsalesProductList->Visible = true;

					 lblSalesProductSelect->Visible = true;
					 // Combobox for sales selection set to invisible
					 // cmbSalesProductAddSelect->Visible= true;
					 cmbSalesProductSelect->Visible = true;
					 // enables textboxes
					 txtSalesProductQuantity->Enabled = true;
					 txtSalesProductDiscount->Enabled = true;
					 //lstSalesProducts->Visible = true;
					 btnSalesRemoveProduct->Visible = true;
					 dtSalesReceiptDate->Visible = true;
					 btnSalesCreateReceipt->Visible = true;
					 lblSalesReceiptSelect->Visible = false;
					 cmbSalesReceiptSelect->Visible = false;
					 lstSalesProductList->Enabled = true;
					 btnSalesSearch->Visible = false;

					 cmbSalesProductSelect->Enabled = true;
					 txtSalesProductDiscount->Enabled = false;
					 txtSalesProductQuantity->Enabled  = false;
					 btnSalesAddProduct->Enabled = false;
					 txtSalesProductDiscount->Text = "";
					 txtSalesProductQuantity->Text = "";
					 lblsalesProductList->Visible= true;
					 lblsalesProductList->Text = "Product ID |Quantity Sold|Product Name|Discount";
					 // clear combobox
					 cmbSalesProductSelect->Items->Clear();

					 // currentRow string
					 System::String ^ currentRow;

					 // vector to contain the product file contents
					 vector<string> productFile;
					 // retrieve vector containing contents of product file
					 productFile = returnFile("textFiles/product.txt");

					 // insert contents of product file into combobox

					 for(size_t i = 0; i < productFile.size(); i++)
					 {
						 currentRow = gcnew String (productFile[i].c_str()); // convert to System::String^

						 // Add spaces between |'s
						 cmbSalesProductSelect->Items->Add(currentRow->Replace("|", " | "));
					 }
				 } else if(cmbSalesFunction->SelectedIndex == 1)
				 {
					 lstSalesProductList->Items->Clear();
					 cmbSalesProductSelect->Items->Clear();
					 cmbSalesReceiptSelect->Items->Clear();
					 lblSaleProductQuantity->Visible = true;
					 txtSalesProductQuantity->Visible = true;
					 txtSalesProductQuantity->Text= "";
					 lblSalesProductDiscount->Visible = true;
					 txtSalesProductDiscount->Visible = true;
					 txtSalesProductDiscount->Text= "";
					 lblsalesProductList->Visible = true;
					 btnSalesAddProduct->Visible = true;
					 btnSalesModify->Visible = true;
					 btnSalesModifyReceipt->Visible = true;
					 lblSalesProductSelect->Visible = true;
					 cmbSalesProductSelect->Visible = true;
					 lblSalesReceiptSelect->Visible = true;
					 cmbSalesReceiptSelect->Visible = true;

					 lstSalesProductList->Visible = true;
					 // disables textboxes and modify button
					 txtSalesProductQuantity->Enabled = false;
					 txtSalesProductDiscount->Enabled = false;


					 btnSalesAddProduct->Enabled = true;
					 // lstSalesProducts->Visible = true;

					 // lstSalesProductList->Items->Clear();
					 btnSalesRemoveProduct->Visible = true;

					 btnSalesRemoveProduct->Enabled = false;
					 dtSalesReceiptDate->Visible = true;
					 dtSalesReceiptDate->Enabled = true;
					 btnSalesCreateReceipt->Visible = false;

					 lblSalesProductSelect->Enabled = false;
					 cmbSalesProductSelect->Enabled = false;
					 lblSalesReceiptSelect->Enabled = true;
					 cmbSalesReceiptSelect->Enabled = true;

					 btnSalesModifyReceipt->Enabled = true;
					 btnSalesModify->Enabled = true;
					 lstSalesProductList->Enabled = false;
					 btnSalesSearch->Visible = false;
					 // clear combobox

					 lblsalesProductList->Text = "Sales ID | Product ID | Product Name | Quantity Sold | Discount";

					 // currentRow string
					 std::string currentRow;


					 std::string receipt_id; // store invoice_id of current row
					 std::string receipt_date; // store date of current row

					 int delimiter; // first instance of delimiter

					 // vector to contain the invoice file contents
					 vector<string> receiptFile;
					 // retrieve vector containing contents of invoice file
					 receiptFile = returnFile("textFiles/receipt.txt");

					 // display all invoices from the invoiceFile vector to the combobox
					 for(size_t i = 0; i < receiptFile.size(); i++)
					 {
						 currentRow = receiptFile[i].c_str(); //get first row of file

						 delimiter = currentRow.find('|');
						 receipt_id = currentRow.substr(0, delimiter);
						 receipt_date = currentRow.substr(delimiter + 1);

						 cmbSalesReceiptSelect->Items->Add( "Receipt ID: " +
							 gcnew String(receipt_id.c_str()) + "   Receipt Date: " +
							 gcnew String(receipt_date.c_str()));
					 }

				 } 
				 else if(cmbSalesFunction->SelectedIndex == 2)
				 {					 
					 lstSalesProductList->Items->Clear();
					 cmbSalesProductSelect->Items->Clear();
					 cmbSalesReceiptSelect->Items->Clear();
					 txtSalesProductQuantity->Text="";
					 txtSalesProductDiscount->Text="";
					 lblSalesReceiptSelect->Visible = true;;
					 lblSalesReceiptSelect->Visible = true;;
					 lblSalesProductSelect->Visible = false;;
					 cmbSalesProductSelect->Visible = false;

					 cmbSalesReceiptSelect->Visible = true;
					 lblSaleProductQuantity->Visible = false;
					 txtSalesProductQuantity->Visible = false;
					 lblSalesProductDiscount->Visible = false;
					 txtSalesProductDiscount->Visible = false;
					 lstSalesProductList->Visible = true;
					 btnSalesAddProduct->Visible = false;
					 btnSalesRemoveProduct->Visible = false;
					 // lstSalesProducts->Visible = true;
					 dtSalesReceiptDate->Visible = true;
					 lblsalesProductList->Visible= true;
					 btnSalesCreateReceipt->Visible = false;
					 btnSalesModify->Visible = false;
					 btnSalesModifyReceipt->Visible=false;
					 lstSalesProductList->Enabled = false;
					 lblsalesProductList->Text = "Sales ID | Product ID | Product Name | Quantity Sold | Discount";
					 cmbSalesReceiptSelect->Enabled= false;
					 lstSalesProductList->Enabled=false;
					 dtSalesReceiptDate->Enabled=true;
					 btnSalesSearch->Visible = true;
				 }
			 }

			 /// \brief Changes the visibility of certain components for the invoice tab based on the user selected function
			 ///
			 /// SelectedIndex == 0 : "Add"
			 /// SelectedIndex == 1 : "Modify"
	private: System::Void cmbInvoiceFunction_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {

				 if(cmbInvoiceFunction->SelectedIndex == 0)
				 {
					 // Show required controls and clear them
					 lstInvoiceProductList->Items->Clear();
					 cmbInvoiceProductSelect->Items->Clear();
					 cmbInvoiceSelect->Items->Clear();
					 txtInvoiceProductQuantity->Text = "";
					 lblInvoiceSelect->Visible = false;
					 cmbInvoiceSelect->Visible = false;
					 lblInvoiceProductSelect->Visible = true;
					 cmbInvoiceProductSelect->Visible = true;
					 lblInvoiceProductQuantity->Visible = true;
					 txtInvoiceProductQuantity->Visible = true;
					 btnInvoiceAddProduct->Visible = true;
					 lstInvoiceProductList->Visible = true;
					 btnInvoiceRemoveProduct->Visible = true;
					 dtInvoiceDate->Visible = true;
					 btnInvoiceCreateInvoice->Visible = true;
					 btnInvoiceModify->Visible = false;
					 lblInvoiceProductList->Visible = true;
					 btnInvoiceSearch->Visible = false;
					 btnInvoiceModifyInvoiceItem->Visible = false;
					 lblInvoiceProductList->Text = "Product ID | Name | Quantity";

					 // disables  and enables textboxes and buttons
					 txtInvoiceProductQuantity->Enabled = false;
					 lstInvoiceProductList->Enabled = false;
					 btnInvoiceAddProduct->Enabled = false;
					 btnInvoiceCreateInvoice->Enabled = false;
					 dtInvoiceDate->Enabled = false;
					 btnInvoiceRemoveProduct->Enabled = false;
					 lblInvoiceProductSelect->Enabled = true;
					 cmbInvoiceProductSelect->Enabled = true;

					 // clear combobox
					 cmbInvoiceProductSelect->Items->Clear();

					 // currentRow string
					 System::String ^ currentRow;

					 // vector to contain the product file contents
					 vector<string> productFile;
					 // retrieve vector containing contents of product file
					 productFile = returnFile("textFiles/product.txt");

					 // insert contents of product file vector into combobox
					 for(size_t i = 0; i < productFile.size(); i++)
					 {
						 currentRow = gcnew String (productFile[i].c_str()); // convert to System::String^

						 // Add spaces between |'s
						 cmbInvoiceProductSelect->Items->Add(currentRow->Replace("|", " | "));
					 }


				 }
				 else if(cmbInvoiceFunction->SelectedIndex == 1)
				 {
					 lstInvoiceProductList->Items->Clear();
					 cmbInvoiceProductSelect->Items->Clear();
					 cmbInvoiceSelect->Items->Clear();
					 txtInvoiceProductQuantity->Text = "";
					 lstInvoiceProductList->Items->Clear();
					 lblInvoiceSelect->Visible = true;
					 cmbInvoiceSelect->Visible = true;
					 lblInvoiceProductSelect->Visible = true;
					 cmbInvoiceProductSelect->Visible = true;
					 lblInvoiceProductQuantity->Visible = true;
					 txtInvoiceProductQuantity->Visible = true;
					 btnInvoiceAddProduct->Visible = true;
					 lstInvoiceProductList->Visible = true;
					 btnInvoiceRemoveProduct->Visible = true;
					 dtInvoiceDate->Visible = true;
					 btnInvoiceCreateInvoice->Visible = false;
					 btnInvoiceModify->Visible = true;
					 lblInvoiceProductList->Visible = true;
					 btnInvoiceSearch->Visible = false;
					 btnInvoiceModifyInvoiceItem->Visible = true;
					 lblInvoiceProductList->Text = "Invoice Item ID | Product ID | Name | Quantity";

					 // disables  and enables textboxes and buttons
					 txtInvoiceProductQuantity->Enabled = false;
					 lstInvoiceProductList->Enabled = false;
					 btnInvoiceAddProduct->Enabled = false;
					 btnInvoiceModify->Enabled = false;
					 dtInvoiceDate->Enabled = true;
					 btnInvoiceRemoveProduct->Enabled = false;
					 lblInvoiceProductSelect->Enabled = false;
					 cmbInvoiceProductSelect->Enabled = false;
					 cmbInvoiceSelect->Enabled = true;
					 btnInvoiceModifyInvoiceItem->Enabled = false;

					 std::string currentRow; // store current row
					 std::string invoice_id; // store invoice_id of current row
					 std::string date; // store date of current row

					 int delimiter; // first instance of delimiter

					 // vector to contain the invoice file contents
					 vector<string> invoiceFile;
					 // retrieve vector containing contents of invoice file
					 invoiceFile = returnFile("textFiles/invoice.txt");

					 // display all invoices from the invoiceFile vector to the combobox
					 for(size_t i = 0; i < invoiceFile.size(); i++)
					 {
						 currentRow = invoiceFile[i].c_str(); //get first row of file

						 delimiter = currentRow.find('|');
						 invoice_id = currentRow.substr(0, delimiter);
						 date = currentRow.substr(delimiter + 1);

						 cmbInvoiceSelect->Items->Add( "Invoice ID: " +
							 gcnew String(invoice_id.c_str()) + "   Date: " +
							 gcnew String(date.c_str()));
					 }
				 }
				 else if(cmbInvoiceFunction->SelectedIndex == 2)
				 {
					 lstInvoiceProductList->Items->Clear();
					 cmbInvoiceProductSelect->Items->Clear();
					 cmbInvoiceSelect->Items->Clear();
					 txtInvoiceProductQuantity->Text = "";
					 lblInvoiceSelect->Visible = true;
					 cmbInvoiceSelect->Visible = true;
					 lblInvoiceProductSelect->Visible = true;
					 cmbInvoiceProductSelect->Visible = true;
					 lblInvoiceProductQuantity->Visible = false;
					 txtInvoiceProductQuantity->Visible = false;
					 btnInvoiceAddProduct->Visible = false;
					 lstInvoiceProductList->Visible = true;
					 btnInvoiceRemoveProduct->Visible = false;
					 dtInvoiceDate->Visible = true;
					 btnInvoiceCreateInvoice->Visible = false;
					 btnInvoiceModify->Visible = false;
					 lblInvoiceProductList->Visible = true;
					 btnInvoiceSearch->Visible = true;
					 btnInvoiceModifyInvoiceItem->Visible = false;
					 lblInvoiceProductList->Text = "Invoice Item ID | Product ID | Name | Quantity";

					 cmbInvoiceSelect->Enabled = false;
					 cmbInvoiceProductSelect->Enabled = false;
					 dtInvoiceDate->Enabled = true;
					 lstInvoiceProductList->Enabled = false;
				 }
			 }

			 /// \brief Exit button in the "File" menu closes the application
	private: System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 Application::Exit();
			 }

			 /// \brief Selection in the category selector changed
			 ///
			 /// When a selection is made the necessary components are made available to the user
	private: System::Void cmbCategorySelect_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
				 // enable textboxes and modify button
				 txtCategoryName->Enabled = true;
				 txtCategoryDescription->Enabled = true;
				 btnCategoryModify->Enabled = true;
				 btnCategoryDelete->Enabled = true;

				 // string containing contents of the selection in the combobox
				 System::String^ category = cmbCategorySelect->SelectedItem->ToString();

				 // position of delimiter one
				 int delimiter1 = category->IndexOf("|");
				 //position of delimiter two
				 int delimiter2 = category->IndexOf("|", delimiter1 + 1);

				 // string containing name from the combobox
				 System::String ^ name = category->Substring(delimiter2 + 1);

				 // placing name string into textbox
				 txtCategoryName->Text = name;

				 // string containing description from the combobox
				 System::String ^ description = category->Substring(delimiter1 + 1, delimiter2 - delimiter1 - 1);

				 // placing description string into textbox
				 txtCategoryDescription->Text = description;

			 }

			 /// \brief Modify button on the category tab is pressed - performs modify function
	private: System::Void btnCategoryModify_Click(System::Object^  sender, System::EventArgs^  e) {
				 if(txtCategoryName->Text->Contains("|")){
					 MessageBox::Show("| is a reserved character - Please change the category name", "InSys", MessageBoxButtons::OK,MessageBoxIcon::Error);
				 } else if (txtCategoryDescription->Text->Contains("|")) {
					 MessageBox::Show("| is a reserved character - Please change the category description", "InSys", MessageBoxButtons::OK,MessageBoxIcon::Error);
				 } else {
					 // create instance of Category()
					 Table cat = new Category();

					 // string for contents of selction in drop box
					 System::String^ category = cmbCategorySelect->SelectedItem->ToString();
					 // position of first delimiter
					 int delimiter1 = category->IndexOf("|");

					 // string for category ID
					 System::String ^ categoryID = category->Substring(0,delimiter1);

					 // convert System::String to std::string
					 string categoryIDstring(marshal_as<std::string>(categoryID));
					 string categoryNameString(marshal_as<std::string>(txtCategoryName->Text->ToString()));
					 string categoryDescriptionString(marshal_as<std::string>(txtCategoryDescription->Text->ToString()));

					 // perform category::modify function
					 cat->modifyRow(categoryIDstring,"name",categoryNameString);
					 // perform category::modify function
					 cat->modifyRow(categoryIDstring,"description",categoryDescriptionString);

					 // clear text boxes
					 txtCategoryDescription->Text = "";
					 txtCategoryName->Text = "";

					 // clear combobox
					 cmbCategorySelect->Items->Clear();

					 // currentRow string
					 System::String ^ currentRow;

					 // vector to contain the category file contents
					 vector<string> categoriesFile;
					 // retrieve vector containing contents of category file
					 categoriesFile = returnFile("textFiles/category.txt");

					 // insert contents of category file into combobox
					 for(size_t i = 0; i < categoriesFile.size(); i++)
					 {
						 currentRow = gcnew String (categoriesFile[i].c_str());
						 cmbCategorySelect->Items->Add(currentRow);
					 }

					 // disable textboxes and modify button
					 txtCategoryName->Enabled = false;
					 txtCategoryDescription->Enabled = false;
					 btnCategoryModify->Enabled = false;

					 // delete instance of category
					 delete cat;
				 }
			 }

			 /// \brief Add button on the category tab is pressed - performs add function
	private: System::Void btnCategoryAdd_Click(System::Object^  sender, System::EventArgs^  e) {
				 if(txtCategoryName->Text->Contains("|")){
					 MessageBox::Show("| is a reserved character - Please change the category name", "InSys", MessageBoxButtons::OK,MessageBoxIcon::Error);
				 } else if (txtCategoryName->Text->Length == 0) {
					 MessageBox::Show("Category name required - Please enter a category name", "InSys", MessageBoxButtons::OK,MessageBoxIcon::Error);
				 } else if (txtCategoryDescription->Text->Contains("|")) {
					 MessageBox::Show("| is a reserved character - Please change the category description", "InSys", MessageBoxButtons::OK,MessageBoxIcon::Error);
				 } else if (txtCategoryDescription->Text->Length == 0) {
					 MessageBox::Show("Category description required - Please enter a category description", "InSys", MessageBoxButtons::OK,MessageBoxIcon::Error);
				 }else {
					 // create instance of Category()
					 Table cat = new Category();

					 // retrieves category name from the textbox, converts it from System::String^ to std::string and stores it in categoryNameString
					 string categoryNameString(marshal_as<std::string>(txtCategoryName->Text->ToString()));

					 // retrieves category description from the textbox, converts it from System::String^ to std::string and stores it in categoryDescriptionString
					 string categoryDescriptionString(marshal_as<std::string>(txtCategoryDescription->Text->ToString()));

					 // vector to store the name and description for Category::Add
					 vector<string> catVect;

					 // adds description to catVect
					 catVect.push_back(categoryDescriptionString);

					 // adds name to catVect
					 catVect.push_back(categoryNameString);

					 // calls the Category::Add function with the vector parameter
					 cat->add(catVect);

					 // delete instance of category
					 delete cat;

					 // clear text boxes
					 txtCategoryDescription->Text = "";
					 txtCategoryName->Text = "";
				 }
			 }

			 /// \brief Delete button on the category tab is pressed - performs delete function after confirming
			 /// that user wishes to delete
	private: System::Void btnCategoryDelete_Click(System::Object^  sender, System::EventArgs^  e) {

				 if(MessageBox::Show("Are you sure you wish to delete the selected category?", "InSys", MessageBoxButtons::YesNo,MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::Yes)
				 {
					 // create instance of Category()
					 Table cat = new Category();

					 // string for contents of selction in drop box
					 System::String^ category = cmbCategorySelect->SelectedItem->ToString();
					 // position of first delimiter
					 int delimiter1 = category->IndexOf("|");

					 // string for category ID
					 System::String ^ categoryID = category->Substring(0,delimiter1);

					 // convert System::String to std::string
					 string categoryIDstring(marshal_as<std::string>(categoryID));

					 // call Category Delete function
					 cat->deleteRow(categoryIDstring);

					 // delete instance of category
					 delete cat;

					 // clear combobox
					 cmbCategorySelect->Items->Clear();

					 // currentRow string
					 System::String ^ currentRow;

					 // vector to contain the category file contents
					 vector<string> categoriesFile;
					 // retrieve vector containing contents of category file
					 categoriesFile = returnFile("textFiles/category.txt");

					 // insert contents of category file into combobox
					 for(size_t i = 0; i < categoriesFile.size(); i++)
					 {
						 currentRow = gcnew String (categoriesFile[i].c_str());
						 cmbCategorySelect->Items->Add(currentRow);
					 }
				 }
			 }

			 /// \brief Selection in the category search by selector changed
			 ///
			 /// When a selection is made the necessary components are made available to the user
	private: System::Void cmbCategorySearchBy_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
				 // enable textbox and button
				 txtCategorySearch->Enabled = true;
				 btnCategorySearch->Enabled = true;
				 txtCategorySearchResults->Text = "";
				 txtCategorySearch->Text = "";
			 }

			 /// \brief Search button on the category tab is pressed - performs search function
	private: System::Void btnCategorySearch_Click(System::Object^  sender, System::EventArgs^  e) {
				 try{
					 Table cat = new Category();
					 string resultOfSearch;
					 if(cmbCategorySearchBy->SelectedIndex == 0)
					 {
						 resultOfSearch = cat->search("category_id",marshal_as<std::string>(txtCategorySearch->Text->ToString()));
					 } else if(cmbCategorySearchBy->SelectedIndex == 1)
					 {
						 resultOfSearch = cat->search("name",marshal_as<std::string>(txtCategorySearch->Text->ToString()));
					 } else if(cmbCategorySearchBy->SelectedIndex == 2)
					 {
						 resultOfSearch = cat->search("description",marshal_as<std::string>(txtCategorySearch->Text->ToString()));
					 }
					 delete cat;

					 System::String^ results = gcnew String (resultOfSearch.c_str());

					 txtCategorySearchResults->Text = results;

				 } catch (DoesNotExistException e)
				 {
					 MessageBox::Show("Search parameters did not return any results", "InSys", MessageBoxButtons::OK,MessageBoxIcon::Information);
					 txtCategorySearchResults->Text = "";
					 txtCategorySearch->Text = "";
				 } catch(Exception^ e) {
					 MessageBox::Show("An unknown error has occured: " + e->Message, "InSys", MessageBoxButtons::OK,MessageBoxIcon::Error);
				 }
			 }

			 /// \brief Add button on the product tab is pressed - performs add function
	private: System::Void btnProductAdd_Click(System::Object^  sender, System::EventArgs^  e) {
				 try{

					 regex rgxPrice("^[0-9]*[.][0-9][0-9]$");

					 regex rgxID("^[0-9]+$");

					 cmatch match;

					 string productID = marshal_as<std::string>(txtProdID->Text->ToString());
					 const char *targetProdID = productID.c_str();

					 string productPrice = marshal_as<std::string>(txtProductPrice->Text->ToString());
					 const char *targetProductPrice = productPrice.c_str();

					 if(txtProdID->Text->Length == 0)
					 {
						 MessageBox::Show("Please Enter a Product ID", "InSys", MessageBoxButtons::OK,MessageBoxIcon::Error);
					 } else if(!regex_search(targetProdID, match, rgxID)) {
						 MessageBox::Show("Invalid characters - please change the product ID", "InSys", MessageBoxButtons::OK,MessageBoxIcon::Error);
					 } else if(txtProductName->Text->Contains("|")) {
						 MessageBox::Show("| is a reserved character - Please change the product name", "InSys", MessageBoxButtons::OK,MessageBoxIcon::Error);
					 } else if(txtProductName->Text->Length == 0) {
						 MessageBox::Show("Product must have a name - Please add a name", "InSys", MessageBoxButtons::OK,MessageBoxIcon::Error);
					 } else if(txtProductDescription->Text->Contains("|")) {
						 MessageBox::Show("| is a reserved character - Please change the product description", "InSys", MessageBoxButtons::OK,MessageBoxIcon::Error);
					 } else if(txtProductDescription->Text->Length == 0) {
						 MessageBox::Show("Product must have a description - Please add a description", "InSys", MessageBoxButtons::OK,MessageBoxIcon::Error);
					 } else if(!regex_search(targetProductPrice, match, rgxPrice)) {
						 MessageBox::Show("Invalid characters - please change the product price", "InSys", MessageBoxButtons::OK,MessageBoxIcon::Error);
					 } else {
						 Table prod = new Product();

						 // retrieves product id from the textbox, converts it from System::String^ to std::string and stores it in productIDString
						 string productIDString(marshal_as<std::string>(txtProdID->Text->ToString()));
						 // retrieves product name from the textbox, converts it from System::String^ to std::string and stores it in productNameString
						 string productNameString(marshal_as<std::string>(txtProductName->Text->ToString()));
						 // retrieves product description from the textbox, converts it from System::String^ to std::string and stores it in productDescriptionString
						 string productDescriptionString(marshal_as<std::string>(txtProductDescription->Text->ToString()));
						 // retrieves product price from the textbox, converts it from System::String^ to std::string and stores it in productPriceString
						 string productPriceString(marshal_as<std::string>(txtProductPrice->Text->ToString()));

						 // string for contents of selction in drop box
						 System::String^ category = cmbProductCategorySelect->SelectedItem->ToString();
						 // position of first delimiter
						 int delimiter1 = category->IndexOf("|");

						 // string for category ID
						 System::String ^ categoryID = category->Substring(0,delimiter1);

						 // convert System::String to std::string
						 string categoryIDstring(marshal_as<std::string>(categoryID));

						 // vector to use for the add function parameter
						 vector<string> prodVect;

						 // add the product information to the vector
						 prodVect.push_back(productIDString);
						 prodVect.push_back(categoryIDstring);
						 prodVect.push_back(productDescriptionString);
						 prodVect.push_back(productNameString);
						 prodVect.push_back(productPriceString);

						 prod->add(prodVect);

						 delete prod;

						 // clear combobox
						 cmbProductCategorySelect->Items->Clear();

						 // currentRow string
						 System::String ^ currentRow;

						 // vector to contain the category file contents
						 vector<string> categoriesFile;
						 // retrieve vector containing contents of category file
						 categoriesFile = returnFile("textFiles/category.txt");

						 // insert contents of category file into combobox
						 for(size_t i = 0; i < categoriesFile.size(); i++)
						 {
							 currentRow = gcnew String (categoriesFile[i].c_str());
							 cmbProductCategorySelect->Items->Add(currentRow);
						 }

						 txtProdID->Text = "";
						 txtProductName->Text = "";
						 txtProductDescription->Text = "";
						 txtProductPrice->Text = "";
						 btnProductAdd->Enabled = false;
					 }
				 } catch(AlreadyExistsException e)
				 {
					 MessageBox::Show("Product ID already exists\r\nPlease enter a new Product ID", "InSys", MessageBoxButtons::OK,MessageBoxIcon::Exclamation);
				 } catch(Exception^ e)
				 {
					 MessageBox::Show("An unknown error has occured: " + e->Message, "InSys", MessageBoxButtons::OK,MessageBoxIcon::Error);
				 }
			 }

			 /// \brief Selection in the product category selector changed
			 ///
			 /// When a selection is made the necessary components are made available to the user
	private: System::Void cmbProductCategorySelect_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
				 btnProductAdd->Enabled = true;
			 }

			 /// \brief Search button on the product tab is pressed - performs search function
	private: System::Void btnProductSearch_Click(System::Object^  sender, System::EventArgs^  e) {
				 try{
					 Table prod = new Product();
					 string resultOfSearch;
					 if(cmbProductSearch->SelectedIndex == 0)
					 {
						 resultOfSearch = prod->search("product_id",marshal_as<std::string>(txtProductSearch->Text->ToString()));
					 } else if(cmbProductSearch->SelectedIndex == 1)
					 {
						 resultOfSearch = prod->search("category_id",marshal_as<std::string>(txtProductSearch->Text->ToString()));
					 } else if(cmbProductSearch->SelectedIndex == 2)
					 {
						 resultOfSearch = prod->search("name",marshal_as<std::string>(txtProductSearch->Text->ToString()));
					 } else if(cmbProductSearch->SelectedIndex == 3)
					 {
						 resultOfSearch = prod->search("description",marshal_as<std::string>(txtProductSearch->Text->ToString()));
					 } else if(cmbProductSearch->SelectedIndex == 4)
					 {
						 resultOfSearch = prod->search("price",marshal_as<std::string>(txtProductSearch->Text->ToString()));
					 }
					 delete prod;

					 System::String^ results = gcnew String (resultOfSearch.c_str());

					 txtProductSearchResults->Text = results;

				 } catch (DoesNotExistException e)
				 {
					 MessageBox::Show("Search parameters did not return any results", "InSys", MessageBoxButtons::OK,MessageBoxIcon::Information);
					 txtProductSearchResults->Text = "";
					 txtProductSearch->Text = "";
				 } catch(Exception^ e) {
					 MessageBox::Show("An unknown error has occured: " + e->Message, "InSys", MessageBoxButtons::OK,MessageBoxIcon::Error);
				 }
			 }

			 /// \brief Selection in the product search by selector changed
			 ///
			 /// When a selection is made the necessary components are made available to the user
	private: System::Void cmbProductSearch_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
				 txtProductSearchResults->Text = "";
				 txtProductSearch->Text = "";
			 }

			 /// \brief Selection in the product selector changed
			 ///
			 /// When a selection is made the necessary components are made available to the user
			 /// and the data for the selected product is filled into the appropriate components
	private: System::Void cmbProductSelect_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
				 // Enable components for modify function
				 txtProductName->Enabled = true;
				 txtProductDescription->Enabled = true;
				 txtProductPrice->Enabled = true;
				 btnProductModify->Enabled = true;
				 cmbProductCategorySelect->Enabled = true;

				 // Enable button for delete
				 btnProductDelete->Enabled = true;

				 // string containing contents of the selection in the combobox
				 System::String^ product = cmbProductSelect->SelectedItem->ToString();

				 // position of delimiter one
				 int delimiter1 = product->IndexOf("|");
				 //position of delimiter two
				 int delimiter2 = product->IndexOf("|", delimiter1 + 1);
				 //position of delimiter three
				 int delimiter3 = product->IndexOf("|", delimiter2 + 1);
				 //position of delimiter four
				 int delimiter4 = product->IndexOf("|", delimiter3 + 1);

				 // string containing name from the combobox
				 System::String ^ prodID = product->Substring(0,delimiter1);

				 // placing prodID string into textbox
				 txtProdID->Text = prodID;

				 // string containing categoryID from the combobox
				 System::String ^ categoryID = product->Substring(delimiter1 + 1, delimiter2 - delimiter1 - 1);

				 try{
					 Table cat = new Category();
					 string categoryResult = cat->search("category_id",marshal_as<std::string>(categoryID));
					 System::String^ categoryResultString = gcnew String(categoryResult.c_str());
					 cmbProductCategorySelect->SelectedItem = categoryResultString;
				 } catch(DoesNotExistException e){
					 MessageBox::Show("Category does not exist, /r/nPlease select a new category", "InSys", MessageBoxButtons::OK,MessageBoxIcon::Information);
				 } catch (...) {
					 MessageBox::Show("An unknown error has occured", "InSys", MessageBoxButtons::OK,MessageBoxIcon::Error);
				 }

				 // string containing product description from the combobox
				 System::String ^ description = product->Substring(delimiter2 + 1, delimiter3 - delimiter2 - 1);

				 // placing description string into textbox
				 txtProductDescription->Text = description;

				 // string containing product name from the combobox
				 System::String ^ name = product->Substring(delimiter3 + 1, delimiter4 - delimiter3 - 1);

				 // placing description string into textbox
				 txtProductName->Text = name;

				 // string containing product price from the combobox
				 System::String ^ price = product->Substring(delimiter4 + 1);

				 // placing description string into textbox
				 txtProductPrice->Text = price;
			 }

			 /// \brief Modify button on the product tab is pressed - performs modify function
	private: System::Void btnProductModify_Click(System::Object^  sender, System::EventArgs^  e) {

				 regex rgxPrice("^[0-9]*[.][0-9][0-9]$");

				 regex rgxID("^[0-9]+$");

				 cmatch match;

				 string productID = marshal_as<std::string>(txtProdID->Text->ToString());
				 const char *targetProdID = productID.c_str();

				 string productPrice = marshal_as<std::string>(txtProductPrice->Text->ToString());
				 const char *targetProductPrice = productPrice.c_str();

				 // string for contents of selction in drop box
				 System::String^ category = cmbProductCategorySelect->SelectedItem->ToString();
				 // position of first delimiter
				 int delimiter1 = category->IndexOf("|");

				 // string for category ID
				 System::String ^ categoryID = category->Substring(0,delimiter1);

				 // convert System::String to std::string
				 string categoryIDstring(marshal_as<std::string>(categoryID));


				 if(txtProdID->Text->Length == 0)
				 {
					 MessageBox::Show("Please Enter a Product ID", "InSys", MessageBoxButtons::OK,MessageBoxIcon::Error);
				 } else if(!regex_search(targetProdID, match, rgxID)) {
					 MessageBox::Show("Invalid characters - please change the product ID", "InSys", MessageBoxButtons::OK,MessageBoxIcon::Error);
				 } else if(txtProductName->Text->Contains("|")) {
					 MessageBox::Show("| is a reserved character - Please change the product name", "InSys", MessageBoxButtons::OK,MessageBoxIcon::Error);
				 } else if(txtProductDescription->Text->Contains("|")) {
					 MessageBox::Show("| is a reserved character - Please change the product description", "InSys", MessageBoxButtons::OK,MessageBoxIcon::Error);
				 } else if(!regex_search(targetProductPrice, match, rgxPrice)) {
					 MessageBox::Show("Invalid characters - please change the product price", "InSys", MessageBoxButtons::OK,MessageBoxIcon::Error);
				 } else {

					 Table prod = new Product();

					 // perform product modify function for product category id
					 prod->modifyRow(marshal_as<std::string>(txtProdID->Text->ToString()),"categoryID", categoryIDstring);

					 // perform product modify function for product name
					 prod->modifyRow(marshal_as<std::string>(txtProdID->Text->ToString()),"name",
						 marshal_as<std::string>(txtProductName->Text->ToString()));

					 // perform product modify function for product description
					 prod->modifyRow(marshal_as<std::string>(txtProdID->Text->ToString()),"description",
						 marshal_as<std::string>(txtProductDescription->Text->ToString()));

					 // perform product modify function for product price
					 prod->modifyRow(marshal_as<std::string>(txtProdID->Text->ToString()),"price",
						 marshal_as<std::string>(txtProductPrice->Text->ToString()));

					 delete prod;

					 // disable and clear components for modify function
					 txtProdID->Text = "";
					 txtProductName->Enabled = false;
					 txtProductName->Text = "";
					 txtProductDescription->Enabled = false;
					 txtProductDescription->Text = "";
					 txtProductPrice->Enabled = false;
					 txtProductPrice->Text = "";
					 btnProductModify->Enabled = false;
					 cmbProductCategorySelect->Enabled = false;
					 // clear combobox
					 cmbProductCategorySelect->Items->Clear();

					 // currentRow string
					 System::String ^ currentRow;

					 // vector to contain the category file contents
					 vector<string> categoriesFile;
					 // retrieve vector containing contents of category file
					 categoriesFile = returnFile("textFiles/category.txt");

					 // insert contents of category file into combobox
					 for(size_t i = 0; i < categoriesFile.size(); i++)
					 {
						 currentRow = gcnew String (categoriesFile[i].c_str());
						 cmbProductCategorySelect->Items->Add(currentRow);
					 }

					 // populate product selection drop down list
					 cmbProductSelect->Items->Clear();

					 // vector to contain the category file contents
					 vector<string> productFile;
					 // retrieve vector containing contents of category file
					 productFile = returnFile("textFiles/product.txt");

					 // insert contents of category file into combobox
					 for(size_t i = 0; i < productFile.size(); i++)
					 {
						 currentRow = gcnew String (productFile[i].c_str());
						 cmbProductSelect->Items->Add(currentRow);
					 }
				 }
			 }

			 /// \brief Delete button on the product tab is pressed - performs delete function
	private: System::Void btnProductDelete_Click(System::Object^  sender, System::EventArgs^  e) {

				 if(MessageBox::Show("Are you sure you wish to delete the selected product?", "InSys", MessageBoxButtons::YesNo,MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::Yes)
				 {
					 // create instance of Category()
					 Table prod = new Product();

					 // string for contents of selction in drop box
					 System::String^ product = cmbProductSelect->SelectedItem->ToString();
					 // position of first delimiter
					 int delimiter1 = product->IndexOf("|");

					 // string for category ID
					 System::String ^ prodID = product->Substring(0,delimiter1);

					 // convert System::String to std::string
					 string productIDString(marshal_as<std::string>(prodID));

					 // call Category Delete function
					 prod->deleteRow(productIDString);

					 // delete instance of category
					 delete prod;


					 // disable button for delete
					 btnProductDelete->Enabled = false;

					 // clear combobox
					 cmbProductSelect->Items->Clear();

					 // currentRow string
					 System::String ^ currentRow;

					 // vector to contain the category file contents
					 vector<string> productFile;
					 // retrieve vector containing contents of category file
					 productFile = returnFile("textFiles/product.txt");

					 // insert contents of category file into combobox
					 for(size_t i = 0; i < productFile.size(); i++)
					 {
						 currentRow = gcnew String (productFile[i].c_str());
						 cmbProductSelect->Items->Add(currentRow);
					 }
				 }
			 }

	private: System::Void cmbSalesProductSelect_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
				 if (cmbSalesFunction->SelectedIndex == 0 )
				 {
					 Table summary = new Summary();
					 txtSalesProductQuantity->Enabled = true; // enable the product quantity textbox
					 txtSalesProductDiscount->Enabled = true;
					 btnSalesRemoveProduct->Enabled = true;
					 // Integers to store delimiter positions
					 int delimiter1, delimiter2, delimiter3, delimiter4;

					 // strings to store returns of searches
					 std::string qunatityReturned;
					 // string to store current row in of search result of Orders
					 std::string currentRow;

					 System::String^ product_id;

					 std::string qSold;
					 // product_id to add to listbox
					 System::String^ product = cmbSalesProductSelect->SelectedItem->ToString();

					 // find positions of delimiters
					 int delimiter7 = product->IndexOf("|");
					 int delimiter8 = product->IndexOf("|", delimiter7 + 1);

					 // Get product_id, name, and quantity
					 product_id = product->Substring(0, delimiter7-1);

					 std::string pid = marshal_as<std::string>(product_id);
					 qunatityReturned = summary->search("product_id", pid);


					 delimiter3 = qunatityReturned.find("|");
					 delimiter4 = qunatityReturned.find("|", delimiter3+1);
					 qSold = qunatityReturned.substr(delimiter3+1);
					 MessageBox::Show("The total quantity of product#  " + gcnew String (pid.c_str())+ " is " + gcnew String(qSold.c_str()) + "Please enter a quantity below "+ gcnew String (qSold.c_str()));
				 }
				 else if (cmbSalesFunction->SelectedIndex == 0 || cmbSalesFunction->SelectedIndex == 1)

					 txtSalesProductQuantity->Enabled = true; // enable the product quantity textbox
				 txtSalesProductDiscount->Enabled = true;
				 btnSalesRemoveProduct->Enabled = true; // enable remove button

				 // Enable add product only if input of Quantity text is numerical
				 if (System::Text::RegularExpressions::Regex::IsMatch
					 (txtSalesProductQuantity->Text, "^[0-9]*$") &&
					 txtSalesProductQuantity->Text != "")
					 btnSalesAddProduct->Enabled = true;
				 else
					 btnSalesAddProduct->Enabled = false;

				 if (System::Text::RegularExpressions::Regex::IsMatch
					 (txtSalesProductDiscount->Text, "^[0-9]*$") &&
					 txtSalesProductDiscount->Text != "")
					 btnSalesAddProduct->Enabled = true;
				 else
					 btnSalesAddProduct->Enabled = false;
			 }

			 /// \brief Ensures that AddProduct and modify Invoice button is disabled if user input is not numeric
	private: System::Void txtSalesProductQuantity_TextChanged(System::Object^  sender, System::EventArgs^  e) {

				 // Enable AddProduct only if ProductQuantity text is numeric and non-empty
				 if (System::Text::RegularExpressions::Regex::IsMatch
					 (txtSalesProductQuantity->Text, "^[0-9]*$") &&
					 txtSalesProductQuantity->Text != "")
				 {
					 btnSalesAddProduct->Enabled = true;
					 btnSalesModify->Enabled = true;
				 }
				 else
				 {
					 btnSalesAddProduct->Enabled = false;
					 btnSalesModify->Enabled = false;
				 }

			 }

	private: System::Void txtSalesProductDiscount_TextChanged(System::Object^  sender, System::EventArgs^  e) {

				 // Enable AddProduct only if Product discount text is numeric and non-empty

				 if (System::Text::RegularExpressions::Regex::IsMatch
					 (txtSalesProductDiscount->Text, "^[0-9]*$") &&
					 txtSalesProductDiscount->Text != "")
				 {
					 btnSalesAddProduct->Enabled = true;
					 btnSalesModify->Enabled = true;
				 }
				 else
				 {
					 btnSalesAddProduct->Enabled = false;
					 btnSalesModify->Enabled = false;
				 }
			 }

			 /// \brief Adds product, name, discountand quantity to the Product listbox when AddProduct button is clicked
			 ///
			 /// \post Sales is added to database if Modify is selected
	private: System::Void btnSalesAddProduct_Click(System::Object^  sender, System::EventArgs^  e) {
				 lstSalesProductList->Enabled = true;
				 dtSalesReceiptDate->Enabled = true;
				 btnSalesRemoveProduct->Enabled = true;

				 System::String^ product = cmbSalesProductSelect->SelectedItem->ToString();

				 // find positions of delimiters
				 int delimiter7 = product->IndexOf("|");
				 int delimiter8 = product->IndexOf("|", delimiter7 + 1);
				 std::string qunatityReturned;
				 // Get product_id, name, and quantity
				 System::String^ product_id = product->Substring(0, delimiter7-1);
				 std::string pid = marshal_as<std::string>(product_id);
				 Table summary = new Summary();
				 qunatityReturned = summary->search("product_id", pid);
				 std::string qSold1;

				 int  delimiter5 = qunatityReturned.find("|");
				 int  delimiter6 = qunatityReturned.find("|", delimiter5+1);
				 qSold1 = qunatityReturned.substr(delimiter5+1);

				 System::String^ quanSold; // quantity sold to add to listbox

				 // enable listbox, remove product, date, and create receipt


				 //System::String^ product_id; // product_id to add to listbox
				 System::String^ qSold; // quantity sold to add to listbox
				 System::String^ product_name; // name to add to listbox
				 System::String^ discount; //discount to add to listbox
				 System::String^ quantityReturned;

				 // find positions of delimiters
				 int delimiter1 = product->IndexOf("|");
				 int delimiter2 = product->IndexOf("|", delimiter1 + 1);
				 int delimiter3 = product->IndexOf("|", delimiter2 + 1);
				 int delimiter4 = product->IndexOf("|", delimiter3 + 1);

				 // Get product_id, name, and quantity
				 // product_id = product->Substring(0, delimiter1-1);
				 product_name = product->Substring(delimiter3 + 2, delimiter4 - delimiter3 - 3);
				 quanSold = txtSalesProductQuantity->Text;
				 std::string qSTDStr = marshal_as<std::string>(quanSold);

				 //convert strings to integers
				 int stock = atoi(qSold1.c_str());
				 int sold = atoi(qSTDStr.c_str()); 
				 if (stock < sold)
				 {
					 MessageBox::Show("Invalid quantity! Please enter a valid number. ");
					 txtSalesProductDiscount->Text="";
					 txtSalesProductQuantity->Text="";
				 }
				 else{
					 stock = stock - sold;
					 System::String^ test;
					 test = System::Convert::ToString(stock);

					 std::string strStock = marshal_as<std::string>(test);

					 summary->modifyRow(pid, "total_quantity", strStock);
					 delete summary;

					 discount = txtSalesProductDiscount->Text;
					 quanSold = txtSalesProductQuantity->Text;
					 // Add product_id, name, discountand quantity to listbox if on Add function
					 if (cmbSalesFunction->SelectedIndex == 0)
					 {
						 lstSalesProductList->Items->Add(product_id + " | " + quanSold + " | "+ product_name + " | " + discount);
						 btnSalesCreateReceipt->Enabled = true; // Enable creating invoice when Adding invoice
					 }

					 // disable adding product and clear/disable quantity
					 txtSalesProductQuantity->Enabled = false;
					 txtSalesProductQuantity->Text = "";
					 txtSalesProductDiscount->Enabled = false;
					 txtSalesProductDiscount->Text = "";
					 btnSalesAddProduct->Enabled = false;

					 if (cmbSalesFunction->SelectedIndex == 1)
					 {
						 Table sales = new Sales();
						 Table salesSummary = new SalesSummary();
						 Table summary = new Summary ();
						 // get receipt_id of selected invoice by converting selection to std::string
						 // and then breaking it up with delimiters
						 System::String^ receiptStr = cmbSalesReceiptSelect->SelectedItem->ToString();
						 std::string receiptString = marshal_as<std::string>(receiptStr);
						 delimiter1 = receiptString.find(":");
						 delimiter2 = receiptString.find(":", delimiter1 + 1);
						 std::string receipt_id = receiptString.substr(delimiter1 + 2, delimiter2 - delimiter1 - 17); 

						 //Convert product_id and quantity, discount and receipt id to std::string
						 std::string product_idString = marshal_as<std::string>(product_id);
						 std::string quantityString = marshal_as<std::string>(quanSold);

						 std::string discountString = marshal_as<std::string>(discount);
						 std::string salesID; // string to store sales of last row
						 std::string searchResult; // string to store search of sales
						 std::string currentRow; // string to store current row

						 vector<string> addVector; // vector used to add to table

						 // add new row to InvoiceItem
						 addVector.push_back(quantityString); 
						 // quantityCheck();
						 addVector.push_back(product_idString);					
						 addVector.push_back(discountString);
						 sales->add(addVector);
						 addVector.clear();

						 //** find the sales id of newly added product by getting last line of search **//
						 //** for newly added product by breaking the searchResult down line by line         **//

						 // first search for newly added product
						 searchResult = sales->search("productID", product_idString);

						 // get the first row of the search result
						 delimiter1 = searchResult.find("\n");
						 currentRow = searchResult.substr(0, delimiter1+1);

						 // if the current row is not equal to the search result
						 // keep breaking down searchResult line by line until they do
						 // ie, currentRow is the last line of the search result
						 while ( currentRow != searchResult){
							 searchResult = searchResult.substr(delimiter1+1);
							 delimiter1 = searchResult.find("\n");
							 currentRow = searchResult.substr(0, delimiter1 + 1);
						 }

						 // get the latest sales
						 delimiter1 = currentRow.find("|");
						 salesID = currentRow.substr(0, delimiter1);

						 //** end search for newest sales **//

						 // add new row to sales summary
						 addVector.push_back(receipt_id);
						 addVector.push_back(salesID);
						 salesSummary->add(addVector);

						 // add new sales to the Product List
						 lstSalesProductList->Items->Add(gcnew String(salesID.c_str()) + 
							 " | " + product_id + " | " + product_name +" | " + quanSold + " | " + discount);

						 delete salesSummary;
						 delete sales;
					 }
				 }
			 }

			 /// \brief Creates an receipt when Create Receipt is clicked
	private: System::Void btnSalesCreateReceipt_Click(System::Object^  sender, System::EventArgs^  e) {

				 // Create table classes
				 Table receipt = new Receipt();
				 Table sales = new Sales();
				 Table salesSummary = new SalesSummary();

				 // Vectors for adding to tables
				 vector<string> receiptAddVector;
				 vector<string> salesAddVector;
				 vector<string> salesSummaryAddVector;

				 int delimiter1, delimiter2, delimiter3; // position of delimiters in current product and currentRow

				 std::string currentRow; // current row in the search
				 std::string searchResult; // searchResult of sales or receipt search

				 // get the date from date time picker and convert to std::string
				 System::String^ receiptDate = dtSalesReceiptDate->Value.ToString("yyyy-MM-dd");
				 string receipt_date(marshal_as<std::string>(receiptDate));

				 // add invoice to receipt table
				 receiptAddVector.push_back(receipt_date);
				 receipt->add(receiptAddVector);

				 //** find the receipt_id of newly added invoice by getting last line of search **//
				 //** for newly added receipt by breaking the searchResult down line by line     **//

				 // first search for newly added product
				 searchResult = receipt->search("date", receipt_date);

				 // get the first row of the search result
				 delimiter1 = searchResult.find("\n");
				 currentRow = searchResult.substr(0, delimiter1+1);

				 // if the current row is not equal to the search result
				 // keep breaking down searchResult line by line until they do
				 // ie, currentRow is the last line of the search result
				 while ( currentRow != searchResult){
					 searchResult = searchResult.substr(delimiter1+1);
					 delimiter1 = searchResult.find("\n");
					 currentRow = searchResult.substr(0, delimiter1 + 1);
				 }

				 // get the latest receipt_id
				 delimiter1 = currentRow.find("|");
				 string receipt_id = currentRow.substr(0, delimiter1); 

				 //** end search for newest receipt_id **//

				 // copy the contents of Product listbox to an array called productList
				 cli::array<System::String^>^ productList = 
					 gcnew cli::array<String^>(lstSalesProductList->Items->Count);
				 lstSalesProductList->Items->CopyTo(productList, 0);


				 System::String^ productListItem; // store each item of productList
				 std::string productListItemSTD; // std::string equivalent of productListItem
				 std::string salesID, product_id, qSold, discount; // strings to store values to add to table

				 // take first item of productList and convert to std::string
				 productListItem = productList[0];
				 productListItemSTD = marshal_as<std::string>(productListItem);

				 // find postions of delimiters
				 delimiter1 = productListItemSTD.find("|");
				 delimiter2 = productListItemSTD.find("|", delimiter1 + 1);

				 delimiter3 = productListItemSTD.find('|', delimiter2+1);
				 // initiate product_id, discount and quantity
				 product_id = productListItemSTD.substr(0, delimiter1 - 1);
				 qSold = productListItemSTD.substr(delimiter1+1, delimiter2-delimiter1-1);
				 discount = productListItemSTD.substr(delimiter3 + 1);
				 // set up the addVector
				 salesAddVector.push_back(qSold);

				 salesAddVector.push_back(product_id);
				 salesAddVector.push_back(discount);

				 // add the new row to sales
				 sales->add(salesAddVector);

				 //** find the sales_id of newly added product by getting last line of search **//
				 //** for newly added product by breaking the searchResult down line by line         **//

				 // first search for newly added product
				 searchResult = sales->search("productID", product_id);

				 // get the first row of the search result
				 delimiter1 = searchResult.find("\n");
				 currentRow = searchResult.substr(0, delimiter1+1);

				 // if the current row is not equal to the search result
				 // keep breaking down searchResult line by line until they do
				 // ie, currentRow is the last line of the search result
				 while ( currentRow != searchResult){
					 searchResult = searchResult.substr(delimiter1+1);
					 delimiter1 = searchResult.find("\n");
					 currentRow = searchResult.substr(0, delimiter1 + 1);
				 }

				 // get the latest sales id
				 delimiter1 = currentRow.find("|");
				 salesID = currentRow.substr(0, delimiter1);

				 //** end search for newest sales id **//

				 stringstream converter; // stringstream to convert int to string

				 // add new sales_id and receipt_id to receipt
				 salesSummaryAddVector.push_back(receipt_id);
				 salesSummaryAddVector.push_back(salesID);
				 salesSummary->add(salesSummaryAddVector);

				 // continue to add sales and receipts until the all items in the list are added
				 for( int i = 1; i < lstSalesProductList->Items->Count; i++) {
					 // take first item of productList and convert to std::string
					 productListItem = productList[i];
					 productListItemSTD = marshal_as<std::string>(productListItem);

					 // find postions of delimiters
					 delimiter1 = productListItemSTD.find("|");
					 delimiter2 = productListItemSTD.find("|", delimiter1 + 1);
					 delimiter3 = productListItemSTD.find("|", delimiter2 + 1);
					 // initiate product_id and quantity
					 product_id = productListItemSTD.substr(0, delimiter1 - 1);
					 qSold = productListItemSTD.substr(delimiter1+1, delimiter2-delimiter1-1);
					 discount = productListItemSTD.substr(delimiter3 + 1);
					 // clear both add vectors and stringstream converter
					 salesAddVector.clear();
					 salesSummaryAddVector.clear();
					 converter.str("");

					 // set up the addVector for sales
					 salesAddVector.push_back(qSold);
					 salesAddVector.push_back(product_id);					 
					 salesAddVector.push_back(discount);

					 // set up the addVector for summary
					 converter << atoi(salesID.c_str()) + 1; //auto increment
					 salesID = converter.str();
					 salesSummaryAddVector.push_back(receipt_id);
					 salesSummaryAddVector.push_back(salesID);

					 // add the new row to sales
					 sales->add(salesAddVector);
					 salesSummary->add(salesSummaryAddVector);
				 }

				 delete receipt;
				 delete sales;
				 delete salesSummary;

				 // disable controls so another receipt can be added
				 lstSalesProductList->Items->Clear();
				 btnSalesRemoveProduct->Enabled = false;
				 btnSalesCreateReceipt->Enabled = false;
				 dtSalesReceiptDate->Enabled = false;

				 // display a message that invoice has been created
				 MessageBox::Show("Receipt #" + gcnew String(receipt_id.c_str()) + 
					 " created at " + gcnew String(receipt_date.c_str()));
			 }

			 /// \brief Removes an sales from product list when Remove Product is clicked
			 ///
			 /// \post sales is deleted from database if modifying
	private: System::Void btnSalesRemoveProduct_Click(System::Object^  sender, System::EventArgs^  e) {



				 //disable itself,  date, and creat receipt if there is nothing in the listbox
				 if (lstSalesProductList->Items->Count != 0 && cmbSalesFunction->SelectedIndex == 0) // Adding an receipt
				 {
					 // remove selected product
					 lstSalesProductList->Items->Remove(lstSalesProductList->SelectedItem); 

					 btnSalesRemoveProduct->Enabled = true;
					 btnSalesCreateReceipt->Enabled = true;
					 dtSalesReceiptDate->Enabled = true;
				 }
				 else if (cmbSalesFunction->SelectedIndex == 1)
				 {
					 // Must always have one item linked to an receipt
					 if (lstSalesProductList->Items->Count == 1)
					 {
						 MessageBox::Show("You must always have at least one item in an Receipt!");
					 }
					 // delete row from invoice item table and product list
					 // only when something is selected
					 else if (lstSalesProductList->SelectedIndex >= 0)
					 {
						 Table sales = new Sales();
						 Table salesSummary = new SalesSummary();

						 // store current row as an std::string
						 std::string currentRow = marshal_as<std::string>
							 (lstSalesProductList->SelectedItem->ToString());

						 // find the delimiter position
						 int delimiter = currentRow.find('|');

						 // get the sales id
						 std::string salesID = currentRow.substr(0, delimiter - 1);

						 // delete receipt item from database
						 sales->deleteRow(salesID);
						 salesSummary->deleteRow(salesID);

						 // remove selected product from listbox
						 lstSalesProductList->Items->Remove(lstSalesProductList->SelectedItem); 

						 delete sales;
						 delete salesSummary;
					 }
				 }
			 }

			 /// \brief populates the recetpt combobox with receipt of the same date selected
	private: System::Void btnSalesSearch_Click(System::Object^  sender, System::EventArgs^  e) {
				 Table receipt = new Receipt();

				 cmbSalesReceiptSelect->Items->Clear();
				 lstSalesProductList->Items->Clear();
				 lstSalesProductList->Enabled = false;

				 // get the date from date picker and convert to std::string
				 System::String^ receiptDate = dtSalesReceiptDate->Value.ToString("yyyy-MM-dd");
				 std::string date(marshal_as<std::string>(receiptDate));

				 std::string receiptReturned; 

				 // try to find the receipt according to selected date otherwise display an error
				 try { 
					 int delimiter1, delimiter2; // positon of delimiters
					 std::string receipt_id; // store receipt_id of search
					 std::string currentRow; //store the first row of multiple-lined search result

					 receiptReturned = receipt->search("date", date);

					 // get the first row of the search result
					 delimiter1 = receiptReturned.find("\n");
					 currentRow = receiptReturned.substr(0, delimiter1+1);

					 while (!receiptReturned.empty()){
						 delimiter1 = receiptReturned.find("\n");
						 receiptReturned = receiptReturned.substr(delimiter1+1);
						 delimiter2 = currentRow.find("|");
						 receipt_id = currentRow.substr(0, delimiter2);

						 cmbSalesReceiptSelect->Items->Add( "Receipt ID: " +
							 gcnew String(receipt_id.c_str()) + "   Date: " +
							 gcnew String(date.c_str()));

						 delimiter1 = receiptReturned.find("\n");
						 currentRow = receiptReturned.substr(0, delimiter1+1);
					 }

					 cmbSalesReceiptSelect->Enabled = true;
				 }
				 catch (DoesNotExistException e) {
					 MessageBox::Show(gcnew String(e.what())); // error message
				 }
				 delete receipt;
			 }

			 /// \brief Fills the listbox with sales of selected invoice
	private: System::Void cmbSalesReceiptSelect_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {


				 lstSalesProductList->Enabled = true;
				 populateSalesProductList();

				 // if modifying, display a list of products as well
				 if (cmbSalesFunction->SelectedIndex == 1)
				 {
					 cmbSalesProductSelect->Enabled = true;
					 btnSalesModifyReceipt->Enabled = true;

					 // clear combobox
					 cmbSalesProductSelect->Items->Clear();

					 // currentRow string
					 System::String ^ currentRow;

					 // vector to contain the product file contents
					 vector<string> productFile;
					 // retrieve vector containing contents of product file
					 productFile = returnFile("textFiles/product.txt");

					 // insert contents of product file vector into combobox
					 for(size_t i = 0; i < productFile.size(); i++)
					 {
						 currentRow = gcnew String (productFile[i].c_str()); // convert to System::String^

						 // Add spaces between |'s
						 cmbSalesProductSelect->Items->Add(currentRow->Replace("|", " | "));
					 }
				 }
			 }
	private: System::Void lstSalesProductList_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {

				 // make sure something is selected
				 if (lstSalesProductList->SelectedIndex >= 0)
				 {
					 // if Modifying a receipt
					 if (cmbSalesFunction->SelectedIndex == 1)
					 {
						 // Enable the product quantity textbox and button, and remove button
						 txtSalesProductQuantity->Enabled = true;
						 txtSalesProductDiscount->Enabled = true;


						 btnSalesModify->Enabled = true;
						 btnSalesRemoveProduct->Enabled = true;
						 System::String^ sales; // store selected invoice item
						 System::String^ product_id; // product id of selected invoice item
						 System::String^ qSold; // quantity of selected sales
						 System::String^ discount; 
						 int delimiter1, delimiter2, delimiter3, delimiter4; // position of delimiters

						 // store the selected item as a string
						 sales = lstSalesProductList->SelectedItem->ToString();

						 // find positons of delimiters
						 delimiter1 = sales->IndexOf('|');
						 delimiter2 = sales->IndexOf('|', delimiter1 + 1);
						 delimiter3 = sales->IndexOf('|', delimiter2 + 1);

						 delimiter4 = sales->IndexOf('|', delimiter3 + 1);
						 // find product_id and quantity of selected invoice item
						 product_id = sales->Substring(delimiter1 + 1, delimiter2 - delimiter1 - 1);
						 qSold = sales->Substring(delimiter3 + 1, delimiter4 - delimiter3-1); 
						 discount = sales->Substring(delimiter4 + 1);
						 // select the product in the product select combobox
						 cmbSalesProductSelect->SelectedIndex = cmbSalesProductSelect->FindString(product_id);
						 MessageBox::Show("Please select product# "+ gcnew String(product_id) + 
							 " from the Product combo box if you would like to modify a product. "  );

						 // change the text of the Product Quantity combobox to the selected quantity
						 txtSalesProductQuantity->Text = qSold->Trim();
						 txtSalesProductDiscount->Text = discount->Trim();
						 btnSalesAddProduct->Enabled= false;
						 btnSalesModify->Enabled=false;
						 txtSalesProductDiscount->Enabled = false;
						 txtSalesProductQuantity->Enabled = false;

					 } // end if selected function is modify
				 } // end if a list item is currently selecte
			 }

			 /// \brief modifies the selected receipt's date with date selected
	private: System::Void btnSalesModifyReceipt_Click(System::Object^  sender, System::EventArgs^  e) {

				 Table receipt = new Receipt();

				 int delimiter1, delimiter2; // position of delimiters

				 // get the date from date time picker and convert to std::string
				 System::String^ receiptDate = dtSalesReceiptDate->Value.ToString("yyyy-MM-dd");
				 string date(marshal_as<std::string>(receiptDate));

				 // get receipt_id of selected receipt by converting selection to std::string
				 // and then breaking it up with delimiters
				 System::String^ receiptSystemStr = cmbSalesReceiptSelect->SelectedItem->ToString();
				 std::string receiptSTDStr = marshal_as<std::string>(receiptSystemStr);
				 delimiter1 = receiptSTDStr.find(":");
				 delimiter2 = receiptSTDStr.find(":", delimiter1 + 1);
				 std::string receipt_id = receiptSTDStr.substr(delimiter1 + 2, delimiter2 - delimiter1 - 17); 

				 // chage the date of the receipt
				 receipt->modifyRow(receipt_id, "date", date);

				 MessageBox::Show("Receipt Date Modified");

				 // refresh modify
				 cmbSalesFunction->SelectedIndex = 0;
				 cmbSalesFunction->SelectedIndex = 1;

				 delete receipt;

			 }

			 /// \brief modifies the selected sales
	private: System::Void btnSalesModify_Click(System::Object^  sender, System::EventArgs^  e) {

				 Table sales= new Sales();

				 System::String^ Sales; // selected sales;
				 System::String^ Product; // selected product
				 System::String^ salesID; // id of selected sales id
				 System::String^ product_id; // product_id of selected product
				 System::String^ qSold; // changed quantity
				 System::String^ discount;
				 int delimiter1; // position of delimiter

				 // find sales_id
				 Sales = lstSalesProductList->SelectedItem->ToString();
				 delimiter1 = Sales->IndexOf('|');
				 salesID = Sales->Substring(0, delimiter1 - 1);

				 // find product_id
				 Product = cmbSalesProductSelect->SelectedItem->ToString();
				 delimiter1 = Product->IndexOf('|');
				 product_id = Product->Substring(0, delimiter1 - 1);

				 // find quantity
				 qSold = txtSalesProductQuantity->Text;

				 discount = txtSalesProductDiscount->Text;
				 // convert to std::string
				 std::string sales_idString = marshal_as<std::string>(salesID);
				 std::string product_idString = marshal_as<std::string>(product_id);
				 std::string qSoldString = marshal_as<std::string>(qSold);
				 std::string discountString = marshal_as<std::string>(discount);
				 // modify row and dislay message
				 sales->modifyRow(sales_idString, "quantity_sold", qSoldString);

				 sales->modifyRow(sales_idString, "discount", discountString);
				 sales->modifyRow(sales_idString, "productID", product_idString);

				 MessageBox::Show("Sales Modified");

				 // refresh the product list
				 populateSalesProductList();

				 delete sales;
			 }

			 /// \brief Resets what is visible and enabled depending on what function is selected
			 ///
			 /// Selected indices:
			 /// - 0 for Add
			 /// - 1 for Search
			 /// - 2 for Delete
			 /// - 3 for Search
	private: System::Void cmbReturnFunction_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
				 btnFunction->Text = cmbReturnFunction->Text->ToString() + " Return";
				 lblSearchBy->Text = cmbReturnFunction->Text->ToString() + " By:";
				 lblSearchValue->Text = cmbReturnFunction->Text->ToString() + " Value:";

				 switch (cmbReturnFunction->SelectedIndex)
				 {
				 case 0: //Add
					 lblSearchBy->Visible = false;
					 lblSearchValue->Visible = false;
					 cmbSearchBy->Visible = false;
					 txtSearchValue->Visible = false;
					 lblReturns->Visible = true;
					 cmbReturns->Visible = true;
					 lblSale->Visible = true;
					 cmbSale->Visible = true;
					 lblQuantityReturned->Visible = true;
					 txtQuantityReturned->Visible = true;
					 dateReturned->Visible = true;
					 btnFunction->Visible = true;
					 txtSearchReturns->Visible = false;

					 cmbReturns->Enabled = false;
					 cmbSale->Enabled = true;
					 txtQuantityReturned->Enabled = true;
					 btnFunction->Enabled = true;
					 dateReturned->Enabled = true;

					 PopulateSalesID();
					 //PopulateReturnID();

					 cmbReturns->SelectedIndex = -1;

					 break;
				 case 1: //Modify
					 lblSearchBy->Visible = true;
					 lblSearchValue->Visible = true;
					 cmbSearchBy->Visible = true;
					 txtSearchValue->Visible = true;
					 lblReturns->Visible = true;
					 cmbReturns->Visible = true;
					 lblSale->Visible = false;
					 cmbSale->Visible = false;
					 lblQuantityReturned->Visible = false;
					 txtQuantityReturned->Visible = false;
					 dateReturned->Visible = false;
					 btnFunction->Visible = true;
					 txtSearchReturns->Visible = false;

					 cmbReturns->Enabled = true;
					 cmbSale->Enabled = false;
					 txtQuantityReturned->Enabled = false;
					 btnFunction->Enabled = false;
					 dateReturned->Enabled = false;
					 txtSearchValue->Enabled = false;
					 cmbSearchBy->Enabled = false;

					 PopulateSalesID();
					 PopulateReturnID();

					 cmbSearchBy->SelectedIndex = -1;

					 break;
				 case 2: //Delete
					 lblSearchBy->Visible = false;
					 lblSearchValue->Visible = false;
					 cmbSearchBy->Visible = false;
					 txtSearchValue->Visible = false;
					 lblReturns->Visible = true;
					 cmbReturns->Visible = true;
					 lblSale->Visible = false;
					 cmbSale->Visible = false;
					 lblQuantityReturned->Visible = false;
					 txtQuantityReturned->Visible = false;
					 dateReturned->Visible = false;
					 btnFunction->Visible = true;
					 txtSearchReturns->Visible = false;
					 dateReturned->Enabled = false;
					 cmbReturns->Enabled = true;

					 //PopulateSalesID();
					 PopulateReturnID();
					 break;
				 case 3: //Search
					 lblSearchBy->Visible = true;
					 lblSearchValue->Visible = true;
					 cmbSearchBy->Visible = true;
					 txtSearchValue->Visible = true;
					 lblReturns->Visible = false;
					 cmbReturns->Visible = false;
					 lblSale->Visible = false;
					 cmbSale->Visible = false;
					 lblQuantityReturned->Visible = false;
					 txtQuantityReturned->Visible = false;
					 dateReturned->Visible = false;
					 btnFunction->Visible = true;
					 txtSearchReturns->Visible = true;

					 txtSearchValue->Enabled = true;
					 cmbSearchBy->Enabled = true;

					 btnFunction->Enabled = true;
					 break;

				 default:
					 lblSearchBy->Visible = false;
					 lblSearchValue->Visible = false;
					 cmbSearchBy->Visible = false;
					 txtSearchValue->Visible = false;
					 lblReturns->Visible = false;
					 cmbReturns->Visible = false;
					 lblSale->Visible = false;
					 cmbSale->Visible = false;
					 lblQuantityReturned->Visible = false;
					 txtQuantityReturned->Visible = false;
					 dateReturned->Visible = false;
					 btnFunction->Visible = false;
					 txtSearchReturns->Visible = false;
					 dateReturned->Enabled = true;
					 break;
				 }
			 }

			 /// \brief Changes what the user will search Returns when selected index changed according to
			 ///
			 /// Selected indices:
			 /// - 0 for sales_id
			 /// - 1 for quantity_returned
			 /// - 2 for date_returned
	private: System::Void cmbSearchBy_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {

				 if (cmbReturnFunction->SelectedIndex != 3)
				 {
					 btnFunction->Visible = true;

					 txtSearchValue->Enabled = true;
				 }
				 else
				 {
					 switch (cmbSearchBy->SelectedIndex)
					 {
					 case 0:
					 case 1:
						 txtSearchValue->Enabled = true;
						 dateReturned->Visible = false;
						 break;
					 case 2:
						 dateReturned->Enabled = true;
						 dateReturned->Visible = true;
						 txtSearchValue->Enabled = false;
						 break;
					 default:
						 break;
					 }


				 }

			 }

			 /// \brief Enables various controls when selected index is changed
	private: System::Void cmbReturns_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
				 cmbSale->Enabled = true;
				 txtQuantityReturned->Enabled = true;
				 btnFunction->Enabled = true;
				 dateReturned->Enabled = true;

				 txtSearchValue->Enabled = true;
				 cmbSearchBy->Enabled = true;
			 }

			 /// \brief When clicked, changes behaviour depending on btnFunction's current text
			 ///
			 /// - Adds a return
			 /// - Modifies a return
			 /// - Deletes a return
			 /// - Searches for a return
	private: System::Void btnFunction_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (btnFunction->Text == "Add Return")
				 {
					 //Create our return table
					 Table ret = new Returns();

					 //System strings to grab the data from our form
					 System::String ^ strAddSalesID = cmbSale->SelectedItem->ToString();
					 System::String ^ strAddQuantityReceived = txtQuantityReturned->Text->ToString();
					 System::String ^ strAddDateReceived = (dateReturned->Value.Year.ToString() + "/" + dateReturned->Value.Month.ToString() + "/" + dateReturned->Value.Day.ToString());

					 //Convert to std::string in order to use our class functions
					 string SalesID(marshal_as<std::string>(strAddSalesID));
					 string QuantityReturned(marshal_as<std::string>(strAddQuantityReceived));
					 string DateReceived(marshal_as<std::string>(strAddDateReceived));

					 //Selects just the sales ID
					 SalesID = SalesID.substr(0, SalesID.find('|'));

					 //Creates our vector with the data to add
					 vector<string> vecReturn;
					 vecReturn.push_back(SalesID);
					 vecReturn.push_back(QuantityReturned);
					 vecReturn.push_back(DateReceived);

					 //Adds the vector
					 ret->add(vecReturn);

					 cmbReturnFunction->SelectedIndex = -1;
				 }
				 else if (btnFunction->Text == "Modify Return")
				 {
					 //Create our return table
					 Table ret = new Returns();

					 //System strings to grab the data from our form
					 System::String ^ strModifyReturnsID = cmbReturns->SelectedItem->ToString();
					 System::String ^ strModifyColumn = cmbSearchBy->SelectedItem->ToString();
					 System::String ^ strModifyDate = (dateReturned->Value.Year.ToString() + "/" + dateReturned->Value.Month.ToString() + "/" + dateReturned->Value.Day.ToString());
					 System::String ^ strModifyValue = txtSearchValue->Text->ToString();

					 //Convert to std::string in order to use our class functions
					 string ReturnsID(marshal_as<std::string>(strModifyReturnsID));
					 string ModifyColumn(marshal_as<std::string>(strModifyColumn));
					 string DateToModify(marshal_as<std::string>(strModifyDate));
					 string ModifyValue(marshal_as<std::string>(strModifyValue));



					 switch (cmbSearchBy->SelectedIndex)
					 {
					 case 0:
					 case 1:
						 ret->modifyRow(ReturnsID, ModifyColumn, ModifyValue);
						 break;
					 case 2:
						 ret->modifyRow(ReturnsID, ModifyColumn, DateToModify);
						 break;
					 default:
						 break;
					 }
					 cmbReturnFunction->SelectedIndex = -1;
				 }
				 else if (btnFunction->Text == "Delete Return")
				 {
					 //Create our return table
					 Table ret = new Returns();

					 //System strings to grab the data from our form
					 System::String ^ strDeleteReturnsID = cmbReturns->SelectedItem->ToString();

					 //Convert to std::string in order to use our class functions
					 string ReturnsID(marshal_as<std::string>(strDeleteReturnsID));

					 //Selects just the returns ID
					 ReturnsID = ReturnsID.substr(0, ReturnsID.find('|'));

					 //Deletes our row
					 ret->deleteRow(ReturnsID);

					 cmbReturnFunction->SelectedIndex = -1;

				 }
				 else if (btnFunction->Text == "Search Return")
				 {
					 //Create our return table
					 Table ret = new Returns();

					 //Convert to system::String
					 System::String ^ strSearchBy = cmbSearchBy->Text->ToString();
					 System::String ^ strSearchFor = txtSearchValue->Text->ToString();
					 System::String ^ strSearchDate = (dateReturned->Value.Year.ToString() + "/" + dateReturned->Value.Month.ToString() + "/" + dateReturned->Value.Day.ToString());

					 string strSearchReturn;
					 //Convert to std::string
					 string Column(marshal_as<std::string>(strSearchBy));
					 string Value(marshal_as<std::string>(strSearchFor));
					 string DateToSearch(marshal_as<std::string>(strSearchDate));

					 //Searches our text file depending on what we are searching by
					 if (cmbSearchBy->SelectedIndex == 2)
					 {// Search by date
						 strSearchReturn = ret->search(Column, DateToSearch);
					 }
					 else
					 {//Search by quantity or sales_id
						 strSearchReturn = ret->search(Column, Value);
					 }
					 //Our search result
					 System::String ^ strSearchValue = gcnew String (strSearchReturn.c_str());
					 txtSearchReturns->Text = strSearchValue;
				 }
				 else
				 {
					 //error
				 }


			 }

			 /// \brief fills the cmbSales combobox with all sales in the database
	private: void PopulateSalesID()
			 {
				 cmbSale->Items->Clear();
				 /// \brief input stream for reading to the sales file
				 ifstream salesInFile;

				 /// \brief file name of text file for sales table
				 string salesTextFile = "textFiles/sales.txt";

				 // opens sales.txt
				 salesInFile.open(salesTextFile);

				 // ensures that categoryInFile is open
				 if(salesInFile.is_open())
				 {
					 // while loop continues as long as there is another line in the text file
					 while(salesInFile.good())
					 {
						 string sale;
						 getline(salesInFile, sale);

						 System::String ^ strSaleReturned = gcnew String (sale.c_str());
						 cmbSale->Items->Add(strSaleReturned);
					 }
				 }

			 }

			 /// \brief fills the cmbReturns combobox with all returns in the database
	private: void PopulateReturnID()
			 {
				 cmbReturns->Items->Clear();
				 /// \brief input stream for reading to the returns file
				 ifstream returnsInFile;

				 /// \brief file name of text file for returns table
				 string returnsTextFile = "textFiles/returns.txt";

				 // opens sales.txt
				 returnsInFile.open(returnsTextFile);

				 // ensures that categoryInFile is open
				 if(returnsInFile.is_open())
				 {
					 // while loop continues as long as there is another line in the text file
					 while(returnsInFile.good())
					 {
						 string returns;
						 getline(returnsInFile, returns);

						 System::String ^ strReturns = gcnew String (returns.c_str());
						 cmbReturns->Items->Add(strReturns);
					 }
				 }
			 }

			 /// \brief Set up the Product quantity textbox, and disable the add Product button
	private: System::Void cmbInvoiceProductSelect_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {

				 txtInvoiceProductQuantity->Enabled = true; // enable the product quantity textbox
				 btnInvoiceRemoveProduct->Enabled = true; // enable remove button

				 // Enable add product only if input of Quantity text is numerical
				 if (System::Text::RegularExpressions::Regex::IsMatch
					 (txtInvoiceProductQuantity->Text, "^[0-9]*$") &&
					 txtInvoiceProductQuantity->Text != "")
					 btnInvoiceAddProduct->Enabled = true;
				 else
					 btnInvoiceAddProduct->Enabled = false;

			 }

			 /// \brief Ensures that AddProduct and modify Invoice button is disabled if user input is not numeric
	private: System::Void txtInvoiceProductQuantity_TextChanged(System::Object^  sender, System::EventArgs^  e) {

				 // Enable AddProduct only if ProductQuantity text is numeric and non-empty
				 // and is not only 0
				 if (System::Text::RegularExpressions::Regex::IsMatch
					 (txtInvoiceProductQuantity->Text, "^[0-9]*$") &&
					 !System::Text::RegularExpressions::Regex::IsMatch
					 (txtInvoiceProductQuantity->Text, "^[0]*$") &&
					 txtInvoiceProductQuantity->Text != "")
				 {
					 btnInvoiceAddProduct->Enabled = true;
					 btnInvoiceModifyInvoiceItem->Enabled = true;
				 }
				 else
				 {
					 btnInvoiceAddProduct->Enabled = false;
					 btnInvoiceModifyInvoiceItem->Enabled = false;
				 }
			 }

			 /// \brief Adds product, name, and quantity to the Product listbox when AddProduct button is clicked
			 ///
			 /// \post Invoice Item is added to database if Modify is selected
	private: System::Void btnInvoiceAddProduct_Click(System::Object^  sender, System::EventArgs^  e) {

				 // enable listbox, remove product, datetime, and create invoice
				 lstInvoiceProductList->Enabled = true;
				 dtInvoiceDate->Enabled = true;
				 btnInvoiceRemoveProduct->Enabled = true;

				 System::String^ product_id; // product_id to add to listbox
				 System::String^ name; // name to add to listbox
				 System::String^ quantity; //quantity to add to listbox


				 // store current combo box selection to product
				 System::String^ product = cmbInvoiceProductSelect->SelectedItem->ToString();

				 // find positions of delimiters
				 int delimiter1 = product->IndexOf("|");
				 int delimiter2 = product->IndexOf("|", delimiter1 + 1);
				 int delimiter3 = product->IndexOf("|", delimiter2 + 1);
				 int delimiter4 = product->IndexOf("|", delimiter3 + 1);

				 // Get product_id, name, and quantity
				 product_id = product->Substring(0, delimiter1-1);
				 name = product->Substring(delimiter3 + 2, delimiter4 - delimiter3 - 3);
				 quantity = txtInvoiceProductQuantity->Text->TrimStart('0'); // remove any leading 0's

				 // Add product_id, name, and quantity to listbox if on Add function
				 if (cmbInvoiceFunction->SelectedIndex == 0)
				 {
					 lstInvoiceProductList->Items->Add(product_id + " | " + name + " | " + quantity);
					 btnInvoiceCreateInvoice->Enabled = true; // Enable creating invoice when Adding invoice
				 }

				 // disable adding product and clear/disable quantity
				 txtInvoiceProductQuantity->Enabled = false;
				 txtInvoiceProductQuantity->Text = "";
				 btnInvoiceAddProduct->Enabled = false;

				 // Add invoice item to Orders and InvoiceItem tables if Modify is selected
				 if (cmbInvoiceFunction->SelectedIndex == 1)
				 {
					 Table invoiceItem = new InvoiceItem();
					 Table orders = new Orders();

					 // get invoice_id of selected invoice by converting selection to std::string
					 // and then breaking it up with delimiters
					 System::String^ invoiceSystemStr = cmbInvoiceSelect->SelectedItem->ToString();
					 std::string invoiceString = marshal_as<std::string>(invoiceSystemStr);
					 delimiter1 = invoiceString.find(":");
					 delimiter2 = invoiceString.find(":", delimiter1 + 1);
					 std::string invoice_id = invoiceString.substr(delimiter1 + 2, delimiter2 - delimiter1 - 9); 

					 //Convert product_id and quantity and invoice_id to std::string
					 std::string product_idString = marshal_as<std::string>(product_id);
					 std::string quantityString = marshal_as<std::string>(quantity);

					 std::string invoice_item_id; // string to store invoice_item_id of last row
					 std::string searchResult; // string to store search of InvoiceItem
					 std::string currentRow; // string to store current row

					 vector<string> addVector; // vector used to add to table

					 // add new row to InvoiceItem
					 addVector.push_back(product_idString);
					 addVector.push_back(quantityString);
					 invoiceItem->add(addVector);
					 addVector.clear();

					 //** find the invoice_item_id of newly added product by getting last line of search **//
					 //** for newly added product by breaking the searchResult down line by line         **//

					 // first search for newly added product
					 searchResult = invoiceItem->search("product_id", product_idString);

					 // get the first row of the search result
					 delimiter1 = searchResult.find("\n");
					 currentRow = searchResult.substr(0, delimiter1+1);

					 // if the current row is not equal to the search result
					 // keep breaking down searchResult line by line until they do
					 // ie, currentRow is the last line of the search result
					 while ( currentRow != searchResult){
						 searchResult = searchResult.substr(delimiter1+1);
						 delimiter1 = searchResult.find("\n");
						 currentRow = searchResult.substr(0, delimiter1 + 1);
					 }

					 // get the latest invoice_item_id
					 delimiter1 = currentRow.find("|");
					 invoice_item_id = currentRow.substr(0, delimiter1);

					 //** end search for newest invoice_item_id **//

					 // add new row to orders
					 addVector.push_back(invoice_item_id);
					 addVector.push_back(invoice_id);
					 orders->add(addVector);

					 // add new invoice item to the Product List
					 lstInvoiceProductList->Items->Add(gcnew String(invoice_item_id.c_str()) + 
						 " | " + product_id + " | " + name + " | " + quantity);

					 delete orders;
					 delete invoiceItem;
				 }
			 }

			 /// \brief Creates an invoice when Create Invoice is clicked
	private: System::Void btnInvoiceCreateInvoice_Click(System::Object^  sender, System::EventArgs^  e) {

				 // Create table classes
				 Table invoice = new Invoice();
				 Table invoiceItem = new InvoiceItem();
				 Table orders = new Orders();

				 // Vectors for adding to tables
				 vector<string> invoiceAddVector;
				 vector<string> invoiceItemAddVector;
				 vector<string> ordersAddVector;

				 int delimiter1, delimiter2; // position of delimiters in current product and currentRow

				 std::string currentRow; // current row in the search
				 std::string searchResult; // searchResult of invoiceItem or invoice search

				 // get the date from date time picker and convert to std::string
				 System::String^ invoiceDate = dtInvoiceDate->Value.ToString("yyyy-MM-dd");
				 string date(marshal_as<std::string>(invoiceDate));

				 // add invoice to Invoice table
				 invoiceAddVector.push_back(date);
				 invoice->add(invoiceAddVector);

				 //** find the invoice_id of newly added invoice by getting last line of search **//
				 //** for newly added invoice by breaking the searchResult down line by line     **//

				 // first search for newly added product
				 searchResult = invoice->search("date", date);

				 // get the first row of the search result
				 delimiter1 = searchResult.find("\n");
				 currentRow = searchResult.substr(0, delimiter1+1);

				 // if the current row is not equal to the search result
				 // keep breaking down searchResult line by line until they do
				 // ie, currentRow is the last line of the search result
				 while ( currentRow != searchResult){
					 searchResult = searchResult.substr(delimiter1+1);
					 delimiter1 = searchResult.find("\n");
					 currentRow = searchResult.substr(0, delimiter1 + 1);
				 }

				 // get the latest invoice_id
				 delimiter1 = currentRow.find("|");
				 string invoice_id = currentRow.substr(0, delimiter1); // latest invoice_id

				 //** end search for newest invoice_id **//

				 // copy the contents of Product listbox to an array called productList
				 cli::array<System::String^>^ productList = 
					 gcnew cli::array<String^>(lstInvoiceProductList->Items->Count);
				 lstInvoiceProductList->Items->CopyTo(productList, 0);


				 System::String^ productListItem; // store each item of productList
				 std::string productListItemSTD; // std::string equivalent of productListItem
				 std::string invoice_item_id, product_id, quantity; // strings to store values to add to table

				 // take first item of productList and convert to std::string
				 productListItem = productList[0];
				 productListItemSTD = marshal_as<std::string>(productListItem);

				 // find postions of delimiters
				 delimiter1 = productListItemSTD.find("|");
				 delimiter2 = productListItemSTD.find("|", delimiter1 + 1);

				 // initiate product_id and quantity
				 product_id = productListItemSTD.substr(0, delimiter1 - 1);
				 quantity = productListItemSTD.substr(delimiter2 + 2);

				 // set up the addVector
				 invoiceItemAddVector.push_back(product_id);
				 invoiceItemAddVector.push_back(quantity);

				 // add the new row to InvoiceItem
				 invoiceItem->add(invoiceItemAddVector);

				 //** find the invoice_item_id of newly added product by getting last line of search **//
				 //** for newly added product by breaking the searchResult down line by line         **//

				 // first search for newly added product
				 searchResult = invoiceItem->search("product_id", product_id);

				 // get the first row of the search result
				 delimiter1 = searchResult.find("\n");
				 currentRow = searchResult.substr(0, delimiter1+1);

				 // if the current row is not equal to the search result
				 // keep breaking down searchResult line by line until they do
				 // ie, currentRow is the last line of the search result
				 while ( currentRow != searchResult){
					 searchResult = searchResult.substr(delimiter1+1);
					 delimiter1 = searchResult.find("\n");
					 currentRow = searchResult.substr(0, delimiter1 + 1);
				 }

				 // get the latest invoice_item_id
				 delimiter1 = currentRow.find("|");
				 invoice_item_id = currentRow.substr(0, delimiter1);

				 //** end search for newest invoice_item_id **//

				 stringstream converter; // stringstream to convert int to string

				 // add new invoice_item_id and invoice_id to Orders
				 ordersAddVector.push_back(invoice_item_id);
				 ordersAddVector.push_back(invoice_id);
				 orders->add(ordersAddVector);

				 // continue to add invoice_items and orders until the all items in the list are added
				 for( int i = 1; i < lstInvoiceProductList->Items->Count; i++) {
					 // take first item of productList and convert to std::string
					 productListItem = productList[i];
					 productListItemSTD = marshal_as<std::string>(productListItem);

					 // find postions of delimiters
					 delimiter1 = productListItemSTD.find("|");
					 delimiter2 = productListItemSTD.find("|", delimiter1 + 1);

					 // initiate product_id and quantity
					 product_id = productListItemSTD.substr(0, delimiter1 - 1);
					 quantity = productListItemSTD.substr(delimiter2 + 2);

					 // clear both add vectors and stringstream converter
					 invoiceItemAddVector.clear();
					 ordersAddVector.clear();
					 converter.str("");

					 // set up the addVector for Invoice Item
					 invoiceItemAddVector.push_back(product_id);
					 invoiceItemAddVector.push_back(quantity);

					 // set up the addVector for orders
					 converter << atoi(invoice_item_id.c_str()) + 1; //auto increment
					 invoice_item_id = converter.str();
					 ordersAddVector.push_back(invoice_item_id);
					 ordersAddVector.push_back(invoice_id);

					 // add the new row to InvoiceItem
					 invoiceItem->add(invoiceItemAddVector);
					 orders->add(ordersAddVector);
				 }

				 delete invoice;
				 delete invoiceItem;
				 delete orders;

				 // disable controls so another invoice can be added
				 lstInvoiceProductList->Items->Clear();
				 btnInvoiceRemoveProduct->Enabled = false;
				 btnInvoiceCreateInvoice->Enabled = false;
				 dtInvoiceDate->Enabled = false;

				 // display a message that invoice has been created
				 MessageBox::Show("Invoice #" + gcnew String(invoice_id.c_str()) + 
					 " created at " + gcnew String(date.c_str()));
			 }

			 /// \brief Removes an invoice item from product list when Remove Product is clicked
			 ///
			 /// \post invoice item is deleted from database if modifying
	private: System::Void btnInvoiceRemoveProduct_Click(System::Object^  sender, System::EventArgs^  e) {



				 //disable itself, invoice date, and creat invoice if there is nothing in the listbox
				 if (lstInvoiceProductList->Items->Count != 0 &&
					 cmbInvoiceFunction->SelectedIndex == 0) // Adding an invoice
				 {
					 // remove selected product
					 lstInvoiceProductList->Items->Remove(lstInvoiceProductList->SelectedItem); 

					 btnInvoiceRemoveProduct->Enabled = false;
					 btnInvoiceCreateInvoice->Enabled = false;
					 dtInvoiceDate->Enabled = false;
				 }
				 // if Modifying, remove invoice item from database
				 else if (cmbInvoiceFunction->SelectedIndex == 1)
				 {
					 // Must always have one item linked to an invoice
					 if (lstInvoiceProductList->Items->Count == 1)
					 {
						 MessageBox::Show("You must always have at least one item in an Invoice!");
					 }
					 // delete row from invoice item table and product list
					 // only when something is selected
					 else if (lstInvoiceProductList->SelectedIndex >= 0)
					 {
						 Table invoiceItem = new InvoiceItem();
						 Table orders = new Orders();

						 // store current row as an std::string
						 std::string currentRow = marshal_as<std::string>
							 (lstInvoiceProductList->SelectedItem->ToString());

						 // find the delimiter position
						 int delimiter = currentRow.find('|');

						 // get the invoice_item_id
						 std::string invoice_item_id = currentRow.substr(0, delimiter - 1);

						 // delete invoice item from database
						 invoiceItem->deleteRow(invoice_item_id);
						 orders->deleteRow(invoice_item_id);

						 // remove selected product from listbox
						 lstInvoiceProductList->Items->Remove(lstInvoiceProductList->SelectedItem); 

						 delete invoiceItem;
						 delete orders;
					 }
				 }
			 }

			 /// \brief populates the Invoice combobox with Invoices of the same date selected
	private: System::Void btnInvoiceSearch_Click(System::Object^  sender, System::EventArgs^  e) {
				 Table invoice = new Invoice();

				 cmbInvoiceSelect->Items->Clear();
				 lstInvoiceProductList->Items->Clear();
				 lstInvoiceProductList->Enabled = false;

				 // get the date from date time picker and convert to std::string
				 System::String^ invoiceDate = dtInvoiceDate->Value.ToString("yyyy-MM-dd");
				 std::string date(marshal_as<std::string>(invoiceDate));

				 std::string invoiceReturned; // string to store searched invoice

				 // try to find the invoice according to selected date otherwise display an error
				 try { 
					 int delimiter1, delimiter2; // positon of delimiters
					 std::string invoice_id; // store invoice_id of search
					 std::string currentRow; //store the first row of multiple-lined search result

					 invoiceReturned = invoice->search("date", date);

					 // get the first row of the search result
					 delimiter1 = invoiceReturned.find("\n");
					 currentRow = invoiceReturned.substr(0, delimiter1+1);

					 // if the current row is not equal to the search result
					 // keep breaking down searchResult line by line until they do
					 // ie, currentRow is the last line of the search result
					 // and add the invoice to invoice select combobox
					 while (!invoiceReturned.empty()){
						 delimiter1 = invoiceReturned.find("\n");
						 invoiceReturned = invoiceReturned.substr(delimiter1+1);
						 delimiter2 = currentRow.find("|");
						 invoice_id = currentRow.substr(0, delimiter2);

						 cmbInvoiceSelect->Items->Add( "Invoice ID: " +
							 gcnew String(invoice_id.c_str()) + "   Date: " +
							 gcnew String(date.c_str()));

						 delimiter1 = invoiceReturned.find("\n");
						 currentRow = invoiceReturned.substr(0, delimiter1+1);
					 }

					 cmbInvoiceSelect->Enabled = true;
				 }
				 catch (DoesNotExistException e) {
					 MessageBox::Show(gcnew String(e.what())); // error message
				 }
				 delete invoice;
			 }

			 /// \brief Fills the listbox with invoiceItems of selected invoice
	private: System::Void cmbInvoiceSelect_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {

				 // populate Invoice Product List with selected invoice's items
				 lstInvoiceProductList->Enabled = true;
				 populateInvoiceProductList();

				 // if modifying, display a list of products as well
				 if (cmbInvoiceFunction->SelectedIndex == 1)
				 {
					 cmbInvoiceProductSelect->Enabled = true;
					 btnInvoiceModify->Enabled = true;

					 // clear combobox
					 cmbInvoiceProductSelect->Items->Clear();

					 // currentRow string
					 System::String ^ currentRow;

					 // vector to contain the product file contents
					 vector<string> productFile;
					 // retrieve vector containing contents of product file
					 productFile = returnFile("textFiles/product.txt");

					 // insert contents of product file vector into combobox
					 for(size_t i = 0; i < productFile.size(); i++)
					 {
						 currentRow = gcnew String (productFile[i].c_str()); // convert to System::String^

						 // Add spaces between |'s
						 cmbInvoiceProductSelect->Items->Add(currentRow->Replace("|", " | "));
					 }
				 }



			 }


			 /// \brief selects the product and fills the quantity text box with product and quantity of selected invoice item
	private: System::Void lstInvoiceProductList_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {

				 // make sure something is selected
				 if (lstInvoiceProductList->SelectedIndex >= 0)
				 {
					 // if Modifying an invoice
					 if (cmbInvoiceFunction->SelectedIndex == 1)
					 {
						 // Enable the product quantity textbox and button, and remove button
						 txtInvoiceProductQuantity->Enabled = true;
						 btnInvoiceModifyInvoiceItem->Enabled = true;

						 System::String^ invoice_item; // store selected invoice item
						 System::String^ product_id; // product id of selected invoice item
						 System::String^ quantity; // quantity of selected invoice item

						 int delimiter1, delimiter2, delimiter3; // position of delimiters

						 // store the selected item as a string
						 invoice_item = lstInvoiceProductList->SelectedItem->ToString();

						 // find positons of delimiters
						 delimiter1 = invoice_item->IndexOf('|');
						 delimiter2 = invoice_item->IndexOf('|', delimiter1 + 1);
						 delimiter3 = invoice_item->IndexOf('|', delimiter2 + 1);

						 // find product_id and quantity of selected invoice item
						 product_id = invoice_item->Substring(delimiter1 + 2, delimiter2 - delimiter1 - 2);
						 quantity = invoice_item->Substring(delimiter3 + 2);

						 // select the product in the product select combobox
						 cmbInvoiceProductSelect->SelectedIndex = cmbInvoiceProductSelect->FindString(product_id);

						 // change the text of the Product Quantity combobox to the selected quantity
						 txtInvoiceProductQuantity->Text = quantity->Trim();

					 } // end if selected function is modify
				 } // end if a list item is currently selecte
			 }

			 /// \brief modifies the selected invoice's date with date selected
	private: System::Void btnInvoiceModify_Click_1(System::Object^  sender, System::EventArgs^  e) {

				 Table invoice = new Invoice();

				 int delimiter1, delimiter2; // position of delimiters

				 // get the date from date time picker and convert to std::string

				 System::String^ invoiceDate = dtInvoiceDate->Value.ToString("yyyy-MM-dd");	  	
				 string date(marshal_as<std::string>(invoiceDate));

				 // get invoice_id of selected invoice by converting selection to std::string
				 // and then breaking it up with delimiters	  
				 System::String^ invoiceSystemStr = cmbInvoiceSelect->SelectedItem->ToString();  	
				 std::string invoiceSTDStr = marshal_as<std::string>(invoiceSystemStr);	  	
				 delimiter1 = invoiceSTDStr.find(":"); 	
				 delimiter2 = invoiceSTDStr.find(":", delimiter1 + 1);	  	
				 std::string invoice_id = invoiceSTDStr.substr(delimiter1 + 2, delimiter2 - delimiter1 - 9); 

				 // chage the date of the invoice	  	
				 invoice->modifyRow(invoice_id, "date", date);

				 MessageBox::Show("Invoice Date Modified");

				 // refresh modify
				 cmbInvoiceFunction->SelectedIndex = 0;
				 cmbInvoiceFunction->SelectedIndex = 1;

				 delete invoice;

			 }

			 /// \brief modifies the selected invoice item
	private: System::Void btnInvoiceModifyInvoiceItem_Click(System::Object^  sender, System::EventArgs^  e) {

				 Table invoiceItem = new InvoiceItem();

				 System::String^ invoice_item; // selected invoice_item;
				 System::String^ product; // selected product
				 System::String^ invoice_item_id; // id of selected invoice_item
				 System::String^ product_id; // product_id of selected product
				 System::String^ quantity; // changed quantity

				 int delimiter1; // position of delimiter

				 // find invoice_item_id
				 invoice_item = lstInvoiceProductList->SelectedItem->ToString();
				 delimiter1 = invoice_item->IndexOf('|');
				 invoice_item_id = invoice_item->Substring(0, delimiter1 - 1);

				 // find product_id
				 product = cmbInvoiceProductSelect->SelectedItem->ToString();
				 delimiter1 = product->IndexOf('|');
				 product_id = product->Substring(0, delimiter1 - 1);

				 // find quantity
				 quantity = txtInvoiceProductQuantity->Text;

				 // convert to std::string
				 std::string invoice_item_idString = marshal_as<std::string>(invoice_item_id);
				 std::string product_idString = marshal_as<std::string>(product_id);
				 std::string quantityString = marshal_as<std::string>(quantity);

				 // modify row and dislay message
				 invoiceItem->modifyRow(invoice_item_idString, "quantity", quantityString);
				 invoiceItem->modifyRow(invoice_item_idString, "product_id", product_idString);

				 MessageBox::Show("Invoice Item Modified");

				 // refresh the product list
				 populateInvoiceProductList();

				 delete invoiceItem;
			 }

			 /// \brief set component to visible or invisible based on which report is going to be generated
	private: System::Void cmbReportSelect_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {

				 // clear combobox
				 cmbReportCategorySelect->Items->Clear();

				 // currentRow string
				 System::String ^ currentRow;

				 // vector to contain the category file contents
				 vector<string> categoriesFile;
				 // retrieve vector containing contents of category file
				 categoriesFile = returnFile("textFiles/category.txt");

				 // insert contents of category file into combobox
				 for(size_t i = 0; i < categoriesFile.size(); i++)
				 {
					 currentRow = gcnew String (categoriesFile[i].c_str());
					 cmbReportCategorySelect->Items->Add(currentRow);
				 }

				 if(cmbReportSelect->SelectedIndex == 0) {
					 lblReportCategorySelect->Visible = false;
					 cmbReportCategorySelect->Visible = false;
					 lblReportDescription->Visible = true;
					 txtReportDescription->Visible = true;
					 txtReportDescription->Text = "Displays all products currently out of stock.";
					 lblReportStartDate->Visible = false;
					 dtReportStartDate->Visible = false;
					 lblReportEndDate->Visible = false;
					 dtReportEndDate->Visible = false;
					 btnReportGenerate->Visible = true;
				 } else if(cmbReportSelect->SelectedIndex == 1) {
					 lblReportCategorySelect->Visible = true;
					 cmbReportCategorySelect->Visible = true;
					 lblReportDescription->Visible = true;
					 txtReportDescription->Visible = true;
					 txtReportDescription->Text = "Displays the current stock of all products in the specified category.";
					 lblReportStartDate->Visible = false;
					 dtReportStartDate->Visible = false;
					 lblReportEndDate->Visible = false;
					 dtReportEndDate->Visible = false;
					 btnReportGenerate->Visible = true;
				 } else if(cmbReportSelect->SelectedIndex == 2) {
					 lblReportCategorySelect->Visible = false;
					 cmbReportCategorySelect->Visible = false;
					 lblReportDescription->Visible = true;
					 txtReportDescription->Visible = true;
					 txtReportDescription->Text = "Displays a list of all sales between the start and end dates selected.";
					 lblReportStartDate->Visible = true;
					 dtReportStartDate->Visible = true;
					 lblReportEndDate->Visible = true;
					 dtReportEndDate->Visible = true;
					 btnReportGenerate->Visible = true;
				 } else if(cmbReportSelect->SelectedIndex == 3) {
					 lblReportCategorySelect->Visible = false;
					 cmbReportCategorySelect->Visible = false;
					 lblReportDescription->Visible = true;
					 txtReportDescription->Visible = true;
					 txtReportDescription->Text = "Displays a list of all returns between the start and end dates selected.";
					 lblReportStartDate->Visible = true;
					 dtReportStartDate->Visible = true;
					 lblReportEndDate->Visible = true;
					 dtReportEndDate->Visible = true;
					 btnReportGenerate->Visible = true;
				 } else if(cmbReportSelect->SelectedIndex == 4) {
					 lblReportCategorySelect->Visible = false;
					 cmbReportCategorySelect->Visible = false;
					 lblReportDescription->Visible = true;
					 txtReportDescription->Visible = true;
					 txtReportDescription->Text = "Displays a list of all invoices between the start and end dates selected.";
					 lblReportStartDate->Visible = true;
					 dtReportStartDate->Visible = true;
					 lblReportEndDate->Visible = true;
					 dtReportEndDate->Visible = true;
					 btnReportGenerate->Visible = true;
				 } else if(cmbReportSelect->SelectedIndex == 5) {
					 lblReportCategorySelect->Visible = true;
					 cmbReportCategorySelect->Visible = true;
					 lblReportDescription->Visible = true;
					 txtReportDescription->Visible = true;
					 txtReportDescription->Text = "Displays a list of the 10 products in the selected category that sold the highest quantity between the start and end dates selected.";
					 lblReportStartDate->Visible = true;
					 dtReportStartDate->Visible = true;
					 lblReportEndDate->Visible = true;
					 dtReportEndDate->Visible = true;
					 btnReportGenerate->Visible = true;
				 } else if(cmbReportSelect->SelectedIndex == 6) {
					 lblReportCategorySelect->Visible = false;
					 cmbReportCategorySelect->Visible = false;
					 lblReportDescription->Visible = true;
					 txtReportDescription->Visible = true;
					 txtReportDescription->Text = "Displays the revenue of each category, as well as the total revenue of all categories combined, between the start and end dates selected.";
					 lblReportStartDate->Visible = true;
					 dtReportStartDate->Visible = true;
					 lblReportEndDate->Visible = true;
					 dtReportEndDate->Visible = true;
					 btnReportGenerate->Visible = true;
				 }
			 }

			 /// \brief returns product ID's with no stock in Summary table
			 /// \returns a vector of strings of all the product IDs with no stock in Summary table
	private: vector<string> IDs() {

				 ReportsImpl reports;
				 string searchResult, currentRow, productID;
				 System::String^ strOutput;
				 Table summary = new Summary();		
				 int delimiter; // store position of delimiters
				 vector<string> returnVector;
				 string quantity = "0";

				 searchResult = summary->search("total_quantity", quantity); 

				 while (!searchResult.empty()){
					 // get the first row of the search result
					 delimiter = searchResult.find('\n');
					 currentRow = searchResult.substr(0, delimiter +1 );

					 delimiter = currentRow.find('|');
					 productID = currentRow.substr(0, delimiter);

					 returnVector.push_back(productID);
					 delimiter = searchResult.find('\n');

					 searchResult = searchResult.substr(delimiter + 1);
					 currentRow = searchResult.substr(0, delimiter + 1);
				 }  
				 return returnVector;
			 }

			 /// \brief returns all product IDs of all products in the Summary table
			 /// \returns a vector of strings of all product IDs of all products in the Summary table
	private: vector <string> returnedIDs(){

				 Table product = new Product();
				 Table summary = new Summary();
				 System::String^ category = cmbReportCategorySelect->SelectedItem->ToString();
				 int delimiter1 = category->IndexOf("|");
				 int delimiter2 = category->IndexOf("|", delimiter1 + 1);
				 System::String^ category_id;
				 System::String^ strRawOutput;
				 std::string categoryID, strOutput, productID;
				 std::string quantityReturned, ret;
				 vector<string> vs;
				 string currentRow;
				 // Get product_id, name, and quantity
				 category_id = category->Substring(0, delimiter1);

				 std::string cat = marshal_as<std::string>(category_id);
				 quantityReturned = product->search("category_id", cat);
				 while (! quantityReturned.empty())
				 {

					 int  delimiter = quantityReturned.find('\n');
					 currentRow = quantityReturned.substr(0, delimiter +1 );

					 delimiter = currentRow.find('|');
					 productID = currentRow.substr(0, delimiter);
					 ret = summary->search("product_id", productID);
					 vs.push_back(ret);
					 delimiter = quantityReturned.find('\n');

					 quantityReturned = quantityReturned.substr(delimiter + 1);
					 currentRow = quantityReturned.substr(0, delimiter + 1);
				 }

				 return vs;
			 }

	private: System::Void btnReportGenerate_Click(System::Object^  sender, System::EventArgs^  e) {


				 //Grabs the dates to search by and formats them
				 System::String ^ strStartDate =  dtReportStartDate->Value.ToString("yyyy-MM-dd");
				 System::String ^ strEndDate = dtReportEndDate->Value.ToString("yyyy-MM-dd");

				 //Converts the dates into proper strings to use with classes
				 string strSearchStart(marshal_as<std::string>(strStartDate));
				 string strSearchEnd(marshal_as<std::string>(strEndDate));

				 // Create a new report form
				 ReportForm ^ reportForm = gcnew ReportForm;
				 // Pointer to ReportsInterface class to create reports
				 Reports reports = new ReportsImpl();

				 //String to hold our final output
				 System::String ^ strOutput = "";

				 switch (cmbReportSelect->SelectedIndex)
				 {
				 case 0: //Out of stock report
					 {
						 ReportsImpl reports;
						 Table summary = new Summary();
						 string searchResult, currentRow;
						 System::String^ strRawOutput;
						 std::string str;
						 int delimiter; 
						 vector<string> productID;
						 productID = IDs();
						 std::stringstream ss;

						 std::copy( productID.begin(),productID.end(),std::ostream_iterator< std::string >( ss, " \r\n" "\r\n"));

						 str = ss.str();

						 strRawOutput = gcnew String (str.c_str());
						 strOutput = "Product ID(s) that are currently out of stock:""\r\n\r\n"+"Product ID \r\n"+ strRawOutput ;
					 }
				 case 1: //Current stock report
					 {
						 vector<string> output;
						 output = returnedIDs();
						 std::string str;
						 System::String^ strRawOutput;
						 std::stringstream ss;
						 std::copy( output.begin(),output.end(),std::ostream_iterator< std::string >( ss, " \r\n" "\r\n"));
						 str = ss.str();
						 strRawOutput = gcnew String (str.c_str());
						 reportForm->txtReport->Text =  "The current quantities of existing products: ""\r\n\r\n" +"Product ID | Quantity "+"\r\n"+strRawOutput ;
						 break;
					 }
				 case 2:  //Sales between date report
					 {
						 break;
					 }
				 case 3: //Returns between date report
					 {
						 break;
					 }
				 case 4: //Invoice between date report
					 {
						 break;
					 }
				 case 5: //Top sellers report
					 {
						 try {
							 // get the category ID from the cmbReportCategorySelect and convert to std::string
							 System::String^ reportCategory = cmbReportCategorySelect->Text->ToString();
							 string reportCategorySelected(marshal_as<std::string>(reportCategory));

							 int delimiter = reportCategorySelected.find('|');
							 string reportCategoryID = reportCategorySelected.substr(0, delimiter);

							 strOutput = gcnew String (reports->topSellersReport(reportCategoryID, strSearchStart, strSearchEnd).c_str());
						 }
						 catch (exception e) {
							 strOutput = gcnew String (e.what());
						 }
						 break;
					 }
				 case 6: //Revenue report
					 {
						 // string to hold total revenue report
						 System::String ^ systemStringTotalRevenue;

						 string totalRevenueReport; // string to store the total revenue report

						 // if nothing exists in the date range error will be displayed in report form
						 try {
							 totalRevenueReport = reports->totalRevenueReport(strSearchStart, strSearchEnd);
						 }
						 catch (DoesNotExistException e) {
							 totalRevenueReport = e.what();
						 }

						 //Format totalRevenueReport to a system string and format to Windows endlines
						 systemStringTotalRevenue = gcnew String (totalRevenueReport.c_str());
						 systemStringTotalRevenue = systemStringTotalRevenue->Replace("\n", "\r\n");

						 //Format to change report look
						 systemStringTotalRevenue = systemStringTotalRevenue->Replace
							 ("TOTAL REVENUE", "------------------------------\r\nTOTAL REVENUE");

						 // Ready output string for display
						 strOutput += "Revenue earned from: " + strStartDate + " to " + strEndDate + "\r\n\r\n";
						 strOutput += systemStringTotalRevenue;
						 break;
					 }
				 case 7: 
					 break;
				 default:
					 break;
				 }

				 //Display our outputted data
				 reportForm->txtReport->Text = strOutput;

				 delete reports;
				 //Displays the new form
				 reportForm->ShowDialog();
			 }

			 /// \brief set dtReportStartDate's max date to the currently selected end date
	private: System::Void dtReportEndDate_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
				 dtReportStartDate->MaxDate = dtReportEndDate->Value;
			 }
	};
}

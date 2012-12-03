#include "ReportsImpl.h"

// converts a String of the form YYYY-MM-DD to time_t
// precondition: string is always of the form YYYY-MM-DD
// parameter[in]: date is a string of the form YYYY-MM-DD
// returns: a time_t object representing the date of the converted dateString
time_t ReportsImpl::convertStringToDate(string dateString) { 

	int delimiter1, delimiter2; // positions of the '-' in dateString

	// strings to store year mont and day of the dateString
	string year, month, day;

	// struct tm to hold year, month, and day
	struct tm date_tm = {0};

	// find positions of '-'
	delimiter1 = dateString.find('-');
	delimiter2 = dateString.find('-', delimiter1 + 1);

	// get year, month, and day
	year = dateString.substr(0, delimiter1);
	month = dateString.substr(delimiter1 + 1, delimiter2 - delimiter1 - 1);
	day = dateString.substr(delimiter2 + 1);

	// store year, month, and day into struct tm date_tm
	date_tm.tm_year = atoi(year.c_str()) - 1900; // tm_year counts from 1900
	date_tm.tm_mon = atoi(month.c_str()) - 1; // tm_mon is 0 to 11 for Jan to Dec
	date_tm.tm_mday = atoi(day.c_str()); // day of the month, ie. 1 to 31

	// convert date_tm to time_t and returned
	return mktime(&date_tm); 
}
// Finds all the sales/invoice/return id's between and including two dates
// precondition: The table is set in a way that the first column is the ID (primary key), 
//               the date is in the form YYYY-MM-DD, all other entries are integers    
// parameter[in]: table is a Table of the type to search in
// parameter[in]: startDate is a string of the form YYYY-MM-DD representing the start of the period
// parameter[in]: endDate is  string of the form YYYY-MM-DD representing the end of the period
// returns: a string vector where each element an ID whose date is within the range
vector<string> ReportsImpl::findIDsBetweenDates (Table table, string startDate, string endDate)
	throw (DoesNotExistException) { 

		vector<string> returnVector; // vector of strings to store the vector to be returned

		string searchResult; // string to find return of search
		string currentRow; // stores current row of searchResult
		string ID; // stores ID of the current row
		string date; // stores date of the current row

		time_t dateTime; // time_t conversion of date
		time_t startDateTime = convertStringToDate(startDate); // time_t conversion of startDate string
		time_t endDateTime = convertStringToDate(endDate); // time_t conversion of endDate string

		int delimiter; // store position of delimiters

		searchResult = table->search("all", "all"); // get all entries in the table

		// get the first row of the search result
		delimiter = searchResult.find('\n');
		currentRow = searchResult.substr(0, delimiter +1 );

		// break down searchResult line by line
		// storing each removed line into current row, until searchResult is empty
		// each iteration find the date and add ID to returnVector if date between start and end dates
		while (!searchResult.empty()){

			// find the date of the current row and convert to a time_t
			delimiter = currentRow.find('-');
			date = currentRow.substr(delimiter - 4, 10);
			dateTime = convertStringToDate(date);

			// if the date is between start and end dates, find the ID and add it to returnVector
			if (dateTime >= startDateTime && dateTime <= endDateTime)
			{
				delimiter = currentRow.find('|');
				ID = currentRow.substr(0, delimiter);
				returnVector.push_back(ID);
			}

			// break down the searchResult and find currentRow
			delimiter = searchResult.find('\n');
			searchResult = searchResult.substr(delimiter + 1);
			currentRow = searchResult.substr(0, delimiter + 1);
		}

		// throw DoesNotExistException if there are no entries within date range
		if (returnVector.size() == 0)
			throw DoesNotExistException("Nothing found between " + startDate + " and " + endDate);

		return returnVector; 
}

string ReportsImpl::totalRevenueReport(string startDate, string endDate) { return NULL; }

string ReportsImpl::topSellersReport(string selectedCategory, string startDate, string endDate) 
{
	Table receiptTable = new Receipt();

	// vector to hold all the receipt ID's from between the start and end dates
	vector<string> receiptIDVect;

	// the vector returned by the findIDsBetweenDates function
	receiptIDVect = findIDsBetweenDates(receiptTable, startDate, endDate);

	delete receiptTable;

	Table salesSummaryTable = new SalesSummary();

	// vector to store the sales ID's corresponding to the receipt ID's already obtained
	vector<string> salesIDVect;

	// string to hold the result of each search in the salesSummary table
	string resultOfSearch;

	//string to hold the sale ID from the resulting row returned by the search
	string saleID;

	// initialization of delimiters
	int delimiter;
	int delimiter2;
	int delimiter3;
	int delimiter4;

	// iterates through receiptIDVect and searches for each receiptID in the sales summary table
	for(int i = 0; i < (int) receiptIDVect.size(); i++)
	{
		try{
			if(receiptIDVect[i].size() > 0)
				resultOfSearch = salesSummaryTable->search("receiptID", receiptIDVect[i]);

			// assigns the delimiter position
			delimiter = resultOfSearch.find('|');

			stringstream resultStream(resultOfSearch);
			while(getline(resultStream, saleID))
			{
				// assigns string sales ID with data in the row
				saleID = saleID.substr(delimiter+1);

				salesIDVect.push_back(saleID);
			}

		} catch (DoesNotExistException e){ break; }
	}

	delete salesSummaryTable;

	Table salesTable = new Sales();

	//string to hold the product ID from the resulting row returned by the search
	string productID;

	//string to hold the product quantity from the resulting row returned by the search
	string productQuantity;

	// vector to store the product ID's and quantity corresponding to the sales ID's already obtained
	vector<string> productVect;

	// iterates through salesIDVect and searches for each salesID in the sales table
	for(int i = 0; i < (int) salesIDVect.size(); i++)
	{
		try{
			if(salesIDVect[i].size() > 0)
				resultOfSearch = salesTable->search("sales_id", salesIDVect[i]);

			// finds the first delimiter position and assigns it to int delimiter
			delimiter = resultOfSearch.find('|');

			// finds the second delimiter position and assigns it to int delimiter2
			delimiter2 = resultOfSearch.find('|', delimiter+1);

			// finds the third delimiter position and assigns it to int delimiter3
			delimiter3 = resultOfSearch.find('|', delimiter2+1);

			// retrieves the product ID from the row data and assigns it to productID
			productID = resultOfSearch.substr(delimiter2+1,delimiter3-delimiter2-1);

			// retrieves the product quantity from the row data and assigns it to string productQuantity
			productQuantity = resultOfSearch.substr(delimiter+1, delimiter2-delimiter-1);

			productVect.push_back(productID + "|" + productQuantity);

		} catch (DoesNotExistException e){ break; }
	}

	delete salesTable;

	// vector to contain only products in the selected category
	vector<string> productsInCategoryVect;

	Table productTable = new Product();

	// string to hold the name of the product
	string productName;

	// string to hold the category the product belongs to
	string productCategory;

	// iterates through productVect and searches for each productID in the product table
	for(int i = 0; i < (int) productVect.size(); i++)
	{
		try{
			if(productVect[i].size() > 0) {
				// finds the first two delimiters positions and assign them to int delimiter and delimiter2
				delimiter = productVect[i].find('|');
				delimiter2 = productVect[i].find('|', delimiter + 1);

				productQuantity = productVect[i].substr(delimiter + 1, delimiter2 - delimiter - 1);

				productID = productVect[i].substr(0, delimiter);

				resultOfSearch = productTable->search("product_id", productID);
			}

			// finds the first delimiter position and assigns it to int delimiter
			delimiter = resultOfSearch.find('|');

			// finds the second delimiter position and assigns it to int delimiter2
			delimiter2 = resultOfSearch.find('|', delimiter+1);

			// finds the third delimiter position and assigns it to int delimiter3
			delimiter3 = resultOfSearch.find('|', delimiter2+1);

			// finds the fourth delimiter position and assigns it to int delimiter4
			delimiter4 = resultOfSearch.find('|', delimiter3+1);

			// retrieves the product name from the row data and assigns it to productName
			productName = resultOfSearch.substr(delimiter3+1,delimiter4-delimiter3-1);

			// retrieves the product category from the row data and assigns it to productCategory
			productCategory = resultOfSearch.substr(delimiter+1,delimiter2-delimiter-1);

			if(productCategory == selectedCategory)
			{
				productsInCategoryVect.push_back(productID + " - " + productName + "\tQuantity Sold: " + productQuantity);
			}

		} catch (DoesNotExistException e){ break; }
	}

	delete productTable;

	// vector to hold the products with combined quantities (all sales of each product tallied up)
	vector<string> productsWithQuantities;

	// strings to temporarily store information for comparisons and quantity addition
	string tempProductID;
	string tempProductIDStored;
	string tempProductRow;
	string tempProductRowStored;
	int tempProductQuantity;
	int tempProductQuantityStored;
	string tempProductQuantityStoredString;

	// stores the position of the for loop 'j' counter
	int jPosition;

	// boolean to tell if the product already is stored in the productsWithQuantities vector
	bool productAlreadyStored = false;

	// iterates through the productsInCategoryVect vector
	for(int i = 0; i < (int) productsInCategoryVect.size(); i++)
	{
		delimiter = productsInCategoryVect[i].find(' ');
		tempProductID = productsInCategoryVect[i].substr(0,delimiter);

		// iterates through the productsWithQuantities vector checking if product is already stored
		for(int j = 0; j < (int) productsWithQuantities.size(); j++)
		{
			delimiter = productsWithQuantities[j].find(' ');

			tempProductIDStored = productsWithQuantities[j].substr(0,delimiter);

			jPosition = j;
			if (tempProductID == tempProductIDStored)
			{
				productAlreadyStored = true;
				break;
			}
		}

		// if product is already stored quantity is added to existing quantity
		// else product is stored into productsWithQuantities vector
		if (productAlreadyStored)
		{
			productAlreadyStored = false;

			tempProductRowStored = productsWithQuantities[jPosition];

			tempProductRow = productsInCategoryVect[i];

			delimiter = tempProductRow.find(':');
			tempProductQuantity = atoi(tempProductRow.substr(delimiter + 1).c_str());

			delimiter = tempProductRowStored.find(':');
			tempProductQuantityStored = atoi(tempProductRowStored.substr(delimiter + 1).c_str());

			tempProductQuantityStored += tempProductQuantity;
			stringstream stream;
			stream << tempProductQuantityStored;
			tempProductQuantityStoredString = stream.str();

			tempProductRowStored.replace(delimiter + 1, 20, tempProductQuantityStoredString);

			productsWithQuantities[jPosition] = tempProductRowStored;
		} else {
			productsWithQuantities.push_back(productsInCategoryVect[i]);
		}
	}

	Table categoryTable = new Category();

	// retrieve selected category from categoryTable
	string categoryRow = categoryTable->search("category_id", selectedCategory);

	delete categoryTable;

	// find delimiters in category row
	delimiter = categoryRow.find('|');
	delimiter2 = categoryRow.find('|', delimiter + 1);

	// get substring containing the category name to be used in the report header line
	string categoryName = categoryRow.substr(delimiter2 + 1);

	// strings to contain the retrieved row from the vector
	string currentProduct;
	string currentProductStored;

	// vector to store sorted products
	vector<string> productsSorted;

	//integers for product quantity comparison
	int quantity;
	int quantityStored;

	// loop through every entry in the productsWithQuantities vector and sort by quantity sold
	for(int x = 0; x < (int) productsWithQuantities.size(); x++)
	{
		currentProduct = productsWithQuantities[x];
		delimiter = currentProduct.find(':');
		quantity = atoi(currentProduct.substr(delimiter + 1).c_str());

		if(productsSorted.size() == 0) {
			productsSorted.push_back(currentProduct);
		} else {
			for(int i = 0; i < (int) productsSorted.size(); i++)
			{
				currentProductStored = productsSorted[i];
				delimiter = currentProductStored.find(':');
				quantityStored = atoi(currentProductStored.substr(delimiter + 1).c_str());
				if(quantity > quantityStored) {
					productsSorted.insert(productsSorted.begin() + i,productsWithQuantities[x]);
					break;
				}
			}
			if(quantity <= quantityStored)
			{
				productsSorted.push_back(productsWithQuantities[x]);
			}
		}

	}

	// string to contain the information of the report
	string report;

	// create header line for the Report
	report = "Top Selling Products Report in " + categoryName + "\r\n--------------------------------------------------------------------------------------------------------------------------\r\n";

	// output the first ten products in the vector into the report string
	for(int j = 0; j < (int) productsSorted.size() && j < 10; j++)
	{
		report += productsSorted[j];
		report += "\r\n";
	}

	// return report
	return report;
}

ReportsImpl::~ReportsImpl() {};
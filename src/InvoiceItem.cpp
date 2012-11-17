#include "InvoiceItem.h"

//Add function to insert data into the invoice item text file
// parameter[in]: addVector is a vector of strings for the data to be entered
//            for InvoiceItem. This will be values for 
//            product_id and quantity in that order
// throw: nothing will be thrown because primary key is automatically incremented, thus no duplication
// postcondition: total_quantity in Summary table is appropriately modified
void InvoiceItem::add(vector<string> addVector) throw (AlreadyExistsException) {
	ifstream infstream; // ifstream to be used to read invoiceItem.txt
	ofstream outfstream; // ofstream to be used to write to invoiceItem.txt

	int invoice_item_id = 0; // int used to store the highest invoice__item_id so far
	int delimPos; // int used to store the position of the first delimiter

	string currentRow; // string that stores the current row on the invoice table
	string invoice_item_idTemp; // string used to store the current highest invoice_item_id so far
	string product_id = addVector[0]; // product_id to be added to file
	string quantity = addVector[1]; // quantity to be added to file

	infstream.open(fileName);

	if(infstream.is_open())
	{
		// while loop continues as long as there is another line in the text file
		// finds the highest invoice_id in the file and stores it in invoice_item_id
		while(infstream.good())
		{
			getline(infstream, currentRow); // store next line of textfile in currentRow

			// Searches for the position of the delimiter
			delimPos = currentRow.find('|');

			// stores the current invoice_item_id into invoice_item_idTemp
			invoice_item_idTemp = currentRow.substr(0, delimPos);

			// Assigns current invoice item id to last invoice item id if current is higher
			if(atoi(invoice_item_idTemp.c_str()) > invoice_item_id)
			{
				invoice_item_id = atoi(invoice_item_idTemp.c_str());
			}
		}
	}

	infstream.close();

	// increments invoice_id so it is higher than last invoice_id in the file
	invoice_item_id++;

	outfstream.open(fileName, ios_base::app);

	// writes to invoiceItem.txt the new row data as defined by invoice_item_id, product_id, and quantity
	// with delimiters between each input and a line break at the end
	outfstream << invoice_item_id << "|" + product_id + "|" + quantity + "\n";

	outfstream.close();

	Table summary = new Summary(); // Summary class obj used to add product quantities to Summmary Table

	try {
		// search for product_id in Summary table and store in current row
		currentRow = summary->search("product_id", product_id);

		// Update the product_id's quantity in the summary table if it exists
		delimPos = currentRow.find('|'); // find delimiter in returned row from Summary

		string total_quantity; // total_quantity of product in Summary table

		product_id = currentRow.substr(0, delimPos); // product_id of row in Summary Table
		total_quantity = currentRow.substr(delimPos + 1); // total quantity of row in Summary table

		stringstream totalQuantity; //stringstream to convert int to string

		// Add Invoice Item's quantity to total_quantity in Summary
		// then output to stringstream totalQuantity
		totalQuantity << atoi(total_quantity.c_str()) + atoi(quantity.c_str());

		// change the total_quantity in summary using the stringstream totalQuantity
		summary->modifyRow(product_id, "total_quantity", totalQuantity.str());
	}
	catch (DoesNotExistException e) { // add new product to summary if it doesn't exist there yet
		summary->add(addVector); //addVector is also product_id and quantity (total_quantity in Summary)
	}

	delete summary;
}

// Search function to find a specific row of data and return it as a string
//
// parameter[in]: columnName identifies the name of the column to be searched
// parameter[in]: valueToFind identifies the value to be searched for in the column
// return: a string which contains a concatenation of all values in the row found in the database table
//         if multiple values exist, return all rows with that value, where
//         each row is separated by a new line
// throw: DoesNotExistException when trying to find a row that doesn't exist
string InvoiceItem::search(string columnName, string valueToFind) throw(DoesNotExistException) { 

	ifstream infstream; // ifstream to be used to read invoiceItem.txt

	string returnString = ""; // string used to store the value to be returned
	string currentRow; // string used to store the current row

	//strings to store the invoice_item_id, product_id, and quantity in the current row
	string invoice_item_id, product_id, quantity; 

	// delimiters used to store positions of the delimiters '|' in the current row
	int delimPos1, delimPos2;

	infstream.open(fileName);

	if(infstream.is_open())
	{

		// while loop continues as long as there is another line in the text file
		while(infstream.good())
		{
			getline(infstream, currentRow); // store next line of textfile in currentRow

			// break when an empty string is assigned to currentRow
			// which occurss if there are no more valid entries in the table
			if (currentRow.empty())
				break;

			delimPos1 = currentRow.find('|'); // finds position of first delimiter
			delimPos2 = currentRow.find('|', delimPos1 + 1); // position of second delimiter

			invoice_item_id = currentRow.substr(0, delimPos1); //store first string as invoice_item_id
			product_id = currentRow.substr(delimPos1 + 1, delimPos2-delimPos1-1); //store second string as product_id
			quantity = currentRow.substr(delimPos2 + 1); //store third string as quantity

			// if searching by invoice_item_id
			if(columnName == "invoice_item_id" && 
				invoice_item_id == valueToFind) {
				returnString = currentRow + "\n"; //since invoice_item_id is unique returns current row
				break;
			}
			// if searching by product_id
			else if(columnName == "product_id" &&
				product_id == valueToFind)	{	
				returnString += currentRow + "\n"; //adds any row that contains same product_id
			}
			// if searching by product_id
			else if(columnName == "quantity" &&
				quantity == valueToFind)	{	
				returnString += currentRow + "\n"; //adds any row that contains same quantity
			}
		}
	}

	infstream.close();

	// throws DoesNotExistException if nothing was found
	if(returnString == "")
		throw DoesNotExistException(valueToFind + " does not exist in column: " + columnName); 

	return returnString;
}

// Modify function to change the data in a given row
// Modification will be done by finding a matching value by searching for valueToFind
// in the primary key (invoice_item_id) column of the table. Only product_id and quantity
// can by modified.
// precondition: Row exists. This will be checked by the main program beforehand
// parameter[in]: valueToFind identifies the value to be searched for in the primary key
// parameter[in]: columnNameToModify identifies the column to change data for
// parameter[in]: valueOfModify provides the new data for the desired column
// postcondition: total_quantity in Summary table is modified when quantity is modified
void InvoiceItem::modifyRow(string valueToFind, string columnNameToModify, string valueOfModify) {

	ifstream infstream; // ifstream to be used to read invoiceItem.txt
	ofstream outfstream; // ofstream to be used to write to invoiceItem.txt	
	vector<string> fileVector; // string vector to store each line of the invoice.txt file

	string currentRow; // string to store the current row in the table
	string invoice_item_id; //string to store invoice_item_id of currrent row
	string product_id; //string to store product_id of current row
	string quantity; //string to store quantity of current row
	string summaryProduct_id; //string to store the product_id to modify the summary table

	int delimPos1, delimPos2; // position of delimiters in current row
	int differenceInQuantity; // int to store the difference between quantity and valueOfModify

	infstream.open(fileName);

	if(infstream.is_open())
	{
		// while loop continues as long as there is another line in the text file
		while(infstream.good())
		{
			getline(infstream, currentRow); // store next line of textfile in currentRow

			// break when an empty string is assigned to currentRow
			// which occurss if there are no more valid entries in the table
			if (currentRow.empty())
				break;

			delimPos1 = currentRow.find('|'); // position of first delimiter '|'
			delimPos2 = currentRow.find('|', delimPos1 + 1); // position of second delimiter '|'

			// stores substring before '|' in invoice_item_id
			invoice_item_id = currentRow.substr(0, delimPos1);
			// store second substring between first and second delimiters into product_id
			product_id = currentRow.substr(delimPos1 + 1, delimPos2 - delimPos1 - 1); 
			// store third substring after second delimiter into quantity
			quantity = currentRow.substr(delimPos2 + 1);

			// if the invoice_item_id of the currentRow matches valueToFind, modify the row
			// and push_back the modified string into fileVector
			// otherwise push_back the same string into fileVector
			if (invoice_item_id == valueToFind)
			{
				if (columnNameToModify == "product_id") // modify product_id
					fileVector.push_back(invoice_item_id + "|" + valueOfModify + "|" + quantity + "\n");
				else if (columnNameToModify == "quantity") // modify quantity
				{
					fileVector.push_back(invoice_item_id + "|" + product_id + "|" + valueOfModify + "\n");

					// store valueOfModify - quantityin differenceInQuantity
					differenceInQuantity =  atoi(valueOfModify.c_str()) - atoi(quantity.c_str());
					summaryProduct_id = product_id; //store the modified row's product_id to summaryProduct_id
				}
			}
			else
				fileVector.push_back(invoice_item_id + "|" + product_id + "|" +  quantity + "\n");
		}
	}

	infstream.close();

	// clears invoice.txt so fileVector can be written on it
	outfstream.open(fileName, ios_base::trunc);

	// copy fileVector to invoice.txt
	for(int i = 0; i < (int) fileVector.size(); i++)
		outfstream << fileVector[i];

	outfstream.close();

	// change summary's total quantity if invoice item's quantity is changed
	if (columnNameToModify == "quantity")
	{
	Table summary = new Summary(); // Summary class obj used to change product quantities in Summmary Table

	// search for product_id in Summary table and store in current row
	currentRow = summary->search("product_id", summaryProduct_id); // product_id will always exist

	// Update the product_id's quantity in the summary table
	delimPos1 = currentRow.find('|'); // find delimiter in returned row from Summary

	string total_quantity; // total_quantity of product in Summary table

	summaryProduct_id = currentRow.substr(0, delimPos1); // product_id of row in Summary Table
	total_quantity = currentRow.substr(delimPos1 + 1); // total quantity of row in Summary table

	stringstream totalQuantity; //stringstream to convert int to string

	// Add InvoiceItem's differenceInQuantity to total_quantity in Summary
	// then output to stringstream totalQuantity
	totalQuantity << atoi(total_quantity.c_str()) + differenceInQuantity;

	// change the total_quantity in summary using the stringstream totalQuantity
	summary->modifyRow(summaryProduct_id, "total_quantity", totalQuantity.str());

	delete summary;
	}
}

// Invoice Items should not be deleted from the database
// because all filed invoices will be kept
void InvoiceItem::deleteRow(string valueToFind) {}

// default constructor initialize fileName
InvoiceItem::InvoiceItem() { fileName = "textfiles\\invoiceItem.txt"; }

// destructor
InvoiceItem::~InvoiceItem() {};
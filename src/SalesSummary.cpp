#include "SalesSummary.h"
#include "DoesNotExistException.h"
#include "AlreadyExistsException.h"



/// \brief Add function to insert data into the category text file
/// 
/// \param[in] addVector is a vector of strings for the data to be entered
/// \return returns a string to notify the user whether the add was successful or not
/// \throw AlreadyExistsException when trying to add a primary key that already exists
void SalesSummary :: add(vector<string> addVector)throw (AlreadyExistsException)
{
	string rowReceive;

	// initialization of the delimiter
	int delimiter;

	char delim = '|';

	// assigns string the vector values
	string  receiptID= addVector[0];
	string  salesID =addVector[1];

	// opens salesSummary.txt
	ssInFile.open(salesSummaryTextFile);
	if(ssInFile.is_open())
	{

		while(ssInFile.good())
		{
			// assigns the string to the next line in the file salesSummary.txt
			getline(ssInFile, rowReceive);

			//finds delimitor
			delimiter = rowReceive.find(delim);
						
		}
	}

	// closes the file
	ssInFile.close();
	
	// opens file and appends it to the last line
	ssOutFile.open(salesSummaryTextFile, ios_base::app);

	// writes to salesSummary.txt with delimiters
	ssOutFile<<"" + receiptID + delim + salesID + "\n";

	// closes salesSummary.txt
	ssOutFile.close();

}

/// \brief Search function to find a specific row of data and return it as a string
///
/// \param[in] columnName identifies the name of the column to be searched
/// \param[in] valueToFind identifies the value to be searched for in the column
/// \return a string which contains a concatenation of all values in the row found in the database table
///         if multiple values exist, return all rows with that value, where
///         each row is separated by a new line
/// \throw DoesNotExistException when trying to find a row that doesn't exist
string SalesSummary :: search(string columnName, string valueToFind) throw (DoesNotExistException)
{
	bool resultFound = false;
	string returnString;
	string rowReceive;
	string receiptID, salesID;

	// initialization of delimiters
	int delimiter;
	int delimiter2;
	char delim = '|';

	
	// opens salesSummary.txt
	ssInFile.open(salesSummaryTextFile);

	if(ssInFile.is_open())
	{
		// clears the contents in the string
		returnString.clear();

		while(ssInFile.good())
		{
			// assigns the string with the next line in the file
			getline(ssInFile, rowReceive);

			// break when an empty string is assigned to rowReceive
			// which occurss if there are no more valid entries in the table
			if (rowReceive.empty())
				break;

			// assigns the first delimiter position
			delimiter = rowReceive.find('|');

			// assigns the second delimiter position
			delimiter2 = rowReceive.find('|', delimiter+1);

			// assigns string receipt with data in the row
			receiptID = rowReceive.substr(0, delimiter);

			// assigns string sales ID with data in the row
			salesID = rowReceive.substr(delimiter+1);
			// checks if value to find is equal to the salesID 
		
			if(columnName == "salesID" && 
				salesID == valueToFind)				
				returnString = rowReceive + "\n";
			
			// checks if value to find is equal to the receiptID
			 if(columnName == "receiptID" &&
				receiptID == valueToFind)
				returnString += rowReceive + "\n";
			 // return the whole table
			if(columnName == "all")
				returnString += rowReceive + "\n";
				
		}
	}
	// closes the file
	ssInFile.close();

	// ensures if the return value is less than the minimun value
	if(returnString == "")
		throw DoesNotExistException(valueToFind + " SalesSummary does not exist in column: " + columnName); 

	return returnString;
}

void SalesSummary :: modifyRow( string valueToFind, string columnNameToModify, string valueOfModify){}

void SalesSummary :: deleteRow( string valueToFind) {}

/// Initializes sales summary text file
SalesSummary::SalesSummary() { salesSummaryTextFile = "textFiles\\salesSummary.txt"; }

///Destructor
SalesSummary::~SalesSummary(){}
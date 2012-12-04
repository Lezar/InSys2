#include "Sales.h"
#include "DoesNotExistException.h"
#include "AlreadyExistsException.h"



/// \brief Add function to insert data into the category text file
/// 
/// \param[in] addVector is a vector of strings for the data to be entered
/// \return returns a string to notify the user whether the add was successful or not
/// \throw AlreadyExistsException when trying to add a primary key that already exists
void Sales :: add(vector<string> addVector)throw(AlreadyExistsException)
{
	string rowReceive;

	// string to contain the sales ID from a received row of data
	string salesID;

	// int to store the position of the delimiter
	int delimiter;

	// int to store the sales ID while checking if it is the highest sales ID value used
	int salesIDTemp;

	// int to store the sales ID to be used for adding data to the sales.txt file
	int sID = 0;

	// assign to char delim the | character as the desired delimiter
	char delim = '|';

	// initialization of the delimiter
	delim = '|';

	// assigns string the vector values
	string qSold= addVector[0];
	string productId = addVector[1];
	string disc = addVector[2];

	// opens sales.txt in input form
	salesInFile.open(salesTextFile);

	if(salesInFile.is_open())
	{
		sID = 0;
		while(salesInFile.good())
		{
			// assigns the string to the next line in the file sales.txt
			getline(salesInFile, rowReceive);

			// finds delimiter
			delimiter = rowReceive.find('|');

			//assigns salesID to the row data from file
			salesID = rowReceive.substr(0,delimiter);
			
			//converts string to int data type
			salesIDTemp = atoi(salesID.c_str());

			if(salesIDTemp > sID)
			{
				sID = salesIDTemp;
			}
		}
        //closes the file sales.txt		
		salesInFile.close();

		sID++;
		
		salesID = to_string(sID);
		// opens file and appends it to the last line
		salesOutFile.open(salesTextFile, ios_base::app);
		// writes to salesSummary.txt with delimiters
		salesOutFile<<"" + salesID + delim + qSold + delim + productId + delim + disc + "\n";
		// closes sales.txt
		salesOutFile.close();

	}
}

/// \brief Search function to find a specific row of data and return it as a string
///
/// \param[in] columnName identifies the name of the column to be searched
/// \param[in] valueToFind identifies the value to be searched for in the column
/// \return a string which contains a concatenation of all values in the row found in the database table
///         if multiple values exist, return all rows with that value, where
///         each row is separated by a new line
/// \throw DoesNotExistException when trying to find a row that doesn't exist
string Sales :: search(string columnName, string valueToFind)throw (DoesNotExistException)
{
	bool resultFound = false;
	string returnString;
    string rowReceive;
	string salesID;

	string qSold, disc, productId;

	// initializes delimiters
	int delimiter;
	int delimiter2;
	int delimiter3;
	
	char delim = '|';

	// opens sales.txt
	salesInFile.open(salesTextFile);

	if(salesInFile.is_open())
	{
		// clears the contents in the string
		returnString.clear();

		while(salesInFile.good())
		{
			// assigns the string with the next line in the file
			getline(salesInFile, rowReceive);

			// break when an empty string is assigned to rowReceive
			// which occurss if there are no more valid entries in the table
			if (rowReceive.empty())
				break;

			// assigns the first delimiter position
			delimiter = rowReceive.find('|');

			// assigns the second delimiter position
			delimiter2 = rowReceive.find('|', delimiter+1);

			//assigns the third delimiter position
			delimiter3 = rowReceive.find('|', delimiter2+1);

			// assigns string salesID with data in the row
			salesID = rowReceive.substr(0,delimiter);

			// assigns string quantity sold with data in the row
			qSold = rowReceive.substr(delimiter+1, delimiter2-delimiter-1);

			
			// assigns string productId with data in the row
			productId = rowReceive.substr(delimiter2+1, delimiter3-delimiter2-1);
			// assigns string discount with data in the row
			disc = rowReceive.substr(delimiter3+1);
		
			// checks if value to find is equal to the salesID 
			if(columnName == "sales_id" && 
				atoi(salesID.c_str()) == atoi(valueToFind.c_str()))
			{
				returnString += rowReceive + "\n";
				
			}

		// checks if value to find is equal to the quantity sold
			else if(columnName == "quantity_sold" && 
				qSold == valueToFind)
			{
				returnString += rowReceive + "\n";
				
			}
			// checks if value to find is equal to the discount
			else if(columnName == "discount" &&
				disc == valueToFind)
			{
				returnString += rowReceive + "\n";
			
			}
			// checks if value to find is equal to the product ID
			else if(columnName == "productID" &&
				productId == valueToFind)
			{
				returnString += rowReceive + "\n";
				
			}
			// return the whole table
			else if(columnName == "all")
			{
				returnString += rowReceive + "\n";
			}
		}
	}

	// closes the file
	salesInFile.close();

	// ensures if the return value is less than the minimun value
	
	if(returnString == "")
		throw DoesNotExistException(valueToFind + " does not exist in column: " + columnName); 

	return returnString;
	}


/// \brief Modify function to change the data in a given row
///
/// Modification will be done by finding a matching value by searching for valueToFind
/// in the primary key column of the table
/// \pre Row exists. This will be checked by the main program beforehand
/// \param[in] valueToFind identifies the value to be searched for in the primary key
/// \param[in] columnNameToModify identifies the column to change data for
/// \prama[in] valueOfModify provides the new data for the desired column

void Sales :: modifyRow(string valueToFind, string columnNameToModify, string valueOfModify)
{
	string rowReceive;
	string salesID;
	string qSold, disc, productId;

	// vector to store the required modifications
	vector<string> newVec;

	// initializes delimiters
	int delimiter;
	int delimiter2;
	int delimiter3;

	char delim = '|';

	// opens sales.txt
	salesInFile.open(salesTextFile);

	if(salesInFile.is_open())
	{
		while(salesInFile.good())
		{
			// assigns the string with the next line in the file
			getline(salesInFile, rowReceive);

			
			// assigns the first delimiter position
			delimiter = rowReceive.find('|');
			
			// assigns the second delimiter position
			delimiter2 = rowReceive.find('|', delimiter+1);
			
			// assigns the third delimiter position
			delimiter3 = rowReceive.find('|', delimiter2+1);
			
		// assigns string sales ID with data in the row
			salesID = rowReceive.substr(0,delimiter);

			qSold = rowReceive.substr(delimiter+1, delimiter2-delimiter-1);
						
			// assigns string productId with data in the row
			productId = rowReceive.substr(delimiter2+1, delimiter3-delimiter2-1);
			// assigns string discount with data in the row
			disc = rowReceive.substr(delimiter3+1);
			
			// checks if the value to find does not match
			if( atoi(salesID.c_str()) != atoi(valueToFind.c_str())) 
			{
				newVec.push_back(rowReceive);
			} else {
				// Modifies quantity sold
				if(columnNameToModify == "quantity_sold")
					newVec.push_back(salesID + delim + valueOfModify + delim + disc + delim + productId);
				// Modifies discount
				else if(columnNameToModify == "discount")
					newVec.push_back(salesID + delim + qSold + delim + productId+ delim+  valueOfModify);
				// Modifies product ID 
				else if(columnNameToModify == "productID")
					newVec.push_back(salesID + delim + qSold + delim + valueOfModify+ delim+  disc);
			}
		}
	}

	// closes sales.txt
	salesInFile.close();

	salesOutFile.open(salesTextFile,ios_base::trunc);

	//writes string from vector to sales.txt at appropriate position
	for(int i = 0; i < (int) newVec.size(); i++)
	{
		salesOutFile<<newVec[i]<<endl;
	}

	// closes sales.txt
	salesOutFile.close();

}

/// \brief DeleteRow function to find a specific row of data and remove it from the file
///
/// Deletion will by done by finding the a matching value by searching for valueToFind
/// in the primary key column of the table 
/// \pre Row exists. This will be checked by the main program beforehand
/// \param[in] valueToFind identifies the value to be searched for in the primary key column
void Sales :: deleteRow(string valueToFind)
{
	// vector to store values except the one to be deleted
	vector<string> sFileVect;
	int delimiter;
	int delimiter2;
	int delimiter3;
	char delim;
	string rowReceive;
	string qSold, productId, disc, salesID;
	delim = '|';

	// opens sales.txt
	salesInFile.open(salesTextFile);

	if(salesInFile.is_open())
	{
		while(salesInFile.good())
		{
			// assigns the string with the next line in the file
			getline(salesInFile, rowReceive);

			// assigns the first delimiter position
			delimiter = rowReceive.find('|');

	        // assigns the second delimiter position
			delimiter2 = rowReceive.find('|', delimiter+1);

			// assigns the third delimiter position
			delimiter3 = rowReceive .find('|', delimiter2+1);
			
			
			// assigns string sales ID with data in the row
			salesID = rowReceive.substr(0,delimiter);

			// assigns string quantity sold with data in the row
			qSold = rowReceive.substr(delimiter+1, delimiter2-2);

			
			// assigns string product ID with data in the row
			productId = rowReceive.substr(delimiter2+1, delimiter3-4);
			 
			
			// assigns string discount with data in the row
			disc= rowReceive.substr(delimiter3+1);
			
			//checks if the value to find is not equal to other values
			if(atoi(salesID.c_str()) != atoi(valueToFind.c_str())) 
				
			{
				sFileVect.push_back(rowReceive);
			}
		}
	}

	//closes the file
	salesInFile.close();


	salesOutFile.open(salesTextFile, ios_base::trunc);

	for(int i = 0; i < (int) sFileVect.size(); i++)
	{
		// writes string from vector into sales.txt
		salesOutFile<<sFileVect[i]<<endl;
	}

	// closes sales.txt
	salesOutFile.close();
}

// Initializes salesTextFile
Sales::Sales() { salesTextFile = "textFiles/sales.txt"; }

Sales::~Sales(){}
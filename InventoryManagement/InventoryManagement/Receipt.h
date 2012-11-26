#ifndef RECEIPT_H
#define RECEIPT_H

#include "TableInterface.h"

/// \brief Class for interacting with the Receipt table
class Receipt : public TableInterface {

private:

	/// \brief File name of text file for Receipt table
	string fileName;

public:

	/// \brief Default constructor for Receipt
	Receipt();

	/// \brief Add function to insert data into the receipt text file
 
	void add(vector<string> addVector) throw (AlreadyExistsException);
	
	/// \brief Search function to find a specific row of data and return it as a string
	string search(string columnName, string valueToFind) throw(DoesNotExistException);


	void modifyRow(string valueToFind, string columnNameToModify, string valueOfModify);

	void deleteRow(string valueToFind);

	/// \brief Destructor for Receipt
	~Receipt();

};

#endif
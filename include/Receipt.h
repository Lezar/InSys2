#ifndef RECEIPT_H
#define RECEIPT_H

#include "TableInterface.h"

/// \brief Class for interacting with the Receipt table
///
/// Receipt will interact with a flat file representing the Receipt table in the database
/// Responsibilities of the Receipt class include:
/// - Add a row to the Receipt table
/// - Delete a row to the Receipt table
/// - Search for a row in the Receipt table
/// - Modify a row in the Receipt table
/// - Connect and disconnect from the database
class Receipt : public TableInterface {

private:

	/// \brief File name of text file for Receipt table
	string fileName;

public:

	/// \brief Default constructor for Receipt initialize fileName
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
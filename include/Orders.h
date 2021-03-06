#ifndef ORDERS_H
#define ORDERS_H


#include "TableInterface.h"

/// \brief Class for interacting with the Orders table
///
/// Orders will interact with a flat file representing the Orders table in the database
/// Responsibilities of the Orders class include:
/// - Add a row to the Orders table
/// - Search for a row in the Orders table
/// - Delete a row in the Orders table
/// - Connect and disconnect from the database
class Orders : public TableInterface
{
private:

	/// \brief file name of text file for Orders table
	string fileName;

	/// \brief rows will not be modified
	///
	/// Orders contains two foreign primary keys of InvoiceItem and Invoice.
	/// Both are auto-incremented and thus, never changed
	void modifyRow(string valueToFind, string columnNameToModify, string valueOfModify);

public:

	/// \brief default constructor for Orders
	///
	/// \post fileName initialized
	Orders();

	/// \brief Add function to insert data into the Orders text file
	/// 
	/// \param[in] addVector is a vector of strings for the data to be entered
	///            the first element is invoice_item_id
	///            the second element is invoice_id
	/// \throw AlreadyExistsException when invoice_item_id already exists, since it is unique
	void add(vector<string> addVector) throw (AlreadyExistsException);

	string search(string columnName, string valueToFind) throw(DoesNotExistException);

	/// \brief Orders will delete a row by finding that row's invoice_item_id, which is unique
	///
	/// \param[in] valueToFind is the value of invoice_item_id of the row to be deleted
	void deleteRow(string valueToFind);

	/// \brief Destructor for Orders
	~Orders();
};

#endif
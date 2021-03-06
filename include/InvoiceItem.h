#ifndef INVOICEITEM_H
#define INVOICEITEM_H

#include "TableInterface.h"
#include "Summary.h"

/// \brief Class for interacting with the InvoiceItem table
///
/// InvoiceItem will interact with a flat file representing the InvoiceItem table in the database
/// Responsibilities of the InvoiceItem class include:
/// - Add a row to the InvoiceItem table
/// - Search for a row in the InvoiceItem table
/// - Modify a row in the InvoiceItem table
/// - Delete a row in the InvoiceItem table
/// - Ensure that a new product is added to Summary table when a new product is received
/// - Ensure that changes to existing product quantities are updated in the Summary table
/// - Connect and disconnect from the database
class InvoiceItem : public TableInterface {
private:

	/// \brief file name of text file for InvoiceItem table
	string fileName;

public:

	/// \brief default constructor for InvoiceItem
	///
	/// \post fileName initialized
	InvoiceItem();

	/// \brief Add function to insert data into the invoice item text file
	/// 
	/// \param[in] addVector is a vector of strings for the data to be entered
	///            for InvoiceItem. This will be values for 
	///            product_id and quantity in that order
	/// \throw nothing will be thrown because primary key is automatically incremented, thus no duplication
	/// \post total_quantity in Summary table is appropriately modified
	void add(vector<string> addVector) throw (AlreadyExistsException);

	string search(string columnName, string valueToFind) throw(DoesNotExistException);

	/// \brief Modify function to change the data in a given row
	///
	/// Modification will be done by finding a matching value by searching for valueToFind
	/// in the primary key (invoice_item_id) column of the table. Only product_id and quantity
	/// can by modified.
	/// \pre Row exists. This will be checked by the main program beforehand
	/// \param[in] valueToFind identifies the value to be searched for in the primary key
	/// \param[in] columnNameToModify identifies the column to change data for
	/// \param[in] valueOfModify provides the new data for the desired column
	/// \post total_quantity of product in Summary table is changed by the difference of the new value
	///       when quantity in Invoice Item is changed
	/// \post total_quantities of old and new products are modified in Summary when product_id is chaged
	///       in Invoice Item
	void modifyRow(string valueToFind, string columnNameToModify, string valueOfModify);

	/// \brief Deletes a row by searching for that row's invoice_item_id
	///
	/// \param[in] valueToFind is the value of the invoice_item_id of the row to delete
	/// \post the deleted product's quantity is subtracted from that product's total quantity in summary
	void deleteRow(string valueToFind);

	/// \brief Destructor for InvoiceItem
	~InvoiceItem();
};


#endif
#ifndef SALES_H
#define SALES_H

#include "TableInterface.h"

/// \brief Class for interacting with the Sales table
///
/// Sales will interact with a flat file representing the Sales table in the database
/// Responsibilities of the Sales class include:
/// - Add a row to the Sales table
/// - Delete a row to the Sales table
/// - Search for a row in the Sales table
/// - Modify a row in the Sales table
/// - Connect and disconnect from the database
class Sales : public TableInterface
{
private:
	/// \brief output stream for writing to the Sales file
	ofstream salesOutFile;

	/// \brief output stream for writing to the Sales file
	ifstream salesInFile;

	/// \brief file name of Sales text file
	string salesTextFile;

public:
	/// \brief default constructor for Sales
	Sales();

	void add(vector<string> addVector) throw(AlreadyExistsException);

	string search(string columnName, string valueToFind) throw(DoesNotExistException);

	void deleteRow(string valueToFind);

	void modifyRow(string valueToFind, string columnNameToModify, string valueOfModify);

	/// \brief Destructor for Sales
	~Sales();
};

#endif
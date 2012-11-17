#ifndef RETURNS_H
#define RETURNS_H

#include "TableInterface.h"

/// \brief Class for interacting with the Returns table
///
/// Returns will interact with a flat file representing the Returns table in the database
/// Responsibilities of the Returns class include:
/// - Add a row to the Returns table
/// - Delete a row to the Returns table
/// - Search for a row in the Returns table
/// - Modify a row in the Returns table
/// - Connect and disconnect from the database
class Returns : public TableInterface
{
private:
	/// \brief output file stream for writing to the returns.txt file
	ofstream returnsOutFile;

	/// \brief input file stream for reading the returns.txt file
	ifstream returnsInFile;

	/// \brief variable to contain the filename to be used for the category data
	string returnsTextFile;
	

public:

	/// \brief Default Constructor initializes returnsTextFile
	Returns();

	void add(vector<string> addVector);

	string search(string columnName, string valueToFind);

	void deleteRow(string valueToFind);

	void modifyRow(string valueToFind, string columnNameToModify, string valueOfModify);

	/// \brief default destructor
	~Returns();
};

#endif
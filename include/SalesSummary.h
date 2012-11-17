#ifndef SALESSUMMARY_H
#define SALESSUMMARY_H

#include "TableInterface.h"

/// \brief Class for interacting with the SalesSummary table
///
/// SalesSummary will interact with a flat file representing the SalesSummary table in the database
/// Responsibilities of the SalesSummary class include:
/// - Add a row to the SalesSummary table
/// - Delete a row to the SalesSummary table
/// - Search for a row in the SalesSummary table
/// - Modify a row in the SalesSummary table
/// - Connect and disconnect from the database
class SalesSummary : public TableInterface
{
private:
	/// \brief Output file streams for writing to the salessummary.txt file
	ofstream ssOutFile;

	/// \brief Input file stream for read from the salessummary.txt file
	ifstream ssInFile;

	/// \brief File name for sales summary text file
	string salesSummaryTextFile;

	/// \brief rows will not be deleted
	///
	/// deleteRow will not do anything because rows will not be deleted.
	/// Both Receipt ID and Sales ID are auto-incremented
	void deleteRow(string valueToFind);

	/// \brief rows will not be modified
	///
	/// Orders contains two foreign primary keys of Receipt ID and Sales ID
	/// Both are auto-incremented and thus, never changed
	void modifyRow(string valueToFind, string columnNameToModify, string valueOfModify);

public:

	/// \brief Constructor for SalesSummary initializes salesSummaryTextFile
	SalesSummary();

	void add(vector<string> addVector) throw(AlreadyExistsException);

	string search(string columnName, string valueToFind) throw(DoesNotExistException);
	
	/// \brief Destructor for Sales Summary
	~SalesSummary();
};

#endif
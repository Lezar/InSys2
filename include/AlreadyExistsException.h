#ifndef ALREADYEXISTSEXCEPTION_H
#define ALREADYEXISTSEXCEPTION_H

#include <exception>
#include <string>

using namespace std;

/// \brief Exception class thrown when an entry already exists in the database
class AlreadyExistsException : public exception {
public:

	/// \brief Constructor to change the message diplayed by what()
	AlreadyExistsException( string message ) : exception(message.c_str()) {};
};

#endif
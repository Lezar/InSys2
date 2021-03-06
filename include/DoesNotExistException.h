#ifndef DOESNOTEXISTEXCEPTION_H
#define DOESNOTEXISTEXCEPTION_H

#include <exception>
#include <string>

using namespace std;

/// \brief Exception class thrown when accessing an entry in that database that doesn't exist
class DoesNotExistException : public exception {
public:

	/// \brief Constructor to change the message diplayed by what()
	DoesNotExistException( string message ) : exception(message.c_str()) {};
};

#endif
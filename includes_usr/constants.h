#pragma once
#include <string>

const std::string BOOKFILE			= "./tmp/bookfile.txt";		// to read from
const std::string BOOKFILE_EMPTY	= "./tmp/bookfileMT.txt";	// to write to
const std::string PATRONFILE		= "./tmp/patronfile.txt";	// to read from
const std::string PATRONFILE_EMPTY	= "./tmp/patronfileMT.txt";	// to write to

const std::string NON_EXISTANT = "./tmp/nonexistantfile";		// ???????????
const std::string TMP_FILE = "./tmp/tmp.txt";					// for manipulations before final saves?

const int BOGUS_USER 			= 10000;
const int BOGUS_BOOK 			= 10000;

const int MAX_BOOKS_ALLOWED_OUT = 5;	// Max books allowed out per patron	(different per patron)
const int UNINITIALIZED			= -1;	// Used in datastructures.h to give a "non-id value" to new users/books	(for patrons and books)
const int NO_ONE				= -2;	// The value set to loaned_to_patron_id if the book is not checked out/is a new book (different per book)
const int NONE					=  0;	// The value set to number_books_checked_out if the patron has no books checked out/is new user (different per patron)
const int PATRON_0				=  0;	// ?????????????????

//return codes
const int SUCCESS				= 0;			// Success state
const int DONT_HAVE				= SUCCESS-1;	// Book was never in the library
const int NOT_AVAILABLE			= SUCCESS-2;	// Book is checked out
const int TOO_MANY_OUT			= SUCCESS-3;	// patron.number_books_checked_out = MAX_BOOKS_ALLOWED_OUT
const int DUPLICATE_NAME		= SUCCESS-4;	// patron.name already exists
const int PATRON_NOT_ENROLLED	= SUCCESS-5;	// patron with UNINITIALIZED patron_id tries to check out a book
const int BOOK_NOT_IN_COLLECTION= SUCCESS-6;	//

const int COULD_NOT_OPEN_FILE	= SUCCESS-7;
const int NO_BOOKS_IN_LIBRARY	= SUCCESS-8;	// Empty library
const int NO_PATRONS_IN_LIBRARY	= SUCCESS-9;	// No patrons

const int ONE_POINTS = 1;
const int TWO_POINTS = 2;
const int THREE_POINTS = 3;
const int FIVE_POINTS = 5;
const int TEN_POINTS = 10;
const int FIFTEEN_POINTS = 15;

#include "../includes_usr/fileIO.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <stdlib.h>
#include <ctype.h>

using namespace std;

// Used as a delimiter
const char CHAR_TO_SEARCH_FOR = ',';

/* clears, then loads books from the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_BOOKS_IN_LIBRARY if there are 0 entries in books
 * 			SUCCESS if all data is loaded
 * */
int loadBooks(std::vector<book> &books, const char* filename) // from File to Vector
{
	// CLEARS the input vector
	books.clear();

	// Create an input stream for the books from the input file and open it
	ifstream inStreamBook;
	inStreamBook.open(filename, ios::in);

	// CHECK : is the file open?
	if (!inStreamBook.is_open()){ // if file did not open
		return COULD_NOT_OPEN_FILE; // RETURN because the file was not opened
	}
	// From here on out, assume the file is open

	// Variable to hold parts parsed from the input file
	std::string curbook; // string to hold line of text from file containing all the book's info
	std::string curbookid; // string to hold curbook's ID
	std::string curbooktitle; // string to hold curbook's title
	std::string curbookauthor; // string to hold curbook's author
	std::string curbookstate; // string to hold curbook's state
	std::string curbookpatron; // string to hold the patron id of who is loaning curbook

	// Create temp book object from struct to fill with info and push into vector books
	book tempbook;
	std::stringstream ss;

	// While the end of the file has not been reached
	while (!inStreamBook.eof()){
		std::getline(inStreamBook, curbook); // load new line (each line = 1 book's info)
		ss.str(curbook); // convert book stream from input stream to string stream and store in ss

		// CHECK : does the file have any data? TODO Might not be the right place to put this
		if (curbook == ""){
			return NO_BOOKS_IN_LIBRARY;
		}

		// Take data from input and stores data
		std::getline(ss, curbookid, CHAR_TO_SEARCH_FOR); // reads line until delim, gets curbookid
		std::getline(ss, curbooktitle, CHAR_TO_SEARCH_FOR); // reads line until delim, gets curbooktitle
		std::getline(ss, curbookauthor, CHAR_TO_SEARCH_FOR); // reads line until delim, gets curbookauthor
		std::getline(ss, curbookstate, CHAR_TO_SEARCH_FOR); // reads line until delim, gets curbookstate
		std::getline(ss, curbookpatron, CHAR_TO_SEARCH_FOR); // reads line until delim, gets curbookpatron

		// Convert data as necessary and push into tempbook struct
		tempbook.book_id = stoi(curbookid);
		tempbook.title = curbooktitle;
		tempbook.author = curbookauthor;
		switch(stoi(curbookstate)) { // switch statement to make setting the state easier
		case 0:
			tempbook.state = (UNKNOWN); break;
		case 1:
			tempbook.state = (IN); break;
		case 2:
			tempbook.state = (OUT); break;
		}
		tempbook.loaned_to_patron_id = stoi(curbookpatron);

		// Push temporary book onto the back of the books vector
		books.push_back(tempbook);
		// Clear the string stream to prepare for a new line of input
		ss.clear();
	}
	// By this point all the data should have been parsed and organized

	// CLOSE file
	inStreamBook.close();

	// CHECK : does the books vector have any data?
	if (books.size() == 0){	// if no books in library
		return NO_BOOKS_IN_LIBRARY;	// RETURN because the input vector was empty
	}

	return SUCCESS; // RETURN because file was successfully iterated through and placed into a container
}

/* serializes books to the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_BOOKS_IN_LIBRARY if there are 0 entries books (do not create file)
 * 			SUCCESS if all data is saved
 * */
int saveBooks(std::vector<book> &books, const char* filename) // from Vector to File
{
	// CHECK : does the input vector have any data?
	if (books.size() == 0){	// if no books in library
		return NO_BOOKS_IN_LIBRARY;	// RETURN because the input vector was empty
	}
	// From here on out, assume the input vector is not empty

	// Create an output stream for the books from the output file and open it
	ofstream outStreamBook;
	outStreamBook.open(filename, ios::out);

	// CHECK : is the file open?
	if (!outStreamBook.is_open()){ // if file did not open
		return COULD_NOT_OPEN_FILE; // RETURN because the file was not opened
	}
	// From here on out, assume the file has been opened and the input vector is not empty

	// Temporary string to hold a line of text containing a book's data
	std::string bookLine;
	// Fill bookLine with data and push it onto the file's ofstream
	for (int i = 0; i < books.size(); i++){
		bookLine = to_string(books[i].book_id) + "," +
					books[i].title + "," +
					books[i].author + "," +
					to_string(books[i].state) + "," +
					to_string(books[i].loaned_to_patron_id);
		outStreamBook << bookLine << "\n";
	}
	// CLOSE file
	outStreamBook.close();

	return SUCCESS; // RETURN because all data was saved successfully
}

/* clears, then loads patrons from the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_PATRONS_IN_LIBRARY if there are 0 entries in patrons
 * 			SUCCESS if all data is loaded
 * */
int loadPatrons(std::vector<patron> &patrons, const char* filename)
{
	return SUCCESS;
}

/* serializes patrons to the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_PATRONS_IN_LIBRARY if there are 0 entries in patrons  (do not create file)
 * 			SUCCESS if all data is saved
 * */
int savePatrons(std::vector<patron> &patrons, const char* filename)
{
	return SUCCESS;
}

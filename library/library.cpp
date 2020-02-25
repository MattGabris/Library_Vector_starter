#include <string>
#include <vector>
#include <algorithm>
#include <time.h>
#include <iostream>

#include "../includes_usr/library.h"
#include "../includes_usr/datastructures.h"
#include "../includes_usr/fileIO.h"
using namespace std;

// TODO ----- Why do these not work? I'm unsure how to get a const* char
// 				to point to the file names and get the laodFile and loadPatrons functions to work
char* bkFile = BOOKFILE;
char* ptFile = PATRONFILE;
char* emptybkFile = BOOKFILE_EMPTY; // To be used in reload all data??
char* emptyptFile = PATRONFILE_EMPTY;	// To be used in reload all data??
// This affects all the loadBook, loadPatron, saveBook, and savePatron calls as well as many others
// When this issue is resolved, go through code again and make sure each of these works as intended

// create vectors to hold books and patrons
vector<book> bvec;
vector<patron> pvec;
// constant vectors for clearing
const vector<book> emptybvec;
const vector<patron> emptypvec;

//NOTE: please ensure patron and book data are loaded from disk before calling the following
//NOTE: also make sure you save patron and book data to disk any time you make a change to them
//NOTE: for files where data is stored see constants.h BOOKFILE and PATRONFILE

/**
 * helper method to make clearing containers easier
 * Only used to clear the containers and re-initialize them to their empty state
 */
void clearContainers(){
	bvec = emptybvec;
	pvec = emptypvec;
}

/*
 * clear books and patrons containers
 * then reload them from disk 
 */
void reloadAllData() {
	// Clear containers (vectors)
	clearContainers();
	// Reload from disk
	loadBooks(bvec, bkFile); 	// TODO ----- CONST CHAR* ISSUE
	loadPatrons(pvec, ptFile);	// TODO ----- CONST CHAR* ISSUE
}

/**
 * helper method developed by me to make saving easier
 * Saves all data from containers to disk and then clears the containers
 */
void saveAllData(){
	// Save to disk
	saveBooks(bvec, bkFile);	// TODO ----- CONST CHAR* ISSUE
	savePatrons(pvec, ptFile);	// TODO ----- CONST CHAR* ISSUE

	// Clear containers (vectors)
	clearContainers();
}

/**
 * helper method to streamline checking if the patron is enrolled
 * returns the index of where the patron is in pvec
 * 			PATRON_NOT_ENROLLED
 */
int isPatronEnrolled(int patronid){
	for (int i = 0; i < pvec.size(); i++){
		if (patronid == pvec[i].patron_id){ // patron is enrolled, break from loop
			return i;
		}
	}
	return PATRON_NOT_ENROLLED; // RETURN because the patron is not in the enrolled
}

/**
 * helper method to streamline checking if the book is enrolled
 * returns the index of where the book is in bvec
 * 			BOOK_NOT_IN_COLLECTION
 */
int isBookInCollection(int bookid){
	for (int i = 0; i < bvec.size(); i++){
		if (bookid == bvec[i].book_id){ // book found, break from loop
			return i;	// Assign the index of the book in bvec
		}
	}
	return BOOK_NOT_IN_COLLECTION; // RETURN because the book is not in the collection
}

/* checkout a book to a patron
 * first load books and patrons from disk
 * make sure patron enrolled (patronid is assigned to a patron in patrons container)
 * make sure book in collection (bookid is assigned to a book in books container)
 * 
 * see if patron can check out any more books 
 * 	if not return TOO_MANY_OUT patron has the MAX_BOOKS_ALLOWED_OUT
 * 	
 * if so then check the book out to the patron, set the following fields for the book in the
 * books container
 * book.loaned_to_patron_id = patronid;
 * book.state = OUT;
 * 
 * Finally save the contents of the books and patrons containers to disk
 * 
 * returns SUCCESS checkout worked
 *         PATRON_NOT_ENROLLED
 * 		   BOOK_NOT_IN_COLLECTION
 *         TOO_MANY_OUT patron has the max number of books allowed checked out
 */
int checkout(int bookid, int patronid) {
	// LOAD books and patrons into vectors
	reloadAllData();
	// Variables to reassign later if both of these exist in the library/are enrolled for easier access to the entire object
	int patronIndexHolder;
	int bookIndexHolder;

	// CHECK : is patron enrolled?
	patronIndexHolder = isPatronEnrolled(patronid);
	if (patronIndexHolder == PATRON_NOT_ENROLLED){
		return PATRON_NOT_ENROLLED; // RETURN because the patron is not enrolled
	}
	// From here on out, assume the patron is enrolled
	patron thisPatron = pvec[patronIndexHolder];

	// CHECK : is book in the collection?
	bookIndexHolder = isBookInCollection(bookid);
	if (bookIndexHolder == BOOK_NOT_IN_COLLECTION){
		return BOOK_NOT_IN_COLLECTION; // RETURN because the book is not in the collection
	}
	// From here on out, assume patron is enrolled and book is in library
	book thisBook = bvec[bookIndexHolder];

	// CHECK : does the patron have the MAX_BOOKS_ALLOWED_OUT?
	if (thisPatron.number_books_checked_out == MAX_BOOKS_ALLOWED_OUT){
		return TOO_MANY_OUT; // RETURN because the patron has the max allowed number of books checked out
	}
	// From here on out, assume the patron is enrolled, the book is in the library, and the patron can check it out

	// Assign the book to the patron and set the state to out
	thisBook.loaned_to_patron_id = patronid;
	thisBook.state = OUT;
	thisPatron.number_books_checked_out++;
	// SAVE books and patrons
	saveAllData(); // ----- Hopefully this works?? I wrote a helper method
	return SUCCESS; // RETURN because all checks passed and the data was successfully saved
}

/* check a book back in 
 * first load books and patrons from disk
 * make sure book in collection (bookid is assigned to a book in books container)
 *  	
 * if so find the the patron the book is checked out to and decrement his/hers number_books_checked_out
 * then check the book back in by marking the book.loaned_to_patron_id = NO_ONE and the book.state = IN;
 
 * Finally save the contents of the books and patrons containers to disk
 * 
 * returns SUCCESS checkout worked
 * 		   BOOK_NOT_IN_COLLECTION
 */
int checkin(int bookid) {
	// LOAD books and patrons into vectors
	reloadAllData();
	// Variables to reassign later if both of these exist in the library/are enrolled for easier access to the entire object
	int patronIndexHolder = 0;
	int bookIndexHolder = 0;

	// CHECK : is book in the collection?
	bookIndexHolder = isBookInCollection(bookid);
	if (bookIndexHolder == BOOK_NOT_IN_COLLECTION){
		return BOOK_NOT_IN_COLLECTION; // RETURN because the book is not in the collection
	}
	// From here on out, assume patron is enrolled and book is in library
	book thisBook = bvec[bookIndexHolder];

	// Find the patron who turned in the book
	patronIndexHolder = bvec[bookIndexHolder].loaned_to_patron_id;
	patron thisPatron = pvec[patronIndexHolder];
	// Logic for checking in the book
	thisPatron.number_books_checked_out--;
	thisBook.loaned_to_patron_id = NO_ONE;
	thisBook.state = IN;
	// SAVE books and patrons
	saveAllData(); // ----- Hopefully this works?? I wrote a helper method
	return SUCCESS; // RETURN because all checks passed and the data was successfully saved
}

/*
 * enroll a patron, duplicate names are fine as patrons are uniquely identified by their patronid
 * first load books and patrons from disk
 * create a patron object, initialize its fields as appropriate, assign him/her the nextPatronID
 * then push the patron onto the patrons container
 * save all changes to the patrons container to disk
 * return 
 *    the patron_id of the person added
 */
int enroll(std::string &name) {
	// LOAD patrons (and books, but they won't be changed so this can be neglected)
	reloadAllData();
	// Create new patron object
	patron newPatron;
	// Generate patron_id
	int thisPatron_id = numbPatrons() + 1;
	// Initialize other fields such as name and number_books_checked_out
	newPatron.name = name;
	newPatron.number_books_checked_out = NONE;
	// push the new patron onto the back of the container
	pvec.push_back(newPatron);
	// SAVE patrons
	saveAllData(); // ----- Hopefully this works?? I wrote a helper method
	return thisPatron_id; // Return the id number of this new patron
}

/*
 * the number of books in the books container
 * (ie. if 3 books returns 3)
 * 
 */
int numbBooks() {
	return bvec.size();
}

/*
 * the number of patrons in the patrons container
 * (ie. if 3 patrons returns 3)
 */
int numbPatrons() {
	return pvec.size();
}

/*the number of books patron has checked out
 *
 *returns a positive number indicating how many books are checked out 
 *        or PATRON_NOT_ENROLLED         
 */
int howmanybooksdoesPatronHaveCheckedOut(int patronid) {
	// LOAD all patrons (and books, but they won't be changed so this can be neglected)
	reloadAllData();
	// Variable to reassign later if a patron is enrolled
	int patronIndexHolder = 0;

	// CHECK : is patron enrolled?
	patronIndexHolder = isPatronEnrolled(patronid);
	if (patronIndexHolder == PATRON_NOT_ENROLLED){
		return PATRON_NOT_ENROLLED; // RETURN because the patron is not enrolled
	}
	// From here on out, assume the patron is enrolled
	patron thisPatron = pvec[patronIndexHolder];

	return thisPatron.number_books_checked_out; // RETURN number of books checked out
}

/* search through patrons container to see if patronid is there
 * if so returns the name associated with patronid in the variable name
 * 
 * returns SUCCESS found it and name in name
 *         PATRON_NOT_ENROLLED no patron with this patronid
 */
int whatIsPatronName(std::string &name, int patronid) {
	return SUCCESS;
}





// Old Code before helper methods were introduced, just in case
//
//
//int checkout(int bookid, int patronid) {
//	// LOAD books and patrons into vectors
//	reloadAllData();
//	// Variables to reassign later if both of these exist in the library/are enrolled for easier access to the entire object
//	int patronIndexHolder = 0;
//	int bookIndexHolder = 0;
//	// CHECK : is patron enrolled?
//	for (int i = 0; i < pvec.size(); i++){
//		if (patronid == pvec[i].patron_id){ // patron is enrolled, break from loop
//			patronIndexHolder = i;	// Assign the index of the patron in pvec
//			break;
//		}
//		if (i == pvec.size() - 1){	// every patron enrolled has been checked, this is not in there ((MIGHT NOT NEED THE "-1"))
//			return PATRON_NOT_ENROLLED;// RETURN because patron is not enrolled
//		}
//	}
//	// From here on out, assume the patron is enrolled
//	patron thisPatron = pvec[patronIndexHolder];
//
//	// CHECK : is book in the collection?
//	for (int i = 0; i < bvec.size(); i++){
//		if (bookid == bvec[i].book_id){ // book found, break from loop
//			bookIndexHolder = i;	// Assign the index of the book in bvec
//			break;
//		}
//		if (i == bvec.size() - 1){ // every book has been checked, this is not in the library ((MIGHT NOT NEED THE "-1"))
//			return BOOK_NOT_IN_COLLECTION; // RETURN because the book is not in the collection
//		}
//	}
//	// From here on out, assume patron is enrolled and book is in library
//	book thisBook = bvec[bookIndexHolder];
//
//	// CHECK : does the patron have the MAX_BOOKS_ALLOWED_OUT?
//	if (thisPatron.number_books_checked_out == MAX_BOOKS_ALLOWED_OUT){
//		return TOO_MANY_OUT; // RETURN because the patron has the max allowed number of books checked out
//	}
//	// From here on out, assume the patron is enrolled, the book is in the library, and the patron can check it out
//
//	// Assign the book to the patron and set the state to out
//	thisBook.loaned_to_patron_id = patronid;
//	thisBook.state = OUT;
//	thisPatron.number_books_checked_out++;
//	// SAVE books and patrons
//	saveAllData(); // ----- Hopefully this works?? I wrote a helper method
//	return SUCCESS; // RETURN because all checks passed and the data was successfully saved
//}
//
//
//
//int checkin(int bookid) {
//	// LOAD books and patrons into vectors
//	reloadAllData();
//	// Variables to reassign later if both of these exist in the library/are enrolled for easier access to the entire object
//	int patronIndexHolder = 0;
//	int bookIndexHolder = 0;
//	// CHECK : is the book in the collection?
//	for (int i = 0; i < bvec.size();i++){
//		if (bookid = bvec[i].book_id){ // book is in collection, break from loop
//			bookIndexHolder = i; // Assign the index of the book in bvec
//			break;
//		}
//		if (i == bvec.size() - 1){	// every patron enrolled has been checked, this is not in there ((MIGHT NOT NEED THE "-1"))
//			return BOOK_NOT_IN_COLLECTION;// RETURN because patron is not enrolled
//		}
//	}
//	// From here on out, assume that the book is in the collection
//	book thisBook = bvec[bookIndexHolder];
//
//	// Find the patron who turned in the book
//	patronIndexHolder = bvec[bookIndexHolder].loaned_to_patron_id;
//	patron thisPatron = pvec[patronIndexHolder];
//	// Logic for checking in the book
//	thisPatron.number_books_checked_out--;
//	thisBook.loaned_to_patron_id = NO_ONE;
//	thisBook.state = IN;
//	// SAVE books and patrons
//	saveAllData(); // ----- Hopefully this works?? I wrote a helper method
//	return SUCCESS; // RETURN because all checks passed and the data was successfully saved
//}

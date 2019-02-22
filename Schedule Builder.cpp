// Schedule Builder.cpp : 
/*
This is a personal project that is meant to give students an easier method to signing up for their classes. 
The program will display all their classes appropriately along with data they input from sources such as 
rate my professor. Since we do not have access to your class data, users must input the data and the program 
will sort through all the data appropriately and display them back. (Currently only have the back end working)
*/

// Currently the code is very long and messy. It is neccessary in some cases becuase the information we're trying
// to extract is sometimes very hard to locate correctly, as there are many exceptions.
// I haven't had the time to optimize it.

#include "pch.h"
#include <iostream>
#include <string>
#include <algorithm>    // std::sort
using namespace std;

// This method prioritizes users preferences of day, time, and campus to find class sections that closely 
// align to what they want. This method is irrellevant to the class data extraction as well as the next method
void Priority(string priority[], string &dayPreference, string &timePreference, string &campusPreference) {
	dayPreference = "NA";
	timePreference = "NA";
	campusPreference = "NA";

	priority[0] = "Day";
	priority[1] = "Time";
	priority[2] = "Campus";
 }
// ===========================================================================================================================
void ProfessorRating(string professor[], double overallQuality[], double difficulty[], int classes) {

	for (int i = 0; i < classes; i++) {
		overallQuality[i] = 20 + i;
		difficulty[i] = 20 + i;
		cout << "\n\nOverall Quality of Professor " << i << ": " << overallQuality[i];
		cout << "\nLevel of Difficulty of Professor " << i << ": " << difficulty[i];
	}

	overallQuality[0] = 3.4;
	overallQuality[1] = 4.5;
	overallQuality[2] = 2.3;

	cout << "\n";
}
// ===========================================================================================================================
// Currently Kennesaw State has 3 options to look at their classes, this method is meant to identify which source the user has 
// inputed so it can extract the data more precicely, currently I can only identify two out of the three sources
void PlatformFinder(string &platform, string courseInfo) { 

	// Only degree works contains the words "out of"
	if (courseInfo.find("out of") != string::npos) { 
		platform = "Degree Works";
	}
	else {
		platform = "Registration";
	}
} 
// ===========================================================================================================================
// This method creates a new line within the string so it can identify when a new section starts and ends within the program
void NewLineCreator(string platform, string &courseInfo2, int classes) { 

	if (platform == "Registration") {
		int crn = 0;                                    // Used to keep track of consecutive 5 digit numbers (crn's)
		int crnlocation = 0;                            // Where within the string crn number starts
		int newlineLoc = 0;                             // Where within the string the new line should be inserted
		int end = courseInfo2.find("Location") + 8;     // Used to remove irrelevant information (anything before "Location")
		courseInfo2.erase(0, end);                      // Erases everything from beginning of the string to "Location"

		/* 1. The program goes through the entire string and looks for the crn, which is recognized by 5 digits in a row. 
		   2. If it is a digit then the "int crn" is incremented by one 3. if it isn't then crn = 0 because that means it 
		   isn't consecutive 4. Once crn reaches 5 or higher then it checks that there are no numbers after it and then 
		   defines the int crnlocation by i - 4, which is the begging of the crn. 5. If the variable in front of the crn 
		   is either a digit(part of the room number) or is an 'A' (part of TBA) then it defines new line and inserts
		   a line break and an @ symbol at the begging of the line to identify where each line starts 6. If it is neither 
		   then it must be a C(for closed section), in which case it defines the new line two space back to account for this 
		   exception, putting it in front of the C. Then it keeps looping through the string, repeating the process.
		*/

		for (size_t i = 0; i < courseInfo2.size(); i++) {                      // 1
			if (isdigit(courseInfo2[i])) {                                     // 2
				crn++;
			}
			if (!(isdigit(courseInfo2[i]))) {                                  // 3
				crn = 0;
			}
			if (crn >= 5) {                                                    // 4
				if (!(isdigit(courseInfo2[i + 1])) ) {
					crnlocation = i - 4; 
					if (isdigit(courseInfo2[crnlocation - 1]) || (courseInfo2[crnlocation - 1] == 'A')) {   // 5
						newlineLoc = crnlocation;
						courseInfo2.insert(newlineLoc, "\n@");
						crn = 0;
					}
					else {                                                     // 6
						newlineLoc = crnlocation - 2;
						courseInfo2.insert(newlineLoc, "\n@");
						crn = 0;
					}
				}
			}
		}
	}
	else if (platform == "Degree Works") { 
		int crn = 0;              // Used to keep track of consecutive 5 digit numbers (crn's)
		int crnlocation = 0;      // Where within the string crn number starts
		int newlineLoc = 0;       // Where within the string the new line should be inserted
		int letterlocation;       // Where within the string the word "Campus" or "Online" begins
		bool done = false;        // Used for loops

		int end = courseInfo2.find("Instructor") + 10;  // Used to remove irrelevant information (anything before "Location")
		courseInfo2.erase(0, end);                      // Erases everything from beginning of the string to "Location"

		/* The beginning of a section is defined differently in degree works. Each section begins with it's respected campus.
		   And that's where we'll create the new line 1. The program goes through the string and looks for the word campus.
		   And defines int letterlocation as two spaces back which should be the end of the word Kennesaw or Marrieta.
		   2. It then goes backwards through the string and keeps checking for a space which is where the word must end.
		   then it respectfully defines the newlinelocation and inserts a new line followed by an @ symbol. Sometimes it 
		   doesn't correctly find the right space so it also checks for a 'K' or 'M' which is also the beginning of the 
		   campus word 4. And sometimes there is no campus because it is online so it looks for the word online and does 
		   a similar process.
		*/
		for (size_t i = 0; i < courseInfo2.size(); i++) {                                                             // 1
			if ((courseInfo2[i] == 'C') && (courseInfo2[i + 1] == 'a') && (courseInfo2[i + 2] == 'm')
				&& (courseInfo2[i + 3] == 'p') && (courseInfo2[i + 4] == 'u') && (courseInfo2[i + 5] == 's')) {
				letterlocation = i - 2;
				for (int j = 0; done == false; j++) {                                                                 // 2
					if ((courseInfo2[letterlocation - j] == ' ') || (courseInfo2[letterlocation - j] == '\t')) {
						newlineLoc = (letterlocation - j) + 1;
						courseInfo2.insert(newlineLoc, "\n@");
						done = true;
						i = i + 2; // two items were inserted so it jumps two items to account for this
					}
					else if ((courseInfo2[letterlocation - j] == 'K') || (courseInfo2[letterlocation - j] == 'M')) {  // 3
						newlineLoc = (letterlocation - j);
						courseInfo2.insert(newlineLoc, "\n@");
						done = true;
						i = i + 2;
					}
				}
			}
			else if ((courseInfo2[i] == 'O') && (courseInfo2[i + 1] == 'n') && (courseInfo2[i + 2] == 'l')           // 4
				&& (courseInfo2[i + 3] == 'i') && (courseInfo2[i + 4] == 'n') && (courseInfo2[i + 5] == 'e')) {
				letterlocation = i - 2;
				for (int j = 0; done == false; j++) {
					if ((courseInfo2[letterlocation - j] == ' ') || (courseInfo2[letterlocation - j] == '\t')) {
						newlineLoc = (letterlocation - j) + 1;
						courseInfo2.insert(newlineLoc, "\n@");
						done = true;
						i = i + 2;
					}
					else if ((courseInfo2[letterlocation - j] == 'K') || (courseInfo2[letterlocation - j] == 'M')) {
						newlineLoc = (letterlocation - j);
						courseInfo2.insert(newlineLoc, "\n@");
						done = true;
						i = i + 2;
					}
				}
			}
			done = false;
		}
	}
	cout << "\n" << courseInfo2;

} // Inserts an @ at the begging of a new section 
// ===========================================================================================================================
// There are exections when trying to find some data in class sections when there is no data the program can find
// This pogram correctly identifies where there are TBA professors so the rest of the program doesn't get confused
void TBAProfessorFinder(string platform, string courseInfo2, int classes, int &tbaprofessors, string tbaprofessorLoc[]) {
	string courseInfo2copy = courseInfo2;     // Makes a copy of the string so we can alter it however we want
	int line = 0;                             // Used to keep track of which line we're on. Increments tbaprofessorLoc[]
	int availableprofessors = 0;              // Used to keep track of which teachers aren't tba
	int atLoc;                                // Marks the location of the @ symbol
	int tbalocation;                          // Marks the location of the word 'TBA'

	/* 1. The program goes through the string and if it finds an @ symbol then it registers that as a new line
	   2. It then checks the string for the word TBA
	   3. Makes sure it doesn't cause an error by looking too far in a string. The professor tba won't be near the end anyway.
	   4. Checks 7 spaces forward to make sure it has an '/'. So as to not confuse with the class tba. Then it register which
	   line the tba professor is found and increments the amount of tba professors.
	   Repeats process until it has gone through the entire string
	*/

	if (platform == "Registration") {
		for (size_t i = 0; i < courseInfo2copy.size(); i++) {           // 1
			if ((courseInfo2copy[i] == '@') && (i > 1))					
				line++;													
																		
			if (courseInfo2copy[i] == 'T') {							// 2
				if (courseInfo2copy[i + 1] == 'B') {					
					if (courseInfo2copy[i + 2] == 'A') {				
						if (i < courseInfo2copy.size() - 5) {			// 3
							if (courseInfo2copy[i + 7] == '/') {		// 4
								tbaprofessorLoc[line] = "TBA";			
								tbaprofessors++;						
							}											
						}
					}
				}
			}
		}

		availableprofessors = classes - tbaprofessors;
	}
	/* 1. This program increments only as many times as need (number of classes + 1: to account for the last line)
	   2. Looks through the string and finds where the "@" is located, sets atloc as that location and tbalocation
	   as four spaces back, since the professor tba is found at the end of a line in degree works. 
	   3. If there is no @ symbol found then that means we're on the last line, which doesn't have an @ symbol in front of it
	   so it sets it's location as 3 spaces back the size of the string.
	   4. If i != 0 because there is an @ symbol on the first line but not a professor on the line above because it's the first,
	   if it finds the words tba then it registers it respectfully 
	   5. Deletes part of the string where @ symbol was found, so once it loops again, it doesn't keep looking at the same @ symbol
	   and the same professor
	   Repeats process until there are no more class sections
	*/
	else if (platform == "Degree Works") {										 // 1
		for (int i = 0; i < classes + 1; i++) {									 
			if (courseInfo2copy.find("@") != string::npos) {					 // 2
				atLoc = courseInfo2copy.find("@");								  
				tbalocation = atLoc - 4;										  
			}																	  
			else {																  
				tbalocation = courseInfo2copy.size() - 3;						 // 3
			}																	  
			if (i != 0) {														 // 4
				if (courseInfo2copy[tbalocation] == 'T') {						 
					if (courseInfo2copy[tbalocation + 1] == 'B') {				 
						if (courseInfo2copy[tbalocation + 2] == 'A') {			 
							tbaprofessorLoc[i - 1] = "TBA";						  
							tbaprofessors++;									  
						}														  
					}															  
				}																  
			}																	  
			courseInfo2copy.erase(0, (atLoc + 6));								 // 5
		}
		availableprofessors = classes - tbaprofessors;
	}

	cout << "\n\n" << "Classes: " << classes;
	cout << "\n" << "TBA Professors: " << tbaprofessors;
	cout << "\n" << "Available Professors: " << availableprofessors;
}
// ===========================================================================================================================
void TBATimeFinder(string platform, string courseInfo2, int classes, int &tbaclasses, string tbaclassLoc[]) {
	string courseInfo2copy = courseInfo2;     // Makes a copy of the string so we can alter it however we want
	int line = 0;                             // Used to keep track of which line we're on
	int classroomclasses = 0;                 // Used to determine which classes meet in a classroom (not online)

	/* This program is simpler than most, all tba classes have the word "Online" in it so we just need to check for it
	   1. Goes through the entire string character by character and if it encounters an @ symbol then it increments the line varibale
	   2. Then checks the string for the word "Online". If it is found then it registers which line the online class is respectfully and 
	   increments the number of tbaclasses.
	*/

	for (size_t i = 0; i < courseInfo2copy.size(); i++) {				 // 1
		if ((courseInfo2copy[i] == '@'))								 
			line++;														 
																		 
		if (courseInfo2copy[i] == 'O') {								 // 2
			if (courseInfo2copy[i + 1] == 'n') {						  
				if (courseInfo2copy[i + 2] == 'l') {					  
					if (courseInfo2copy[i + 3] == 'i') {				  
						if (courseInfo2copy[i + 4] == 'n') {			  
							if (courseInfo2copy[i + 5] == 'e') {		 
								tbaclassLoc[line - 1] = "TBA";			  
								tbaclasses++;							  
							}											  
						}												  
					}													  
				}														  
			}															  
		}																  
	}																	  
	classroomclasses = classes - tbaclasses;

	cout << "\n\n" << "Classes: " << classes;
	cout << "\n" << "TBA Classes: " << tbaclasses;
	cout << "\n" << "Classroom Classes: " << classroomclasses;
}
// ===========================================================================================================================
void StatusFinder(string platform, string courseInfo, int classes, string status[]) {
	// Need to edit this but for now, all classes status will be WR(web registered)
	cout << "\n\n" << "Status: ";
	for (int k = 0; k < classes; k++) {
		status[k] = "WR";
		cout << "\n" << status[k];
	}
}
// ===========================================================================================================================
void WaitlistFinder(string platform, string courseInfo, int classes, string waitlist[]) {
	string waitlistCharacter = "";          
	int num = 0;
	int waitlistNum = 0;
	int waitlistlocation = 0;

	if (platform == "Registration") {
		for (size_t i = 0; i < courseInfo.size(); i++) {
			if (!(isdigit(courseInfo[i]))) {
				waitlistNum = 0;
			}
			if (isdigit(courseInfo[i])) {
				waitlistNum++;
			}
			if (waitlistNum >= 3) {
				if ((courseInfo[i + 1] == ' ') || (courseInfo[i + 1] == '\t')) {
					if ((courseInfo[i - 3] == ' ') || (courseInfo[i - 3] == '\t')) {
						waitlistlocation = i - 4;
						for (int j = 0; j < 5; j++) {
							waitlistCharacter += courseInfo[waitlistlocation + j];
						}
						waitlist[num] = waitlistCharacter;
						waitlistCharacter = "";
						num++;
						waitlistNum = 0;
					}
				}
			}
		}
	}
	else if (platform == "Degree Works") {

	}
	cout << "\n\n" << "Waitlists: ";
	for (int k = 0; k < classes; k++) {
		cout << "\n" << waitlist[k];
	}
}
// ===========================================================================================================================
void CrnFinder(string platform, string courseInfo, int &classes, string crn[]) {
	string crnCharacter = "";          // Used to store the crn string (we can get rid of this later)
	int n = 0;                         // Used to increment crn[] 
	int crnNum = 0;                    // Used to keep track of consecutive 5 digit numbers (crn's)
	int crnlocation = 0;               // Where within the string crn number starts

	/* This program looks for 5 digits that are side by side each other, only the crns have 5 digits
	   1. Program goes through the entire string and if it encounters a digit then it increments crnNum
	   2. If it isn't then it resets crnNum since the digits aren't side by side
	   3. Once crnNum reaches 5 digits then it makes sure the next number isn't a digit(which would make a 6 digit number) and 
	   then registers the crnlocation as 4 spaces back(the beginning of the crn number)
	   4. It then goes into a loop, going through each character of the number and storing it in the crnCharacter string
	   5. Then it stores the string into crn[n], increments n for the next section and resets everything.
	   6. Number of class sections equals n
	*/

	for (size_t i = 0; i < courseInfo.size(); i++) {					 // 1
		if (isdigit(courseInfo[i])) {									 
			crnNum++;
		}
		if (!(isdigit(courseInfo[i]))) {								 // 2
			crnNum = 0;													 
		}																 																 
		if (crnNum >= 5) {												 // 3
			if (!(isdigit(courseInfo[i + 1]))) {						 
				crnlocation = i - 4;									 
				for (int j = 0; j < 5; j++) {							 // 4
					crnCharacter += courseInfo[crnlocation + j];		 
				}														 
				crn[n] = crnCharacter;									 // 5
				crnCharacter = "";										 
				n++;													 
				crnNum = 0;												 
			}															 
		}																 
	}																	 
	classes = n;														 // 6

	cout << "\n\n" << "CRNs: ";
	for (int k = 0; k < classes; k++) {
		cout << "\n" << crn[k];
	}
}
// ===========================================================================================================================
void HourFinder(string platform, string courseInfo, int classes, string hoursChar[], int hoursInt[]) {
	string courseInfocopy = courseInfo;    // Makes a copy of the string so we can alter it however we want
	int periodLoc;                         // Where within the string the first period is found
	string hourStr = "";                   // Used to store the hour str
	string placeholder = "";               // Used to convert string hourStr to an integer for hoursInt[]
	int n = 0;                             // Used to increment crn[] 

	/* 1. The program increments as many times as there are classes, it first locates where a period is found and sets it's 
	   location as one back (actually looking at the variable in front of the of the period, which is ideally our number for hours)
	   2. It then goes in a loop, going five space forward from the periodLoc and registering each character into the hourStr
	   3. Stores that string in hourChar[n] and does some code to convert the first number into hourInt[n], resets and increments
	   4. Cuts part of the string that is no longer relevant
	*/

	if (platform == "Registration") {
		for (int i = 0; i < classes; i++) {						   // 1
			periodLoc = courseInfocopy.find(".") - 1;			   
			for (int j = 0; j < 5; j++) {						   // 2
				hourStr += courseInfocopy[periodLoc + j];		   
			}													   
			hoursChar[n] = hourStr;								   // 3
			string placeholder = hoursChar[n];					   
			placeholder += courseInfocopy[periodLoc];			   
			hoursInt[n] = stoi(placeholder);					   
			hourStr = "";										   
			n++;												   
			courseInfocopy.erase(0, (periodLoc + 4));			   // 4
		}
	}
	/* 1. Locates the word credit, and sets creditLoc as two spaces back (which is where the actual number should be ex. "3 Credits")
	   2. Sets the first hoursChar[] to that number and adds ".000" to it
	   3. Extracts the first character in hoursChar[0] and sets it in placeholder (placeholder now holding only one number "3")
	   4. Sets the rest of the sections to the same hoursChar[] and hoursInt[], since all sections have the same credit hours 
	   since it's the same class
	*/
	else if (platform == "Degree Works") {						   
		int creditLoc;											   
		creditLoc = courseInfocopy.find("Credit") - 2;			   // 1
		hoursChar[0] = courseInfocopy[creditLoc];				   // 2
		hoursChar[0] += ".000";									   
		string placeholder;										   
		placeholder = hoursChar[0];								   // 3
		placeholder = placeholder[0];							   
		for (int i = 0; i < classes; i++) {						   // 4
			hoursChar[i] = hoursChar[0];						   
			hoursInt[i] = stoi(placeholder);					   
		}														   
	}

	cout << "\n\n" << "Hours: ";
	for (int k = 0; k < classes; k++) {
		cout << "\n" << hoursChar[k] << "  " << hoursInt[k];
	}
}
// ===========================================================================================================================
void MeetingFinder(string platform, string courseInfo, int classes, string meeting[], int hours[]) {
	// If hours is greater than 1 then it must be a lecture, only labs 1 credit hours
	cout << "\n\n" << "Meeting: ";
	for (int i = 0; i < classes; i++) {
		if (hours[i] > 1)
			meeting[i] = "Lecture";
		else
			meeting[i] = "Lab";
		cout << "\n" << meeting[i];
	}
}
// ===========================================================================================================================
void DateFinder(string platform, string courseInfo2, int classes, string date[]) {
	string courseInfo2copy = courseInfo2;   // Makes a copy of the string so we can alter it however we want
	int slashLoc;                           // Where within the string the "/" is found
	string dateStr = "";                    // Used to store the string dateStr
	int n = 0;                              // Used to increment crn[] 
	
	/* Follows pretty much the same precedure as the others. Locates "/" and the stores only the needed character into date[]
	   1. If it encounters a space, then it checks to see if the space in front or in back of it is "-", if it is then it
	   adds the space into the string, other wise the space isn't needed
	   2. If it's not a space or tab then it can print whatever it is
	*/
	if (platform == "Registration") { 
		for (int i = 0; i < classes; i++) {
			slashLoc = courseInfo2copy.find("/") - 3;
			for (int j = 0; j < 17; j++) {
				if ((courseInfo2copy[slashLoc + j] == ' ') || (courseInfo2copy[slashLoc + j] == '\t')) {     // 1
					if (courseInfo2copy[slashLoc + j + 1] == '-')											 
						dateStr += courseInfo2copy[slashLoc + j];
					else if (courseInfo2copy[slashLoc + j - 1] == '-')
						dateStr += courseInfo2copy[slashLoc + j];
					else
						dateStr += "";
				}
				else
					dateStr += courseInfo2copy[slashLoc + j];												 // 2
			}
			date[n] = dateStr;
			dateStr = "";
			n++;
			courseInfo2copy.erase(0, (slashLoc + 17));
		}
	}
	// Looks for the words "Fall", "Spring", and "Summer", if it finds any then it correctly sets the right dates in date[]
	else if (platform == "Degree Works") {
		if (courseInfo2copy.find("Fall") != string::npos) {
			date[0] = "08/13 - 12/10";
		}
		else if (courseInfo2copy.find("Spring") != string::npos) {
			date[0] = "01/07 - 05/06";
		}
		else if (courseInfo2copy.find("Summer") != string::npos) {
			date[0] = "05/30 - 07/25";
		}
		// Sets the rest of section dates to the same date as the first.
		for (int i = 0; i < classes; i++) {
			date[i] = date[0];
		}
	}

	cout << "\n\n" << "Dates: ";
	for (int k = 0; k < classes; k++) {
		cout << "\n" << date[k];
	}

}
// ===========================================================================================================================
void DayFinder(string platform, string courseInfo2, int classes, string days[]) {
	string courseInfo2copy = courseInfo2;   // Makes a copy of the string so we can alter it however we want
	int line = 0;                           // Keeps track of which line we're on

	/* Follows the same precedure as ones mentioned above with some exceptions. Increments where it identfies a new line and 
	   looks for the online first. If it finds it then that means days[] on whatever line it is, is tba
	*/

	for (size_t k = 0; k < courseInfo2copy.size() - 2; k++) {
		if ((courseInfo2copy[k] == '@'))
			line++;

		if (courseInfo2copy[k] == 'O') {
			if (courseInfo2copy[k + 1] == 'n') {
				if (courseInfo2copy[k + 2] == 'l') {
					if (courseInfo2copy[k + 3] == 'i') {
						if (courseInfo2copy[k + 4] == 'n') {
							if (courseInfo2copy[k + 5] == 'e') {
								days[line - 1] += "TBA";
							}
						}
					}
				}
			}
		}
		/* Then if first checks to see if the character two spaces in front of it aren't both digits or aren't both letters
		   Days shouldn't be followed by numbers or letters, it should be something like (" MW "), with the excetion of MWF
		*/ 
		/* If it finds one of those unique letters (T, TR, MW, W, etc.) then it stores the string in days[line] (which ever line 
		   it's on. An exception is put in for 'M', which makes sure the word infront of it isn't Campus. Since sometimes the string
		   contians something like "Marrieta Campus M" - This is not a day
		*/
		if ((!isdigit(courseInfo2copy[k + 1])) || (!isdigit(courseInfo2copy[k + 2])) || (courseInfo2copy[k + 2] == 'F')) {
			if ((!isalpha(courseInfo2copy[k + 1])) || (!isalpha(courseInfo2copy[k + 2]))) {

				if ((courseInfo2copy[k] == 'T') && (courseInfo2copy[k + 1] == 'u')) {
					days[line - 1] += "T";
				}
				if ((courseInfo2copy[k] == 'T') && (courseInfo2copy[k + 1] == 'h')) {
					days[line - 1] += "R";
				}
				if ((courseInfo2copy[k] == 'T') && (courseInfo2copy[k + 1] == 'R')) {
					days[line - 1] += "TR";
				}
				if ((courseInfo2copy[k] == 'M')) {
					if ((courseInfo2copy[k - 7] != 'C') && (courseInfo2copy[k - 6] != 'a') && (courseInfo2copy[k - 5] != 'm')
						&& (courseInfo2copy[k - 4] != 'p') && (courseInfo2copy[k - 3] != 'u') && (courseInfo2copy[k - 2] != 's')) {
						days[line - 1] += "M";
					}
				}
				if (courseInfo2copy[k] == 'W') {
					days[line - 1] += "W";
				}
				if (courseInfo2copy[k] == 'F') {
					days[line - 1] += "F";
				}
				if ((courseInfo2copy[k] == 'S') && (courseInfo2copy[k + 1] == 'a')) {
					days[line - 1] += "Sa";
				}
				if ((courseInfo2copy[k] == 'S') && (courseInfo2copy[k + 1] == 'u')) {
					days[line - 1] += "Su";
				}
			}
		}
	}

	cout << "\n\n" << "Days: ";
	for (int k = 0; k < classes; k++) {
		cout << "\n" << days[k];
	}
}
// ===========================================================================================================================
void TimeFinder(string platform, string courseInfo2, int classes, int tbaclasses, string tbaclassLoc[], string timeChar[], int timeInt[]) {
	string courseInfo2copy = courseInfo2;    // Makes a copy of the string so we can alter it however we want
	int colonLoc;                            // Where within the string the ":" is found
	string timeStr = "";                     // Used to hold the string timeStr
	string timeNumStr = "";                  // Used to help convert string timeStr to timeInt[]
	int classroomclasses = (classes - tbaclasses) * 2;    // Only classroom classes have times and each class has a 
	                                                      // beginning time and an ending time(so times 2)
	int n = 0;                                            // Used to increment crn[]
	string beginend[2];
	int iterator = 0;
	/* 1. The program first checks the tbaclassLoc[] array to see if TbaTimeFiner() found any tba classes. If the section it's
	   on finds that it's a tba class, then it sets colonLoc where it finds the word "TBA" and stores "TBA" in timeChar[i]
	   2. Other wise it looks in the string for the ":" in "2:00 pm - 3:15 pm"
	   Goes 23 space forward and stores the relevant texts (which is only if it's 'p', 'm', a digit, or ':', '-') in timeStr
	   If it's a digit then it also stores the number in timNumStr, which ultimately converts it to int and stores it in timeInt[]
	*/

	for (int i = 0; i < classes; i++) {
		if (tbaclassLoc[i] == "TBA") {												   // 1
			colonLoc = courseInfo2copy.find("TBA") + 3;
			timeStr += "TBA";
			timeChar[i] = timeStr;
			n++;
		}
		else {
			colonLoc = courseInfo2copy.find(":") - 3;                                  // 2
			for (int j = 0; j < 23; j++) {
				if (isalpha(courseInfo2copy[colonLoc + j])) {
					if (courseInfo2copy[colonLoc + j] == 'm')
						timeStr += courseInfo2copy[colonLoc + j];

					if (courseInfo2copy[colonLoc + j] == 'p') {
						timeStr += courseInfo2copy[colonLoc + j];
						beginend[iterator] = "1";
						iterator++;
					}
					else if (courseInfo2copy[colonLoc + j] == 'a') {
						timeStr += courseInfo2copy[colonLoc + j];
						beginend[iterator] = "0";
						iterator++;
					}
				}
				else if (isdigit(courseInfo2copy[colonLoc + j])) {
					timeStr += courseInfo2copy[colonLoc + j];
					timeNumStr += courseInfo2copy[colonLoc + j];
				}
				else if ((courseInfo2copy[colonLoc + j] == ':') || (courseInfo2copy[colonLoc + j] == '-')) {
					timeStr += courseInfo2copy[colonLoc + j];
				}
				else
					timeStr += "";
			}
			timeChar[i] = timeStr;
			string placeholder = timeNumStr;
			string placeholder1 = beginend[0];
			string placeholder2 = beginend[1];

			/* Times have two times- a starting time and an ending time, hence the two placeholders. The first one
			   stores the first half of the time and the second stores the second half
			*/
			for (int k = 0; k < (placeholder.size() / 2); k++) {
				placeholder1 += placeholder[k];
			}
			for (int k = (placeholder.size() / 2); k < placeholder.size(); k++) {
				placeholder2 += placeholder[k];
			}
			timeInt[n] = stoi(placeholder1);
			timeInt[n + 1] = stoi(placeholder2);
			n = n + 2; // Needs to jump 2 to account for the two times (timeInt[] will have twice as many characters as timeChar[])
			// which is two for each class
		}
		timeStr = "";
		timeNumStr = "";
		iterator = 0;
		courseInfo2copy.erase(0, (colonLoc + 23));
	}

	cout << "\n\n" << "Times: ";
	for (int k = 0; k < classes; k++) {
		cout << "\n" << timeChar[k];
	}

	for (int k = 0; k < classroomclasses; k++) {
		cout << "\n" << timeInt[k];
	}
}
// ===========================================================================================================================
void BuildingFinder(string platform, string courseInfo2, int classes, string tbaclassLoc[], string building[]) {
	string courseInfo2copy = courseInfo2;   // Makes a copy of the string so we can alter it however we want
	string buildingStr = "";                // Used to store the building string
	int atLoc;                              // Where within the string the "@" symbol is found
	int buildinglocation;                   // Where within the string the building name is found
	int buildinglength;                     // How many letters the building name has
	bool done = false;                      // Used for loops
	int n = 0;                              // Used to increment building[]

	/* This method is a bit different from others, It
	*/

	if (platform == "Registration") {
		for (int i = 0; i < classes + 1; i++) {
			if (courseInfo2copy.find("@") != string::npos) {
				atLoc = courseInfo2copy.find("@");
				buildinglocation = atLoc - 3;
			}
			else {
				buildinglocation = courseInfo2copy.size() - 2;
			}

			if ((i != 0) && (courseInfo2copy[buildinglocation + 1] == 'A')) {
				building[n] = "TBA";
				n++;
			}
			else {
				if (i != 0) {
					buildinglocation = buildinglocation - 2;
					for (int j = 0; done == false; j++) {
						if ((courseInfo2copy[buildinglocation - j] == ' ') || (courseInfo2copy[buildinglocation - j] == '\t')) {
							if (isdigit(courseInfo2copy[buildinglocation - j - 1])) {
								buildinglocation = (buildinglocation - j) + 1;
								buildinglength = j;
								done = true;
							}
						}
					}
					for (int b = 0; b < buildinglength; b++) {
						buildingStr += courseInfo2copy[buildinglocation + b];
					}
					building[n] = buildingStr;
					n++;
				}
			}
			done = false;
			buildingStr = "";
			courseInfo2copy.erase(0, (atLoc + 1));
		}
	}
	else if (platform == "Degree Works") {
		for (int i = 0; i < classes; i++) {
			if (tbaclassLoc[i] == "TBA") {
				buildinglocation = courseInfo2copy.find("TBA") + 3;
				buildingStr += "TBA";
				building[i] = buildingStr;
			}
			else {
				buildinglocation = courseInfo2copy.find("/") - 2;
				for (int j = 0; done == false; j++) {
					if ((courseInfo2copy[buildinglocation - j] == ' ') || (courseInfo2copy[buildinglocation - j] == '\t')
						|| (courseInfo2copy[buildinglocation - j] == '.')) {
						buildinglocation = (buildinglocation - j) + 1;
						buildinglength = j;
						done = true;
					}
				}
				for (int b = 0; b < buildinglength; b++) {
					buildingStr += courseInfo2copy[buildinglocation + b];
				}
				building[i] = buildingStr;
			}
			done = false;
			buildingStr = "";
			courseInfo2copy.erase(0, (buildinglocation + 4));
		}
	}
	cout << "\n\n" << "Locations: ";
	for (int k = 0; k < classes; k++) {
		cout << "\n" << building[k];
	}
}
// ===========================================================================================================================
void RoomFinder(string platform, string courseInfo2, int classes, string tbaclassLoc[], string room[]) {
	string courseInfo2copy = courseInfo2;    // Makes a copy of the string so we can alter it however we want
	string roomStr = "";                     // Used to store the room string
	int atLoc;                               // Where within the string the "@" symbol is found
	int roomlocation;                        // Where within the string the room # is found
	int n = 0;                               // Used to increment room[]

	if (platform == "Registration") {
		for (int i = 0; i < classes + 1; i++) {
			if (courseInfo2copy.find("@") != string::npos) {
				atLoc = courseInfo2copy.find("@");
				roomlocation = atLoc - 3;
			}
			else {
				roomlocation = courseInfo2copy.size() - 2;
			}
			if (i != 0) {
				roomStr += courseInfo2copy[roomlocation - 1];
				roomStr += courseInfo2copy[roomlocation];
				roomStr += courseInfo2copy[roomlocation + 1];
				room[n] = roomStr;
				n++;
			}
			roomStr = "";
			courseInfo2copy.erase(0, (atLoc + 1));
		}
	}
	else if (platform == "Degree Works") {
		for (int i = 0; i < classes + 1; i++) {
			if (tbaclassLoc[i] == "TBA") {
				roomlocation = courseInfo2copy.find("TBA") + 3;
				roomStr += "TBA";
				room[i] = roomStr;
			}
			else {
				roomlocation = courseInfo2copy.find("/") + 3;
				roomStr += courseInfo2copy[roomlocation - 1];
				roomStr += courseInfo2copy[roomlocation];
				roomStr += courseInfo2copy[roomlocation + 1];
				room[i] = roomStr;
			}
			roomStr = "";
			courseInfo2copy.erase(0, (roomlocation));
		}
	}
	cout << "\n\n" << "Rooms: ";
	for (int k = 0; k < classes; k++) {
		cout << "\n" << room[k];
	}
}
// ===========================================================================================================================
void CourseFinder(string platform, string courseInfo, int classes, string course[]) {
	string courseCharacter = "";               // Used to store the course string
	int n = 0;								   // Used to incremenet course[]
	int courseNum = 0;						   // 
	int courselocation = 0;					   // 
	int letterlocation = 0;					   // 
	int letterlength = 0;					   // 
	bool done = false;						   // 

	if (platform == "Registration") {
		for (size_t i = 0; i < courseInfo.size(); i++) {
			if (!(isdigit(courseInfo[i]))) {
				courseNum = 0;
			}
			if (isdigit(courseInfo[i])) {
				courseNum++;
			}
			if (courseNum >= 4) {
				if ((courseInfo[i + 1] == ' ') || (courseInfo[i + 1] == '\t')) {
					if ((courseInfo[i - 4] == ' ') || (courseInfo[i - 4] == '\t')) {
						courselocation = i - 3;
						letterlocation = courselocation - 2;
						for (int j = 0; done == false; j++) {
							if ((courseInfo[letterlocation - j] == ' ') || (courseInfo[letterlocation - j] == '\t')) {
								letterlocation = (letterlocation - j) + 1;
								done = true;
								letterlength = j;
							}
						}
						for (int j = 0; j < (5 + letterlength); j++) {
							courseCharacter += courseInfo[letterlocation + j];
						}
						course[n] = courseCharacter;
						courseCharacter = "";
						done = false;
						n++;
						courseNum = 0;
					}
				}
			}
		}
	}
	else if (platform == "Degree Works") {
		courselocation = courseInfo.find("Credit") + 8;
		for (int j = 0; done == false; j++) {
			if ((courseInfo[courselocation + j + 1] == 'P') && (courseInfo[courselocation + j + 2] == 'r')
				&& (courseInfo[courselocation + j + 3] == 'i') && (courseInfo[courselocation + j + 4] == 'n')
				&& (courseInfo[courselocation + j + 5] == 't')) {
				done = true;
				letterlength = j;
			}
		}
		for (int c = 0; c < letterlength; c++) {
			courseCharacter += courseInfo[courselocation + c];
		}
		course[0] = courseCharacter;
		courseCharacter = "";
		done = false;

		for (int k = 0; k < classes; k++) {
			course[k] = course[0];
		}
	}

	cout << "\n\n" << "Courses: ";
	for (int k = 0; k < classes; k++) {
		cout << "\n" << course[k];
	}
}
// ===========================================================================================================================
void CourseNameFinder(string platform, string courseInfo, int classes, string coursename[]) {
	string courseCharacter = "";
	int courselocation = 0;
	int letterlength = 0;
	bool done = false;

	if (platform == "Registration") {
		for (int i = 0; i < classes; i++) {
			courselocation = courseInfo.find(".000") + 7;
			for (int j = 0; done == false; j++) {
				if ((courseInfo[courselocation + j + 1] == ' ') || (courseInfo[courselocation + j + 2] == '\t')) {
					done = true;
					letterlength = j;
				}
			}
			for (int c = 0; c < letterlength; c++) {
				courseCharacter += courseInfo[courselocation + c];
			}
			coursename[0] = courseCharacter;
			courseCharacter = "";
			done = false;
		}
	}
	else if (platform == "Degree Works") {
		courselocation = courseInfo.find("Credit") + 8;
		for (int j = 0; done == false; j++) {
			if ((courseInfo[courselocation + j + 1] == 'P') && (courseInfo[courselocation + j + 2] == 'r')
				&& (courseInfo[courselocation + j + 3] == 'i') && (courseInfo[courselocation + j + 4] == 'n')
				&& (courseInfo[courselocation + j + 5] == 't')) {
				done = true;
				letterlength = j;
			}
		}
		for (int c = 0; c < letterlength; c++) {
			courseCharacter += courseInfo[courselocation + c];
		}
		coursename[0] = courseCharacter;
		courseCharacter = "";
		done = false;

		for (int k = 0; k < classes; k++) {
			coursename[k] = coursename[0];
		}
	}

	cout << "\n\n" << "Course Name: ";
	for (int k = 0; k < classes; k++) {
		cout << "\n" << coursename[k];
	}
}
// ===========================================================================================================================
void CampusFinder(string platform, string courseInfo2, int classes, int tbaclasses, string tbaclassLoc[], string campus[]) {
	string courseInfo2copy = courseInfo2;      // Makes a copy of the string so we can alter it however we want
	string campusStr = "";					   // Used to store the campus string
	int campusLoc;							   // Where within the string the word "Campus" is found
	int letterlocation;						   // Where within the string the end of the campus name ends/begins
	int letterlength;						   // The length of the campus name (Kennesaw = 8)
	int n = 0;								   // Used to increment campus[]
	bool done = false;						   // Used for loops
											   
	for (int i = 0; i < classes; i++) {
		if (tbaclassLoc[i] == "TBA") {
			campusLoc = courseInfo2copy.find("TBA") + 3;
			campusStr += "TBA";
			campus[n] = campusStr;
			n++;
		}
		else {
			campusLoc = courseInfo2copy.find("Campus");
			letterlocation = campusLoc - 2;
			for (int j = 0; done == false; j++) {
				if ((courseInfo2copy[letterlocation - j] == ' ') || (courseInfo2copy[letterlocation - j] == '\t')
					|| (courseInfo2copy[letterlocation - j] == '@')) {
					letterlocation = (letterlocation - j) + 1;
					letterlength = j;
					done = true;
				}
				else if ((courseInfo2copy[letterlocation - j] == 'K') || (courseInfo2copy[letterlocation - j] == 'M')) {
					letterlocation = (letterlocation - j);
					letterlength = j + 1;
					done = true;
				}
			}
			for (int j = 0; j < (7 + letterlength); j++) {
				campusStr += courseInfo2copy[letterlocation + j];
			}
			campus[n] = campusStr;
			n++;
		}
		campusStr = "";
		done = false;
		courseInfo2copy.erase(0, (campusLoc + 6));
	}

	cout << "\n\n" << "Campus: ";
	for (int k = 0; k < classes; k++) {
		cout << "\n" << campus[k];
	}
}
// ===========================================================================================================================
void PartoftermFinder(string platform, string courseInfo, int classes, string partofterm[]) {
	// As of right now all terms will be full term
	cout << "\n\n" << "Part of terms: ";
	for (int k = 0; k < classes; k++) {
		partofterm[k] = "Full term";
		cout << "\n" << partofterm[k];
	}
}
// ===========================================================================================================================
void ProfessorFinder(string platform, string courseInfo2, int classes, string tbaprofessorLoc[], string professor[]) {
	string courseInfo2copy = courseInfo2;      // Makes a copy of the string so we can alter it however we wantm
	int namelocation = 0;				       // Professors name location in the string
	int namelength = 0;						   // How many letters the professors name has
	int atLoc;								   // Where within the string the @ symbol is found
	string professorStr = "";				   // Used to store the professors name 
	int n = 0;					     		   // Used to increment professor[]
	bool done = false;						   // Used for loops

	if (platform == "Registration") {
		for (size_t i = 0; i < courseInfo2copy.size(); i++) {
			if ((courseInfo2copy[i] == '-')) {
				if ((isalpha(courseInfo2copy[i - 2])) && (isalpha(courseInfo2copy[i + 2]))) {
					namelocation = i - 2;
					for (int j = 0; done == false; j++) {
						if ((courseInfo2copy[namelocation - j] == ' ') || (courseInfo2copy[namelocation - j] == '\t')) {
							namelocation = (namelocation - j) + 1;
							namelength = j;
							done = true;
						}
					}
					done = false;
					for (int j = 0; done == false; j++) {
						if (isdigit(courseInfo2copy[namelocation + j])) {
							namelength = j - 1;
							done = true;
						}
					}
					for (int j = 0; j < namelength; j++) {
						professorStr += courseInfo2copy[namelocation + j];
					}
					if (tbaprofessorLoc[n] == "TBA") {
						professor[n] = "TBA";
						n++;
					}
					professor[n] = professorStr;
					done = false;
					professorStr = "";
					n++;
					courseInfo2copy.erase(0, (i + 4));
				}
			}
		}
	}
	else if (platform == "Degree Works") {
		int namelocation;

		for (int i = 0; i < classes + 1; i++) {
			if (courseInfo2copy.find("@") != string::npos) {
				atLoc = courseInfo2copy.find("@");
				namelocation = atLoc - 2;
			}
			else {
				namelocation = courseInfo2copy.size() - 1;
			}
			if (i != 0) {
				for (int j = 0; done == false; j++) {
					if ((courseInfo2copy[namelocation - j] == ' ') || (courseInfo2copy[namelocation - j] == '\t')) {
						if (isdigit(courseInfo2copy[namelocation - j - 1]) || (courseInfo2copy[namelocation - j - 1] == ' ')
							|| (courseInfo2copy[namelocation - j - 1] == '\t')) {
							namelocation = (namelocation - j) + 1;
							namelength = j;
							done = true;
						}
						else if ((courseInfo2copy[namelocation - j - 3] == 'T') && (courseInfo2copy[namelocation - j - 2] == 'B')
							&& (courseInfo2copy[namelocation - j - 1] == 'A')) {
							namelocation = (namelocation - j) + 1;
							namelength = j;
							done = true;
						}
					}
				}
				for (int j = 0; j < namelength; j++) {
					professorStr += courseInfo2copy[namelocation + j];
				}
				if (tbaprofessorLoc[n] == "TBA") {
					professor[n] = "TBA";
					n++;
				}
				professor[n] = professorStr;
				done = false;
				professorStr = "";
				n++;
			}
			courseInfo2copy.erase(0, (atLoc + 1));
		}
	}

	cout << "\n\n" << "Professors: ";
	for (int k = 0; k < classes; k++) {
		cout << "\n" << professor[k];
	}
}
// ===========================================================================================================================
void ScheduleBuilder(int classes, string professor[], string days[], int timeInt[], string campus[], double overallQuality[], double difficulty[], 
	string priority[], string dayPreference, string timePreference, string campusPreference) {

	/* Variables: 
	   overallQuality = 40 points      	 multiplied by overallQuality
	   priority 1 = 30 points			 multiplied by overallQuality
	   priority 2 = 20 points			 multiplied by overallQuality
	   priority 3 = 10 points			 multiplied by overallQuality

	   SectionScore = overallQuality + priority1 + priority2 + priority3;
	*/
	double quality[250];
	double priority1[250];
	double priority2[250];
	double priority3[250];

	double sectionScore[250];

	for (int i = 0; i < classes; i++) {
		quality[i] = (overallQuality[i] * .40);

		for (int j = 0; j < classes; j++) {

			if (priority[j] == "Day") {
				if (dayPreference == days[i]) {
					priority1[i] = (overallQuality[i] * .30);
				}
			}
			else if (priority[j] == "Time") {
				if (timePreference == "") {
					priority2[i] = (overallQuality[i] * .20);
				}
			}
			else if (priority[j] == "Campus") {
				if (campusPreference == campus[i]) {
					priority3[i] = (overallQuality[i] * .10);
				}
			}

		}
		
		priority1[i] = (overallQuality[i] * .30);
		priority2[i] = (overallQuality[i] * .20);
		priority3[i] = (overallQuality[i] * .10);

		double qualityholder = quality[i];
		double priority1holder = priority1[i];
		double priority2holder = priority2[i];
		double priority3holder = priority3[i];

		sectionScore[i] = qualityholder + priority1holder + priority2holder + priority3holder;
	}

	// sort(overallQuality, overallQuality + classes);

	/**/

	cout << "\n\nSection Stats" << endl;
	for (int i = 0; i < classes; ++i) {
		cout << "\nSection " << i << " Stats: "
			<< "  Quality Score: " << quality[i]
			<< "  Priority1 Score: " << priority1[i]
			<< "  Priority2 Score: " << priority2[i]
			<< "  Priority3 Score: " << priority3[i]
			<< "  Section Score: " << sectionScore[i];
	}
}
// ===========================================================================================================================
// ===========================================================================================================================
int main()
{
	// *************************************  Course Schedule Needed Text
	// Holds all the different information for one class in a semester
	// Ex. crn[0] = 13937 & crn[1] = 13938 (every class section has a different Crn)
	// English Composition 2 has about 205 different class sections this semester
	string status[250];          // Ex. WR(web registered) or WL(waitlisted)
	string waitlist[250];        // Ex. Number of people waitlisted before you
	string crn[250];             // Ex. Unique 5 digit code for each class section (Section 01 = 13937) 
	string course[250];          // Ex. Unique 4 digit code for each course (Artificial Intelligence = CS 4242)
	string hoursChar[250];       // Ex. Number of credit hours a course has (Calculus = 4 credit hours)
	string meeting[250];         // Ex. Lecuture or Lab
	string date[250];            // Ex. When the class ends and starts (Spring = "01/07 - 05/06") 
	string days[250];            // Ex. Days when the class meets (Monday & Wednesday = MW)
	string timeChar[250];        // Ex. Time when the class meets (2:00 pm - 3:15 pm)
	string building[250];        // Ex. Building where the class meets (Atrium Building)
	string room[250];            // Ex. Room where the class meets (Room 152)

	int hoursInt[250];           // Ex. Int form of credit hours (to ensure hours don't exeed 18)
	int timeInt[250];            // Ex. Int form of time (to ensure times don't overlap)

	// Course Detail Information Parts
	string coursename[250];      // Ex. What the course is called (Discrete Mathmatics)
	string campus[250];          // Ex. Which campus it is held on (Marrieta Campus or Kennesaw Campus)
	string partofterm[250];      // Ex. Idk what this means, I just put "Full Term" for every one
	string professor[250];       // Ex. Professor that is teaching the class (Jose Garrido)

	// TBA class exception variables
	int classes = 0;               // Number of class section there are (CS 4242 has 3 sections)
	int tbaprofessors = 0;         // Number of teacher that are currently not decided
	int tbaclasses = 0;            // Number of classes that are online
	string tbaprofessorLoc[250];   // Array locations of where the TBA professors are 
	string tbaclassLoc[250];       // Array locations of where the online classes are

	double overallQuality[250];       // Overall Quality of the professor
	double difficulty[250];           // Level of Difficulty of the professor

	string priority[3];
	string dayPreference;
	string timePreference;
	string campusPreference;

	// =======================================================  Example text coming from the registration page on owl express
	string registration = "CS-Computer Science"
		"Select	CRN	Subj	Crse	Sec	Cmp	Cred	Title	Days	Time	Cap	Act	Rem	WL Cap	WL Act	WL Rem	Instructor	Date(MM / DD)	Location"
		"C	13938	CS	4242	01	Marietta Campus	3.000	Artificial Intelligence	TR	05:00 pm - 06 : 15 pm	40	40	0	999	2	997	Chih - Cheng Hung(P)	01 / 07 - 05 / 06	Atrium Building 152"
		"13939	CS	4242	02	Marietta Campus	3.000	Artificial Intelligence	MW	06 : 30 pm - 07 : 45 pm	40	27	13	999	0	999	TBA	01 / 07 - 05 / 06	Atrium Building 152"
		"C	13943	CS	4242	W01	KSU Online	3.000	Artificial Intelligence	 	TBA	40	40	0	999	11	988	Chih - Cheng Hung(P)	01 / 07 - 05 / 06	TBA";

	// =======================================================  Example text coming from degree works
	string degreeworks = "CS 4242	3 Credits	Artificial Intelligence	Print"
		"Prerequisites : (CS 3305  minimum grade of C   and CS 3305L  minimum grade of C) or CS 3304  minimum grade of C"
		"Sections : Term	Campus	Crn	Section	Seats Open	Meeting Times	Bldg / Room	Instructor"
		"Spring 2019	Marietta Campus M	13938	01	0 (out of 40)"
		"Tu 		Th"
		"5 : 00p.m. - 6 : 15p.m.J / 152	C Hung"
		//------------------------------------------------------------------------ New class section info starts here
		"Marietta Campus M	13939	02	14 (out of 40)"
		"M 		W"
		"6 : 30p.m. - 7 : 45p.m.J / 152	S Lee"
		//------------------------------------------------------------------------ New class section info starts here
		"KSU Online O	13943	W01	0 (out of 40)"
		"TBA		C Hung";

	// ========================================================  Example text coming from schedule search on owl express
	string schedulesearch = "Course Section WaitlistBlank spaceCS 4242/01 - Artificial Intelligence"
		"CRN	Credit Hrs	Part / Term	Capacity	Enrolled	Seats Available	Waitlist Capacity	Waitlist Count	Waitlist Availability"
		"13938	3.000	Full Term	40	 	 	999	2	997"
		"Campus	Instructional Method	Where	Days	Time	Start Date	End Date	Instructors"
		"Marietta Campus	Classroom - 100 % Atrium Building"
		"Room 152"
		"U	M	T	W	R	F	S"
		"X	 	X"
		"5:00 pm - 6 : 15 pm"
		"Lecture	Jan 07, 2019	May 06, 2019	Chih - Cheng Hung(P)E - mail"

		"Course Section OpenBlank spaceCS 4242 / 02 - Artificial Intelligence"
		"CRN	Credit Hrs	Part / Term	Capacity	Enrolled	Seats Available	Waitlist Capacity	Waitlist Count	Waitlist Availability"
		"13939	3.000	Full Term	40	27	13	999	0	999"
		"Campus	Instructional Method	Where	Days	Time	Start Date	End Date	Instructors"
		"Marietta Campus	Classroom - 100 % Atrium Building"
		"Room 152"
		"U	M	T	W	R	F	S"
		"X	 	X"
		"6:30 pm - 7 : 45 pm"
		"Lecture	Jan 07, 2019	May 06, 2019	TBA"

		"Course Section WaitlistBlank spaceCS 4242 / W01 - Artificial Intelligence"
		"CRN	Credit Hrs	Part / Term	Capacity	Enrolled	Seats Available	Waitlist Capacity	Waitlist Count	Waitlist Availability"
		"13943	3.000	Full Term	40	 	 	999	11	988"
		"Campus	Instructional Method	Where	Days	Time	Start Date	End Date	Instructors"
		"KSU Online	Online - 100 % Online	D2L Brightspace	TBA	TBA	Jan 07, 2019	May 06, 2019	Chih - Cheng Hung(P)";

	// ========================================================================  Class text information 
	string platform;                    // Holds where the string is coming form (Ex. Platform = "Degree Works")
	string courseInfo = registration;    // Holds all the text info of it's appropriate platform and not just the name like platform
	string courseInfo2 = courseInfo;    // Makes of copy of that string so we can later alter it
										// CourseInfo2 will be the one where we delete irrelevant information, create lines for
										// each seperate class section and put an @ symbol in the begging of each line

	// ========================================================================  Calls all methods to gather information
	PlatformFinder(platform, courseInfo);
	NewLineCreator(platform, courseInfo2, classes);

	CrnFinder(platform, courseInfo, classes, crn);

	TBAProfessorFinder(platform, courseInfo2, classes, tbaprofessors, tbaprofessorLoc);
	TBATimeFinder(platform, courseInfo2, classes, tbaclasses, tbaclassLoc);

	StatusFinder(platform, courseInfo, classes, status);                                     // Needs work
	WaitlistFinder(platform, courseInfo, classes, waitlist);                                 // Needs work
	CrnFinder(platform, courseInfo, classes, crn);                                           // Full proof
	CourseFinder(platform, courseInfo, classes, course);                                     // Needs work
	HourFinder(platform, courseInfo, classes, hoursChar, hoursInt);                          // Needs work (PRIORITY)
	MeetingFinder(platform, courseInfo, classes, meeting, hoursInt);                         // Full proof, if hours work
	DateFinder(platform, courseInfo2, classes, date);                                        // Full proof, but should check
	DayFinder(platform, courseInfo2, classes, days);                                         // Full proof
	TimeFinder(platform, courseInfo2, classes, tbaclasses, tbaclassLoc, timeChar, timeInt);  // Full proff but still should look at
	BuildingFinder(platform, courseInfo2, classes, tbaclassLoc, building);                   //
	RoomFinder(platform, courseInfo2, classes, tbaclassLoc, room);                           //

	CourseNameFinder(platform, courseInfo, classes, coursename);                             // 
	CampusFinder(platform, courseInfo2, classes, tbaclasses, tbaclassLoc, campus);           // 
	PartoftermFinder(platform, courseInfo, classes, partofterm);                             //
	ProfessorFinder(platform, courseInfo2, classes, tbaprofessorLoc, professor);             //

	// ==========================================  Prints out all rellevant information with their respected section
	// ==========================================  Purely for testing purposes

	cout << "\n\n         Status | CRN | Course Detail Information | " <<
		"Hours | Meeting | Date(MM/DD) | Days |     Time      | Location ";
	cout << "\n";
	for (int i = 0; i < classes; i++) {
		cout << "\nSection " << i << ": " <<
			status[i] << "  " <<
			/* waitlist[i] << */ "  " <<
			crn[i] << "  " <<
			course[i] << "     " <<
			hoursChar[i] << "   " <<
			meeting[i] << "  " <<
			date[i] << "   " <<
			days[i] << "    " <<
			timeChar[i] << "   " <<
			building[i] << "  " <<
			room[i] << "  ";

		cout << "\n" <<
			"			" << coursename[i] << "\n" <<
			"			Campus: " << campus[i] << "\n" <<
			"			Term: " << partofterm[i] << "\n" <<
			"			Instructor: " << professor[i] << "\n";
	}

	Priority(priority, dayPreference, timePreference, campusPreference);
	ProfessorRating(professor, overallQuality, difficulty, classes);

	ScheduleBuilder(classes, professor, days, timeInt, campus, overallQuality, difficulty,
		priority, dayPreference, timePreference, campusPreference);

}
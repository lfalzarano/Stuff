#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <cctype>

#include <sstream>
#include <algorithm>

using std::vector;
using std::string;
using std::map;
using std::cout;
using std::cin;
using std::endl;

using std::stringstream;

//This function checks if a given string is valid phone number
//Inputs: string
//Outputs: boolean
//Phone numbers are sequences of digit (0–9) and hyphen ("-") characters, and must start and end with a digit.
bool is_valid_phone_number(string &phone_number) {
  if(phone_number == ""){ return true; } //used for checking data for directive "R"
  if(  !(isdigit(phone_number[0]))    ) {return false;}
  if(  !(isdigit(phone_number[phone_number.length()-1]))    ) {return false;}
  for(unsigned i=0; i<phone_number.length(); i++) {
    if(phone_number[i] != '-' && !(isdigit(phone_number[i])) ) {
      return false;
    }
  }
  return true;
}

int main() {
  // TODO: implement
  //This struct is passed as the the third argument when the phone_db map is created
  //This struct ensures that comparisons between keys of the map are done case insensitive
  struct compare_case_insensitive {
    bool operator()(const vector<string> &left, const vector<string> &right) const {
      string left_first = left[0];
      string left_second = left[1];
      string right_first = right[0];
      string right_second = right[1];
      //Make the strings upper case for comparison
      transform(left_first.begin(), left_first.end(), left_first.begin(), ::toupper);
      transform(right_first.begin(), right_first.end(), right_first.begin(), ::toupper);
      //compare first string
      if(left_first < right_first) {
        return true;
      }
      //if the two first strings are equal then we must check the second strings
      else if(left_first == right_first) {
        transform(left_second.begin(), left_second.end(), left_second.begin(), ::toupper);
        transform(right_second.begin(), right_second.end(), right_second.begin(), ::toupper);
        if (left_second < right_second) {
          return true;
        }
        else {
          return false;
        } 
      }
      else {
        return false;
      }
    }
  };

  //line and ss for reading input
  string line;
  stringstream ss;

  //variables for storing strings from the user input
  string first_name;
  string second_name;
  string number_type;
  string phone_number;

  //Creating containers for the phone_number data base
  vector<string> Name;
  map<string, string> PhoneNumberCollection;
  map< vector<string>, map<string, string>,  compare_case_insensitive> phone_db;

  //Iterators for finding data a iterating through data
  map< vector<string>, map<string, string> >::iterator itd;
  map< vector<string>, map<string, string> >::iterator it;

  cout << "Info: Welcome to the Phone Database!" << endl;
  
  //Continuously read input until user quits the program
  while(getline(cin, line)) {
    string directive;
    stringstream ss(line);

    //Read in the directive to determine what action the user wants
    ss >> directive;

    //Create a contact with the first_name and second_name that the user specifies
    if( directive == "C") {
      ss >> first_name;
      ss >> second_name;
      Name.push_back(first_name);
      Name.push_back(second_name);
      //Check that the contact doesn't exist
      itd = phone_db.find(Name);
      if (itd != phone_db.end()) {
        cout << "Error: Contact already exists" << endl;
      }
      else {
        //Make all phone numbers for the contact intially empty
        phone_db[Name] = {  {"HOME", ""}, {"CELL", ""}, {"WORK", ""}, {"FAX", ""}, {"VOIP", ""}  };
        cout << "Result: Contact created" << endl;
      }
      Name.clear();
    }

    //Delete a contact with the first_name and second_name that the user specifies
    else if( directive == "D") {
      ss >> first_name;
      ss >> second_name;
      Name.push_back(first_name);
      Name.push_back(second_name);
      itd = phone_db.find(Name);
      //Find the contact and delete it
      if (itd != phone_db.end()) {
        phone_db.erase(itd);
        cout << "Result: Contact deleted" << endl;
      }
      //If contact doesn't exist then print an error message
      else {
        cout << "Error: Contact not found" << endl;
      }
      Name.clear();
    }

    //Loop through the map elements and print out the contact names
    else if( directive == "L") {
      for(it = phone_db.begin(); it != phone_db.end(); ++it) {
        cout << "Result: " << it->first[0] << "," << it->first[1] << endl;
      }
      //Once loop finishes, print an info message with the number of elements
      cout << "Info: There are " << phone_db.size() << " contact(s)" << endl; 
    }

    else if( directive == "P") {
      ss >> first_name;
      ss >> second_name;
      Name.push_back(first_name);
      Name.push_back(second_name);
      itd = phone_db.find(Name);
      if(itd == phone_db.end()) {
        cout << "Error: Contact not found" << endl;
      }
      else { //might want to add info message for having no phone numbers
        for(map<string, string>::iterator it = (itd->second).begin(); it != (itd->second).end(); it++) {
          if (it->second != "") {
            cout << "Result: " << it->first << "," << it->second << endl;
          }
        }
      }
      Name.clear();
    }

    else if( directive == "N") {
      ss >> first_name;
      ss >> second_name;
      Name.push_back(first_name);
      Name.push_back(second_name);
      
      ss >> number_type;
      ss >> phone_number;
      
      itd = phone_db.find(Name);

      //error checking
      if(itd == phone_db.end()) {
        cout << "Error: Contact not found" << endl;
      }
      else if(number_type != "CELL" && number_type != "HOME" && number_type != "WORK" && number_type != "FAX" && number_type != "VOIP") {
        cout << "Error: Invalid phone number type" << endl;
      }
      else if(!(is_valid_phone_number(phone_number))) { //go back and think about extra cases for invalid phone numbers
        cout << "Error: Not a valid phone number" << endl;
      }

      //if everyhting was good then we can add the phone numbers
      else if ((itd->second)[number_type] == "") {
        (itd->second)[number_type] = phone_number;
        cout << "Result: Phone number added" << endl;
      }
      else {
        (itd->second)[number_type] = phone_number;
        cout << "Result: Phone number replaced" << endl;
      }
      Name.clear();
    }

    else if( directive == "X") {
      ss >> first_name;
      ss >> second_name;
      Name.push_back(first_name);
      Name.push_back(second_name);
      
      ss >> number_type;
      
      map< vector<string>, map<string, string> >::iterator itd; //might only need to define this once at the beginning
      itd = phone_db.find(Name);

      //error checking
      if(itd == phone_db.end()) {
        cout << "Error: Contact not found" << endl;
      }
      else if(itd->second[number_type] == "") {
        cout << "Error: No phone number of that type" << endl;
      }
      else {
        itd->second[number_type] = "";
        cout << "Result: Phone number deleted" << endl;
      }
      Name.clear();
    }

    else if( directive == "S") {
      string ofile_name;
      ss >> ofile_name;
      std::ofstream ofile(ofile_name);
      if(!(ofile.is_open())) {
        cout << "Error: Could not open output file" << endl;
      }
      else {
        for(map< vector<string>, map<string, string> >::iterator it = phone_db.begin(); it != phone_db.end(); ++it) {
          ofile << it->first[0] << " " << it->first[1] << " ";
          for(map<string, string>::iterator it2 = (it->second).begin(); it2 != (it->second).end(); it2++) {
            ofile << it2->first << " "; //might want to change to exclude extra space
            if(it2->second == "") {
              ofile << "NULL ";
            }
            else {
              ofile << it2->second << " ";
            }
          }
          ofile << endl;
        }
        cout << "Result: Database saved" << endl;
      }
    }

    else if( directive == "R") {
      bool invalid_phone_number = false;
      bool invalid_phone_number_type = false;

      string ifile_name;
      ss >> ifile_name;
      std::ifstream ifile(ifile_name);
      if(!(ifile.is_open())) {
        cout << "Error: Could not open input file" << endl;
      }
      phone_db.clear();
      while(getline(ifile, line)) {
        
        stringstream ss(line);
        ss >> first_name; // do error checking
        ss >> second_name;
        Name.push_back(first_name);
        Name.push_back(second_name);
        
        //temp1 and temp2 are used to copy phone numbers and phone number types into the database from the file
        string temp1;
        string temp2;
        
        for(int i = 0; i<5; i++) {
          ss >> temp1;
          ss >> temp2;
          if(temp2 == "NULL") {temp2 = "";}
          if(!(is_valid_phone_number(temp2))) {
            invalid_phone_number = true;
            break;
          }
          else if(temp1 != "CELL" && temp1 != "HOME" && temp1 != "WORK" && temp1 != "FAX" && temp1 != "VOIP") {
            invalid_phone_number_type = true;
            break;
          }
          phone_db[Name][temp1] = temp2;
        }
        if(invalid_phone_number == true) {
          cout << "Error: Invalid database file" << endl;
          break; //breaks out of while loop
        }
        else if(invalid_phone_number_type == true) {
          cout << "Error: Invalid database file" << endl;
          break; 
        }
        Name.clear();
      }
      if(invalid_phone_number == false && invalid_phone_number_type == false) {
        cout << "Result: Database restored" << endl;
      }
    }

    //need an else invalid directive

    else if( directive == "Q") {
      exit(0);
    }

    else {
      cout << "Error: Invalid directive" << endl;
    }
    cout << "Info: Please enter a command" << endl;
  }
  return 0;
}

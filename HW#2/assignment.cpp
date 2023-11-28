#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <ctime>

using namespace std;

struct flight{
  string from;
  string to;
  int hour;
  int min;
  int price;
  int ID;
  
  flight *next;
  flight *prev;
  
  //TO DO: implement constructor
  //Hint: You may need to implement multiple constructors
};

struct airline{
  string name;
  int ID;
  airline *next;
  flight *flights;

  //TO DO: implement constructor
  //Hint: You may need to implement multiple constructors
};


pair<vector<string>, vector<vector<flight>>> read_files(bool input_done){
  
  ///READ FLIGHTS///
  vector<string> airlines;
  vector<vector<flight>> flights(100); //start with a previously sized vector or you get seg fault (for sure)
  
  while(not input_done){
    string airline_name;
    string filename;
    cout << "Enter the name of airline: ";
    cin >> airline_name;
    cout << "Enter the name of airline file: ";
    cin >> filename;
    ifstream input(filename);

    if(filename == "done"){
      input_done = true;
      return make_pair(airlines, flights);
    }
    
    while( input.fail() ){
      cout << "Could not open file: " << filename << endl;
      cout << "Enter the name of airline: ";
      cin >> airline_name;
      cout << "Enter the name of airline file: ";
      cin >> filename;
      input.open(filename);

      if(filename == "done"){
      input_done = true;
      return make_pair(airlines, flights);
      }
    }
    
    airlines.push_back(airline_name);
    
   
    cout << "Processing input file: " << filename <<endl;
    
    int line_no = 0;
    vector<string> lines;
    string line;
    int vector_location = airlines.size() - 1;
    while(getline(input, line)){
      lines.push_back(line);
      line_no += 1;
      if(line_no % 5 == 0){
	flight a_flight(lines[line_no-5], lines[line_no-4], stoi(lines[line_no-3]), stoi(lines[line_no-2]), stoi(lines[line_no-1]));
	flights[vector_location].push_back(a_flight);
      }
    }
  }
  if(input_done){
    cout << "Input files are already read, however you can do manual changes.." << endl;
  }

  return make_pair(airlines, flights);
}



airline* make_linked_list_structure(vector<string> airlines, vector<vector<flight>> flights){
  //TO DO: Implement
}


int pathfinder(airline* head){

  //TO DO: Implement
  //Hint: A recursive search seems like the best solution.
  //Hint: You don't have to use doubly linked list features
  
}


void delete_linked_list(airline* &head){

  //TO DO: Implement
}



//
void printMainMenu() {
  cout << endl;
  cout <<"I***********************************************I"<<endl
       <<"I               0 - DELETE DATA                 I"<<endl
       <<"I               1 - READ FILES                  I"<<endl
       <<"I               2 - PRINT ALL FLIGHTS           I"<<endl
       <<"I               3 - ADD FLIGHT                  I"<<endl
       <<"I               4 - REMOVE FLIGHT               I"<<endl
       <<"I               5 - PATH FINDER                 I"<<endl
       <<"I               6 - EXIT                        I"<<endl
       <<"I***********************************************I"<<endl
       <<">>";
  cout << endl;
}  

void processMainMenu() {

  pair<vector<string>, vector<vector<flight>>> lines_flights;
  airline* head;
  
  char input;
  do{
    printMainMenu();
    cout << "Please enter your option " << endl;
    cin >> input;
    switch (input) {
    case '0':
      cout << "Commented out functionalities are going to be implemented" << endl;
      //delete_linked_list(head);
      //cout << "Data is deleted!" << endl;
      //input_done = false;
      break;
    case '1':
      if(not input_done){
	lines_flights = read_files(input_done);
	head = make_linked_list_structure(lines_flights.first, lines_flights.second);
	cout << "Files are read.." << endl;
      }
      else{
	cout << "Files are already read.." << endl;
      }
      input_done = true;
      break;
    case '2':
      cout << "Commented out functionalities are going to be implemented" << endl;
      //print_all(head);
      break;
    case '3':
      cout << "Commented out functionalities are going to be implemented" << endl;
      //add_flight_with_input(head);
      break;
    case '4':
      cout << "Commented out functionalities are going to be implemented" << endl;
      //remove_flight_with_input(head);
      break;
    case '5':
      cout << "Commented out functionalities are going to be implemented" << endl;
      //pathfinder(head);
      break;
    case '6':
      cout << "Exiting.." << endl;
      exit(0);
    default:
      cout << "Invalid option: please enter again" << endl;
    }
  } while(true);
}
//


int main(){
  
  processMainMenu();
  //Comply with the provided structs and specified linked list structure for a seamless grading
  
  return 0;
  
}



  

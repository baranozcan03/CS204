//CS204 Fall 2023 - HW3: Stacks & Queues and SU services
//Baran Özcan - 30960


//include neccessary libraries
#include <iostream>
#include <fstream>       // for ifstream
#include <cctype>        
#include <string>
#include <vector>
#include <sstream>      // std::stringstream
using namespace std;


//a struct for creating a linedlist for our user tab keeping.
struct Recipt {
	string name;
	int ID;
	int price;
	Recipt* next = nullptr;
};

//a struct for the commands in the each function.
struct Commands {
	string CommandName = "empty";
	Commands* NextCommad = nullptr;
};

//a struct for the each function defined by our input files.
struct Functions {
	string FunctionName = "empty";
	Functions* NextFunction = nullptr;
	Commands* CommandsHead = nullptr;
};

//A struct that we will add in the queues, inclued everything that a request has.
struct Request {
	string RequesterName = "";
	string RequesterJob = "";
	string RequestedFunc = "";
	int RequesterID = 0;

};

//nodes for our queues, they include the request as the value, and next pointer for travelling in the queue.
struct RequestQueueNode {
	Request value;
	RequestQueueNode* next;

	//a constructor that takes a request as a parameter
	RequestQueueNode(Request newRequest, RequestQueueNode* pointer = nullptr) {
		value = newRequest;
		next = pointer;
	}
	//defult constructor
	RequestQueueNode() {}

};

//now we will create a class that will act  as queue, will be used for both queues
class RequestQueue {

	//we just need to keep track of front to dequeue and rear to enqueue
private:
	RequestQueueNode* Front;
	RequestQueueNode* Rear;
	//definition of the functions, implementation will explain their use 
public:
	RequestQueue();
	void enqueue(Request);
	void dequeue(Request&);
	bool isEmpty() const;
	void clear();
	int length();
};

//a struct fo our command stack that will be used by the functions(services).
struct CommandNode
{
	string CommandName;
	CommandNode* next;
};

//class of the said command stack (recursively called for each function(service))
class SharedCommandStack {

	//since we operate in LIFO manner we only need the top
private:
	CommandNode* top;

	//definitions of the class functions, implementatiton will elaborate on their use
public:
	SharedCommandStack(void);
	void push(string);
	void pop(string&);
	bool isEmpty(void);
	void printStack(SharedCommandStack&);
	void ClearAFunc(string);
};


//function prototypes in order not to mess up the call order. 
//each functions implementation will have the explanations of these functions
void Read_Files();
void Print_Services();
void addInstructorWorkload();
void addStudentWorkload();
void processWorkload();
void processWorkload(string, int&, SharedCommandStack&);
void displayUsers();
void DeleteAll();



int InstructorCounter = 0; //keep counts of the amount of the instructor that we processed, be aware of 10
Recipt* ReciptHead; //This is the linkedlist head of the price list where we keep the tabs.
Functions* FunctionsHead; //head of the functions linked list. Every function is also the head of their command linked list.
RequestQueue InstructorQueue; //Queue for instructor requests.
RequestQueue StudentQueue;//Queue for student requests. 

//precondition= nothing
//postcondition= the program is executed, if successfully return 0
//function= will implement a program that takes requests of services and processes them
int main()
{
	//we get the  an input form the user to understand whether they wnat to give an input file or not 
	string add_file;
	cout << "If you want to open a service (function) defining file," << endl;
	cout << "then press (Y/y) for 'yes', otherwise press any single key" << endl;
	cin >> add_file;

	//we check the answer and if they wanted it we read an input file form them
	//while loop to keep asking more
	while (add_file == "y" || add_file == "Y") {

		//After reading the function input we ask if they want to add more  function(service) inputs.
		Read_Files();
		cout << "Do you want to open another service defining file?" << endl;
		cout << "Press (Y/y) for 'yes', otherwise press anykey" << endl;
		cin >> add_file;
	}

	//we print out the functions and thier comments that we ahve read.
	Print_Services();

	//now we get in to the menu loop to ask for instructions.
	while (true) {
		//give the menu and ask for next step.
		cout << endl;
		cout << "**********************************************************************" << endl
			<< "**************** 0 - EXIT PROGRAM                        *************" << endl
			<< "**************** 1 - ADD AN INSTRUCTOR SERVICE REQUEST   *************" << endl
			<< "**************** 2 - ADD A STUDENT SERVICE REQUEST       *************" << endl
			<< "**************** 3 - SERVE (PROCESS) A REQUEST           *************" << endl
			<< "**************** 4 - DISPLAY USER PAYMENTS               *************" << endl
			<< "**********************************************************************" << endl;
		cout << endl;
		int option;
		cout << "Pick an option from above (int number from 0 to 3): ";
		cin >> option;
		switch (option)
		{
		case 0: //get out of the program
			cout << "PROGRAM EXITING ... " << endl;
			DeleteAll();
			exit(0);

		case 1: //will add a Instructor request to the queue
			addInstructorWorkload();
			break;
		case 2://will add a Student request to the queue
			addStudentWorkload();
			break;
		case 3://Will look at the queues and process a request. Instructors will be priorities untill 10 of them processed.
			processWorkload();
			break;
		case 4://will display the tab of the users, their ID their Name and the amount of money they owe to the SU.
			displayUsers();
			break;
		default:
			cout << "INVALID OPTION!!! Try again" << endl; //we dont want any other input
		}
	}
	return 0;
}


//precondition=none, Postcondition=a new Queue
//function= Request queue defult constructor, Fron and rear initialised as nullptr. 
RequestQueue::RequestQueue() {
	Front = nullptr;
	Rear = nullptr;
}

//precondition=the reques obj that we wnat to enqueue, Postcondition=the queue is updated accordingly 
//function= Adds the given queue object to the rear of the queue
void RequestQueue::enqueue(Request newRequest) {

	//if the queue is empty, we add the obj as the only obj and it becomes both rear and front
	if (isEmpty()) {
		Front = new RequestQueueNode(newRequest);
		Rear = Front;
	}
	//added to the rear.
	else {
		Rear->next = new RequestQueueNode(newRequest);
		Rear = Rear->next;
	}
}

//precondititon= the obj that you want to be the become the values that will be dequed
//postcondition= nothing but the input parameter has changed the the vlaue that we dequed
//function= removes the element at the front and gives it with the referance parameter 
void RequestQueue::dequeue(Request& output) {
	RequestQueueNode* temp;

	//we wont be dequeing an elemtn from an empty list.
	if (isEmpty()) {
		cout << "Attempting to dequeue on empty queue, exiting program...\n";
		exit(1);
	}
	//update the output as the value of the thing that we will get rid of,
	//fix the rest accordingly
	else {
		output = Front->value;
		temp = Front;
		Front = Front->next;
		delete temp;
	}


}

//precondition=nothing but we have member variables .
//postcondition=a bool value is returned, true if empty flase if not empty.
//function= check wheter a queue is empty or not and returns the proper bool value.
bool RequestQueue::isEmpty() const {
	if (Front == nullptr) {
		return true;
	}
	else {
		return false;
	}
}

//precondititon= nothing but we reach the memeber variables of the queue.
//postcondtition= a nullptr as the head of a queue, allocated hep deleted
//function= clears the queue and every memeory location allocated for it.
void RequestQueue::clear()
{
	//deque everything untill the queue becomes emoty.
	Request value;
	while (!isEmpty()) {
		dequeue(value);
	}
}

//precondition= nothing but we reach the memeber variables of the queue.
//postcondtiton= int that represents the lenght of the queue
//fucntion= calculates the amount of nodes in a queue
int RequestQueue::length() {
	//start with 0
	int Length = 0;
	RequestQueueNode* temp;
	temp = Front;//start at front
	while (temp) {//add untill you reach end
		Length++;
		temp = temp->next;
	}
	return Length;
}

//precondtition=nothing, postcondition=a stack is constructed
//function= creates an empty stack with top as the nullptr.
SharedCommandStack::SharedCommandStack()
{
	top = nullptr;
}

//preconditon=the obj that we want to push in to our stack
//postcondtition= the object is added to the stack, top ndoe changes
//function= adding the desired obj on top of the stack
void SharedCommandStack::push(string commandName)
{
	//allocate the memory for the new obj
	CommandNode* newNode;
	newNode = new CommandNode;
	newNode->CommandName = commandName;

	//if the stack is empty 
	if (isEmpty()) {
		top = newNode;
		newNode->next = nullptr;
	}
	//if the stack is not empty
	else {
		newNode->next = top;
		top = newNode;
	}
}

//precondittion= the referance for the obj that we want to cahnge,
//postcondition= nothing but the top cahnges
//function= pops the top of the stack, arranges a new top
void SharedCommandStack::pop(string& CommandName)
{
	//not mess with the original top
	CommandNode* temp;

	//if it is empty
	if (isEmpty()) {
		cout << "The stack is empty.\n";
	}
	//if not empty
	else {
		CommandName = top->CommandName;//give the values to the referance
		temp = top->next;
		delete top;//deallocate the memory
		top = temp;
	}

	//


}

//precondition= nothing but we acces the memeber variables
//postcondition= a bool, true if stack is empty, false if not
//function=Check whether a stack is empty or not
bool SharedCommandStack::isEmpty(void)
{
	bool status;
	if (top == nullptr)
		status = true;
	else
		status = false;
	return status;
}

//precondition= the comment that we want to print out.
//postcondititon = nothing chagnes
//function= couts the values in the stack in the order that we have added them
void SharedCommandStack::printStack(SharedCommandStack& CommandStack) {

	SharedCommandStack ReverseStack;
	string temp;
	//we need to reverse the stack in order to cout in reverse order
	while (!CommandStack.isEmpty()) {
		CommandStack.pop(temp);
		ReverseStack.push(temp);
	}
	//we need to get back the original stack so pop every element in reverse stack
	//and push it to the new stack
	while (!ReverseStack.isEmpty())
	{

		ReverseStack.pop(temp);
		cout << temp << endl;
		CommandStack.push(temp);
	}

}


//precondtition=the func name that we want to get rid of
//postcondition=The same stack but all values that has Endedfunc in them are removed 
//function= look thourgh the stack and delete the values that has EndedFunc, clear EndedFunc form the stack
void SharedCommandStack::ClearAFunc(string EndedFunc) {


	CommandNode* tempNode;
	tempNode = top; //start form top and move thorugh the stack
	string Dummy; //we will pop the unwanted values to this dummy string

	while (tempNode) {
		if (tempNode->CommandName.substr(0, EndedFunc.length()) == EndedFunc) { //if it includes the endedFunc
			tempNode = tempNode->next;//move through
			pop(Dummy);//we dlete if with pop
		}
		else { tempNode = tempNode->next; }//if it not involves EndedFunc, pass it
	}

}


//precondtition= nothing
//postcondition=nothing but the functions linkedlist is updated according to input files
//function= reads the user given input files and adds the functions in to the linkedlist.
void Read_Files() {

	string file_name;
	cout << "Enter the input file name: "; //get the filname from the user
	cin >> file_name;
	ifstream input;
	input.open(file_name.c_str()); //define the stream and open the file

	if (input.fail()) { //if you cant open the file abort the execution
		cout << "Could not open the file " << file_name;
		DeleteAll();
		exit(0);
	}

	//for safely itterating throgh the linkedkist
	Functions* tempHead = nullptr;
	string line;
	Commands* tempCommand = nullptr;

	//if the function linkedlist is empty
	if (!FunctionsHead) {
		FunctionsHead = new Functions; //create a new function in the heap
		tempHead = FunctionsHead;//and make head show the newly allocated obj
		tempHead->NextFunction = new Functions;//allocate an extra space, gives ease of itteration
	}
	else { //if the linkedlist is not epmty
		tempHead = FunctionsHead;
		while ((tempHead) && (tempHead->NextFunction != nullptr))//go untill the end of the list
		{
			tempHead = tempHead->NextFunction;

		}
		tempHead->NextFunction = new Functions;//allocate an extra space, gives ease of itteration
	}

	//temp head now shows the last node of the function linekd list
	//but keep in mind we have an empty dummy node after temp head.  
	getline(input, line);
	tempHead->FunctionName = (line); //now get the name for the function
	tempHead->CommandsHead = new Commands; //create a space for its first command


	tempCommand = tempHead->CommandsHead;//make the temp command show the first command of the func

	//now get commands line by line and add it to the command likedlsit
	while (getline(input, line))
	{
		tempCommand->CommandName = (line);
		tempCommand->NextCommad = new Commands;
		tempCommand = tempCommand->NextCommad;
	}


	//code block donw below deletes the reduntandt command space allocated in by the last command
	tempCommand = tempHead->CommandsHead;//go to the end
	while (tempCommand->NextCommad->NextCommad) {
		tempCommand = tempCommand->NextCommad;
	}
	delete tempCommand->NextCommad;//delete redundant
	tempCommand->NextCommad = nullptr;

	//modify the command name of the last print stack function, omit the ";", customer request.
	tempCommand=tempHead->CommandsHead;
	Commands *lastPrint=nullptr;
	//find the last occurance of the print stack
	while (tempCommand != nullptr) {
        if (tempCommand->CommandName.substr(0,5) == "print") {
            lastPrint = tempCommand;
        }
        tempCommand = tempCommand->NextCommad;
    }
	//delete its ";"
	lastPrint->CommandName = lastPrint->CommandName.substr(0, lastPrint->CommandName.length() - 1);

}

//precondition=nothing
//postcondition=nothing, reaches the global function linkedlist head
//function= couts every functions together with their 
void Print_Services() {

	cout << "------------------------------------------------------------------" << endl;
	cout << "PRINTING AVAILABLE SERVICES (FUNCTIONS) TO BE CHOSEN FROM THE USERS" << endl;
	cout << "------------------------------------------------------------------" << endl << endl << endl;

	Functions* tempFunc = FunctionsHead;
	if (tempFunc) {//itterate for every funciton
		while (tempFunc->NextFunction->NextFunction) {//go ontill the last second one, remember this is the viwe. second->fisrt>dummy(last)
			cout << tempFunc->FunctionName << endl;//first give out the function.
			Commands* tempCommand = tempFunc->CommandsHead;
			while (tempCommand->NextCommad) {//itterate over the commands
				cout << tempCommand->CommandName << ", ";//give out the commands.
				tempCommand = tempCommand->NextCommad;
			}
			cout << tempCommand->CommandName << "." << endl;//end it with a "." and newline
			cout << endl;
			tempFunc = tempFunc->NextFunction;
		}
		//this par is for the last, nondummy function(service pointer), since per user request i shouldn hvae an endl i put it sperefrom loop
		cout << tempFunc->FunctionName << endl;//first give out the function.
			Commands* tempCommand = tempFunc->CommandsHead;
			while (tempCommand->NextCommad) {//itterate over the commands
				cout << tempCommand->CommandName << ", ";//give out the commands.
				tempCommand = tempCommand->NextCommad;
			}
			cout << tempCommand->CommandName << "." << endl;//end it with a "." and newline

	}
}

//precondition=nothing
//postcondition=nothing, but the queue is updated
//function= add a Instructor request to queue of instructors
void addInstructorWorkload() {

	cout << "Add a service (function) that the instructor wants to use:" << endl;

	string DesiredFunction;
	cin >> DesiredFunction;
	DesiredFunction = DesiredFunction + ":";//first ask for the function

	Functions* TempFuncHead = FunctionsHead;

	while (TempFuncHead) {//itterate thorugh untill you see the function
		if (TempFuncHead->FunctionName == DesiredFunction) {//return tempFuncHead when you find it
			break;
		}
		else {
			TempFuncHead = TempFuncHead->NextFunction;//will return nullptr if cant find it
		}
	}


	if (!TempFuncHead) {//since nullptr when didnt find it, cout a warning, go bakc to the main menu
		cout << "The requested service (function) does not exist." << endl;
		cout << "GOING BACK TO MAIN MENU" << endl;
	}

	else {//if there is such a function
		string InstructorName;
		int InstructorID;

		//take the details of the instructer
		cout << "Give instructor's name: ";
		cin >> InstructorName;
		cout << "Give instructor's ID (an int): ";
		cin >> InstructorID;

		//create a request obj with these data
		Request newRequest;
		newRequest.RequestedFunc = DesiredFunction;
		newRequest.RequesterID = InstructorID;
		newRequest.RequesterJob = "Instructor";
		newRequest.RequesterName = InstructorName;

		//enqueq this reques object to our instructor queue
		InstructorQueue.enqueue(newRequest);

		//notfiy the user
		cout << "Prof. " << InstructorName << "'s service request of " << DesiredFunction.substr(0, DesiredFunction.length() - 1) << endl
			<< "has been put in the instructor's queue." << endl << "Waiting to be served..." << endl;

		//since an instructor is succesfully added, create a recipt for it to keep in tab.
		Recipt* newRecipt;
		newRecipt = new Recipt;
		newRecipt->ID = InstructorID;
		newRecipt->name = InstructorName;
		newRecipt->price = 0;

		Recipt* temp = ReciptHead;

		//With alreadyIN we check whether this unique instructor,ID tupel has appeared in our tab before 
		bool AlreadyIn = false;
		while (temp)
		{
			if (temp->ID == newRequest.RequesterID && temp->name == newRequest.RequesterName) {
				AlreadyIn = true;//if you find it update the ALreadyIN
				break;
			}
			temp = temp->next;//finishes untouched if you cant find it
		}


		temp = ReciptHead;
		if (!AlreadyIn) {//if it is not already in the tab
			if (!temp) {//if the tab is empty
				ReciptHead = newRecipt;//the head shows the new recipt
			}
			else {
				while (temp->next) {//if not empty go to the end
					temp = temp->next;
				}
				temp->next = newRecipt;//simply add to the end
			}
		}
	}


}


//precondition=nothing
//postcondition=nothing, but the queue is updated
//function= add a student request to queue of students
void addStudentWorkload() {

	cout << "Add a service (function) that the student wants to use:" << endl;

	string DesiredFunction;
	cin >> DesiredFunction;
	DesiredFunction = DesiredFunction + ":";//first ask for the function

	Functions* TempFuncHead = FunctionsHead;

	while (TempFuncHead) {//itterate thorugh untill you see the function
		if (TempFuncHead->FunctionName == DesiredFunction) {//return tempFuncHead when you find it
			break;
		}
		else {
			TempFuncHead = TempFuncHead->NextFunction;//will return nullptr if cant find it
		}
	}

	if (!TempFuncHead) {//since nullptr when didnt find it, cout a warning, go bakc to the main menu
		cout << "The requested service (function) does not exsit." << endl;
		cout << "GOING BACK TO MAIN MENU" << endl;
	}

	else {//if there is such a function
		string StudentName;
		int StudentID;

		//take the details of the student
		cout << "Give student's name: ";
		cin >> StudentName;
		cout << "Give student's ID (an int): ";
		cin >> StudentID;

		//create a request obj with these data
		Request newRequest;
		newRequest.RequestedFunc = DesiredFunction;
		newRequest.RequesterID = StudentID;
		newRequest.RequesterJob = "Student";
		newRequest.RequesterName = StudentName;

		//enqueq this reques object to our student queue
		StudentQueue.enqueue(newRequest);

		//notify the user
		cout << StudentName << "'s service request of " << DesiredFunction.substr(0, DesiredFunction.length() - 1) <<
			" has been put in the student's queue." << endl << "Waiting to be served..." << endl;

		//since an student is succesfully added, create a recipt for it to keep in tab.
		Recipt* newRecipt;
		newRecipt = new Recipt;
		newRecipt->ID = StudentID;
		newRecipt->name = StudentName;
		newRecipt->price = 0;


		Recipt* temp = ReciptHead;

		//With alreadyIN we check whether this unique student,ID tupel has appeared in our tab before
		bool AlreadyIn = false;
		while (temp)
		{
			if (temp->ID == newRequest.RequesterID && temp->name == newRequest.RequesterName) {
				AlreadyIn = true;//if you find it update the ALreadyIN
				break;
			}
			temp = temp->next;//finishes untouched if you cant find it
		}




		temp = ReciptHead;
		if (!AlreadyIn) {//if it is not already in the tab
			if (!temp) {//if the tab is empty
				ReciptHead = newRecipt;
			}
			else {
				while (temp->next) {//if not empty go to the end
					temp = temp->next;
				}
				temp->next = newRecipt;//simply add to the end
			}
		}
	}
}

//precondition=nothing
//postcondition=nothing
//function=displays the payment amounts namdes and IDs of the users
void displayUsers() {
	Recipt* temp = ReciptHead;
	if (!temp) {//temp will be nullptr if the recipt linkedlist is empty
		cout << "No payments defined." << endl;
	}
	while (temp) {//cout and itterate through the linkedlist if not empty
		cout << "Name: " << temp->name << " ID: " << temp->ID << " " << temp->price << " TRY" << endl;
		temp = temp->next;
	}

}


//precondition=the name of the function that we are currently processing, total rpice of the request of the user passed as referance, 
//the command stack of the whole processes passed as referance,
//postcondition=nothing, but referance paramaters will be updated
//function=goes thorugh and apllys all the commands in the function fiven as the parameter
void processWorkload(string FunctionName, int& Price, SharedCommandStack& CommandStack) {

	Functions* tempFunc = FunctionsHead;
	//fisrt ittarate over the functions until you find the desierd function and make it tempFunc
	while (tempFunc->FunctionName != FunctionName && tempFunc->NextFunction) {
		tempFunc = tempFunc->NextFunction;
	}

	//if the funciton that we are in is not empty make the current comand the first command of that func
	Commands* currentCommand = nullptr;
	if (tempFunc->CommandsHead) {
		currentCommand = tempFunc->CommandsHead;
	}

	//while this command exists
	while (currentCommand) {

		//if the current command is about printing
		if (currentCommand->CommandName.substr(0, 5) == "print") {
			//inform the user that we are printing the stack
			cout << "Executing " << currentCommand->CommandName << " command from " << FunctionName.substr(0, FunctionName.length() - 1) << endl;
			cout << "PRINTING THE STACK TRACE:" << endl;
			//cant prin the stack if its empty
			if (CommandStack.isEmpty()) {
				cout << "The stack is empty" << endl;
			}
			//print the stack
			else {
				CommandStack.printStack(CommandStack);
			}
			//update the price accordingly
			Price = Price + 2;

		}

		//if the current command is about defining something new
		else if (currentCommand->CommandName.substr(0, 6) == "define") {
			string newCommand;
			//push this new command to the shared stack of out whole process
			newCommand = FunctionName + " " + currentCommand->CommandName;
			CommandStack.push(newCommand);
			//update the price
			Price = Price + 1;
		}

		//if its newither print nor define then it is calling a new function
		else {
			string newFunc;
			newFunc = currentCommand->CommandName.substr(5, string::npos);
			newFunc = newFunc.substr(0, newFunc.length() - 1) + ":";//modify the name to fit into customer request
			//first inform the user
			cout << "Calling " << newFunc.substr(0, newFunc.length() - 1) << " from " << tempFunc->FunctionName.substr(0, tempFunc->FunctionName.length() - 1) << endl;
			//recursively call this function to process this new service.
			processWorkload(newFunc, Price, CommandStack);
			//update the price when recursion ends
			Price = Price + 5;
		}

		//go to the next command
		currentCommand = currentCommand->NextCommad;
	}


	if (tempFunc->CommandsHead) {
		cout << tempFunc->FunctionName.substr(0, tempFunc->FunctionName.length() - 1) << " is finished. Clearing the stack from it's data..." << endl;
		CommandStack.ClearAFunc(FunctionName);
	}
}

//precondition=nothing
//postcondition=nothing, but queues are updated
//function=looks at the current situations of the queues, and processes a request according to the rules
void processWorkload()
{
	SharedCommandStack CommandStack;

	//we start by the Instructors becasue they have priority, unles wee did 10 of them, or their queue is empty
	if (!InstructorQueue.isEmpty() && !((InstructorCounter == 10) && (!StudentQueue.isEmpty())))
	{
		Request currentProcess;
		InstructorQueue.dequeue(currentProcess);//dequeue one and start processing it

		//inform the user
		cout << "Processing instructors queue..." << endl;

		cout << "Processing prof." << currentProcess.RequesterName << "'s request (with ID "
			<< currentProcess.RequesterID << ") of service (function):\n" << currentProcess.RequestedFunc.substr(0, currentProcess.RequestedFunc.length() - 1) << endl;

		cout << "-------------------------------------------------------" << endl;
		string RequestedFunction = currentProcess.RequestedFunc;//set the desired function
		int Price = 0; //set starting price


		processWorkload(RequestedFunction, Price, CommandStack); //call the overloaded recursive function to call on services

		//a new recipt should be created after the recursive operations are done and the price is calculated
		Recipt* TempRecipt = ReciptHead;
		while (TempRecipt) {//itterate thorugh the tab
			//if you find the user is already in, update the price
			if ((TempRecipt->ID == currentProcess.RequesterID) && (TempRecipt->name == currentProcess.RequesterName)) {
				TempRecipt->price = TempRecipt->price + Price;
				break;
			}
			//if the user is not in it update the recipt linked list.
			else {
				TempRecipt = TempRecipt->next;
			}
		}

		InstructorCounter++;//increment the counter on instructers to check whether we did 10 instructors back to back or not.
		cout << "GOING BACK TO MAIN MENU" << endl;
	}

	//now is the turn of the student queue, if not empty
	else if (!StudentQueue.isEmpty())
	{
		Request currentProcess;
		StudentQueue.dequeue(currentProcess);//dequeue one and start processing it

		//inform the user wheher 10 instructers are served or, now instructors left.
		if (InstructorCounter == 10) {
			cout << "10 instructors are served. Taking 1 student from the queue..." << endl;
			InstructorCounter = 0;
		}
		else {
			cout << "Instructors queue is empty. Proceeding with students queue..." << endl;
		}

		//inform the user about the current process
		cout << "Processing " << currentProcess.RequesterName << "'s request (with ID "
			<< currentProcess.RequesterID << ") of service (function):\n" << currentProcess.RequestedFunc.substr(0, currentProcess.RequestedFunc.length() - 1) << endl;

		cout << "-------------------------------------------------------" << endl;


		string RequestedFunction = currentProcess.RequestedFunc;//set the desired function
		int Price = 0;//set starting price

		processWorkload(RequestedFunction, Price, CommandStack);//call the overloaded recursive function to call on services

		//a new recipt should be created after the recursive operations are done and the price is calculated
		Recipt* TempRecipt = ReciptHead;
		while (TempRecipt) {//itterate thorugh the tab
			//if you find the user is already in, update the price
			if ((TempRecipt->ID == currentProcess.RequesterID) && (TempRecipt->name == currentProcess.RequesterName)) {
				TempRecipt->price = TempRecipt->price + Price;
				break;
			}
			//if the user is not in it update the recipt linked list.
			else {
				TempRecipt = TempRecipt->next;
			}
		}


		cout << "GOING BACK TO MAIN MENU" << endl;
	}

	else //if we cant dequeue anything that means there is nothing to dequeue. Inform  the user.
	{
		cout << "Both instructor's and student's queue is empty.\nNo request is processed." << endl << "GOING BACK TO MAIN MENU" << endl;
	}
}

//precontition=none, postcondition=none
//function = frees all memory form the heap allocated during the progrram
void DeleteAll() {

		//for no memory leak
		InstructorQueue.clear();
		StudentQueue.clear();

		//now delete the function Linkedlist
		Functions* currentRow = FunctionsHead;
		while (currentRow) {
			Commands* currentCol = currentRow->CommandsHead;
			while (currentCol) {
				Commands* temp = currentCol;
				currentCol = currentCol->NextCommad;
				delete temp;  // Free the memory of the command node
			}
			Functions* tempRow = currentRow;
			currentRow = currentRow->NextFunction;
			delete tempRow;  // Free the memory of the current function node
		}
		FunctionsHead = nullptr;
		
		//now delete the tabslist
		Recipt* temp = ReciptHead;
		while (temp) {
			Recipt* temp2 = temp;
			temp = temp->next;
			delete temp2;
		}
		ReciptHead=nullptr;

}
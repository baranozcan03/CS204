//Baran Özcan 30960
#include <iostream>


//create  a struct element with data and next pointer, 
struct Element {
  int data; //the data in the node
  Element* next; //a pointer to the next element in the set
};

//create a class LinkedListSet
class LinkedListSet {
  
  //every linkedlistset has a head pointer, which points to the first element in the set
  //which is a node with data and next pointer
  Element* head;

public:

  //precondtion=none, postcondition=none
  //function= linkedlistset constructor
  LinkedListSet() {
    head = nullptr; //set the head to nullptr, default constructor
  }

  //precondtion=none, postcondition=none
  //function= linkedlistset destructor
  ~LinkedListSet(){
    
    Element* temp = head;
    Element* next;
    //delete all the nodes in the linked list untill you hit nullptr
    while (temp != nullptr) {
      next = temp->next;
      delete temp;
      temp = next;
    
    }
  }

  //precondtion=value to be added to the set, postcondition=new set is returned with the value added
  //function= adds a value to the set while keeping the set sorted.
  LinkedListSet& operator+(int value) {

    //if the number is already in the set, dont add it
    if(contains(value)){
      return *this;
    }
    
    //if the set is empty just add the value and modify the head
    if (head == nullptr) {
      head = new Element;
      head->data = value;
      head->next = nullptr;
    } 
    
    //if the set not empty be careful about the order 
    
    else {
      //keep 2 pointers, one to the current node and one to the previous node
      //so you dont lose the previous nodes value
      Element* temp = head;
      Element* prev = nullptr;

      //now we find the correct position to add the value
      while (temp != nullptr) {
        //if the value is less than the current node, add it before the current node
        if (temp->data > value) {
          //create a new node and add it before the current node
          Element* newElement = new Element;
          newElement->data = value;
          newElement->next = temp;
          //modify the head if the value is to be added before the first element
          if (prev == nullptr) {
            head = newElement;
          }
          //modify the previous node if the value is to be added after the first element
          else {
            prev->next = newElement;
          }
          return *this;
        }
        //itterate through the linked list
        prev = temp;
        temp = temp->next;
      }
      //if the value is greater than all the values in the set, add it at the end
      Element* newElement = new Element;
      newElement->data = value;
      newElement->next = nullptr;
      prev->next = newElement;
    }
    
    return *this;
  }

  //precondtion=value to be deleted from the set, postcondition=new set is returned with the value deleted
  LinkedListSet& operator-(int value) {

    //Since it is an ordered set we will move forward until we find the value
    //If we find the value, delete and return the set. if we dont, return the set as it is.
    //keep 2 pointers, one to the current node and one to the previous node
    //so you dont lose the previous nodes value
    Element* temp = head; 
    Element* prev = nullptr;
    while (temp) {
      //if the value is found delete it and return the set
      if (temp->data == value) {
        //if the value is the first element in the set, delete it, and modify the head
        if (prev == nullptr) {
          head = temp->next;
          delete temp;
          return *this;
        }
        //if the value is not the first element in the set, simply delete it
        else {
          prev->next = temp->next;
          delete temp;
          return *this;
        }
      }
      //itterate through the linked list
      prev = temp;
      temp = temp->next;
    }
    return *this;
  }

  //precondtion=2 sets, postcondition=new set is returned with the union of the 2 sets
  LinkedListSet& operator+=(LinkedListSet& set) {
    Element* temp = set.head;
    //itterate throug the linked list
    while (temp != nullptr) {
      //add the value from the second set to the first set
      //keep in mind our + operator keeps the set sorted and checks for duplicates
      *this + temp->data;
      temp = temp->next;
    }
    return *this;
  }

  //precondtion=2 sets, postcondition=new set is returned with the intersection of the 2 sets
  LinkedListSet& operator&=(LinkedListSet& set) {
    Element* temp = head;
    //itterate throug the linked list
    while (temp != nullptr) {
      //if the value is not in the second set, delete it from the first set, since it is not in the intersection
      if (!set.contains(temp->data)) {
        *this - temp->data;
      }
      //move along the linked list
      temp = temp->next;
    }
    return *this;
  }

  //precondtion=2 sets, postcondition=new set is returned with the difference of the 2 sets
  LinkedListSet& operator-=(LinkedListSet& set) {
    Element* temp = head;
    //itterate throug the linked list
    while (temp != nullptr) {
      //if the value is in the second set, delete it from the first set
      if (set.contains(temp->data)) {
        *this - temp->data;
      }
      //move along the linked list
      temp = temp->next;
    }
    return *this;
  }

    //precondtition=a set, postcondition=set is displayed
    //function= displays the set when applied on a class obj
    void display() {
      std::cout <<  "LinkedListSet{";
      Element* temp = head;
      //itterate throug the linked list and print the values
      while (temp != nullptr) {
        std::cout << temp->data << ", ";
        temp = temp->next;
      }
    std::cout <<"}" << std::endl;
  }

private:
  //precondtion=value to be checked, postcondition=returns true if the value is in the set, false otherwise
  bool contains(int value) {
    Element* temp = head;
    //itterate throug the linked list and check if the value is in the set
    while (temp != nullptr) {
      //if it is in the set return true
      if (temp->data == value) {
        return true;
      }
      temp = temp->next;
    }
    //if it is not in the set return false
    return false;
  }

};

int main() {

    // Example usage
  LinkedListSet set1;
  set1 + 1 + 2 + 3 + 17; // Insertion
  set1.display();   
  
  set1 - 2; // Deletion
  set1.display(); 
  
  LinkedListSet set2;
  set2 + 3 + 4 + 5; 
  set2.display();   
  
  set2 += set1 + 7 + 3 + 19; //Insertion with multiple right-hand values
  set2.display();
  
  LinkedListSet set3;
  set3 + 3 + 4 + 7 + 17 + 41;
  set3.display();
  
  LinkedListSet set4;
  set4 + 41 + 37+ 7 + 19 + 41;
  set4.display();
  
  // Set Union
  set1 += set2;
  set1.display(); 
  
  // Set Difference
  set1 -= set3;
  set1.display(); 
  
  // Set Intersection
  set1 &= set4;
  set1.display(); 
  
  return 0;
}

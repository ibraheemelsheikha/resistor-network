//
//  main.cpp
//  Lab 3 The Resistor Network Program
//
//  Created by Nathan Hung on 2024-09-11.
//  Modified by Salma Emara on 2024-09-30
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.
//
//  ECE244 Student: Complete the skeletal code of the main function in this file

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "Node.h"
#include "Resistor.h"

using namespace std;

Resistor** resistors = nullptr;  // Pointer that should point to an array of Resistor pointers
Node* nodes = nullptr;  // pointer that should hold address to an array of Nodes
int maxNodeNumber = 0;  // maximum number of nodes as set in the command line
int maxResistors = 0;  // maximum number of resistors as set in the command line
int resistorsCount = 0;  // count the number of resistors

string errorArray[10] = {
    "invalid command",                                  // 0
    "invalid argument",                                 // 1
    "negative resistance",                              // 2
    "node value is out of permitted range",             // 3
    "resistor name cannot be keyword \"all\"",          // 4
    "both terminals of resistor connect to same node",  // 5
    "too few arguments",                                // 6
};

// Function Prototypes
bool getInteger(stringstream& ss, int& x);
bool getString(stringstream& ss, string& s);
bool getDouble(stringstream& ss, double& s);

void handleMaxVal(stringstream& ss);
void handleInsertR(stringstream& ss);
void handleModifyR(stringstream& ss);
void handlePrintR(stringstream& ss);
void handleDeleteR(stringstream& ss);
void handleSetV(stringstream& ss);

int main() {
  string line;
  cout << ">>> ";
  cout.flush();
  // TODO: Implement the main function here
  getline(cin, line);
  line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1); //Citation: @569 Vir Patel on Piazza

  while (!cin.eof()) {
    string command;
    stringstream ss(line);
    ss >> command;

    if (command == "maxVal") {
      handleMaxVal(ss);
    }
    else if (command == "insertR") {
      handleInsertR(ss);
    }
    else if (command == "modifyR") {
      handleModifyR(ss);
    }
    else if (command == "printR") {
      handlePrintR(ss);
    }
    else if (command == "deleteR") {
      handleDeleteR(ss);
    }
    else if (command == "setV") {
      handleSetV(ss);
    }
    else {
      cout << "Error: invalid command" << endl;
    }
    cout << ">>> ";
    getline(cin, line);
    line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1); //Citation: @569 Vir Patel on Piazza
  }
  
  return 0;
  
  
}

// TODO: Implement functions here
bool getInteger(stringstream& ss, int& x) {
  if (ss.eof()) {
    ss.clear();
    cout << "Error: too few arguments" << endl;
    return false;
  }
  
  ss >> x;
  
  if (ss.fail() || x < 0) {
    ss.clear();
    cout << "Error: invalid argument" << endl;
    return false;
  }

  return true;
}

bool getString(stringstream& ss, string& s) {
  if (ss.eof()) {
    ss.clear();
    cout << "Error: too few arguments" << endl;
    return false;
  }
  
  ss >> s;
  
  if (ss.fail()) {
    ss.clear();
    cout << "Error: invalid argument" << endl;;
    return false;
  }

  return true;
}

bool getDouble(stringstream& ss, double& s) {
  if (ss.eof()) {
    ss.clear();
    cout << "Error: too few arguments" << endl;
    return false;
  }
  
  ss >> s;
  
  if (ss.fail()) {
    ss.clear();
    cout << "Error: invalid argument" << endl;
    return false;
  }

  return true;
}

void handleMaxVal(stringstream& ss) {  
  int tempMaxNodeNumber, tempMaxResistors;
  if (!getInteger(ss, tempMaxNodeNumber) || !getInteger(ss, tempMaxResistors)) {
    return;
  }
  
  maxNodeNumber = tempMaxNodeNumber;
  maxResistors = tempMaxResistors;
  
  delete [] nodes;
  nodes = nullptr;
    
  if (resistors != nullptr) {
    for (int i = 0; i < resistorsCount; i++) {
      delete resistors[i];
      resistors[i] = nullptr;
    }
    delete [] resistors;
    resistors = nullptr;
  }
  

  nodes = new Node[maxNodeNumber];
  resistors = new Resistor*[maxResistors];
  resistorsCount = 0;
    
  for (int i = 0; i < maxResistors; i++) {
    resistors[i] = nullptr;
  }
  cout << "New network: max node number is " << maxNodeNumber << "; max resistors is " << maxResistors << endl;
  return;
}


void handleInsertR(stringstream& ss){
  string name;
  double resistance;
  int firstNodeID, secondNodeID;
  
  if (!getString(ss, name) || !getDouble(ss, resistance) || !getInteger(ss, firstNodeID) || !getInteger(ss, secondNodeID)) {
    return;
  }
  
  if (name == "all") {
    cout << "Error: resistor name cannot be keyword \"all\"" << endl;
    return;
  }
  if (resistance < 0) {
    cout << "Error: negative resistance" << endl;
  }
  if ((firstNodeID < 1 || firstNodeID > maxNodeNumber) || (secondNodeID < 1 || secondNodeID > maxNodeNumber)) {
    cout << "Error: node value is out of permitted range 1 -  "<< maxNodeNumber << endl;
    return;
  }
  if (firstNodeID == secondNodeID) {
    cout << "Error: both terminals of resistor connect to same node	" << endl;
    return;
  }
  for (int i = 0; i < resistorsCount; i++) {
    if (resistors[i]->getName() == name ) {
      cout << "Error: resistor " << name << " already exists" << endl;
      return;
    }
  }
  
  int endpoints[2] = {firstNodeID, secondNodeID};
  Resistor* newResistor = new Resistor(name, resistance, endpoints);

  resistors[resistorsCount] = newResistor;
  nodes[firstNodeID - 1].addResistor(resistorsCount);
  nodes[secondNodeID - 1].addResistor(resistorsCount);
  resistorsCount++;

  cout << "Inserted: resistor " << name << " " << fixed << setprecision(2) << resistance << " Ohms " <<  firstNodeID  << " -> " <<  secondNodeID << endl;
  return;
}


void handleModifyR(stringstream& ss) {
  string name;
  double newResistance;
  if (!getString(ss, name) || !getDouble(ss, newResistance)) {
    return;
  }

  int index = 0;
  while (resistors[index]->getName() != name && index < resistorsCount - 1) {
    index++;
  }
  if (index > resistorsCount - 1) {
    cout << "Error: resistor " << name << " not found" << endl;
    return;
  }

  if (newResistance < 0) {
    cout << "Error: negative resistance" << endl;
  }
  
  double oldResistance = resistors[index]->getResistance();
  resistors[index]->setResistance(newResistance);
  cout << "Modified: resistor" << name << " from " << oldResistance << " Ohms to " << newResistance << " Ohms" << endl;
  return;
}


void handlePrintR(stringstream& ss) {
  string name;
  if (!getString(ss, name)) {
    return;
  }
  int index = 0;
  while (resistors[index]->getName() != name && index < resistorsCount - 1) {
    index++;
  }
  if (index > resistorsCount - 1) {
    cout << "Error: resistor " << name << " not found" << endl;
    return;
  }
  
  cout << "Print:" << endl;
  resistors[index]->print();
  return;
}


void handleDeleteR(stringstream& ss) {
  string word;
  ss >> word;
  if (word != "all") {
    cout << "Error: invalid argument" << endl;
  }
  delete [] nodes;
  nodes = nullptr;
    
  if (resistors != nullptr) {
    for (int i = 0; i < resistorsCount; i++) {
      delete resistors[i];
      resistors[i] = nullptr;
    }
    delete [] resistors;
  }
  resistorsCount = 0;
  cout << "Deleted: all resistors" << endl;
  return;
}
void handleSetV(stringstream& ss) {
  int nodeID;
  double voltage;
  if (!getInteger(ss, nodeID) || !getDouble(ss, voltage)) {
    return;
  }
  if (nodeID < 1 || nodeID > maxNodeNumber) {
    cout << "Error: node value is out of permitted range 1 -  "<< maxNodeNumber << endl;
    return;
  }
  nodes[nodeID - 1].setVoltage(voltage);
  cout << "Set: node " << nodeID << " to " << voltage << " Volts" << endl;
  return;
}

//
//  Node.cpp
//  Lab 3 The Resistor Network Program
// Author: Ibraheem El Sheikha
// Date: 2024-10-01
#include "Node.h"

Node::Node() {
  numRes = 0; 
  voltage = 0; 
  voltageIsSet = false;
  for (int i = 0; i < MAX_RESISTORS_PER_NODE; i++) {
    resIDArray[i] = -1;
  }
}

bool Node::canAddResistor() {
  return (numRes < MAX_RESISTORS_PER_NODE);
}


void Node::addResistor(int rIndex) {
  if (canAddResistor()) {
    resIDArray[numRes] = rIndex;
    numRes++;
  }
  else {
    cout << "Error: too many resistors in one node" << endl;
  }
}  // adds a resistor to resIDArray array and increments number of resistors


double Node::getVoltage() { return voltage; }


void Node::setVoltage(double voltage_) {
  voltage = voltage_;
  voltageIsSet = true;
}

void Node::setVoltageInternally(double voltage_) {
  voltage = voltage_;
}  // sets voltage temporarily during iterations for solve command

bool Node::isVoltageSet() { return voltageIsSet; }

int Node::getNumRes() { return numRes; }

int* Node::getResIDArray() { return resIDArray; }
//
//  Resistor.cpp
//  Lab 3 The Resistor Network Program
// Author: Ibraheem El Sheikha
// Date: 2024-10-01

#include "Resistor.h"
#include <iostream>
#include <iomanip>

void Resistor::print() {
  cout << std::left << std::setw(20) << name << std::right << std::setw(8)
       << std::fixed << std::setprecision(2) << resistance << " Ohms "
       << endpointNodeIDs[0] << " -> " << endpointNodeIDs[1] << endl;
}

Resistor::Resistor(string name_, double resistance_, int endpoints_[2]) {
  name = name_;
  resistance = resistance_;
  for (int i = 0; i < 2; i++) {
    endpointNodeIDs[i] = endpoints_[i];
  }
}

string Resistor::getName() {
  return name;
}
double Resistor::getResistance() {
  return resistance;
}
void Resistor::setResistance(double resistance_) {
  resistance = resistance_;
}  // sets the resistance

int getOtherEndpoint(int nodeIndex);
#include "Student.h"
#include <iostream>
using namespace std;

Student::Student(string id, string name) : id(id), name(name) {}

void Student::setId(string id) {
    this->id = id;
}

string Student::getId() const {
    return id;
}

void Student::setName(string name) {
    this->name = name;
}

string Student::getName() const {
    return name;
}

void Student::displayStudent() const {
    cout << "Student ID: " << id << ", Name: " << name << endl;
}

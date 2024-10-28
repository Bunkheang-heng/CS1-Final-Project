#include "Student.h"
#include <iostream>

Student::Student(std::string id, std::string name) : id(id), name(name) {}

void Student::setId(std::string id) {
    this->id = id;
}

std::string Student::getId() const {
    return id;
}

void Student::setName(std::string name) {
    this->name = name;
}

std::string Student::getName() const {
    return name;
}

void Student::displayStudent() const {
    std::cout << "Student ID: " << id << ", Name: " << name << std::endl;
}

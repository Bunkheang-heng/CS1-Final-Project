#ifndef STUDENT_H
#define STUDENT_H

#include <string>
using namespace std;

class Student {
private:
    string id;
    string name;

public:
    Student(string id, string name);
    void setId(string id);
    string getId() const;
    void setName(string name);
    string getName() const;
    void displayStudent() const;
};

#endif

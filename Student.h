#ifndef STUDENT_H
#define STUDENT_H

#include <string>

class Student {
private:
    std::string id;
    std::string name;

public:
    Student(std::string id, std::string name);
    void setId(std::string id);
    std::string getId() const;
    void setName(std::string name);
    std::string getName() const;
    void displayStudent() const;
};

#endif

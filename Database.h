#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>
#include "Student.h"
#include "Grade.h"

class Database {
private:
    sqlite3* db;

public:
    Database();
    ~Database();
    void addStudent(const Student& student);
    void addGrade(const Grade& grade);
    Student findStudentById(const std::string &id);
    void displayAllStudents();
    void displayAllGrades();

    bool studentExists(const std::string &id);
    bool login(const std::string &username, const std::string &password);
    bool updateStudent(const std::string &id, const std::string &newName);
    bool deleteStudent(const std::string &id);

    bool addCourse(const std::string& courseId, const std::string& courseName);
    bool assignCourseToStudent(const std::string& studentId, const std::string& courseId);
    void displayAllCourses();
    bool courseExists(const std::string& courseId);
    void displayStudentCourses(const std::string& studentId);
    void displayStudentGrades(const std::string& studentId);

    sqlite3* getDb() const { return db; }
};

#endif

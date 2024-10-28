#ifndef GRADE_H
#define GRADE_H

#include <string>

class Grade {
private:
    std::string studentId;
    double score;
    std::string letterGrade;

    void calculateLetterGrade(); // Function to calculate letter grade from score

public:
    Grade(const std::string &studentId, double score);
    void setScore(double score);
    double getScore() const;
    std::string getLetterGrade() const;
    std::string getStudentId() const;
    void displayGrade() const;
};

#endif

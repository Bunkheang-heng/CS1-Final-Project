#ifndef GRADE_H
#define GRADE_H

#include <string>

class Grade {
private:
    std::string studentId;
    std::string courseId;
    double score;
    std::string letterGrade;

    void calculateLetterGrade();

public:
    Grade(const std::string &studentId, const std::string &courseId, double score);
    Grade(const std::string &studentId, double score);  // Changed from string to std::string
    void setScore(double score);
    double getScore() const;
    std::string getLetterGrade() const;
    std::string getStudentId() const;
    std::string getCourseId() const;
};
#endif

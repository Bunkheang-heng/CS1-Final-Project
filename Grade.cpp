#include "Grade.h"
using namespace std;

Grade::Grade(const string &studentId, const string &courseId, double score)
    : studentId(studentId), courseId(courseId), score(score) {
    calculateLetterGrade();
}

void Grade::setScore(double newScore) {
    score = newScore;
    calculateLetterGrade();
}

double Grade::getScore() const {
    return score;
}

string Grade::getLetterGrade() const {
    return letterGrade;
}

string Grade::getStudentId() const {
    return studentId;
}

string Grade::getCourseId() const {
    return courseId;
}

void Grade::calculateLetterGrade() {
    if (score >= 90) {
        letterGrade = "A";
    } else if (score >= 80) {
        letterGrade = "B";
    } else if (score >= 70) {
        letterGrade = "C";
    } else if (score >= 60) {
        letterGrade = "D";
    } else {
        letterGrade = "F";
    }
}

Grade::Grade(const std::string& studentId, double score) {
    this->studentId = studentId;
    this->score = score;
    calculateLetterGrade();
}

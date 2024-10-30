#include "Grade.h"
using namespace std;

Grade::Grade(const string &studentId, double score)
    : studentId(studentId), score(score) {
    calculateLetterGrade(); // Automatically calculate the letter grade based on score
}

void Grade::setScore(double newScore) {
    score = newScore;
    calculateLetterGrade(); // Update letter grade based on new score
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

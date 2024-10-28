#include "Report.h"
#include <iostream>

void Report::generateReport(const Student &student, const Grade &grade) const {
    student.displayStudent();
    std::cout << "Score: " << grade.getScore() << std::endl;
    std::cout << "Grade: " << grade.getLetterGrade() << std::endl;
    std::cout << "Final Report Generated.\n" << std::endl;
}

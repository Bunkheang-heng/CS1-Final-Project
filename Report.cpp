#include "Report.h"
#include <iostream>

using namespace std;
void Report::generateReport(const Student &student, const Grade &grade) const {
    student.displayStudent();
   cout << "Score: " << grade.getScore() <<endl;
   cout << "Grade: " << grade.getLetterGrade() <<endl;
   cout << "Final Report Generated.\n" <<endl;
}

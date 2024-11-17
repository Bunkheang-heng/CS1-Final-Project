#ifndef REPORT_H
#define REPORT_H

#include "Student.h"
#include "Grade.h"
#include "Database.h"
#include <vector>
#include <string>

class Report {
public:
    void generateReport(const std::string& studentId, Database& db) const;
private:
    double calculateOverallGrade(const std::string& studentId, Database& db) const;
    std::string calculateLetterGrade(double score) const;
};

#endif

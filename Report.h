#ifndef REPORT_H
#define REPORT_H

#include "Student.h"
#include "Grade.h"

class Report {
public:
    void generateReport(const Student &student, const Grade &grade) const;
};

#endif

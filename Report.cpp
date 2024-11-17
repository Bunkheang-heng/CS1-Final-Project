#include "Report.h"
#include <iostream>
#include <iomanip>

// Add these color definitions
#define RED "\033[31m"
#define BLUE "\033[34m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

using namespace std;
void Report::generateReport(const std::string& studentId, Database& db) const {
    Student student = db.findStudentById(studentId);
    
    if (student.getId().empty()) {
        std::cout << RED "❌ Student not found." RESET << std::endl;
        return;
    }
    
    std::cout << BLUE;
    std::cout << "\n╔═══════════════════════════════════════════════════╗\n";
    std::cout << "║               STUDENT GRADE REPORT                ║\n";
    std::cout << "╚═══════════════════════════════════════════════════╝\n" << RESET;
    
    // Display student information
    std::cout << YELLOW "\n📚 Student Information:" RESET << std::endl;
    std::cout << "ID: " << student.getId() << std::endl;
    std::cout << "Name: " << student.getName() << std::endl;

    // Get and display all grades for the student
    std::cout << YELLOW "\n📊 Course Grades:" RESET << std::endl;
    std::cout << BLUE;
    std::cout << "╔═══════════════════════════════════════════════════╗\n";
    std::cout << "║ Course Name         Score      Grade              ║\n";
    std::cout << "╠═══════════════════════════════════════════════════╣\n" << RESET;
    
    db.displayStudentGrades(studentId);
    
    // Calculate and display overall grade
    double overallGrade = calculateOverallGrade(studentId, db);
    std::string letterGrade = calculateLetterGrade(overallGrade);
    
    std::cout << BLUE;
    std::cout << "╠═══════════════════════════════════════════════════╣\n";
    std::cout << "║ Overall Performance:                              ║\n";
    std::cout << "║ Average Score: " << std::fixed << std::setprecision(2) 
              << overallGrade << "                                ║\n";
    std::cout << "║ Final Grade: " << letterGrade << "                                    ║\n";
    std::cout << "╚═══════════════════════════════════════════════════╝\n" << RESET;
}

double Report::calculateOverallGrade(const std::string& studentId, Database& db) const {
    string sql = "SELECT AVG(Score) FROM Grades WHERE StudentID = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db.getDb(), sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, studentId.c_str(), -1, SQLITE_STATIC);
    
    double average = 0.0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        average = sqlite3_column_double(stmt, 0);
    }
    
    sqlite3_finalize(stmt);
    return average;
}

std::string Report::calculateLetterGrade(double score) const {
    if (score >= 90) return "A";
    if (score >= 80) return "B";
    if (score >= 70) return "C";
    if (score >= 60) return "D";
    return "F";
}

#include "Database.h"
#include <iostream>
#include <iomanip>

Database::Database() {
    int exit = sqlite3_open("StudentManagement.db", &db);
    if (exit) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Database opened successfully!" << std::endl;
    }

    // Create tables if they don’t exist
    std::string create_users_table = "CREATE TABLE IF NOT EXISTS Users ("
                                     "Username TEXT PRIMARY KEY, "
                                     "Password TEXT NOT NULL);";
    std::string create_students_table = "CREATE TABLE IF NOT EXISTS Students ("
                                        "ID TEXT PRIMARY KEY, "
                                        "Name TEXT NOT NULL);";
    std::string create_grades_table = "CREATE TABLE IF NOT EXISTS Grades ("
                                      "StudentID TEXT, "
                                      "Score REAL, "   // Changed to Score REAL
                                      "FOREIGN KEY(StudentID) REFERENCES Students(ID));";

    sqlite3_exec(db, create_users_table.c_str(), nullptr, nullptr, nullptr);
    sqlite3_exec(db, create_students_table.c_str(), nullptr, nullptr, nullptr);
    sqlite3_exec(db, create_grades_table.c_str(), nullptr, nullptr, nullptr);

    // Insert default user for testing purposes
    std::string insert_user = "INSERT OR IGNORE INTO Users (Username, Password) VALUES ('admin', 'password');";
    sqlite3_exec(db, insert_user.c_str(), nullptr, nullptr, nullptr);
}

Database::~Database() {
    sqlite3_close(db);
}

bool Database::login(const std::string &username, const std::string &password) {
    std::string sql = "SELECT COUNT(*) FROM Users WHERE Username = ? AND Password = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    bool success = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        success = sqlite3_column_int(stmt, 0) > 0;
    }

    sqlite3_finalize(stmt);
    return success;
}

bool Database::studentExists(const std::string &id) {
    std::string sql = "SELECT COUNT(*) FROM Students WHERE ID = ?;";
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_STATIC);
    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = sqlite3_column_int(stmt, 0) > 0;
    }
    sqlite3_finalize(stmt);
    return exists;
}


bool Database::updateStudent(const std::string &id, const std::string &newName) {
    if (!studentExists(id)) {
        std::cerr << "Error: Student with ID " << id << " does not exist." << std::endl;
        return false;
    }

    std::string sql = "UPDATE Students SET Name = ? WHERE ID = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, newName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, id.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error updating student: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    std::cout << "Student updated successfully." << std::endl;
    return true;
}

bool Database::deleteStudent(const std::string &id) {
    if (!studentExists(id)) {
        std::cerr << "Error: Student with ID " << id << " does not exist." << std::endl;
        return false;
    }

    // Delete associated grades first (if any)
    std::string deleteGradesSql = "DELETE FROM Grades WHERE StudentID = ?;";
    sqlite3_stmt* stmtGrades;
    sqlite3_prepare_v2(db, deleteGradesSql.c_str(), -1, &stmtGrades, nullptr);
    sqlite3_bind_text(stmtGrades, 1, id.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmtGrades) != SQLITE_DONE) {
        std::cerr << "Error deleting grades: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmtGrades);
        return false;
    }
    sqlite3_finalize(stmtGrades);

    // Delete student from Students table
    std::string deleteStudentSql = "DELETE FROM Students WHERE ID = ?;";
    sqlite3_stmt* stmtStudent;
    sqlite3_prepare_v2(db, deleteStudentSql.c_str(), -1, &stmtStudent, nullptr);
    sqlite3_bind_text(stmtStudent, 1, id.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmtStudent) != SQLITE_DONE) {
        std::cerr << "Error deleting student: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmtStudent);
        return false;
    }

    sqlite3_finalize(stmtStudent);
    std::cout << "Student deleted successfully." << std::endl;
    return true;
}


void Database::addStudent(const Student& student) {
    if (studentExists(student.getId())) {
        std::cerr << "Error: Student with ID " << student.getId() << " already exists." << std::endl;
        return;
    }

    std::string sql = "INSERT INTO Students (ID, Name) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, student.getId().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, student.getName().c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error adding student: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Student added successfully." << std::endl;
    }

    sqlite3_finalize(stmt);
}

void Database::addGrade(const Grade& grade) {
    std::string sql = "INSERT INTO Grades (StudentID, Score, Grade) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, grade.getStudentId().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 2, grade.getScore());  // Bind the score
    sqlite3_bind_text(stmt, 3, grade.getLetterGrade().c_str(), -1, SQLITE_STATIC);  // Bind the letter grade

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error adding grade: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Grade added successfully." << std::endl;
    }

    sqlite3_finalize(stmt);
}




Student Database::findStudentById(const std::string &id) {
    std::string sql = "SELECT ID, Name FROM Students WHERE ID = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_STATIC);

    Student student("", "");
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        student = Student(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)),
                          reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        std::cout << "Student found: " << student.getName() << std::endl;
    } else {
        std::cerr << "Student not found with ID: " << id << std::endl;
    }

    sqlite3_finalize(stmt);
    return student;
}

void Database::displayAllStudents() {
    std::string sql = "SELECT ID, Name FROM Students;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    // Table Header
    std::cout << "\n\033[1;33m+----------------------+--------------------------------+\033[0m" << std::endl;
    std::cout << "\033[1;33m| " << std::left << std::setw(18) << "Student ID"
              << "| " << std::setw(30) << "Name" << " |\033[0m" << std::endl;
    std::cout << "\033[1;33m+----------------------+--------------------------------+\033[0m" << std::endl;

    // Table Rows
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::cout << "| "  <<"  "<<sqlite3_column_text(stmt, 0)
                    <<"                   "<< sqlite3_column_text(stmt, 1) << std::endl;
    }

    // Footer
    std::cout << "\033[1;33m+----------------------+--------------------------------+\033[0m" << std::endl;

    sqlite3_finalize(stmt);
    std::cout << std::endl;  // Space after the table
}
void Database::displayAllGrades() {
    std::string sql = "SELECT StudentID, Score, Grade FROM Grades;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    // Display grades in a table format
    std::cout << "\n--- Grades ---" << std::endl;
    std::cout << "+----------------+-------+-------+" << std::endl;
    std::cout << "| Student ID     | Score | Grade |" << std::endl;
    std::cout << "+----------------+-------+-------+" << std::endl;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::cout << "| " << std::setw(14) << sqlite3_column_text(stmt, 0) << " | "
                  << std::setw(5) << sqlite3_column_double(stmt, 1) << " | "
                  << std::setw(5) << sqlite3_column_text(stmt, 2) << " |" << std::endl;
    }

    std::cout << "+----------------+-------+-------+" << std::endl;
    sqlite3_finalize(stmt);
}





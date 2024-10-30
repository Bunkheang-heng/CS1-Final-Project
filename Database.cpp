#include "Database.h"
#include <iostream>
#include <iomanip>
using namespace std;

Database::Database() {
    int exit = sqlite3_open("StudentManagement.db", &db);
    if (exit) {
        cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
    } else {
        cout << "Database opened successfully!" << endl;
    }

    // Create tables if they don’t exist
    string create_users_table = "CREATE TABLE IF NOT EXISTS Users ("
                                     "Username TEXT PRIMARY KEY, "
                                     "Password TEXT NOT NULL);";
    string create_students_table = "CREATE TABLE IF NOT EXISTS Students ("
                                        "ID TEXT PRIMARY KEY, "
                                        "Name TEXT NOT NULL);";
    string create_grades_table = "CREATE TABLE IF NOT EXISTS Grades ("
                                      "StudentID TEXT, "
                                      "Score REAL, "   // Changed to Score REAL
                                      "FOREIGN KEY(StudentID) REFERENCES Students(ID));";

    sqlite3_exec(db, create_users_table.c_str(), nullptr, nullptr, nullptr);
    sqlite3_exec(db, create_students_table.c_str(), nullptr, nullptr, nullptr);
    sqlite3_exec(db, create_grades_table.c_str(), nullptr, nullptr, nullptr);

    // Insert default user for testing purposes
    string insert_user = "INSERT OR IGNORE INTO Users (Username, Password) VALUES ('admin', 'password');";
    sqlite3_exec(db, insert_user.c_str(), nullptr, nullptr, nullptr);
}

Database::~Database() {
    sqlite3_close(db);
}

bool Database::login(const string &username, const string &password) {
    string sql = "SELECT COUNT(*) FROM Users WHERE Username = ? AND Password = ?;";
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

bool Database::studentExists(const string &id) {
    string sql = "SELECT COUNT(*) FROM Students WHERE ID = ?;";
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
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


bool Database::updateStudent(const string &id, const string &newName) {
    if (!studentExists(id)) {
        cerr << "Error: Student with ID " << id << " does not exist." << endl;
        return false;
    }

    string sql = "UPDATE Students SET Name = ? WHERE ID = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, newName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, id.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "Error updating student: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    cout << "Student updated successfully." << endl;
    return true;
}

bool Database::deleteStudent(const string &id) {
    if (!studentExists(id)) {
        cerr << "Error: Student with ID " << id << " does not exist." << endl;
        return false;
    }

    // Delete associated grades first (if any)
    string deleteGradesSql = "DELETE FROM Grades WHERE StudentID = ?;";
    sqlite3_stmt* stmtGrades;
    sqlite3_prepare_v2(db, deleteGradesSql.c_str(), -1, &stmtGrades, nullptr);
    sqlite3_bind_text(stmtGrades, 1, id.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmtGrades) != SQLITE_DONE) {
        cerr << "Error deleting grades: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmtGrades);
        return false;
    }
    sqlite3_finalize(stmtGrades);

    // Delete student from Students table
    string deleteStudentSql = "DELETE FROM Students WHERE ID = ?;";
    sqlite3_stmt* stmtStudent;
    sqlite3_prepare_v2(db, deleteStudentSql.c_str(), -1, &stmtStudent, nullptr);
    sqlite3_bind_text(stmtStudent, 1, id.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmtStudent) != SQLITE_DONE) {
        cerr << "Error deleting student: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmtStudent);
        return false;
    }

    sqlite3_finalize(stmtStudent);
    cout << "Student deleted successfully." << endl;
    return true;
}


void Database::addStudent(const Student& student) {
    if (studentExists(student.getId())) {
        cerr << "Error: Student with ID " << student.getId() << " already exists." << endl;
        return;
    }

    string sql = "INSERT INTO Students (ID, Name) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, student.getId().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, student.getName().c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "Error adding student: " << sqlite3_errmsg(db) << endl;
    } else {
        cout << "Student added successfully." << endl;
    }

    sqlite3_finalize(stmt);
}

void Database::addGrade(const Grade& grade) {
    string sql = "INSERT INTO Grades (StudentID, Score, Grade) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, grade.getStudentId().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 2, grade.getScore());  // Bind the score
    sqlite3_bind_text(stmt, 3, grade.getLetterGrade().c_str(), -1, SQLITE_STATIC);  // Bind the letter grade

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "Error adding grade: " << sqlite3_errmsg(db) << endl;
    } else {
        cout << "Grade added successfully." << endl;
    }

    sqlite3_finalize(stmt);
}




Student Database::findStudentById(const string &id) {
    string sql = "SELECT ID, Name FROM Students WHERE ID = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_STATIC);

    Student student("", "");
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        student = Student(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)),
                          reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        cout << "Student found: " << student.getName() << endl;
    } else {
        cerr << "Student not found with ID: " << id << endl;
    }

    sqlite3_finalize(stmt);
    return student;
}

void Database::displayAllStudents() {
    string sql = "SELECT ID, Name FROM Students;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    // Table Header
    cout << "\n\033[1;33m+----------------------+--------------------------------+\033[0m" << endl;
    cout << "\033[1;33m| " << left << setw(18) << "Student ID"
              << "| " << setw(30) << "Name" << " |\033[0m" << endl;
    cout << "\033[1;33m+----------------------+--------------------------------+\033[0m" << endl;

    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << "| "  <<"  "<<sqlite3_column_text(stmt, 0)
                    <<"                   "<< sqlite3_column_text(stmt, 1) << endl;
    }

    // Footer
    cout << "\033[1;33m+----------------------+--------------------------------+\033[0m" << endl;

    sqlite3_finalize(stmt);
    cout << endl;  // Space after the table
}
void Database::displayAllGrades() {
    string sql = "SELECT StudentID, Score, Grade FROM Grades;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    // Display grades in a table format
    cout << "\n--- Grades ---" << endl;
    cout << "+----------------+-------+-------+" << endl;
    cout << "| Student ID     | Score | Grade |" << endl;
    cout << "+----------------+-------+-------+" << endl;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << "| " << setw(14) << sqlite3_column_text(stmt, 0) << " | "
                  << setw(5) << sqlite3_column_double(stmt, 1) << " | "
                  << setw(5) << sqlite3_column_text(stmt, 2) << " |" << endl;
    }

    cout << "+----------------+-------+-------+" << endl;
    sqlite3_finalize(stmt);
}





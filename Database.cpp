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
                                 "CourseID TEXT, "
                                 "Score REAL, "
                                 "Grade TEXT, "
                                 "FOREIGN KEY(StudentID) REFERENCES Students(ID), "
                                 "FOREIGN KEY(CourseID) REFERENCES Courses(CourseID));";
    string create_courses_table = "CREATE TABLE IF NOT EXISTS Courses ("
                                  "CourseID TEXT PRIMARY KEY, "
                                  "CourseName TEXT NOT NULL);";
    string create_student_courses_table = "CREATE TABLE IF NOT EXISTS StudentCourses ("
                                          "StudentID TEXT, "
                                          "CourseID TEXT, "
                                          "FOREIGN KEY(StudentID) REFERENCES Students(ID), "
                                          "FOREIGN KEY(CourseID) REFERENCES Courses(CourseID));";

    sqlite3_exec(db, create_users_table.c_str(), nullptr, nullptr, nullptr);
    sqlite3_exec(db, create_students_table.c_str(), nullptr, nullptr, nullptr);
    sqlite3_exec(db, create_grades_table.c_str(), nullptr, nullptr, nullptr);
    sqlite3_exec(db, create_courses_table.c_str(), nullptr, nullptr, nullptr);
    sqlite3_exec(db, create_student_courses_table.c_str(), nullptr, nullptr, nullptr);

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
    if (!studentExists(grade.getStudentId())) {
        cerr << "\033[1;31mError: Student does not exist.\033[0m" << endl;
        return;
    }
    if (!courseExists(grade.getCourseId())) {
        cerr << "\033[1;31mError: Course does not exist.\033[0m" << endl;
        return;
    }

    // Check if student is enrolled in the course
    string checkEnrollmentSql = "SELECT COUNT(*) FROM StudentCourses "
                               "WHERE StudentID = ? AND CourseID = ?;";
    sqlite3_stmt* enrollmentStmt;
    sqlite3_prepare_v2(db, checkEnrollmentSql.c_str(), -1, &enrollmentStmt, nullptr);
    sqlite3_bind_text(enrollmentStmt, 1, grade.getStudentId().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(enrollmentStmt, 2, grade.getCourseId().c_str(), -1, SQLITE_STATIC);

    bool isEnrolled = false;
    if (sqlite3_step(enrollmentStmt) == SQLITE_ROW) {
        isEnrolled = sqlite3_column_int(enrollmentStmt, 0) > 0;
    }
    sqlite3_finalize(enrollmentStmt);

    if (!isEnrolled) {
        cerr << "\033[1;31mError: Student is not enrolled in this course.\033[0m" << endl;
        return;
    }

    // Check if grade already exists
    string existingGradeSql = "SELECT Score FROM Grades "
                          "WHERE StudentID = ? AND CourseID = ?;";
    sqlite3_stmt* gradeCheckStmt;
    sqlite3_prepare_v2(db, existingGradeSql.c_str(), -1, &gradeCheckStmt, nullptr);
    sqlite3_bind_text(gradeCheckStmt, 1, grade.getStudentId().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(gradeCheckStmt, 2, grade.getCourseId().c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(gradeCheckStmt) == SQLITE_ROW) {
        double currentScore = sqlite3_column_double(gradeCheckStmt, 0);
        sqlite3_finalize(gradeCheckStmt);

        char choice;
        cout << "\033[1;33mA grade of " << currentScore << " already exists for this course.\033[0m" << endl;
        cout << "\033[1;36mWould you like to update it? (Y/N): \033[0m";
        cin >> choice;

        if (toupper(choice) == 'Y') {
            string updateSql = "UPDATE Grades SET Score = ?, Grade = ? "
                             "WHERE StudentID = ? AND CourseID = ?;";
            sqlite3_stmt* updateStmt;
            sqlite3_prepare_v2(db, updateSql.c_str(), -1, &updateStmt, nullptr);
            sqlite3_bind_double(updateStmt, 1, grade.getScore());
            sqlite3_bind_text(updateStmt, 2, grade.getLetterGrade().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(updateStmt, 3, grade.getStudentId().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(updateStmt, 4, grade.getCourseId().c_str(), -1, SQLITE_STATIC);

            if (sqlite3_step(updateStmt) != SQLITE_DONE) {
                cerr << "\033[1;31mError updating grade: " << sqlite3_errmsg(db) << "\033[0m" << endl;
            } else {
                cout << "\033[1;32mGrade updated successfully.\033[0m" << endl;
            }
            sqlite3_finalize(updateStmt);
        }
        return;
    }
    sqlite3_finalize(gradeCheckStmt);

    // Insert new grade
    string sql = "INSERT INTO Grades (StudentID, CourseID, Score, Grade) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, grade.getStudentId().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, grade.getCourseId().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, grade.getScore());
    sqlite3_bind_text(stmt, 4, grade.getLetterGrade().c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "\033[1;31mError adding grade: " << sqlite3_errmsg(db) << "\033[0m" << endl;
    } else {
        cout << "\033[1;32mGrade added successfully.\033[0m" << endl;
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
    string sql = "SELECT s.Name, c.CourseName, g.Score, g.Grade "
                 "FROM Grades g "
                 "JOIN Students s ON g.StudentID = s.ID "
                 "JOIN Courses c ON g.CourseID = c.CourseID "
                 "ORDER BY s.Name, c.CourseName;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    cout << "\n\033[1;33m+----------------------+----------------------+-------+-------+\033[0m" << endl;
    cout << "\033[1;33m| Student Name         | Course Name         | Score | Grade |\033[0m" << endl;
    cout << "\033[1;33m+----------------------+----------------------+-------+-------+\033[0m" << endl;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << "| " << setw(20) << sqlite3_column_text(stmt, 0) << " | "
             << setw(20) << sqlite3_column_text(stmt, 1) << " | "
             << setw(5) << sqlite3_column_double(stmt, 2) << " | "
             << setw(5) << sqlite3_column_text(stmt, 3) << " |" << endl;
    }

    cout << "\033[1;33m+----------------------+----------------------+-------+-------+\033[0m" << endl;
    sqlite3_finalize(stmt);
}

bool Database::courseExists(const string& courseId) {
    string sql = "SELECT COUNT(*) FROM Courses WHERE CourseID = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, courseId.c_str(), -1, SQLITE_STATIC);
    
    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = sqlite3_column_int(stmt, 0) > 0;
    }
    sqlite3_finalize(stmt);
    return exists;
}

bool Database::addCourse(const string& courseId, const string& courseName) {
    if (courseExists(courseId)) {
        cerr << "\033[1;31mError: Course with ID " << courseId << " already exists.\033[0m" << endl;
        return false;
    }

    string sql = "INSERT INTO Courses (CourseID, CourseName) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, courseId.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, courseName.c_str(), -1, SQLITE_STATIC);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

bool Database::assignCourseToStudent(const string& studentId, const string& courseId) {
    if (!studentExists(studentId)) {
        cerr << "\033[1;31mError: Student does not exist.\033[0m" << endl;
        return false;
    }
    if (!courseExists(courseId)) {
        cerr << "\033[1;31mError: Course does not exist.\033[0m" << endl;
        return false;
    }

    // Check if student already has this course
    string checkDuplicateSql = "SELECT COUNT(*) FROM StudentCourses WHERE StudentID = ? AND CourseID = ?;";
    sqlite3_stmt* dupStmt;
    sqlite3_prepare_v2(db, checkDuplicateSql.c_str(), -1, &dupStmt, nullptr);
    sqlite3_bind_text(dupStmt, 1, studentId.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(dupStmt, 2, courseId.c_str(), -1, SQLITE_STATIC);
    
    if (sqlite3_step(dupStmt) == SQLITE_ROW && sqlite3_column_int(dupStmt, 0) > 0) {
        cerr << "\033[1;31mError: Student is already enrolled in this course.\033[0m" << endl;
        sqlite3_finalize(dupStmt);
        return false;
    }
    sqlite3_finalize(dupStmt);

    // Check current course count
    string countSql = "SELECT COUNT(*) FROM StudentCourses WHERE StudentID = ?;";
    sqlite3_stmt* countStmt;
    sqlite3_prepare_v2(db, countSql.c_str(), -1, &countStmt, nullptr);
    sqlite3_bind_text(countStmt, 1, studentId.c_str(), -1, SQLITE_STATIC);
    
    int courseCount = 0;
    if (sqlite3_step(countStmt) == SQLITE_ROW) {
        courseCount = sqlite3_column_int(countStmt, 0);
    }
    sqlite3_finalize(countStmt);

    // Check course limits
    if (courseCount >= 5) {
        cerr << "\033[1;31mError: Student has reached the maximum limit of 5 courses.\033[0m" << endl;
        return false;
    }

    // Insert the new course
    string sql = "INSERT INTO StudentCourses (StudentID, CourseID) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, studentId.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, courseId.c_str(), -1, SQLITE_STATIC);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    if (success) {
        // Check if student now has at least 4 courses
        courseCount++;
        if (courseCount >= 4) {
            cout << "\033[1;32mStudent has met the minimum course requirement (4 courses).\033[0m" << endl;
        } else {
            cout << "\033[1;33mWarning: Student needs " << (4 - courseCount) 
                 << " more course(s) to meet the minimum requirement of 4 courses.\033[0m" << endl;
        }
    }

    return success;
}

void Database::displayAllCourses() {
    string sql = "SELECT CourseID, CourseName FROM Courses;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    cout << "\n\033[1;33m+---------------+--------------------------------+\033[0m" << endl;
    cout << "\033[1;33m| Course ID     | Course Name                     |\033[0m" << endl;
    cout << "\033[1;33m+---------------+--------------------------------+\033[0m" << endl;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << "| " << setw(13) << sqlite3_column_text(stmt, 0) 
             << " | " << setw(30) << sqlite3_column_text(stmt, 1) << " |" << endl;
    }

    cout << "\033[1;33m+---------------+--------------------------------+\033[0m" << endl;
    sqlite3_finalize(stmt);
}

void Database::displayStudentCourses(const string& studentId) {
    string sql = "SELECT c.CourseID, c.CourseName FROM Courses c "
                 "JOIN StudentCourses sc ON c.CourseID = sc.CourseID "
                 "WHERE sc.StudentID = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, studentId.c_str(), -1, SQLITE_STATIC);

    cout << "\n\033[1;36mCourses for Student ID " << studentId << ":\033[0m" << endl;
    cout << "\033[1;33m+---------------+--------------------------------+\033[0m" << endl;
    cout << "\033[1;33m| Course ID     | Course Name                     |\033[0m" << endl;
    cout << "\033[1;33m+---------------+--------------------------------+\033[0m" << endl;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << "| " << setw(13) << sqlite3_column_text(stmt, 0) 
             << " | " << setw(30) << sqlite3_column_text(stmt, 1) << " |" << endl;
    }

    cout << "\033[1;33m+---------------+--------------------------------+\033[0m" << endl;
    sqlite3_finalize(stmt);
}

void Database::displayStudentGrades(const string& studentId) {
    // First check if student has any courses
    string checkSql = "SELECT COUNT(*) FROM StudentCourses WHERE StudentID = ?";
    sqlite3_stmt* checkStmt;
    sqlite3_prepare_v2(db, checkSql.c_str(), -1, &checkStmt, nullptr);
    sqlite3_bind_text(checkStmt, 1, studentId.c_str(), -1, SQLITE_STATIC);
    
    if (sqlite3_step(checkStmt) == SQLITE_ROW) {
        int courseCount = sqlite3_column_int(checkStmt, 0);
        if (courseCount == 0) {
            cout << "\n\033[1;31mThis student has not enrolled in any courses yet.\033[0m" << endl;
            sqlite3_finalize(checkStmt);
            return;
        }
    }
    sqlite3_finalize(checkStmt);

    // Continue with existing grade display logic
    string sql = "SELECT c.CourseName, g.Score, g.Grade "
                 "FROM Grades g "
                 "JOIN Courses c ON g.CourseID = c.CourseID "
                 "WHERE g.StudentID = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, studentId.c_str(), -1, SQLITE_STATIC);

    cout << "\n\033[1;36mGrades for Student ID " << studentId << ":\033[0m" << endl;
    cout << "\033[1;33m+----------------------+-------+-------+\033[0m" << endl;
    cout << "\033[1;33m| Course Name         | Score | Grade |\033[0m" << endl;
    cout << "\033[1;33m+----------------------+-------+-------+\033[0m" << endl;

    bool hasGrades = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        hasGrades = true;
        cout << "| " << setw(20) << sqlite3_column_text(stmt, 0) << " | "
             << setw(5) << sqlite3_column_double(stmt, 1) << " | "
             << setw(5) << sqlite3_column_text(stmt, 2) << " |" << endl;
    }

    if (!hasGrades) {
        cout << "\n\033[1;33mThis student is enrolled in courses but has no grades yet.\033[0m" << endl;
    } else {
        cout << "\033[1;33m+----------------------+-------+-------+\033[0m" << endl;
    }
    
    sqlite3_finalize(stmt);
}




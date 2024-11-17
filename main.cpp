#include <iostream>
#include <iomanip>
#include "Student.h"
#include "Grade.h"
#include "Database.h"
#include "Report.h"

using namespace std;

#define RESET   "\033[0m"
#define BLUE    "\033[1;34m"
#define GREEN   "\033[1;32m" 
#define CYAN    "\033[1;36m"
#define MAGENTA "\033[1;35m"
#define YELLOW  "\033[1;33m"
#define RED     "\033[1;31m"

// Function declarations
void displayMenu();
void manageStudents(Database &db);
void enterGrades(Database &db);
void generateReports(Database &db);
void displayLogo();
bool continueOrExit();
void manageCourses(Database &db);
void clearScreen();

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Main program
int main() {
    Database db;
    string username, password;

    displayLogo();
    cout << CYAN "                                   ╔══════════════════════════════════════════════╗" << endl;
    cout << CYAN "                                   ║        Student Management System Login       ║" << endl;
    cout << CYAN "                                   ╚══════════════════════════════════════════════╝" RESET << endl;

    cout << "\n" << BLUE "👤 Username: " RESET;
    cin >> username;
    cout << BLUE "🔒 Password: " RESET;
    cin >> password;

    if (!db.login(username, password)) {
        cout << RED "⚠️  Invalid credentials. Access denied." RESET << endl;
        return 1;
    }

    cout << GREEN "✅ Login successful! Welcome to the system.\n" RESET << endl;

    int choice;
    do {
        displayMenu();
        cout << CYAN "📌 Please select an option: " RESET;
        cin >> choice;

        switch (choice) {
            case 1: manageStudents(db); break;
            case 2: enterGrades(db); break;
            case 3: generateReports(db); break;
            case 4: manageCourses(db); break;
            case 5:
                cout << GREEN "\n🎓 Thank you for using our system! Have a great day! 🎓" RESET << endl;
                return 0;
            default:
                cout << RED "\n❌ Invalid selection. Please try again." RESET << endl;
        }
    } while (continueOrExit());

    cout << GREEN "\n🎓 System shutting down. Goodbye! 🎓" RESET << endl;
    return 0;
}

// Display functions
void displayLogo() {
    cout << BLUE;
    cout << R"(
                                            ███████╗████████╗██╗   ██╗██████╗ ███████╗███╗   ██╗████████╗
                                            ██╔════╝╚══██╔══╝██║   ██║██╔══██╗██╔════╝████╗  ██║╚══██╔══╝
                                            ███████╗   ██║   ██║   ██║██║  ██║█████╗  ██╔██╗ ██║   ██║   
                                            ╚════██║   ██║   ██║   ██║██║  ██║██╔══╝  ██║╚██╗██║   ██║   
                                            ███████║   ██║   ╚██████╔╝██████╔╝███████╗██║ ╚████║   ██║   
                                            ╚══════╝   ╚═╝    ╚═════╝ ╚═════╝ ╚══════╝╚═╝  ╚═══╝   ╚═╝   
                                                                                                        
                                            ███╗   ███╗ █████╗ ███╗   ██╗ █████╗  ██████╗ ███████╗███╗   ███╗███████╗███╗   ██╗████████╗
                                            ████╗ ████║██╔══██╗████╗  ██║██╔══██╗██╔════╝ ██╔════╝████╗ ████║██╔════╝████╗  ██║╚══██╔══╝
                                            ██╔████╔██║███████║██╔██╗ ██║███████║██║  ███╗█████╗  ██╔████╔██║█████╗  ██╔██╗ ██║   ██║   
                                            ██║╚██╔╝██║██╔══██║██║╚██╗██║██╔══██║██║   ██║██╔══╝  ██║╚██╔╝██║██╔══╝  ██║╚██╗██║   ██║   
                                            ██║ ╚═╝ ██║██║  ██║██║ ╚████║██║  ██║╚██████╔╝███████╗██║ ╚═╝ ██║███████╗██║ ╚████║   ██║   
                                            ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═══╝   ╚═╝   

                                                                                █████╗ ██╗   ██╗██████╗ ██████╗ 
                                                                                ██╔══██╗██║   ██║██╔══██╗██╔══██╗
                                                                                ███████║██║   ██║██████╔╝██████╔╝
                                                                                ██╔══██║██║   ██║██╔═══╝ ██╔═══╝ 
                                                                                ██║  ██║╚██████╔╝██║     ██║     
                                                                                ╚═╝  ╚═╝ ╚═════╝ ╚═╝     ╚═╝     )" << endl;
    cout << RESET "\n" << endl;
}

void displayMenu() {
    clearScreen();
    cout << BLUE;
    cout << "                                            ╔═══════════════════════════════════════════════════╗\n";
    cout << "                                            ║              MAIN MENU DASHBOARD                  ║\n";
    cout << "                                            ╠═══════════════════════════════════════════════════╣\n";
    cout << "                                            ║                                                   ║\n";
    cout << "                                            ║  [1] 📚 Student Management                        ║\n";
    cout << "                                            ║  [2] 📝 Grade Entry Portal                        ║\n";
    cout << "                                            ║  [3] 📊 Analytics & Reports                       ║\n";
    cout << "                                            ║  [4] 📚 Course Management                         ║\n";
    cout << "                                            ║  [5] 🚪 Exit System                               ║\n";
    cout << "                                            ║                                                   ║\n";
    cout << "                                            ╚═══════════════════════════════════════════════════╝\n" << RESET;
}

bool continueOrExit() {
    char choice;
    cout << CYAN "\n↩️  Return to main menu? (Y/N): " RESET;
    cin >> choice;
    return (choice == 'Y' || choice == 'y');
}

// Core functionality
void manageStudents(Database &db) {
    cout << BLUE;
    cout << "\n                                          ╔═══════════════════════════════════════════╗\n";
    cout << "                                            ║          STUDENT MANAGEMENT PORTAL        ║\n";
    cout << "                                            ╠═══════════════════════════════════════════╣\n";
    cout << "                                            ║ [1] ➕ Add New Student                     ║\n";
    cout << "                                            ║ [2] 📋 View Student Directory              ║\n";
    cout << "                                            ║ [3] ✏️  Update Student Information         ║\n";
    cout << "                                            ║ [4] 🗑️  Remove Student Record              ║\n";
    cout << "                                            ╚════════════════════════════════════════════╝\n" << RESET;

    int option;
    string id, name;

    cout << CYAN "Select operation: " RESET;
    cin >> option;

    switch(option) {
        case 1: {
            cout << BLUE "📝 Enter Student ID: " RESET;
            cin >> id;
            
            if (db.studentExists(id)) {
                cout << RED "❌ Student with ID " << id << " already exists in the system." RESET << endl;
                break;
            }     
            cout << BLUE "📝 Enter Student Name: " RESET;
            cin.ignore();
            getline(cin, name);
            db.addStudent(Student(id, name));
            cout << GREEN "✅ Student successfully added to system." RESET << endl;
            break;
        }
        case 2:
            cout << YELLOW "\n📋 STUDENT DIRECTORY" RESET << endl;
            db.displayAllStudents();
            break;
        case 3: {
            cout << BLUE "🔍 Enter Student ID to update: " RESET;
            cin >> id;
            cout << BLUE "📝 Enter new name: " RESET;
            cin.ignore();
            getline(cin, name);
            if (db.updateStudent(id, name)) {
                cout << GREEN "✅ Student record updated successfully." RESET << endl;
            } else {
                cout << RED "❌ Update failed - Student not found." RESET << endl;
            }
            break;
        }
        case 4: {
            cout << BLUE "🔍 Enter Student ID to remove: " RESET;
            cin >> id;
            if (db.deleteStudent(id)) {
                cout << GREEN "✅ Student record removed successfully." RESET << endl;
            } else {
                cout << RED "❌ Deletion failed - Student not found." RESET << endl;
            }
            break;
        }
        default:
            cout << RED "❌ Invalid selection. Please try again." RESET << endl;
    }
}

void enterGrades(Database &db) {
    cout << BLUE;
    cout << "\n                                          ╔═══════════════════════════════════════════╗\n";
    cout << "                                            ║            GRADE ENTRY PORTAL             ║\n";
    cout << "                                            ╚═══════════════════════════════════════════╝\n" << RESET;

    string studentId;
    cout << BLUE "🔍 Enter Student ID: " RESET;
    cin >> studentId;
    
    Student student = db.findStudentById(studentId);
    if (student.getId() == "") {
        cout << RED "❌ Error: Student ID not found in system." RESET << endl;
        return;
    }

    cout << YELLOW "\n📚 Available Courses for " << student.getName() << ":" RESET << endl;
    db.displayStudentCourses(studentId);

    int numCourses;
    cout << BLUE "\n📝 Number of courses to grade (max 5): " RESET;
    cin >> numCourses;

    if (numCourses <= 0 || numCourses > 5) {
        cout << RED "❌ Invalid number of courses. Please enter 1-5." RESET << endl;
        return;
    }

    for (int i = 0; i < numCourses; i++) {
        cout << YELLOW "\n📝 Grade Entry #" << (i + 1) << RESET << endl;
        
        string courseId;
        double score;

        cout << BLUE "🔍 Enter Course ID: " RESET;
        cin >> courseId;

        if (!db.courseExists(courseId)) {
            cout << RED "❌ Error: Course does not exist." RESET << endl;
            i--;
            continue;
        }

        cout << BLUE "📝 Enter Score (0-100): " RESET;
        cin >> score;

        if (score < 0.0 || score > 100.0) {
            cout << RED "❌ Invalid score. Must be 0-100." RESET << endl;
            i--;
            continue;
        }

        db.addGrade(Grade(studentId, courseId, score));
    }

    cout << YELLOW "\n📊 Updated Grades for " << student.getName() << ":" RESET << endl;
    db.displayStudentGrades(studentId);
}

void generateReports(Database &db) {
    cout << BLUE;
    cout << "\n                                          ╔═══════════════════════════════════════════╗\n";
    cout << "                                            ║         ANALYTICS & REPORTS PORTAL        ║\n";
    cout << "                                            ╚═══════════════════════════════════════════╝\n" << RESET;
    
    string studentId;
    cout << BLUE "🔍 Enter Student ID: " RESET;
    cin >> studentId;
    
    Report report;
    report.generateReport(studentId, db);
}

void manageCourses(Database &db) {
    clearScreen();
    cout << BLUE;
    cout << "                                            ╔═══════════════════════════════════════════╗\n";
    cout << "                                            ║          COURSE MANAGEMENT PORTAL         ║\n";
    cout << "                                            ╠═══════════════════════════════════════════╣\n";
    cout << "                                            ║ [1] ➕ Add New Course                     ║\n";
    cout << "                                            ║ [2] 📋 View Course Catalog                ║\n";
    cout << "                                            ║ [3] 🔗 Assign Course to Student           ║\n";
    cout << "                                            ║ [4] 📊 View Student Course Load           ║\n";
    cout << "                                            ╚═══════════════════════════════════════════╝\n" << RESET;

    int option;
    string courseId, courseName, studentId;

    cout << CYAN "Select operation: " RESET;
    cin >> option;
    cin.ignore();

    switch (option) {
        case 1: {
            cout << BLUE "📝 Number of courses to add: " RESET;
            int numCourses;
            cin >> numCourses;
            cin.ignore();

            for (int i = 0; i < numCourses; i++) {
                cout << BLUE "\n📚 Course #" << (i + 1) << RESET << endl;
                cout << BLUE "📝 Course ID: " RESET;
                getline(cin, courseId);
                cout << BLUE "📝 Course Name: " RESET;
                getline(cin, courseName);
                
                if (db.addCourse(courseId, courseName)) {
                    cout << GREEN "✅ Course added successfully." RESET << endl;
                } else {
                    cout << RED "❌ Failed to add course." RESET << endl;
                }
            }
            break;
        }
        case 2:
            cout << YELLOW "\n📚 COURSE CATALOG" RESET << endl;
            db.displayAllCourses();
            break;
        case 3: {
            cout << BLUE "🔍 Student ID: " RESET;
            getline(cin, studentId);
            
            if (!db.studentExists(studentId)) {
                cout << RED "❌ Student not found." RESET << endl;
                break;
            }

            cout << YELLOW "\n📚 Available Courses:" RESET << endl;
            db.displayAllCourses();

            cout << BLUE "\n📝 Number of courses to assign: " RESET;
            int numCourses;
            cin >> numCourses;
            cin.ignore();

            for (int i = 0; i < numCourses; i++) {
                cout << BLUE "\n📚 Assignment #" << (i + 1) << RESET << endl;
                cout << BLUE "🔍 Course ID: " RESET;
                getline(cin, courseId);
                
                if (db.assignCourseToStudent(studentId, courseId)) {
                    cout << GREEN "✅ Course assigned successfully." RESET << endl;
                } else {
                    cout << YELLOW "\nTry another course? (Y/N): " RESET;
                    char retry;
                    cin >> retry;
                    cin.ignore();
                    if (toupper(retry) == 'Y') i--;
                }
            }
            
            cout << YELLOW "\n📋 Final Course Load:" RESET << endl;
            db.displayStudentCourses(studentId);
            break;
        }
        case 4:
            cout << BLUE "🔍 Student ID: " RESET;
            getline(cin, studentId);
            db.displayStudentCourses(studentId);
            break;
        default:
            cout << RED "❌ Invalid selection." RESET << endl;
    }
}

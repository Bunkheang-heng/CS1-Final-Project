#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include "Student.h"
#include "Grade.h"
#include "Database.h"
#include "Report.h"

using namespace std;
using namespace chrono_literals;

void typeEffect(const string &message, int delay = 50) {
    for (char ch : message) {
        cout << ch << flush;
        this_thread::sleep_for(chrono::milliseconds(delay));
    }
    cout << endl;
}

// Loading effect
void loadingAnimation(const string &message = "Loading") {
    typeEffect(message, 100);
    for (int i = 0; i < 3; ++i) {
        cout << "." << flush;
        this_thread::sleep_for(500ms);
    }
    cout << endl;
}

void displayMenu();
void manageStudents(Database &db);
void enterGrades(Database &db);
void generateReports(Database &db);
void displayLogo();
void displayFooter();
bool continueOrExit();
void displayLoadingBar();

int main() {
    Database db;
    string username, password;

    // Animated login prompt
    typeEffect("=== Welcome to the \033[1;34mStudent Management System\033[0m ===\n");
    typeEffect("\033[1;36mPlease log in to access the system.\033[0m");

    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    // Loading effect
    displayLoadingBar();

    // Check login
    if (!db.login(username, password)) {
        typeEffect("\033[1;31mInvalid username or password. Exiting...\033[0m", 40);
        return 1;
    }

    typeEffect("\033[1;32mLogin successful! Access granted.\033[0m\n");

    int choice;
    bool firstRun = true;

    do {
        if (firstRun) {
            displayLogo();  // Show the logo only on the first run
            firstRun = false;
        }

        displayMenu();
        displayFooter();
        typeEffect("\033[1;36mEnter your choice: \033[0m", 30);
        cin >> choice;

        switch (choice) {
            case 1:
                manageStudents(db);
                break;
            case 2:
                enterGrades(db);
                break;
            case 3:
                generateReports(db);
                break;
            case 4:
                typeEffect("\n\033[1;32m🎓 Thank you for using the Student Exam Grade Management System! 🎓\033[0m");
                return 0;
            default:
                typeEffect("\n\033[1;31m❌ Invalid choice. Please try again.\033[0m", 30);
                break;
        }
    } while (continueOrExit());

    typeEffect("\n\033[1;32m🎓 Thank you for using the Student Exam Grade Management System! 🎓\033[0m");
    return 0;
}

void displayLogo() {
    cout << "\033[1;34m";  // Bright Blue color
    typeEffect(R"(
                                                  ____  _             _            _     ____                _
                                                 / ___|| |_ _   _  __| | ___ _ __ | |_  / ___|_ __ __ _  ___| | ___
                                                 \___ \| __| | | |/ _` |/ _ \ '_ \| __| | |  _| '__/ _` |/ __| |/ _ \
                                                  ___) | |_| |_| | (_| |  __/ | | | |_  | |_| | | | (_| | (__| |  __/
                                                 |____/ \__|\__,_|\__,_|\___|_| |_|\__|  \____|_|  \__,_|\___|_|\___|

                                                      __  __                                                  _
                                                     |  \/  | __ _ _ __   __ _  __ _  ___ _ __ ___   ___ _ __ | |_
                                                     | |\/| |/ _` | '_ \ / _` |/ _` |/ _ \ '_ ` _ \ / _ \ '_ \| __|
                                                     | |  | | (_| | | | | (_| | (_| |  __/ | | | | |  __/ | | | |_
                                                     |_|  |_|\__,_|_| |_|\__,_|\__, |\___|_| |_| |_|\___|_| |_|\__|
                                                                               |___/
                                                         ____            _
                                                        / ___| _   _ ___| |_ ___ _ __ ___
                                                        \___ \| | | / __| __/ _ \ '_ ` _ \
                                                         ___) | |_| \__ \ ||  __/ | | | | |
                                                        |____/ \__, |___/\__\___|_| |_| |_|
                                                               |___/
    )", 1);
    cout << "\033[0m\n" << endl;
}

void displayMenu() {
    typeEffect("\n\033[1;33m--- Main Menu ---\033[0m", 20);
    cout << "\033[1;32m1. 📚 Manage Students\033[0m" << endl;
    cout << "\033[1;32m2. 📝 Enter Grades\033[0m" << endl;
    cout << "\033[1;32m3. 📊 Generate Reports\033[0m" << endl;
    cout << "\033[1;32m4. 🚪 Exit\033[0m\n" << endl;
}

void displayFooter() {
    cout << "\033[1;35m" << setfill('=') << setw(60) << "" << "\033[0m\n" << endl;
}

bool continueOrExit() {
    char choice;
    typeEffect("\n\033[1;36mWould you like to return to the main menu? (Y/N): \033[0m", 20);
    cin >> choice;
    return (choice == 'Y' || choice == 'y');
}

void manageStudents(Database &db) {
    int option;
    string id, name;

    typeEffect("\n\033[1;34m--- Manage Students ---\033[0m", 20);
    cout << "1. ➕ Add Student" << endl;
    cout << "2. 📋 Display All Students" << endl;
    cout << "3. ✏️ Update Student" << endl;
    cout << "4. 🗑️ Delete Student" << endl;
    cout << "Enter your option: ";
    cin >> option;

    if (option == 1) {
        cout << "Enter Student ID: ";
        cin >> id;
        cout << "Enter Student Name: ";
        cin.ignore();
        getline(cin, name);
        db.addStudent(Student(id, name));
        typeEffect("\033[1;32m✅ Student added successfully.\033[0m");
    } else if (option == 2) {
        typeEffect("\033[1;33m--- All Students ---\033[0m");
        db.displayAllStudents();
    } else if (option == 3) {
        cout << "Enter Student ID to update: ";
        cin >> id;
        cout << "Enter new name: ";
        cin.ignore();
        getline(cin, name);
        if (db.updateStudent(id, name)) {
            typeEffect("\033[1;32m✅ Student updated successfully.\033[0m");
        } else {
            typeEffect("\033[1;31m❌ Failed to update student.\033[0m");
        }
    } else if (option == 4) {
        cout << "Enter Student ID to delete: ";
        cin >> id;
        if (db.deleteStudent(id)) {
            typeEffect("\033[1;32m✅ Student deleted successfully.\033[0m");
        } else {
            typeEffect("\033[1;31m❌ Failed to delete student.\033[0m");
        }
    } else {
        typeEffect("\033[1;31m❌ Invalid option. Please select a valid choice.\033[0m");
    }
}

void enterGrades(Database &db) {
    string id;
    double score;

    cout << "\n\033[1;34m--- Enter Grades ---\033[0m" << endl;
    cout << "Enter Student ID: ";
    cin >> id;
    Student student = db.findStudentById(id);

    if (student.getId() != "") {
        cout << "Enter Score (0-100): ";
        cin >> score;

        // Validate score range
        if (score < 0.0 || score > 100.0) {
            cout << "\033[1;31m❌ Invalid score. Please enter a score between 0 and 100.\033[0m" << endl;
            return;
        }

        Grade grade(id, score);
        db.addGrade(grade);

        cout << "\033[1;32m✅ Grade added successfully.\033[0m" << endl;
        cout << "Assigned Grade: " << grade.getLetterGrade() << endl;
    } else {
        cout << "\033[1;31m❌ Student not found.\033[0m" << endl;
    }
}

void displayLoadingBar() {
    cout << "\n\033[1;32mPlease wait a second...\033[0m\n" << endl;
    cout << "\033[1;34m[";
    for (int i = 0; i < 30; ++i) {
        this_thread::sleep_for(chrono::milliseconds(100));  // Adjust speed here
        cout << "=" << flush;
    }
    cout << "]\033[0m" << endl;
}

void generateReports(Database &db) {
    typeEffect("\n\033[1;34m--- Generate Reports ---\033[0m", 20);
    db.displayAllGrades();
    typeEffect("\033[1;32m✅ Report generated successfully.\033[0m");
}

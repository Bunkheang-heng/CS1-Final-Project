#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include "Student.h"
#include "Grade.h"
#include "Database.h"
#include "Report.h"

using namespace std::chrono_literals;

// Helper function for typing animation
void typeEffect(const std::string &message, int delay = 50) {
    for (char ch : message) {
        std::cout << ch << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
    std::cout << std::endl;
}

// Loading effect
void loadingAnimation(const std::string &message = "Loading") {
    typeEffect(message, 100);
    for (int i = 0; i < 3; ++i) {
        std::cout << "." << std::flush;
        std::this_thread::sleep_for(500ms);
    }
    std::cout << std::endl;
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
    std::string username, password;

    // Animated login prompt
    typeEffect("=== Welcome to the \033[1;34mStudent Management System\033[0m ===\n");
    typeEffect("\033[1;36mPlease log in to access the system.\033[0m");

    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

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
        std::cin >> choice;

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
    std::cout << "\033[1;34m";  // Bright Blue color
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
    )", 1);  // Faster animation for the logo
    std::cout << "\033[0m\n" << std::endl;  // Reset color
}

void displayMenu() {
    typeEffect("\n\033[1;33m--- Main Menu ---\033[0m", 20);
    std::cout << "\033[1;32m1. 📚 Manage Students\033[0m" << std::endl;
    std::cout << "\033[1;32m2. 📝 Enter Grades\033[0m" << std::endl;
    std::cout << "\033[1;32m3. 📊 Generate Reports\033[0m" << std::endl;
    std::cout << "\033[1;32m4. 🚪 Exit\033[0m\n" << std::endl;
}

void displayFooter() {
    std::cout << "\033[1;35m" << std::setfill('=') << std::setw(60) << "" << "\033[0m\n" << std::endl;
}

bool continueOrExit() {
    char choice;
    typeEffect("\n\033[1;36mWould you like to return to the main menu? (Y/N): \033[0m", 20);
    std::cin >> choice;
    return (choice == 'Y' || choice == 'y');
}

void manageStudents(Database &db) {
    int option;
    std::string id, name;

    typeEffect("\n\033[1;34m--- Manage Students ---\033[0m", 20);
    std::cout << "1. ➕ Add Student" << std::endl;
    std::cout << "2. 📋 Display All Students" << std::endl;
    std::cout << "3. ✏️ Update Student" << std::endl;
    std::cout << "4. 🗑️ Delete Student" << std::endl;
    std::cout << "Enter your option: ";
    std::cin >> option;

    if (option == 1) {
        std::cout << "Enter Student ID: ";
        std::cin >> id;
        std::cout << "Enter Student Name: ";
        std::cin.ignore();
        std::getline(std::cin, name);
        db.addStudent(Student(id, name));
        typeEffect("\033[1;32m✅ Student added successfully.\033[0m");
    } else if (option == 2) {
        typeEffect("\033[1;33m--- All Students ---\033[0m");
        db.displayAllStudents();
    } else if (option == 3) {
        std::cout << "Enter Student ID to update: ";
        std::cin >> id;
        std::cout << "Enter new name: ";
        std::cin.ignore();
        std::getline(std::cin, name);
        if (db.updateStudent(id, name)) {
            typeEffect("\033[1;32m✅ Student updated successfully.\033[0m");
        } else {
            typeEffect("\033[1;31m❌ Failed to update student.\033[0m");
        }
    } else if (option == 4) {
        std::cout << "Enter Student ID to delete: ";
        std::cin >> id;
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
    std::string id;
    double score;

    std::cout << "\n\033[1;34m--- Enter Grades ---\033[0m" << std::endl;
    std::cout << "Enter Student ID: ";
    std::cin >> id;
    Student student = db.findStudentById(id);

    if (student.getId() != "") {
        std::cout << "Enter Score (0-100): ";
        std::cin >> score;

        // Validate score range
        if (score < 0.0 || score > 100.0) {
            std::cout << "\033[1;31m❌ Invalid score. Please enter a score between 0 and 100.\033[0m" << std::endl;
            return;
        }

        // Create Grade object with the score (letter grade calculated internally)
        Grade grade(id, score);
        db.addGrade(grade);

        std::cout << "\033[1;32m✅ Grade added successfully.\033[0m" << std::endl;
        std::cout << "Assigned Grade: " << grade.getLetterGrade() << std::endl;
    } else {
        std::cout << "\033[1;31m❌ Student not found.\033[0m" << std::endl;
    }
}

void displayLoadingBar() {
    std::cout << "\n\033[1;32mPlease wait a second...\033[0m\n" << std::endl;
    std::cout << "\033[1;34m[";
    for (int i = 0; i < 30; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Adjust speed here
        std::cout << "=" << std::flush;
    }
    std::cout << "]\033[0m" << std::endl;
}

void generateReports(Database &db) {
    typeEffect("\n\033[1;34m--- Generate Reports ---\033[0m", 20);
    db.displayAllGrades();
    typeEffect("\033[1;32m✅ Report generated successfully.\033[0m");
}

# Student Management System

A console-based C++ application designed to manage student information and exam grades. This system allows administrators to add, update, and delete student records, enter scores that are converted into letter grades automatically, and generate reports. It features SQLite for data storage and includes a secure login interface.

## Features

- **Login System**: Secure login for authorized access.
- **Student Management**: Add, update, view, and delete student records.
- **Grade Entry**: Enter scores which are then converted into letter grades (A, B, C, etc.).
- **Report Generation**: Generate and view reports of students' grades in a structured format.
- **Data Storage**: Uses SQLite for persistent data storage.

## Grade Calculation

Grades are automatically assigned based on scores:
- **A**: 85-100
- **B**: 70-84
- **C**: 50-69
- **D**: Below 50

## Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/your-username/student-management-system.git
   cd student-management-system
   ```

2. **Dependencies**:
   - Ensure you have SQLite and a compatible C++ compiler (GCC, Clang, or similar).
   - This project uses C++20. Make sure your compiler supports this version.

3. **Build the Project**:
   - Using **CMake**:
     ```bash
     mkdir build
     cd build
     cmake ..
     make
     ```

## Usage

1. **Run the Application**:
   ```bash
   ./SMS
   ```

2. **Login**:
   - The default login credentials are:
     - **Username**: `admin`
     - **Password**: `password`
   - Update the credentials in the database if needed.

3. **Navigate through the menu**:
   - Use the options provided in the menu to manage students, enter grades, and generate reports.

## Database Schema

This application uses an SQLite database with the following tables:

- **Students**:
  - `ID`: Unique identifier for each student.
  - `Name`: Full name of the student.

- **Grades**:
  - `StudentID`: References the `ID` in the Students table.
  - `Score`: Numerical score for the student's performance.
  - `Grade`: Letter grade (A, B, C, etc.) based on the score.

- **Users**:
  - `Username`: Unique username for login.
  - `Password`: Password for login.

## Screenshots

Include relevant screenshots here, if possible. For example:

- Login screen
- Main menu
- Grade entry and report generation

## Contributing

Feel free to fork this repository and make improvements. Contributions are welcome!

1. Fork the project.
2. Create your feature branch (`git checkout -b feature/YourFeature`).
3. Commit your changes (`git commit -m 'Add your feature'`).
4. Push to the branch (`git push origin feature/YourFeature`).
5. Open a pull request.

## License

Distributed under the MIT License. See `LICENSE` for more information.

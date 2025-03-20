#include <iostream>
#include <cstring>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

const int MAX_STUDENTS = 100;
const int MAX_NAME_LENGTH = 50;
const int MAX_COURSE_LENGTH = 50;

// Admin credentials (hashed password)
const string ADMIN_USERNAME = "Lucifer";
const string ADMIN_PASSWORD_HASH = "6969";

// ANSI color codes
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

struct Student {
    int id;
    char name[MAX_NAME_LENGTH];
    int age;
    char course[MAX_COURSE_LENGTH];
};

Student students[MAX_STUDENTS];
int studentCount = 0;

// Function to center text
void printCentered(const string &text, int width, const string &color = WHITE) {
    int padding = (width - text.length()) / 2;
    for (int i = 0; i < padding; ++i) {
        cout << " ";
    }
    cout << color << text << RESET << endl;
}

// Function to check if ID is unique
bool isUniqueID(int id) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id) {
            return false;
        }
    }
    return true;
}

// Simple hash function (for illustration purposes only)
string simpleHash(const string &input) {
    hash<string> hasher;
    return to_string(hasher(input));
}

// Function to validate admin login
bool adminLogin() {
    string username, password;
    printCentered("", 100, BLUE);
    printCentered("Admin Login", 100, CYAN);
    printCentered("", 100, BLUE);

    printCentered("Enter Username: ", 100, YELLOW);
    cin >> username;
    printCentered("", 100, BLUE);
    printCentered("Enter Password: ", 100, YELLOW);
    cin >> password;
    printCentered("", 100, BLUE);

    return (username == ADMIN_USERNAME && simpleHash(password) == simpleHash(ADMIN_PASSWORD_HASH));
}

// Function to load student records from file
void loadStudentsFromFile() {
    ifstream file("students.txt");
    if (file.is_open()) {
        studentCount = 0;
        while (file >> students[studentCount].id) {
            file.ignore();
            file.getline(students[studentCount].name, MAX_NAME_LENGTH);
            file >> students[studentCount].age;
            file.ignore();
            file.getline(students[studentCount].course, MAX_COURSE_LENGTH);
            studentCount++;
        }
        file.close();
    }
}

// Function to save student records to file
void saveStudentsToFile() {
    ofstream file("students.txt");
    for (int i = 0; i < studentCount; i++) {
        file << students[i].id << "\n";
        file << students[i].name << "\n";
        file << students[i].age << "\n";
        file << students[i].course << "\n";
    }
    file.close();
}

// Function to insert a new student record
void insertStudent() {
    if (studentCount >= MAX_STUDENTS) {
        printCentered("Student record list is full.", 100, RED);
        return;
    }

    int id;
    printCentered("Enter Student ID: ", 100, YELLOW);
    cin >> id;
    while (!isUniqueID(id)) {
        printCentered("ID already exists. Enter a unique Student ID: ", 100, RED);
        cin >> id;
    }
    students[studentCount].id = id;

    cin.ignore();

    printCentered("Enter Student Name: ", 100, YELLOW);
    cin.getline(students[studentCount].name, MAX_NAME_LENGTH);

    printCentered("Enter Student Age: ", 100, YELLOW);
    cin >> students[studentCount].age;
    while (cin.fail() || students[studentCount].age <= 0 || students[studentCount].age > 100) {
        cin.clear();
        cin.ignore(1000, '\n');
        printCentered("Invalid age. Please enter a valid age (1-100): ", 100, RED);
        cin >> students[studentCount].age;
    }
    cin.ignore();

    printCentered("Enter Student Course: ", 100, YELLOW);
    cin.getline(students[studentCount].course, MAX_COURSE_LENGTH);

    studentCount++;
    printCentered("Student record inserted successfully.", 100, GREEN);
    saveStudentsToFile();
}

// Function to view all student records
void viewAllStudents() {
    if (studentCount == 0) {
        printCentered("No student records found.", 100, RED);
        return;
    }
    printCentered("ID\tName\t\tAge\tCourse", 100, CYAN);
    printCentered("-------------------------------------------", 100, BLUE);
    for (int i = 0; i < studentCount; i++) {
        cout << setw(10) << students[i].id << "\t"
             << setw(20) << students[i].name << "\t"
             << setw(5) << students[i].age << "\t"
             << setw(15) << students[i].course << endl;
    }
}

// Function to search for a student record by ID
void searchStudentByID() {
    int id;
    printCentered("Enter Student ID to search: ", 100, YELLOW);
    cin >> id;
    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id) {
            cout << GREEN << "ID: " << students[i].id << ", Name: " << students[i].name
                 << ", Age: " << students[i].age << ", Course: " << students[i].course << RESET << endl;
            return;
        }
    }
    printCentered("Student record not found.", 100, RED);
}

// Function to search for a student record by name
void searchStudentByName() {
    string name;
    cin.ignore();
    printCentered("Enter Student Name to search: ", 100, YELLOW);
    getline(cin, name);

    bool found = false;
    for (int i = 0; i < studentCount; i++) {
        if (name == students[i].name) {
            cout << GREEN << "ID: " << students[i].id << ", Name: " << students[i].name
                 << ", Age: " << students[i].age << ", Course: " << students[i].course << RESET << endl;
            found = true;
        }
    }
    if (!found) {
        printCentered("Student record not found.", 100, RED);
    }
}

// Function to delete a student record
void deleteStudent() {
    int id;
    printCentered("Enter Student ID to delete: ", 100, YELLOW);
    cin >> id;
    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id) {
            for (int j = i; j < studentCount - 1; j++) {
                students[j] = students[j + 1];
            }
            studentCount--;
            printCentered("Student record deleted successfully.", 100, GREEN);
            saveStudentsToFile();
            return;
        }
    }
    printCentered("Student record not found.", 100, RED);
}

// Function to update a student record
void updateStudent() {
    int id;
    printCentered("Enter Student ID to update: ", 100, YELLOW);
    cin >> id;

    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id) {
            cin.ignore();
            printCentered("Enter new Student Name: ", 100, YELLOW);
            cin.getline(students[i].name, MAX_NAME_LENGTH);

            printCentered("Enter new Student Age: ", 100, YELLOW);
            cin >> students[i].age;
            while (cin.fail() || students[i].age <= 0 || students[i].age > 100) {
                cin.clear();
                cin.ignore(1000, '\n');
                printCentered("Invalid age. Please enter a valid age (1-100): ", 100, RED);
                cin >> students[i].age;
            }
            cin.ignore();

            printCentered("Enter new Student Course: ", 100, YELLOW);
            cin.getline(students[i].course, MAX_COURSE_LENGTH);

            printCentered("Student record updated successfully.", 100, GREEN);
            saveStudentsToFile();
            return;
        }
    }
    printCentered("Student record not found.", 100, RED);
}

// Function to sort students by name
void sortStudentsByName() {
    sort(students, students + studentCount, [](const Student &a, const Student &b) {
        return strcmp(a.name, b.name) < 0;
    });
    printCentered("Students sorted by name.", 100, GREEN);
}

// Function to display statistics
void displayStatistics() {
    printCentered("Total Students: " + to_string(studentCount), 100, CYAN);
}

// Main function
int main() {
    loadStudentsFromFile();
    if (!adminLogin()) {
        printCentered("Invalid login. Access denied.", 100, RED);
        return 1;
    }

    int choice;
    do {
        printCentered("", 100, BLUE);
        printCentered("-------------------------Student Management System-----------------------------", 100, CYAN);
        printCentered("1.Insert Student Record", 100, GREEN);
        printCentered("2.View All Student Records", 100, GREEN);
        printCentered("3.Search Student by ID", 100, GREEN);
        printCentered("4.Search Student by Name", 100, GREEN);
        printCentered("5.Delete Student Record", 100, GREEN);
        printCentered("6.Update Student Record", 100, GREEN);
        printCentered("7.Sort Students by Name", 100, GREEN);
        printCentered("8.Display Statistics", 100, GREEN);
        printCentered("0.Exit", 100, GREEN);
        printCentered("Enter your choice: ", 100, YELLOW);
        cin >> choice;

        switch (choice) {
            case 1:
                insertStudent();
                break;
            case 2:
                viewAllStudents();
                break;
            case 3:
                searchStudentByID();
                break;
            case 4:
                searchStudentByName();
                break;
            case 5:
                deleteStudent();
                break;
            case 6:
                updateStudent();
                break;
            case 7:
                sortStudentsByName();
                break;
            case 8:
                displayStatistics();
                break;
            case 0:
                printCentered("Exiting the program...", 100, CYAN);
                break;
            default:
                printCentered("Invalid choice. Please try again.", 100, RED);
        }
    } while (choice != 0);

    return 0;
}

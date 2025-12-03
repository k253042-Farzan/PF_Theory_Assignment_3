#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Student record structure
typedef struct {
    int studentID;
    char fullName[100];
    char batch[20];           // CS, SE, Cyber Security, AI
    char membershipType[10];  // IEEE / ACM
    char registrationDate[11]; // YYYY-MM-DD
    char dateOfBirth[11];     // YYYY-MM-DD
    char interest[20];        // IEEE/ACM/Both
} Student;

// Global variables for in-memory database
Student *database = NULL;
int studentCount = 0;
int capacity = 10;

// Function prototypes
void loadDatabase(const char *filename);
void saveDatabase(const char *filename);
void addStudent(Student s, const char *filename);
void updateStudent(int studentID);
void deleteStudent(int studentID);
void displayAllStudents();
void generateBatchReport();
int isDuplicateID(int studentID);
void initializeDatabase();
void freeDatabase();
void menu();

// Initialize the dynamic array for in-memory storage
void initializeDatabase() {
    database = (Student *)malloc(capacity * sizeof(Student));
    if (database == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for database.\n");
        exit(1);
    }
    studentCount = 0;
}

// Load all student records from file into memory
void loadDatabase(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        if (errno == ENOENT) {
            printf("No existing database found. Starting fresh.\n");
            return;
        } else {
            fprintf(stderr, "Error opening file: %s\n", strerror(errno));
            return;
        }
    }

    // Read number of records first
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    int recordCount = fileSize / sizeof(Student);
    
    // Resize database if needed
    if (recordCount > capacity) {
        capacity = recordCount + 10;
        database = (Student *)realloc(database, capacity * sizeof(Student));
        if (database == NULL) {
            fprintf(stderr, "Error: Memory reallocation failed.\n");
            fclose(file);
            exit(1);
        }
    }

    // Read all records
    studentCount = fread(database, sizeof(Student), recordCount, file);
    
    if (ferror(file)) {
        fprintf(stderr, "Error reading from file.\n");
    } else {
        printf("Successfully loaded %d records from database.\n", studentCount);
    }

    fclose(file);
}

// Save all student records from memory to file
void saveDatabase(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file for writing: %s\n", strerror(errno));
        return;
    }

    size_t written = fwrite(database, sizeof(Student), studentCount, file);
    
    if (written != studentCount) {
        fprintf(stderr, "Error: Failed to write all records to file.\n");
    } else {
        printf("Database saved successfully. %zu records written.\n", written);
    }

    fclose(file);
}

// Check if student ID already exists
int isDuplicateID(int studentID) {
    for (int i = 0; i < studentCount; i++) {
        if (database[i].studentID == studentID) {
            return 1; // Duplicate found
        }
    }
    return 0; // No duplicate
}

// Add a new student record
void addStudent(Student s, const char *filename) {
    // Check for duplicate ID
    if (isDuplicateID(s.studentID)) {
        printf("Error: Student ID %d already exists. Cannot add duplicate.\n", s.studentID);
        return;
    }

    // Expand database if needed
    if (studentCount >= capacity) {
        capacity *= 2;
        database = (Student *)realloc(database, capacity * sizeof(Student));
        if (database == NULL) {
            fprintf(stderr, "Error: Memory reallocation failed while adding student.\n");
            exit(1);
        }
    }

    // Add student to in-memory database
    database[studentCount] = s;
    studentCount++;

    printf("Student %s (ID: %d) added successfully.\n", s.fullName, s.studentID);

    // Save to file
    saveDatabase(filename);
}

// Update a student's information
void updateStudent(int studentID) {
    int found = 0;
    for (int i = 0; i < studentCount; i++) {
        if (database[i].studentID == studentID) {
            found = 1;
            printf("\nCurrent Record:\n");
            printf("ID: %d | Name: %s | Batch: %s | Membership: %s | Interest: %s\n",
                   database[i].studentID, database[i].fullName, 
                   database[i].batch, database[i].membershipType, database[i].interest);

            printf("\nWhat would you like to update?\n");
            printf("1. Batch\n");
            printf("2. Membership Type\n");
            printf("3. Interest\n");
            printf("Enter choice: ");
            int choice;
            scanf("%d", &choice);
            getchar(); // Clear newline

            switch (choice) {
                case 1:
                    printf("Enter new batch (CS/SE/Cyber Security/AI): ");
                    fgets(database[i].batch, sizeof(database[i].batch), stdin);
                    database[i].batch[strcspn(database[i].batch, "\n")] = 0;
                    break;
                case 2:
                    printf("Enter new membership type (IEEE/ACM): ");
                    fgets(database[i].membershipType, sizeof(database[i].membershipType), stdin);
                    database[i].membershipType[strcspn(database[i].membershipType, "\n")] = 0;
                    break;
                case 3:
                    printf("Enter new interest (IEEE/ACM/Both): ");
                    fgets(database[i].interest, sizeof(database[i].interest), stdin);
                    database[i].interest[strcspn(database[i].interest, "\n")] = 0;
                    break;
                default:
                    printf("Invalid choice.\n");
                    return;
            }

            printf("Student record updated successfully.\n");
            saveDatabase("members.dat");
            break;
        }
    }

    if (!found) {
        printf("Error: Student with ID %d not found.\n", studentID);
    }
}

// Delete a student record
void deleteStudent(int studentID) {
    int found = -1;
    for (int i = 0; i < studentCount; i++) {
        if (database[i].studentID == studentID) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("Error: Student with ID %d not found.\n", studentID);
        return;
    }

    // Shift all elements after the deleted one
    for (int i = found; i < studentCount - 1; i++) {
        database[i] = database[i + 1];
    }

    studentCount--;
    printf("Student with ID %d deleted successfully.\n", studentID);

    // Save updated database
    saveDatabase("members.dat");
}

// Display all students
void displayAllStudents() {
    if (studentCount == 0) {
        printf("\nNo students registered yet.\n");
        return;
    }

    printf("\n========== ALL REGISTERED STUDENTS ==========\n");
    printf("%-8s | %-25s | %-15s | %-10s | %-12s | %-12s | %-10s\n",
           "ID", "Name", "Batch", "Member", "Reg Date", "DOB", "Interest");
    printf("--\n");

    for (int i = 0; i < studentCount; i++) {
        printf("%-8d | %-25s | %-15s | %-10s | %-12s | %-12s | %-10s\n",
               database[i].studentID,
               database[i].fullName,
               database[i].batch,
               database[i].membershipType,
               database[i].registrationDate,
               database[i].dateOfBirth,
               database[i].interest);
    }
    printf("Total Students: %d\n\n", studentCount);
}

// Generate batch-wise reports
void generateBatchReport() {
    char batchName[20];
    char interestType[20];

    printf("\nEnter batch (CS/SE/Cyber Security/AI): ");
    fgets(batchName, sizeof(batchName), stdin);
    batchName[strcspn(batchName, "\n")] = 0;

    printf("Enter interest filter (IEEE/ACM/Both/All): ");
    fgets(interestType, sizeof(interestType), stdin);
    interestType[strcspn(interestType, "\n")] = 0;

    printf("\n==== BATCH REPORT: %s =====\n", batchName);
    printf("%-8s | %-25s | %-10s | %-10s\n", "ID", "Name", "Member", "Interest");
    printf("--\n");

    int count = 0;
    for (int i = 0; i < studentCount; i++) {
        int batchMatch = strcmp(database[i].batch, batchName) == 0;
        int interestMatch = (strcmp(interestType, "All") == 0) || 
                           (strcmp(database[i].interest, interestType) == 0);

        if (batchMatch && interestMatch) {
            printf("%-8d | %-25s | %-10s | %-10s\n",
                   database[i].studentID,
                   database[i].fullName,
                   database[i].membershipType,
                   database[i].interest);
            count++;
        }
    }

    printf("Total matching students: %d\n\n", count);
}

// Free allocated memory
void freeDatabase() {
    if (database != NULL) {
        free(database);
        database = NULL;
    }
}

// Main menu
void menu() {
    int choice;
    const char *filename = "members.dat";

    while (1) {
        printf("\n========== IEEE/ACM MEMBERSHIP SYSTEM ==========\n");
        printf("1. Register New Student\n");
        printf("2. Update Student Information\n");
        printf("3. Delete Student Registration\n");
        printf("4. View All Registrations\n");
        printf("5. Generate Batch-Wise Report\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Clear newline

        switch (choice) {
            case 1: {
                Student newStudent;
                printf("\n--- Register New Student ---\n");
                printf("Student ID: ");
                scanf("%d", &newStudent.studentID);
                getchar();

                printf("Full Name: ");
                fgets(newStudent.fullName, sizeof(newStudent.fullName), stdin);
                newStudent.fullName[strcspn(newStudent.fullName, "\n")] = 0;

                printf("Batch (CS/SE/Cyber Security/AI): ");
                fgets(newStudent.batch, sizeof(newStudent.batch), stdin);
                newStudent.batch[strcspn(newStudent.batch, "\n")] = 0;

                printf("Membership Type (IEEE/ACM): ");
                fgets(newStudent.membershipType, sizeof(newStudent.membershipType), stdin);
                newStudent.membershipType[strcspn(newStudent.membershipType, "\n")] = 0;

                printf("Registration Date (YYYY-MM-DD): ");
                fgets(newStudent.registrationDate, sizeof(newStudent.registrationDate), stdin);
                newStudent.registrationDate[strcspn(newStudent.registrationDate, "\n")] = 0;

                printf("Date of Birth (YYYY-MM-DD): ");
                fgets(newStudent.dateOfBirth, sizeof(newStudent.dateOfBirth), stdin);
                newStudent.dateOfBirth[strcspn(newStudent.dateOfBirth, "\n")] = 0;

                printf("Interest (IEEE/ACM/Both): ");
                fgets(newStudent.interest, sizeof(newStudent.interest), stdin);
                newStudent.interest[strcspn(newStudent.interest, "\n")] = 0;

                addStudent(newStudent, filename);
                break;
            }
            case 2: {
                int id;
                printf("Enter Student ID to update: ");
                scanf("%d", &id);
                getchar();
                updateStudent(id);
                break;
            }
            case 3: {
                int id;
                printf("Enter Student ID to delete: ");
                scanf("%d", &id);
                getchar();
                deleteStudent(id);
                break;
            }
            case 4:
                displayAllStudents();
                break;
            case 5:
                generateBatchReport();
                break;
            case 6:
                printf("Saving database and exiting...\n");
                saveDatabase(filename);
                freeDatabase();
                printf("Goodbye!\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

int main() {
    const char *filename = "members.dat";
    
    // Initialize in-memory database
    initializeDatabase();
    
    // Load existing records from file
    loadDatabase(filename);
    
    // Run the menu-driven interface
    menu();
    
    return 0;
}
#include <stdio.h>
#include <string.h>

// Employee Structure
struct Employee {
    int id;
    char name[50];
    char designation[50];
    float salary;
};

// Display all employees
void displayEmployees(struct Employee e[], int n) {
    printf("\nEmployee Records:\n");
    printf("--\n");
    printf("ID\tName\t\tDesignation\tSalary\n");
    printf("--\n");

    for (int i = 0; i < n; i++) {
        printf("%d\t%-10s\t%-10s\t%.2f\n",
               e[i].id, e[i].name, e[i].designation, e[i].salary);
    }
}

// Find highest salary employee
void findHighestSalary(struct Employee e[], int n) {
    int index = 0;

    for (int i = 1; i < n; i++) {
        if (e[i].salary > e[index].salary) {
            index = i;
        }
    }

    printf("\nEmployee with Highest Salary:\n");
    printf("ID: %d\n", e[index].id);
    printf("Name: %s\n", e[index].name);
    printf("Designation: %s\n", e[index].designation);
    printf("Salary: %.2f\n", e[index].salary);
}

// Search employee by ID or Name
void searchEmployee(struct Employee e[], int n) {
    int choice;
    printf("\nSearch Employee:\n1. By ID\n2. By Name\nEnter choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        int id;
        printf("Enter ID: ");
        scanf("%d", &id);

        for (int i = 0; i < n; i++) {
            if (e[i].id == id) {
                printf("Record Found:\nID: %d\nName: %s\nDesignation: %s\nSalary: %.2f\n",
                       e[i].id, e[i].name, e[i].designation, e[i].salary);
                return;
            }
        }
        printf("Employee Not Found!\n");
    }

    else if (choice == 2) {
        char name[50];
        printf("Enter Name: ");
        scanf("%s", name);

        for (int i = 0; i < n; i++) {
            if (strcmp(e[i].name, name) == 0) {
                printf("Record Found:\nID: %d\nName: %s\nDesignation: %s\nSalary: %.2f\n",
                       e[i].id, e[i].name, e[i].designation, e[i].salary);
                return;
            }
        }
        printf("Employee Not Found!\n");
    }

    else {
        printf("Invalid choice!\n");
    }
}

// Bonus function (Update Salary by Reference)
void applyBonus(struct Employee e[], int n, float threshold) {
    for (int i = 0; i < n; i++) {
        if (e[i].salary < threshold) {
            e[i].salary = e[i].salary + (e[i].salary * 0.10);
        }
    }
}

int main() {
    int n;

    printf("Enter number of employees: ");
    scanf("%d", &n);

    struct Employee e[n];

    printf("\nEnter Employee Details:\n");
    for (int i = 0; i < n; i++) {
        printf("\nEmployee %d:\n", i + 1);

        printf("ID: ");
        scanf("%d", &e[i].id);

        printf("Name: ");
        scanf("%s", e[i].name);

        printf("Designation: ");
        scanf("%s", e[i].designation);

        printf("Salary: ");
        scanf("%f", &e[i].salary);
    }

    displayEmployees(e, n);
    findHighestSalary(e, n);
    searchEmployee(e, n);

    // Example: Apply bonus to employees earning below 50,000
    applyBonus(e, n, 50000);

    printf("\nAfter Applying Bonus (Salary < 50000 gets 10%% increase):\n");
    displayEmployees(e, n);

    return 0;
}
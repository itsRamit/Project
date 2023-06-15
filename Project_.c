#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_REGISTRATION_NUMBER_LENGTH 15

typedef struct
{
    char name[MAX_NAME_LENGTH];
    char registrationNumber[MAX_REGISTRATION_NUMBER_LENGTH];
    int grades[5]; // Assuming there are 5 subjects
    float cgpa;
    int rank;
} Student;

typedef struct Node
{
    Student student;
    struct Node *left;
    struct Node *right;
} Node;

Node *root = NULL;
// Function prototypes
void insertNode(Node **root, Student student);
void Ranking(Node *root, int *c);
void displayTree(Node *root);
void searchStudent(Node *root, char *name);
void searchCgpa(Node *root, float grade);
void searchRank(Node *root, int Rank);
void searchgrade(Node *root, int grade);
void generateReportSummary(Node *root, int c);

int main()
{

    // Read data from the Excel data sheet
    FILE *file = fopen("grades.csv", "r");
    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        return 1;
    }

    char line[100]; // assumed that each line of the csv file have atmax 100 characters
    int c = 0;
    while (fgets(line, sizeof(line), file) != NULL) // Reading per line at a time if there is not line it will return NULL
    {
        Student student;

        char *token = strtok(line, ","); // pointing to first token
        strncpy(student.name, token, sizeof(student.name));

        token = strtok(NULL, ","); // pointing to next token
        strncpy(student.registrationNumber, token, sizeof(student.registrationNumber));
        float grade = 0;
        for (int i = 0; i < 5; i++)
        {
            token = strtok(NULL, ","); // pointing to next next token
            student.grades[i] = atoi(token);
            grade += student.grades[i];
        }
        student.cgpa = grade / 5.0;

        insertNode(&root, student);
        c++;
    }
    int x = c + 1;
    Ranking(root, &x);

    fclose(file);
    int choice;
    char searchName[MAX_NAME_LENGTH];
    float cgpa;
    int Rank;
    int grade;
    do
    {

        printf("...........................................................................\n");
        printf("............... WELCOME TO GRADE CARD GENERATION ..........................\n");
        printf("1)DISPLAY ALL STUDENTS GRADE CARD\n");
        printf("2)SEARCH STUDENT'S GRADE CARD BY NAME\n");
        printf("3)SEARCH STUDENT'S GRADE CARD BY CGPA\n");
        printf("4)SEARCH STUDENT'S GRADE CARD BY RANK\n");
        printf("5)SEARCH STUDENTS BY GRADE\n");
        printf("6)DISPLAY CLASS AVG CGPA SCORE\n");
        printf("7)EXIT\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        printf("\n");
        switch (choice)
        {
        case 1:
            displayTree(root);
            break;
        case 2:
            printf("Enter the name of the student to search: ");
            scanf(" %s", searchName);
            searchName[strcspn(searchName, "\n")] = '\0';
            searchStudent(root, searchName);
            break;
        case 3:
            printf("Enter the Cgpa to search: ");
            scanf("%f", &cgpa);
            searchCgpa(root, cgpa);
            break;
        case 4:
            printf("Enter the rank Of the Student: ");
            scanf("%d", &Rank);
            searchRank(root, Rank);
            break;
        case 5:
            printf("Enter the grade to search: ");
            scanf("%d", &grade);
            searchCgpa(root, grade);

            break;
        case 6:
            generateReportSummary(root, c);
            break;
        case 7:
            break;
        default:
            printf("Invalid input!!\n");
        }
        printf("\n");
    } while (choice != 7);

    return 0;
}
void Ranking(Node *root, int *c)
{

    if (root != NULL)
    {

        Ranking(root->left, c);
        (*c)--;
        root->student.rank = *c;
        Ranking(root->right, c);
    }
    return;
}
void insertNode(Node **root, Student student)
{
    if (*root == NULL)
    {
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->student = student;
        newNode->left = NULL;
        newNode->right = NULL;
        *root = newNode;
    }
    else
    {
        if (student.cgpa < (*root)->student.cgpa)
        {
            insertNode(&((*root)->left), student);
        }
        else
        {
            insertNode(&((*root)->right), student);
        }
    }
}

void displayTree(Node *root)
{
    if (root != NULL)
    {

        displayTree(root->left);
        printf("Name: %s, Registration Number: %s, CGPA: %.2f , Rank: %d\n",
               root->student.name, root->student.registrationNumber, root->student.cgpa, root->student.rank);
        displayTree(root->right);
    }
}

void searchStudent(Node *root, char *name)
{
    if (root == NULL)
    {
        return;
    }
    searchStudent(root->left, name);
    int cmp = strcmp(name, root->student.name);
    if (cmp == 0)
    {
        printf("Name: %s, Registration Number: %s, CGPA: %.2f , Rank:%d\n",
               root->student.name, root->student.registrationNumber, root->student.cgpa, root->student.rank);
    }
    searchStudent(root->right, name);
}
void searchRank(Node *root, int Rank)
{
    if (root == NULL)
    {
        return;
    }
    else if (root->student.rank == Rank)
    {
        printf("Name: %s, Registration Number: %s, CGPA: %.2f , Rank:%d\n",
               root->student.name, root->student.registrationNumber, root->student.cgpa, root->student.rank);
    }

    searchRank(root->left, Rank);
    searchRank(root->right, Rank);
}
void searchgrade(Node *root, int grade)
{
    if (root == NULL)
    {
        return;
    }
    else if (root->student.rank == grade)
    {
        printf("Name: %s, Registration Number: %s, CGPA: %.2f , Rank:%d\n",
               root->student.name, root->student.registrationNumber, root->student.cgpa, root->student.rank);
    }

    searchgrade(root->left, grade);
    searchgrade(root->right, grade);
}
void searchCgpa(Node *root, float grade)
{
    if (root == NULL)
    {
        // printf("No students found with grade %d.\n", cgpa);
        return;
    }

    if (root->student.cgpa == grade)
    {
        printf("Name: %s, Registration Number: %s, CGPA: %.2f , Rank:%d\n",
               root->student.name, root->student.registrationNumber, root->student.cgpa, root->student.rank);
    }

    searchCgpa(root->left, grade);
    searchCgpa(root->right, grade);
}

void generateReportSummary(Node *root, int c)
{
    if (root == NULL)
    {
        printf("No students found.\n");
        return;
    }
    static int x = 0;
    static float count = 0.0;
    static float totalCGPA = 0;

    // Traverse the tree in-order
    if (root->left != NULL)
    {
        generateReportSummary(root->left, c);
    }

    count++;
    totalCGPA += root->student.cgpa;

    if (root->right != NULL)
    {
        generateReportSummary(root->right, c);
    }

    float averageCGPA = totalCGPA / count;
    if (count == c && x == 0)

    {
        x++;
        printf("\nClass Average CGPA: %.2f\n", averageCGPA);
    }
}

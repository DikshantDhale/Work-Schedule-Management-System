#include <stdio.h>  
#include <string.h> 


#define EMPLOYEES 1000
#define TEAMS 100
#define TEAM_MEMBERS 5
#define DAYS_IN_WEEK 7
#define NAME_LEN 100
#define DEPT_LEN 50
#define LINE_BUFFER 1024


typedef struct {
    int employeeID;
    char name[NAME_LEN];
    int age;
    char department[DEPT_LEN];
    int weeklyWorkingHours[DAYS_IN_WEEK];
    int weeklyTargetHours;
} Employee;

typedef struct {
    int teamID;
    char teamName[NAME_LEN];
    int memberIDs[TEAM_MEMBERS];
    int memberCount;
    int weeklyTeamTarget;
} Team;


typedef struct {
    int empIndex;
    int totalHours;
} EmpRank;

typedef struct {
    int teamIndex;
    int totalHours;
} TeamRank;


void chompNewline(char* str);

int findEmployeeIndex(Employee empList[], int count, int empID);
int findTeamIndex(Team teamList[], int count, int teamID);
int getEmployeeTotalHours(Employee* emp);
int isEmployeeInAnyTeam(Team teamList[], int teamCount, int empID);
int getTeamTotalHours(Employee empList[], int empCount, Team* team);

void Add_Employee(Employee empList[], int* empCount);
void Create_Team(Team teamList[], int* teamCount, Employee empList[], int empCount);

void quickSortEmpRanks(EmpRank arr[], int low, int high);
void quickSortTeamRanks(TeamRank arr[], int low, int high);
void quickSortInts(int arr[], int low, int high);

void Top_5_Employees(Employee empList[], int empCount);
void Check_Team_Achievement(Team teamList[], int teamCount, Employee empList[], int empCount, int teamID);
void Generate_Team_Ranking(Team teamList[], int teamCount, Employee empList[], int empCount);
void Check_Employee_Reward(Employee empList[], int empCount, int empID);
void Update_or_Delete_Employee(Employee empList[], int* empCount, Team teamList[], int teamCount, int empID);
void Delete_Team(Team teamList[], int* teamCount, int teamID);
void Merge_Teams(Team teamList[], int* teamCount, Employee empList[], int empCount, int teamID1, int teamID2);
void Display_Team_Info(Team teamList[], int teamCount, Employee empList[], int empCount);
void displayAllEmployees(Employee empList[], int empCount);

void loadData(Employee empList[], int* empCount, Team teamList[], int* teamCount);
void saveData(Employee empList[], int empCount, Team teamList[], int teamCount);
void printMenu(void);


Employee employeeList[EMPLOYEES];
Team teamList[TEAMS];
int employeeCount = 0;
int teamCount = 0;


int main() {
    int running = 1; 
    char choice = '\0';
    char inputBuffer[LINE_BUFFER]; 

    loadData(employeeList, &employeeCount, teamList, &teamCount);

    while (running) {
        printMenu();
        
        if (fgets(inputBuffer, sizeof(inputBuffer), stdin) == NULL) {
            choice = 'q'; 
        } else {
            sscanf(inputBuffer, " %c", &choice);
        }


        switch (choice) {
            case 'a':
                Add_Employee(employeeList, &employeeCount);
                break;
            case 'b':
                Create_Team(teamList, &teamCount, employeeList, employeeCount);
                break;
            case 'c':
                Top_5_Employees(employeeList, employeeCount);
                break;
            case 'd': { 
                int teamID = 0;
                printf("Enter Team ID to check: ");
                fgets(inputBuffer, sizeof(inputBuffer), stdin);
                if (sscanf(inputBuffer, "%d", &teamID) == 1) {
                    Check_Team_Achievement(teamList, teamCount, employeeList, employeeCount, teamID);
                } else {
                    printf("Invalid input.\a\n");
                }
                break;
            }
            case 'e':
                Generate_Team_Ranking(teamList, teamCount, employeeList, employeeCount);
                break;
            case 'f': {
                int empID = 0;
                printf("Enter Employee ID to check reward: ");
                fgets(inputBuffer, sizeof(inputBuffer), stdin);
                if (sscanf(inputBuffer, "%d", &empID) == 1) {
                    Check_Employee_Reward(employeeList, employeeCount, empID);
                } else {
                    printf("Invalid input.\a\n");
                }
                break;
            }
            case 'g': {
                int empID = 0;
                printf("Enter Employee ID to update/delete: ");
                fgets(inputBuffer, sizeof(inputBuffer), stdin);
                if (sscanf(inputBuffer, "%d", &empID) == 1) {
                    Update_or_Delete_Employee(employeeList, &employeeCount, teamList, teamCount, empID);
                } else {
                    printf("Invalid input.\a\n");
                }
                break;
            }
            case 'h': {
                int teamID = 0;
                printf("Enter Team ID to delete: ");
                fgets(inputBuffer, sizeof(inputBuffer), stdin);
                if (sscanf(inputBuffer, "%d", &teamID) == 1) {
                    Delete_Team(teamList, &teamCount, teamID);
                } else {
                    printf("Invalid input.\a\n");
                }
                break;
            }
            case 'i': {
                int teamID1 = 0, teamID2 = 0;
                printf("Enter Team ID 1 to merge: ");
                fgets(inputBuffer, sizeof(inputBuffer), stdin);
                if (sscanf(inputBuffer, "%d", &teamID1) == 1) {
                    printf("Enter Team ID 2 to merge: ");
                    fgets(inputBuffer, sizeof(inputBuffer), stdin);
                    if (sscanf(inputBuffer, "%d", &teamID2) == 1) {
                        Merge_Teams(teamList, &teamCount, employeeList, employeeCount, teamID1, teamID2);
                    } else {
                        printf("Invalid input.\a\n");
                    }
                } else {
                    printf("Invalid input.\a\n");
                }
                break;
            }
            case 'j':
                Display_Team_Info(teamList, teamCount, employeeList, employeeCount);
                break;
            case 'k':
                displayAllEmployees(employeeList, employeeCount);
                break;
            case 'q':
                printf("Saving data to files...\n");
                saveData(employeeList, employeeCount, teamList, teamCount);
                printf("Exiting system.\n");
                running = 0; 
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        } 
    } 

    return 0; 
}


void printMenu(void) {
    printf("\n    Work Schedule Management System     \n");
    printf("a. Add Employee\n");
    printf("b. Create Team\n");
    printf("c. Show Top 5 Employees\n");
    printf("d. Check Team Achievement\n");
    printf("e. Generate Team Ranking\n");
    printf("f. Check Employee Reward\n");
    printf("g. Update or Delete Employee\n");
    printf("h. Delete Team\n");
    printf("i. Merge Teams\n");
    printf("j. Display All Team Info\n");
    printf("k. Display All Employees\n");
    printf("q. Save and Quit\n");
    printf("                                                \n");
    printf("Enter your choice: ");
}


void chompNewline(char* str) {
    int i = 0;
    if (str == NULL) return;
    
    while (str[i] != '\0') {
        i++;
    }
    
    if (i > 0) {
        i--;
    }
    
    if (str[i] == '\n') {
        str[i] = '\0'; 
    }
}


int findEmployeeIndex(Employee empList[], int count, int empID) {
    int return_value = -1;
    int found = 0;
    int low = 0;
    int high = (count - 1);

    while (low <= high && !found) {
        int mid = low + (high - low) / 2;
        if (mid < 0 || mid >= count) { 
            low = high + 1;
        } else {
            if (empList[mid].employeeID == empID) {
                return_value = mid;
                found = 1;
            } else if (empList[mid].employeeID < empID) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
    }
    return return_value;
}


int findTeamIndex(Team teamList[], int count, int teamID) {
    int return_value = -1;
    int found = 0;
    int low = 0;
    int high = (count - 1);

    while (low <= high && !found) {
        int mid = low + (high - low) / 2;
        if (mid < 0 || mid >= count) {
            low = high + 1;
        } else {
            if (teamList[mid].teamID == teamID) {
                return_value = mid;
                found = 1;
            } else if (teamList[mid].teamID < teamID) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
    }
    return return_value;
}


int getEmployeeTotalHours(Employee* emp) {
    int total = 0;
    int i = 0;
    for (i = 0; i < DAYS_IN_WEEK; i++) {
        total += emp->weeklyWorkingHours[i];
    }
    return total;
}


int isEmployeeInAnyTeam(Team teamList[], int teamCount, int empID) {
    int return_value = 0;
    int i = 0;
    int j = 0;
    int found = 0;

    for (i = 0; i < teamCount && !found; i++) {
        for (j = 0; j < teamList[i].memberCount && !found; j++) {
            if (teamList[i].memberIDs[j] == empID) {
                return_value = 1;
                found = 1;
            }
        }
    }
    return return_value;
}


int getTeamTotalHours(Employee empList[], int empCount, Team* team) {
    int totalHours = 0;
    int i = 0;
    for (i = 0; i < team->memberCount; i++) {
        int empIndex = findEmployeeIndex(empList, empCount, team->memberIDs[i]);
        if (empIndex != -1) {
            totalHours += getEmployeeTotalHours(&empList[empIndex]);
        }
    }
    return totalHours;
}



void swapEmpRanks(EmpRank* a, EmpRank* b) {
    EmpRank t = *a;
    *a = *b;
    *b = t;
}

int partitionEmpRanksDesc(EmpRank arr[], int low, int high) {
    EmpRank pivot = arr[low]; 
    int i = low - 1;
    int j = high + 1;

    while (1) {
       
        do {
            i++;
        } while (arr[i].totalHours > pivot.totalHours);

        do {
            j--;
        } while (arr[j].totalHours < pivot.totalHours);

        if (i >= j) {
            return j;
        }

        swapEmpRanks(&arr[i], &arr[j]);
    }
}

void quickSortEmpRanks(EmpRank arr[], int low, int high) {
    if (low < high) {
        int pi = partitionEmpRanksDesc(arr, low, high);
        quickSortEmpRanks(arr, low, pi);
        quickSortEmpRanks(arr, pi + 1, high);
    }
}


void swapTeamRanks(TeamRank* a, TeamRank* b) {
    TeamRank t = *a;
    *a = *b;
    *b = t;
}

int partitionTeamRanksDesc(TeamRank arr[], int low, int high) {
    TeamRank pivot = arr[low];
    int i = low - 1;
    int j = high + 1;

    while (1) {
       
        do {
            i++;
        } while (arr[i].totalHours > pivot.totalHours);

        do {
            j--;
        } while (arr[j].totalHours < pivot.totalHours);

        if (i >= j) {
            return j;
        }
        swapTeamRanks(&arr[i], &arr[j]);
    }
}

void quickSortTeamRanks(TeamRank arr[], int low, int high) {
    if (low < high) {
        int pi = partitionTeamRanksDesc(arr, low, high);
        quickSortTeamRanks(arr, low, pi);
        quickSortTeamRanks(arr, pi + 1, high);
    }
}


void swapInts(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partitionIntsAsc(int arr[], int low, int high) {
    int pivot = arr[low];
    int i = low - 1;
    int j = high + 1;

    while (1) {
        
        do {
            i++;
        } while (arr[i] < pivot);

        do {
            j--;
        } while (arr[j] > pivot);

        if (i >= j) {
            return j;
        }
        swapInts(&arr[i], &arr[j]);
    }
}

void quickSortInts(int arr[], int low, int high) {
    if (low < high) {
        int pi = partitionIntsAsc(arr, low, high);
        quickSortInts(arr, low, pi);
        quickSortInts(arr, pi + 1, high);
    }
}


void Add_Employee(Employee empList[], int* empCount) {
    int done = 0;
    int success = 0;
    int i = 0;
    char inputBuffer[LINE_BUFFER]; 

    if (*empCount >= EMPLOYEES) {
        printf("Error: Employee list is full.\n");
        
        done = 1;
    }

    if (!done) {
        Employee newEmp;
        
        newEmp.employeeID = 0; newEmp.name[0] = '\0'; newEmp.age = 0;
        newEmp.department[0] = '\0'; newEmp.weeklyTargetHours = 0;
        for (i = 0; i < DAYS_IN_WEEK; i++) {
            newEmp.weeklyWorkingHours[i] = 0;
        }

        printf("Enter new Employee ID: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        if (sscanf(inputBuffer, "%d", &newEmp.employeeID) != 1) {
            printf("Invalid input.\n");
            done = 1;
        } else {
            if (findEmployeeIndex(empList, *empCount, newEmp.employeeID) != -1) {
                printf("Error: Employee ID %d already exists.\n", newEmp.employeeID);
                done = 1;
            } else {
                printf("Enter Name: ");
                fgets(newEmp.name, NAME_LEN, stdin);
                chompNewline(newEmp.name);

                printf("Enter Age: ");
                fgets(inputBuffer, sizeof(inputBuffer), stdin);
                sscanf(inputBuffer, "%d", &newEmp.age);

                printf("Enter Department: ");
                fgets(newEmp.department, DEPT_LEN, stdin);
                chompNewline(newEmp.department);
                
                printf("Enter Weekly Target Hours (max 50): ");
                fgets(inputBuffer, sizeof(inputBuffer), stdin);
                sscanf(inputBuffer, "%d", &newEmp.weeklyTargetHours);
                if (newEmp.weeklyTargetHours > 50) newEmp.weeklyTargetHours = 50;

                printf("Enter hours for 7 days (Mon-Sun): ");
                fgets(inputBuffer, sizeof(inputBuffer), stdin);
            
                sscanf(inputBuffer, "%d %d %d %d %d %d %d",
                    &newEmp.weeklyWorkingHours[0],
                    &newEmp.weeklyWorkingHours[1],
                    &newEmp.weeklyWorkingHours[2],
                    &newEmp.weeklyWorkingHours[3],
                    &newEmp.weeklyWorkingHours[4],
                    &newEmp.weeklyWorkingHours[5],
                    &newEmp.weeklyWorkingHours[6]);

                {
                    int insertPos = 0;
                    while (insertPos < *empCount && empList[insertPos].employeeID < newEmp.employeeID) {
                        insertPos++;
                    }
                    for (i = *empCount; i > insertPos; i--) {
                        empList[i] = empList[i - 1];
                    }
                    empList[insertPos] = newEmp;
                    (*empCount)++;
                    success = 1;
                }
            }
        }
    }

    if (success) {
        printf("Employee added successfully.\n");
    }
    return;
}


void Create_Team(Team teamList[], int* teamCount, Employee empList[], int empCount) {
    int done = 0;
    int success = 0;
    int i = 0;
    char inputBuffer[LINE_BUFFER];

    if (*teamCount >= TEAMS) {
        printf("Error: Team list is full.\n");
        done = 1;
    }

    if (!done) {
        Team newTeam;
        
        newTeam.teamID = 0; newTeam.teamName[0] = '\0'; newTeam.weeklyTeamTarget = 0;
        newTeam.memberCount = 0;
        for (i = 0; i < TEAM_MEMBERS; i++) { newTeam.memberIDs[i] = 0; }

        int numMembers = 0;
        
        printf("Enter new Team ID: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        if (sscanf(inputBuffer, "%d", &newTeam.teamID) != 1) {
            printf("Invalid input.\n");
            done = 1;
        } else {
            if (findTeamIndex(teamList, *teamCount, newTeam.teamID) != -1) {
                printf("Error: Team ID %d already exists.\n", newTeam.teamID);
                done = 1;
            } else {
                printf("Enter Team Name: ");
                fgets(newTeam.teamName, NAME_LEN, stdin);
                chompNewline(newTeam.teamName);

                printf("Enter Weekly Team Target Hours: ");
                fgets(inputBuffer, sizeof(inputBuffer), stdin);
                sscanf(inputBuffer, "%d", &newTeam.weeklyTeamTarget);

                printf("Enter number of members (max %d): ", TEAM_MEMBERS);
                fgets(inputBuffer, sizeof(inputBuffer), stdin);
                sscanf(inputBuffer, "%d", &numMembers);
                
                if (numMembers > TEAM_MEMBERS) numMembers = TEAM_MEMBERS;
                if (numMembers < 0) numMembers = 0;

                newTeam.memberCount = 0;
                for (i = 0; i < numMembers; i++) {
                    int empID = 0;
                    printf("Enter Employee ID for member %d: ", i + 1);
                    fgets(inputBuffer, sizeof(inputBuffer), stdin);
                    if (sscanf(inputBuffer, "%d", &empID) != 1) {
                        printf("Invalid ID entered. Skipping.\n");
                    } else {
                        if (findEmployeeIndex(empList, empCount, empID) == -1) {
                            printf("Error: Employee ID %d not found. Skipping.\n", empID);
                        } else {
                            if (isEmployeeInAnyTeam(teamList, *teamCount, empID)) {
                                printf("Error: Employee ID %d already belongs to a team. Skipping.\n", empID);
                            } else {
                                if (newTeam.memberCount < TEAM_MEMBERS) {
                                    newTeam.memberIDs[newTeam.memberCount++] = empID;
                                }
                            }
                        }
                    }
                }

                {
                    int insertPos = 0;
                    while (insertPos < *teamCount && teamList[insertPos].teamID < newTeam.teamID) {
                        insertPos++;
                    }
                    for (i = *teamCount; i > insertPos; i--) {
                        teamList[i] = teamList[i - 1];
                    }
                    teamList[insertPos] = newTeam;
                    (*teamCount)++;
                    success = 1;
                }
            }
        }
    }

    if (success) {
        printf("Team created successfully.\n");
    }
    return;
}


void Top_5_Employees(Employee empList[], int empCount) {
    int eligibleCount = 0;
    int i = 0;
    EmpRank eligibleEmps[EMPLOYEES]; 

    for (i = 0; i < empCount; i++) {
        int totalHours = getEmployeeTotalHours(&empList[i]);
        if (totalHours >= empList[i].weeklyTargetHours) {
            eligibleEmps[eligibleCount].empIndex = i;
            eligibleEmps[eligibleCount].totalHours = totalHours;
            eligibleCount++;
        }
    }

    if (eligibleCount == 0) {
        printf("No employees met their weekly target.\n");
    } else {
        quickSortEmpRanks(eligibleEmps, 0, eligibleCount - 1);
        
        printf("    Top 5 Employees    \n");
        int limit = (eligibleCount < 5) ? eligibleCount : 5;
        for (i = 0; i < limit; i++) {
            int empIndex = eligibleEmps[i].empIndex;
            int totalHours = eligibleEmps[i].totalHours;
            Employee* emp = &empList[empIndex];
            printf("Rank %d: %s (ID: %d), Total Hours: %d (Target: %d)\n",
                   i + 1, emp->name, emp->employeeID, totalHours, emp->weeklyTargetHours);
            if (totalHours > (emp->weeklyTargetHours * 3) / 2) { 
                printf("  -> Suggestion: Grant time-off (exceeded target by >50%%).\n");
            }
        }
    }

    return;
}


void Check_Team_Achievement(Team teamList[], int teamCount, Employee empList[], int empCount, int teamID) {
    int teamIndex = findTeamIndex(teamList, teamCount, teamID);
    int found = 0;
    int totalHours = 0;

    if (teamIndex == -1) {
        printf("Error: Team ID %d not found.\n", teamID);
        found = 0;
    } else {
        found = 1;
    }

    if (found) {
        Team* team = &teamList[teamIndex];
        totalHours = getTeamTotalHours(empList, empCount, team);
        printf("   Team Achievement: %s (ID: %d)    \n", team->teamName, team->teamID);
        printf("Team Target: %d hours\n", team->weeklyTeamTarget);
        printf("Total Hours Worked: %d hours\n", totalHours);
        if (totalHours >= team->weeklyTeamTarget) {
            printf("Result: ACHIEVED\n");
        } else {
            printf("Result: NOT ACHIEVED\n");
        }
    }

    return;
}


void Generate_Team_Ranking(Team teamList[], int teamCount, Employee empList[], int empCount) {
    int i = 0;
    if (teamCount == 0) {
        printf("No teams to rank.\n");
    } else {
        TeamRank ranks[TEAMS];
        for (i = 0; i < teamCount; i++) {
            ranks[i].teamIndex = i;
            ranks[i].totalHours = getTeamTotalHours(empList, empCount, &teamList[i]);
        }
        
        quickSortTeamRanks(ranks, 0, teamCount - 1);

        printf("    Team Ranking    \n");
        for (i = 0; i < teamCount; i++) {
            Team* team = &teamList[ranks[i].teamIndex];
            printf("Rank %d: %s (ID: %d), Total Hours: %d\n",
                   i + 1, team->teamName, team->teamID, ranks[i].totalHours);
        }
    }
    return;
}


void Check_Employee_Reward(Employee empList[], int empCount, int empID) {
    int empIndex = findEmployeeIndex(empList, empCount, empID);
    int valid = 0;
    int totalHours = 0;
    int points = 0;

    if (empIndex == -1) {
        printf("Error: Employee ID %d not found.\n", empID);
        valid = 0;
    } else {
        valid = 1;
    }

    if (valid) {
        Employee* emp = &empList[empIndex];
        totalHours = getEmployeeTotalHours(emp);
        if (totalHours < emp->weeklyTargetHours) {
            points = 0;
        } else {
            EmpRank eligibleEmps[EMPLOYEES];
            int eligibleCount = 0;
            int i = 0;
            for (i = 0; i < empCount; i++) {
                int th = getEmployeeTotalHours(&empList[i]);
                if (th >= empList[i].weeklyTargetHours) {
                    eligibleEmps[eligibleCount].empIndex = i;
                    eligibleEmps[eligibleCount].totalHours = th;
                    eligibleCount++;
                }
            }
            if (eligibleCount > 0) {
                quickSortEmpRanks(eligibleEmps, 0, eligibleCount - 1);
                
                int rank = -1;
                int r = -1;
                int k = 0;
                for (k = 0; k < eligibleCount; k++) {
                    if (eligibleEmps[k].empIndex == empIndex) {
                        r = k + 1;
                        k = eligibleCount; 
                    }
                }
                rank = r;
                
                if (rank == 1) points = 100;
                else if (rank == 2) points = 75;
                else if (rank == 3) points = 50;
                else if (rank > 3 && totalHours > emp->weeklyTargetHours) points = 20;
                else points = 10;
            } else {
                points = 10;
            }
        }

        printf("    Employee Reward: %s (ID: %d)    \n", emp->name, emp->employeeID);
        printf("Total Hours: %d (Target: %d)\n", totalHours, emp->weeklyTargetHours);
        printf("Reward Points: %d\n", points);
    }

    return;
}


void Update_or_Delete_Employee(Employee empList[], int* empCount, Team teamList[], int teamCount, int empID) {
    int empIndex = findEmployeeIndex(empList, *empCount, empID);
    int valid = 0;
    int done = 0;
    char inputBuffer[LINE_BUFFER];

    if (empIndex == -1) {
        printf("Error: Employee ID %d not found.\n", empID);
        valid = 0;
    } else {
        valid = 1;
    }

    if (valid) {
        printf("Employee found: %s\n", empList[empIndex].name);
        printf("1. Update Employee\n");
        printf("2. Delete Employee\n");
        printf("Choose an option: ");
        int choice = 0;
        
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        if (sscanf(inputBuffer, "%d", &choice) != 1) {
            printf("Invalid choice.\n");
            done = 1;
        } else {
            if (choice == 1) {
                printf("Enter new Name: ");
                fgets(empList[empIndex].name, NAME_LEN, stdin);
                chompNewline(empList[empIndex].name);

                printf("Enter new Age: ");
                fgets(inputBuffer, sizeof(inputBuffer), stdin);
                sscanf(inputBuffer, "%d", &empList[empIndex].age);

                printf("Enter new Department: ");
                fgets(empList[empIndex].department, DEPT_LEN, stdin);
                chompNewline(empList[empIndex].department);

                printf("Enter new Weekly Target Hours (max 50): ");
                fgets(inputBuffer, sizeof(inputBuffer), stdin);
                sscanf(inputBuffer, "%d", &empList[empIndex].weeklyTargetHours);
                if (empList[empIndex].weeklyTargetHours > 50) empList[empIndex].weeklyTargetHours = 50;

                printf("Enter new hours for 7 days (Mon-Sun): ");
                fgets(inputBuffer, sizeof(inputBuffer), stdin);
                sscanf(inputBuffer, "%d %d %d %d %d %d %d",
                    &empList[empIndex].weeklyWorkingHours[0],
                    &empList[empIndex].weeklyWorkingHours[1],
                    &empList[empIndex].weeklyWorkingHours[2],
                    &empList[empIndex].weeklyWorkingHours[3],
                    &empList[empIndex].weeklyWorkingHours[4],
                    &empList[empIndex].weeklyWorkingHours[5],
                    &empList[empIndex].weeklyWorkingHours[6]);

                printf("Employee details updated.\n");
            } else if (choice == 2) {
                {
                    int i = 0;
                    for (i = 0; i < teamCount; i++) {
                        int j = 0;
                        int memberPos = -1;
                        int foundInTeam = 0;
                        for (j = 0; j < teamList[i].memberCount && !foundInTeam; j++) {
                            if (teamList[i].memberIDs[j] == empID) {
                                memberPos = j;
                                foundInTeam = 1; 
                            }
                        }

                        if (foundInTeam) {
                            int k = 0;
                            for (k = memberPos; k < teamList[i].memberCount - 1; k++) {
                                teamList[i].memberIDs[k] = teamList[i].memberIDs[k + 1];
                            }
                            teamList[i].memberCount--;
                            printf("Removed from team %s.\n", teamList[i].teamName);
                            i = teamCount; 
                        }
                    }
                }
                {
                    int i = 0;
                    for (i = empIndex; i < *empCount - 1; i++) {
                        empList[i] = empList[i + 1];
                    }
                    (*empCount)--;
                    printf("Employee %d deleted successfully.\n", empID);
                }
            } else {
                printf("Invalid choice.\n");
            }
        }
    }

    return;
}


void Delete_Team(Team teamList[], int* teamCount, int teamID) {
    int teamIndex = findTeamIndex(teamList, *teamCount, teamID);
    if (teamIndex == -1) {
        printf("Error: Team ID %d not found.\n", teamID);
    } else {
        int i = 0;
        for (i = teamIndex; i < *teamCount - 1; i++) {
            teamList[i] = teamList[i + 1];
        }
        (*teamCount)--;
        printf("Team %d deleted. Employees are now unassigned.\n", teamID);
    }
    return;
}


void Merge_Teams(Team teamList[], int* teamCount, Employee empList[], int empCount, int teamID1, int teamID2) {
    int index1 = findTeamIndex(teamList, *teamCount, teamID1);
    int index2 = findTeamIndex(teamList, *teamCount, teamID2);
    int canMerge = 1;
    int i = 0;
    char inputBuffer[LINE_BUFFER];

    if (index1 == -1 || index2 == -1) {
        printf("Error: One or both Team IDs not found.\n");
        canMerge = 0;
    }
    if (canMerge && index1 == index2) {
        printf("Error: Cannot merge a team with itself.\n");
        canMerge = 0;
    }

    if (canMerge) {
        Team* team1 = &teamList[index1];
        Team* team2 = &teamList[index2];
        int allMembers[TEAM_MEMBERS * 2];
        int totalMembers = 0;

        for (i = 0; i < team1->memberCount; i++) {
            allMembers[totalMembers++] = team1->memberIDs[i];
        }
        for (i = 0; i < team2->memberCount; i++) {
            allMembers[totalMembers++] = team2->memberIDs[i];
        }

        if (totalMembers == 0) {
        
        } else {
            quickSortInts(allMembers, 0, totalMembers - 1);
            
            {
                int uniqCount = 0;
                int k = 0;
                for (k = 0; k < totalMembers; k++) {
                    if (k == 0 || allMembers[k] != allMembers[k - 1]) {
                        allMembers[uniqCount++] = allMembers[k];
                    }
                }
                totalMembers = uniqCount;
            }
        }

        Team newTeam;
        
        newTeam.teamID = 0; newTeam.teamName[0] = '\0'; newTeam.weeklyTeamTarget = 0;
        newTeam.memberCount = 0;
        for (i = 0; i < TEAM_MEMBERS; i++) { newTeam.memberIDs[i] = 0; }

        printf("Enter new Team ID for merged team: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        if (sscanf(inputBuffer, "%d", &newTeam.teamID) != 1) {
            printf("Invalid input. Aborting merge.\n");
            canMerge = 0;
        } else {
            if (canMerge && findTeamIndex(teamList, *teamCount, newTeam.teamID) != -1) {
                printf("Error: Team ID %d already exists. Aborting merge.\n", newTeam.teamID);
                canMerge = 0;
            }
        }

        if (canMerge) {
            printf("Enter new Team Name: ");
            fgets(newTeam.teamName, NAME_LEN, stdin);
            chompNewline(newTeam.teamName);

            printf("Enter new Weekly Team Target: ");
            fgets(inputBuffer, sizeof(inputBuffer), stdin);
            sscanf(inputBuffer, "%d", &newTeam.weeklyTeamTarget);

            {
                int limit = (totalMembers < TEAM_MEMBERS) ? totalMembers : TEAM_MEMBERS;
                for (i = 0; i < limit; i++) {
                    newTeam.memberIDs[i] = allMembers[i];
                }
                newTeam.memberCount = limit;
            }

            {
                int delIndex1 = (index1 > index2) ? index1 : index2;
                int delIndex2 = (index1 < index2) ? index1 : index2;
                int delTeamID1 = teamList[delIndex1].teamID;
                int delTeamID2 = teamList[delIndex2].teamID;
                Delete_Team(teamList, teamCount, delTeamID1);
                delIndex2 = findTeamIndex(teamList, *teamCount, delTeamID2);
                if (delIndex2 != -1) {
                    Delete_Team(teamList, teamCount, delTeamID2);
                }
            }

            {
                int insertPos = 0;
                while (insertPos < *teamCount && teamList[insertPos].teamID < newTeam.teamID) {
                     insertPos++;
                }
                for (i = *teamCount; i > insertPos; i--) {
                    teamList[i] = teamList[i - 1];
                }
                teamList[insertPos] = newTeam;
                (*teamCount)++;
                printf("Teams merged successfully into new team %s (ID: %d).\n", newTeam.teamName, newTeam.teamID);
            }
        }
    }

    return;
}


void Display_Team_Info(Team teamList[], int teamCount, Employee empList[], int empCount) {
    int i = 0, j = 0;
    if (teamCount == 0) {
        printf("No teams to display.\n");
    } else {
        TeamRank ranks[TEAMS];
        for(i=0; i < TEAMS; i++) { ranks[i].teamIndex = 0; ranks[i].totalHours = 0; }

        for (i = 0; i < teamCount; i++) {
            ranks[i].teamIndex = i;
            ranks[i].totalHours = getTeamTotalHours(empList, empCount, &teamList[i]);
        }
        
        quickSortTeamRanks(ranks, 0, teamCount - 1);

        int rankMap[TEAMS];
        for(i=0; i < TEAMS; i++) rankMap[i] = 0; 

        for (i = 0; i < teamCount; i++) {
            rankMap[ranks[i].teamIndex] = i + 1;
        }

        printf("\n--- All Team Information ---\n");
        for (i = 0; i < teamCount; i++) {
            Team* team = &teamList[i];
            int totalHours = getTeamTotalHours(empList, empCount, team);
            printf("                                \n");
            printf("Team Name: %s (ID: %d)\n", team->teamName, team->teamID);
            printf("Rank: %d\n", rankMap[i]);
            printf("Weekly Goal: %d | Current Total Hours: %d\n", team->weeklyTeamTarget, totalHours);
            printf("Members (%d):\n", team->memberCount);
            for (j = 0; j < team->memberCount; j++) {
                int empIndex = findEmployeeIndex(empList, empCount, team->memberIDs[j]);
                if (empIndex != -1) {
                    Employee* emp = &empList[empIndex];
                    int empHours = getEmployeeTotalHours(emp);
                    printf("  - %s (ID: %d), Hours: %d\n", emp->name, emp->employeeID, empHours);
                }
            }
        }
        printf("                       \n");
    }
    return;
}


void displayAllEmployees(Employee empList[], int empCount) {
    int i = 0;
    if (empCount == 0) {
        printf("No employees in the system.\n");
    } else {
        printf("\n All Employees (Sorted by ID) \n");
        for (i = 0; i < empCount; i++) {
            Employee* emp = &empList[i];
            int totalHours = getEmployeeTotalHours(emp);
            printf("ID: %d | Name: %s | Dept: %s | Target: %d | Worked: %d\n",
                   emp->employeeID, emp->name, emp->department,
                   emp->weeklyTargetHours, totalHours);
        }
        printf("                            \n");
    }
    return;
}


void loadData(Employee empList[], int* empCount, Team teamList[], int* teamCount) {
    FILE *f_emp = NULL, *f_team = NULL;
    char line[LINE_BUFFER];
    int i = 0;

    f_emp = fopen("employees.txt", "r");
    if (f_emp == NULL) {
        printf("Could not find employees.txt. Starting with 0 employees.\n");
        *empCount = 0;
    } else {
        *empCount = 0;
    
        while (fgets(line, sizeof(line), f_emp) && *empCount < EMPLOYEES) {
            Employee* emp = &empList[*empCount];

            emp->employeeID = 0; emp->name[0] = '\0'; emp->age = 0;
            emp->department[0] = '\0'; emp->weeklyTargetHours = 0;
            for (i = 0; i < DAYS_IN_WEEK; i++) { emp->weeklyWorkingHours[i] = 0; }

            int sscanf_result = sscanf(line, "%d,%[^,],%d,%[^,],%d,%d,%d,%d,%d,%d,%d,%d",
                      &emp->employeeID,
                      emp->name,
                      &emp->age,
                      emp->department,
                      &emp->weeklyWorkingHours[0],
                      &emp->weeklyWorkingHours[1],
                      &emp->weeklyWorkingHours[2],
                      &emp->weeklyWorkingHours[3],
                      &emp->weeklyWorkingHours[4],
                      &emp->weeklyWorkingHours[5],
                      &emp->weeklyWorkingHours[6],
                      &emp->weeklyTargetHours
                     );
            
            if (sscanf_result == 12) {
                (*empCount)++;
            }
        }
        fclose(f_emp);
        printf("Loaded %d employees from employees.txt.\n", *empCount);
    }

    f_team = fopen("teams.txt", "r");
    if (f_team == NULL) {
        printf("Could not find teams.txt. Starting with 0 teams.\n");
        *teamCount = 0;
    } else {
        *teamCount = 0;
        while (fgets(line, sizeof(line), f_team) && *teamCount < TEAMS) {
            Team* team = &teamList[*teamCount];

            team->teamID = 0; team->teamName[0] = '\0'; team->weeklyTeamTarget = 0;
            team->memberCount = 0;
            for (i = 0; i < TEAM_MEMBERS; i++) { team->memberIDs[i] = 0; }
            
            int pos = 0;
            int sscanf_result = sscanf(line, "%d,%[^,],%d,%d%n",
                      &team->teamID,
                      team->teamName,
                      &team->weeklyTeamTarget,
                      &team->memberCount,
                      &pos 
                     );

            if (sscanf_result == 4) {
                int member_pos = pos;
                int id = 0;
                int chars_read = 0;
                for (i = 0; i < team->memberCount && i < TEAM_MEMBERS; i++) {
                    if (sscanf(&line[member_pos], "%*c%d%n", &id, &chars_read) == 1) {
                        team->memberIDs[i] = id;
                        member_pos = member_pos + chars_read;
                    }
                }
                (*teamCount)++;
            }
        }
        fclose(f_team);
        printf("Loaded %d teams from teams.txt.\n", *teamCount);
    }

    return;
}


void saveData(Employee empList[], int empCount, Team teamList[], int teamCount) {
    FILE *f_emp = NULL, *f_team = NULL;
    f_emp = fopen("employees.txt", "w");
    if (f_emp == NULL) {
        printf("Error: Could not open employees.txt for writing.\n");
    } else {
        int i = 0, j = 0;
        for (i = 0; i < empCount; i++) {
            Employee* emp = &empList[i];
            fprintf(f_emp, "%d,%s,%d,%s", 
                    emp->employeeID, emp->name, emp->age, emp->department);
            for (j = 0; j < DAYS_IN_WEEK; j++) {
                fprintf(f_emp, ",%d", emp->weeklyWorkingHours[j]);
            }
            fprintf(f_emp, ",%d\n", emp->weeklyTargetHours);
        }
        fclose(f_emp);
        printf("Saved %d employees to employees.txt.\n", empCount);
    }

    f_team = fopen("teams.txt", "w");
    if (f_team == NULL) {
        printf("Error: Could not open teams.txt for writing.\n");
    } else {
        int i = 0, j = 0;
        for (i = 0; i < teamCount; i++) {
            Team* team = &teamList[i];
            /* Print main team data */
            fprintf(f_team, "%d,%s,%d,%d", 
                    team->teamID, team->teamName, team->weeklyTeamTarget, team->memberCount);
            for (j = 0; j < team->memberCount; j++) {
                fprintf(f_team, ",%d", team->memberIDs[j]);
            }
            fprintf(f_team, "\n"); 
        }
        fclose(f_team);
        printf("Saved %d teams to teams.txt.\n", teamCount);
    }

    return;
}

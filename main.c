#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char *candidate_names[] = {"Candidate1", "Candidate2", "Candidate3", "Candidate4", "Candidate5"};
char admin_username[] = "Admin";
char admin_password[] = "Admin";

// Structure to represent a voter
typedef struct {
    char voter_id[50];
    char password[50];
    char randomPasscode[5];
    int voted;
} Voter;

// Structure to represent a node in the linked list
typedef struct Node {
    Voter voter;
    struct Node* next;
} Node;

// Structure to represent a candidate
typedef struct {
    char name[50];
    int votes;
} Candidate;

// Structure to represent a node in the candidate linked list
typedef struct CandidateNode {
    Candidate candidate;
    struct CandidateNode* next;
} CandidateNode;

char* generateRandomPasscode() {
    srand(time(NULL));
    static char passcode[5];
    sprintf(passcode, "%d", rand() % 9000 + 1000); // Generates a random 4-digit passcode
    return passcode;
}

// Function to hash the password using XOR
void hashPassword(char* password) {
    int key = 0x5A; // XOR key
    int len = strlen(password);
    for (int i = 0; i < len; i++) {
        password[i] = password[i] ^ key;
    }
}

// Function to dehash the password using XOR
void dehashPassword(char* hashedPassword) {
    int key = 0x5A; // XOR key
    int len = strlen(hashedPassword);
    for (int i = 0; i < len; i++) {
        hashedPassword[i] = hashedPassword[i] ^ key;
    }
}

// Function to register a new voter
void registerVoter(Node** voterHead) {
    Voter newVoter;
    printf("Enter Voter id: ");
    scanf("%s", newVoter.voter_id);

    // Check if the voter_id already exists
    Node* current = *voterHead;
    while (current != NULL) {
        if (strcmp(current->voter.voter_id, newVoter.voter_id) == 0) {
            printf("Voter id already exists. Please choose a different Voter id.\n");
            registerVoter(voterHead);
            return; // Recursively call the function to register a new voter while valid voter_id is not provided
        }
        current = current->next;
    }

    printf("Enter password: ");
    scanf("%s", newVoter.password);
    hashPassword(newVoter.password); // Hash the password
    strcpy(newVoter.randomPasscode, generateRandomPasscode());
    printf("Registration successful!\n");
    printf("Your 2FA passcode is: %s\n", newVoter.randomPasscode);
    newVoter.voted = 0;

    // Create a new node for the voter
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->voter = newVoter;
    newNode->next = NULL;

    // If the list is empty, make the new node the head
    if (*voterHead == NULL) {
        *voterHead = newNode;
    } else {
        // Find the last node in the list and append the new node
        Node* current = *voterHead;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }

    printf("\n");
}

// Function to register a new candidate
void intialiseCandidates(CandidateNode** candidateHead, char *candidate_names[], int numCandidates) {
    // Intialise the candidates
    for (int i = 0; i < numCandidates; ++i) {
        Candidate newCandidate;
        newCandidate.votes = 0;
        strcpy(newCandidate.name, candidate_names[i]);

        // Create a new node for the candidate
        CandidateNode* newNode = (CandidateNode*)malloc(sizeof(CandidateNode));
        newNode->candidate = newCandidate;
        newNode->next = NULL;

        // If the list is empty, make the new node the head
        if (*candidateHead == NULL) {
            *candidateHead = newNode;
        } else {
            // Find the last node in the list and append the new node
            CandidateNode* current = *candidateHead;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newNode;
        }
    }
    printf("\n");
}

// Function to authenticate a voter
int authenticateVoter(Node* voterHead, char* voter_id, char* password, char* randomPasscode) {
    Node* current = voterHead;
    int index = 0;

    while (current != NULL) {
        if (strcmp(current->voter.voter_id, voter_id) == 0) {
            char hashedPassword[50];
            strcpy(hashedPassword, password);
            hashPassword(hashedPassword); // Hash the input password
            if (strcmp(current->voter.password, hashedPassword) == 0 && strcmp(current->voter.randomPasscode, randomPasscode) == 0) {
                return index;  // Return the index of the authenticated voter
            }
        }
        current = current->next;
        index++;
    }

    return -1;  // Return -1 if authentication fails
}

// Function to authenticate an Admin
int authenticateAdmin(char* voter_id, char* password) {
    if (strcmp(admin_username, voter_id) == 0 || strcmp(admin_password, password) == 0) {
            return 1; // Return 1 if the admin is authenticated
        }
    return -1;  // Return -1 if authentication fails
}

// Function to cast a vote
void castVote(Node* voterHead, CandidateNode* candidateHead, Node* voterNode) {
    Voter* voter = &(voterNode->voter);

    if (voter->voted) {
        printf("You have already voted!\n");
        return;
    }

    printf("Candidates:\n");
    CandidateNode* current = candidateHead;
    int index = 1;
    while (current != NULL) {
        printf("   %d. %s\n", index, current->candidate.name);
        current = current->next;
        index++;
    }

    int choice;
    printf("Enter the number of your chosen candidate: ");
    scanf("%d", &choice);

    if (choice >= 1 && choice <= index-1) {
        current = candidateHead;
        for (int i = 1; i < choice; i++) {
            current = current->next;
        }
        current->candidate.votes++;
        printf("Vote cast successfully!\n");
        voter->voted = 1;
    } else {
        printf("Invalid choice.\n");
    }
    printf("\n");
}

// Function to display election results
void displayResults(CandidateNode* candidateHead) {
    printf("Election Results:\n");
    CandidateNode* current = candidateHead;
    while (current != NULL) {
        printf("%s: %d votes\t", current->candidate.name, current->candidate.votes);
        current = current->next;
    }
    printf("\n\n");
}

void displayCandidates(CandidateNode* candidateHead) {
    printf("Candidates:\n");
    CandidateNode* current = candidateHead;
    int index = 1;
    while (current != NULL) {
        printf("\t %d. %s\n", index, current->candidate.name);
        current = current->next;
        index++;
    }
    printf("\n");
}

int main() {
    Node* voterHead = NULL;
    CandidateNode* candidateHead = NULL;

    int choice;
    char voter_id[50];
    char password[50];
    char randomPasscode[5];

    int numCandidates = sizeof(candidate_names) / sizeof(candidate_names[0]);
    intialiseCandidates(&candidateHead, candidate_names, numCandidates);

    do {
        printf("-------------------------------------------");
        printf("\n1. Register Voter\n2. Cast Vote\n3. Display Candidates\n4. Display Results (Admin)\n0. Exit\n");
        printf("-------------------------------------------\n\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerVoter(&voterHead);
                break;

            case 2:
                printf("Enter voter_id: ");
                scanf("%s", voter_id);
                printf("Enter password: ");
                scanf("%s", password);
                printf("Enter 2FA password: ");
                scanf("%s", randomPasscode);

                int voterIndex = authenticateVoter(voterHead, voter_id, password, randomPasscode);

                if (voterIndex != -1) {
                    printf("Login successful!\n\n");
                    Node* current = voterHead;
                    for (int i = 0; i < voterIndex; i++) {
                        current = current->next;
                    }
                    castVote(voterHead, candidateHead, current);
                } else {
                    printf("Login failed. Invalid credentials.\n");
                }
                break;

            case 3:
                displayCandidates(candidateHead);
                break;

            case 4:
                printf("Enter Admin username: ");
                scanf("%s", admin_username);
                printf("Enter Admin password: ");
                scanf("%s", admin_password);

                int adminAccess = authenticateAdmin(admin_username,admin_password);

                if (adminAccess != -1) {
                    printf("Login successful!\n");
                    displayResults(candidateHead);
                } else {
                    printf("Login failed. Invalid credentials.\n\n");
                }
                break;

            case 0:
                printf("Exiting program.\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n\n");
                break;
        }

    } while (choice != 0);

    // Free the memory allocated for the linked lists
    Node* currentVoter = voterHead;
    while (currentVoter != NULL) {
        Node* tempVoter = currentVoter;
        currentVoter = currentVoter->next;
        free(tempVoter);
    }

    CandidateNode* currentCandidate = candidateHead;
    while (currentCandidate != NULL) {
        CandidateNode* tempCandidate = currentCandidate;
        currentCandidate = currentCandidate->next;
        free(tempCandidate);
    }

    return 0;
}


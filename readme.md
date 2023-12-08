# Simple Voting System

This is a simple project for a Voting system implemented using a Data structures. 
The Voting System is a solution designed to address the challenges of security, accessibility, and usability in the domain of online voting. It implements basic encryption to safeguard voter information, candidate details, and vote tallies, ensuring the integrity and confidentiality of the electoral process.

## Key Features
- Two-Factor Authentication: Integrating a two-factor authentication mechanism to enhance user verification, reducing the risk of unauthorized access.
- Checking for Duplicates: Ensuring proper voting takes place by eliminating the possibility of duplicate voter IDs and thus multiple votes under the same name.
- Password Checking: Users can set their own passwords to secure their credentials. The password is checked when the voter wants to vote.
- Secure Password Handling: Implementing secure password storage techniques like hashing, to protect user credentials from potential breaches.
- Candidate Details: Implementing secure data structures to store candidate information, preventing tampering and maintaining the accuracy of candidate profiles.
- Efficient Vote Counting System: Using separate data fields to store raw vote tallies to ensure fast displaying of the votes.


## Data Structures Used

1. Voter Structure: Holds essential voter information and tracks whether the voter has already cast a vote.
2. Candidate Structure: Stores candidate details along with the number of votes they received.

These structures are stored in their own independent singly linked lists.

## Abstract Data Type (ADT)

### Voter ADT:
Properties:
- Voter username, password, randomPasscode, and voted status.

Operations:
- registerVoter: Register a new voter and add them to the linked list.
- authenticateVoter: Authenticate a voter based on username, password, and randomPasscode.
- castVote: Allow a registered voter to cast a vote.

### Candidate ADT:
Properties:
- Candidate name and votes received.

Operations:
- initializeCandidates: Initialize the list of candidates.
- displayCandidates: Display the list of candidates.
- displayResults: Display the election results.

## Compilation and Execution
1. Compile the code using a C compiler (e.g., gcc -o voting_system voting_system.c).
2. Run the compiled executable (./voting_system).
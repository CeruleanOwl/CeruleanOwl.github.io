//=============================================
//Author: Wes Roberts
//CS - 499 SNHU Capstone
//5/31/2024
//=============================================
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string.h>
#include <stdio.h>

using namespace std;

struct Course {
    string cNumber; //unique identifier
    string cName;
    vector <string> prereqs;
};

struct Node {
    Course course;
    Node* left;
    Node* right;
    int height;  // Height of the node

    // Default constructor
    Node() {
        left = nullptr;
        right = nullptr;
        height = 1;  // New node is initially added at leaf
    }

    // Initialize with a course
    Node(Course aCourse) : Node() {
        course = aCourse;
    }
};

class AVLTree {
private:
    Node* root;

    void inOrder(Node* node);
    Node* insert(Node* node, Course course);
    Node* rotateRight(Node* y);
    Node* rotateLeft(Node* x);
    int height(Node* node);
    int getBalance(Node* node);

public:
    AVLTree();
    void InOrder();
    void Insert(Course course);
    Course Search(string cNumber);
};

// Initializing methods
void openFile(ifstream&, string);
void processFile(ifstream&, AVLTree*);
void displayCourse(Course course);
void displayPreReqs(Course course);

AVLTree::AVLTree() {
    // Root is initially nullptr
    root = nullptr;
}

void AVLTree::InOrder() {
    // Call inOrder function and pass root 
    inOrder(root);
}

void AVLTree::Insert(Course course) {
    // Insert course and balance the tree
    root = insert(root, course);
}

Course AVLTree::Search(string cNumber) {
    // Set current node equal to root
    Node* current = root;
    // Keep looping downwards until bottom is reached or matching course is found
    while (current != nullptr) {
        // If match found, return current course
        if (current->course.cNumber.compare(cNumber) == 0) {
            return current->course;
        }
        // If course is smaller than current node then traverse left
        if (cNumber.compare(current->course.cNumber) < 0) {
            current = current->left;
        }
        // Else larger, so traverse right
        else {
            current = current->right;
        }
    }

    Course course;
    return course;
}

Node* AVLTree::insert(Node* node, Course course) {
    // Perform the normal BST insertion
    if (node == nullptr) {
        return new Node(course);
    }

    if (course.cNumber < node->course.cNumber) {
        node->left = insert(node->left, course);
    }
    else if (course.cNumber > node->course.cNumber) {
        node->right = insert(node->right, course);
    }
    else {
        // Duplicate course numbers are not allowed in the tree
        return node;
    }

    // Update the height of this ancestor node
    node->height = 1 + max(height(node->left), height(node->right));

    // Get the balance factor of this ancestor node to check whether this node became unbalanced
    int balance = getBalance(node);

    // If the node becomes unbalanced, then there are 4 base cases, otherwise return unchanged

    // Left Left Case
    if (balance > 1 && course.cNumber < node->left->course.cNumber) {
        return rotateRight(node);
    }

    // Right Right Case
    if (balance < -1 && course.cNumber > node->right->course.cNumber) {
        return rotateLeft(node);
    }

    // Left Right Case
    if (balance > 1 && course.cNumber > node->left->course.cNumber) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right Left Case
    if (balance < -1 && course.cNumber < node->right->course.cNumber) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    // Return the (unchanged) node pointer
    return node;
}

Node* AVLTree::rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Return new root
    return x;
}

Node* AVLTree::rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // Return new root
    return y;
}

int AVLTree::height(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return node->height;
}

int AVLTree::getBalance(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return height(node->left) - height(node->right);
}

void AVLTree::inOrder(Node* node) {
    // If node is not null
    if (node != nullptr) {
        // InOrder left
        inOrder(node->left);
        // Output course number and course name
        displayCourse(node->course);
        // InOrder right
        inOrder(node->right);
    }
}

// Allows you to display just the course name and number
void displayCourse(Course course) {
    cout << course.cNumber << ", " << course.cName << endl;
}

// Allows you to display the prerequisites
void displayPreReqs(Course course) {
    for (int i = 0; i < course.prereqs.size(); i++) {
        cout << course.prereqs.at(i) << " ";
    }
    cout << endl;
}

int main() {
    // Welcome user to the program
    cout << "Welcome to the course planner!" << endl;

    // Create a tree class for the objects to be stored
    AVLTree* bst;
    bst = new AVLTree();
    Course course;

    // 1. Open the file
    ifstream inFile;
    openFile(inFile, "courses.txt");

    // 2. Process the file
    processFile(inFile, bst);

    // 3. Close the file
    inFile.close();

    // Display message to user about loading data into the data structure already
    cout << "Your course information has been loaded from the file." << endl;

    // Display menu and initialize needed variables
    int choice = 0;
    string classChoice = "";
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Display All Courses" << endl;
        cout << "  2. Find Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1 || choice == 2 || choice == 9) {
        }
        else {
            cout << "Your choice is out of range." << endl;
        }
        switch (choice) {
        case 1:
            cout << "Here is a sample schedule:\n" << endl;
            bst->InOrder();
            break;
        case 2:
            cout << "Please enter a class number[Please use caps lock]: ";
            cin >> classChoice;
            course = bst->Search(classChoice);

            // Check if the course was found and produce output
            if (!course.cNumber.empty()) {
                displayCourse(course);
                cout << "Prerequisites: ";
                displayPreReqs(course);
            }
            else {
                cout << "Course Number: " << classChoice << " was not found!" << endl;
            }
            break;
        }
    }
    // Display a goodbye message
    cout << "Thank you for using the course Planner!" << endl;
}

void processFile(ifstream& inFile, AVLTree* bst) {
    string line, token;

    while (!inFile.eof()) {
        // Read line by line from the file [IMPORTANT NOTE]: For some reason, the file must end with an empty line to get all the data (otherwise last line is missing)
        getline(inFile, line);
        if (inFile.good()) {
            Course course;
            vector <string> tokens;
            string delimiter = ",";
            size_t pos = 0;
            string token;

            // Split line into tokens and insert into a vector
            while ((pos = line.find(delimiter)) != string::npos) {
                token = line.substr(0, pos);
                tokens.push_back(token);
                line.erase(0, pos + delimiter.length());
            }
            tokens.push_back(line);

            // Insert vector information into an object
            course.cNumber = tokens.at(0); // Unique identifier
            course.cName = tokens.at(1);
            for (int i = 2; i < tokens.size(); i++) {
                course.prereqs.push_back(tokens.at(i));
            }
            bst->Insert(course);
        }
    }
}

void openFile(ifstream& inFile, string fname) {
    // Open file
    inFile.open(fname);
    // If file opens, let me know
    if (inFile.is_open()) {
        cout << "Successfully opened file." << endl;
    }
    else {
        // If it does not open, end the program
        cout << "Failed to open the file." << endl;
        exit(-1);
    }
}


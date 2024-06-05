//=============================================
//Author: Wes Roberts
//Project 2: Final
//CS: 300 Analysis and Design
//10/12/2022
// The name of this file is QuickFishing.cpp cause I only intended to use this for quickTesting, but ended up using the file after getting so far into the parsing.
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

    //default
    Node() {
        left = nullptr;
        right = nullptr;
    }

    //initialize with a bid
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};


class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course aCourse);
    void inOrder(Node* node);

public:
    BinarySearchTree();
    void InOrder();
    void Insert(Course aCourse);
    Course Search(string bidId);
};

//initializing methods
void openFile(ifstream&, string);
void processFile(ifstream&, BinarySearchTree*);
void displayCourse(Course course);
void displayPreReqs(Course course);

BinarySearchTree::BinarySearchTree() {
    //root is equal to nullptr
    root = nullptr;
}

void BinarySearchTree::InOrder() {
    // call inOrder function and pass root 
    inOrder(root);
}

void BinarySearchTree::Insert(Course course) {
    // if root equal to null ptr
    if (root == nullptr) {
        // root is equal to new node course
        root = new Node(course);
    }
    // else
    else {
        // add Node root and course
        addNode(root, course);
    }

}

Course BinarySearchTree::Search(string cNumber) {
    // set current node equal to root
    Node* current = root;
    // keep looping downwards until bottom reached or matching course found
    while (current != nullptr) {
        // if match found, return current course
        if (current->course.cNumber.compare(cNumber) == 0) {
            return current->course;
        }
        // if course is smaller than current node then traverse left
        if (cNumber.compare(current->course.cNumber) < 0) {
            current = current->left;
        }
        // else larger so traverse right
        else {
            current = current->right;
        }
    }

    Course course;
    return course;
}

void BinarySearchTree::addNode(Node* node, Course course) {
    // if node is larger then add to left
    if (node->course.cNumber.compare(course.cNumber) > 0) {
        // if no left node
        if (node->left == nullptr) {
            // this node becomes left
            node->left = new Node(course);
        }
        // else recurse down the left node
        else {
            this->addNode(node->left, course);
        }
    }
    // else node is smaller then add to right
    else {
        // if no right node
        if (node->right == nullptr) {
            // this node becomes right
            node->right = new Node(course);
        }
        //else
        else {
            this->addNode(node->right, course);
        }
    }

}

void BinarySearchTree::inOrder(Node* node) {
    //if node is not equal to null ptr
    if (node != nullptr) {
        //InOrder node left
        inOrder(node->left);
        //output course number, course name
        displayCourse(node->course);
        //cout << "Visited node" << endl; //used for testing
        //InOrder right
        inOrder(node->right);
    }

}

//allows you to display just the course name and number
void displayCourse(Course course) {
    cout << course.cNumber << ", " << course.cName << endl;
}

//allows you to display the preReqs
void displayPreReqs(Course course) {
    for (int i = 0; i < course.prereqs.size(); i++) {
        cout << course.prereqs.at(i) << " ";
    }
    cout << endl;
}

int main()
{
    //welcome user to the program
    cout << "Welcome to the course planner!" << endl;

    //create a tree class for the objects to be stored.
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;

    //1. open the file
    ifstream inFile;
    openFile(inFile, "courses.txt");

    //2. process the file
    processFile(inFile, bst);

    //3. close the file
    inFile.close();

    //display message to user about loading data into the data structure already
    cout << "Your course information has been loaded from the file." << endl;

    //Display menu and initializes needed variables.
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
            //cout << "displaying courses [temporary output]" << endl; //used for testing
            cout << "Here is a sample schedule:\n" << endl;
            bst->InOrder();
            break;
        case 2:
            //cout << "finding course [temporary output]" << endl; //used for testing
            cout << "Please enter a class number[Please use caps lock]: ";
            cin >> classChoice;
            course = bst->Search(classChoice);

            //check if the course was found and produce output
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
    //display a goodbye message
    cout << "Thank you for using the course Planner!" << endl;
}

void processFile(ifstream& inFile, BinarySearchTree* bst) {
    string line, token;

    while (!inFile.eof()) {
        //read line by line from the file [IMPORTANT NOTE]: For some reason, file must end with an empty line to get all the data (otherwise last line is missing)
        getline(inFile, line);
        if (inFile.good()) {
            Course course;
            vector <string> tokens;
            string delimiter = ",";
            size_t pos = 0;
            std::string token;
            //cout << line << endl;   //used for testing

            //split line into tokens and insert into a vector
            while ((pos = line.find(delimiter)) != std::string::npos) {
                token = line.substr(0, pos);
                //std::cout << token << std::endl; //used for testing
                tokens.push_back(token);
                line.erase(0, pos + delimiter.length());
            }
            //std::cout << line << std::endl; //used for testing
            tokens.push_back(line);

            //insert vector information into an object
            course.cNumber = tokens.at(0); //unique identifier
            course.cName = tokens.at(1);
            for (int i = 2; i < tokens.size(); i++) {
                course.prereqs.push_back(tokens.at(i));
            }
            bst->Insert(course);

            //cout << "cNumber: " << course.cNumber << ", cName: " << course.cName << "Tests preReqs: " << endl;
            //for (int i = 0; i < tokens.size(); i++) {
            //    cout << tokens.at(i) << " ";
            //}
            //cout << endl; //used for testing
        }

    }

}

void openFile(ifstream& inFile, string fname) {
    //open file
    inFile.open(fname);
    //if file opens, let me know.
    if (inFile.is_open()) {
        cout << "successfully opened file." << endl;
    }
    //if it does not open, end the program.
    else {
        cout << "Failed to open the file." << endl;
        exit(-1);
    }
}


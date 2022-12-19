#include "miniGit.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <filesystem>
#include <fstream>
using namespace std;
namespace fs = std::filesystem;

void Repository::addFile(doublyNode* &currentCommit) {
    string fileName;
    bool quit = false;
    while(!quit){
        cout << "Please enter the name of the file you would like to add: ";
        fileName = "";
        cin >> fileName;// Getting the filename
        if (fs::exists(fileName)){
            if (!checkSLL(currentCommit, fileName)){
                singlyNode* fileToAdd = new singlyNode; //creates new node for new file
                fileToAdd->fileName = fileName;
                fileToAdd->fileVersion = fileName + "_" + to_string(fileToAdd->version);
                fileToAdd->next = nullptr;
                if (currentCommit->head == nullptr){
                    currentCommit->head = fileToAdd;
                    //cout << currentCommit->head->fileName << endl;
                } 
                else{
                    singlyNode* node = currentCommit->head;
                    singlyNode* prev = nullptr;
                    while(node != nullptr){
                        prev = node;
                        node = node->next;
                    }
                    prev->next = fileToAdd;// This is so see if the filename exists and if it does then add the file
                }
            } 
            else{
                cout << "A file by the same name cannot be added twice. Please enter a file that has not been added." << endl;
            }// to see if the name is already used
        } 
        else{
            cout << "This file name does not exists. Please enter a correct file name that is in the project directory." << endl;
        }//The file doesn't exists
        quit = true;
    }
}

doublyNode* Repository::getDoublyHead(){
    return doublyHead;
}

bool Repository::isCheckedOut(){
    return checkedOut;
}

int Repository::getNumCommits(){
    return totalCommits;
}

void Repository::setDoublyHead(doublyNode* &commit){
    doublyHead = commit;
}

bool Repository::checkSLL(doublyNode* &currentCommit, string fileName){ //checks if fileName is already in SLL; returns false if fileName is not found in SLL
    singlyNode* head = currentCommit->head;
    while(head != nullptr){
        if(head->fileName == fileName){
            return true;//checks to see if the filename exists
        } 
        else{
            head = head->next;//This is saying the filename doesn't exsit
        }
    }
    return false;
}

void Repository::commit(doublyNode* &currentCommit) {
    singlyNode* node = currentCommit->head;
    if (node != nullptr) {
        while (node != nullptr) {
            if (!fs::exists(".minigit/"+node->fileVersion)) {
                //if file does not exist in .minigit yet
                string line;
                ifstream file;
                ofstream gitFile;
                file.open(node->fileName);
                gitFile.open(".minigit/"+node->fileVersion);
                while (getline(file, line)) { //reads in each line and prints it out on the new doc line by line
                    gitFile << line << endl;
                }
                file.close();
                gitFile.close();
            } 
            else {
                //if it does exists already
                string line = "";
                string myFileString = "";
                string gitFileString = "";
                ifstream myFile, gitFile;
                myFile.open(node->fileName);
                gitFile.open(".minigit/"+node->fileVersion);
                while (getline(myFile, line)){
                    myFileString += line;
                } 
                myFile.close();
                line = "";
                while (getline(gitFile, line)){
                    gitFileString += line;
                }
                gitFile.close();
                if (myFileString != gitFileString){ //this comparison tells whether the files are the same
                    //fs::remove(".minigit/"+node->fileVersion);
                    node->version += 1;
                    node->fileVersion = node->fileName + "_" + to_string(node->version);
                    string l;
                    ifstream file1;
                    ofstream file2;
                    file1.open(node->fileName);
                    file2.open(".minigit/"+node->fileVersion);
                    while (getline(file1, l)){
                        file2 << l << endl;
                    }
                    file1.close();
                    file2.close();
                }
            }
            node = node->next;
        }
        //create new commit
        singlyNode* temp = currentCommit->head;
        doublyNode* newCommit = new doublyNode;
        newCommit->commitNumber = currentCommit->commitNumber + 1;
        newCommit->previous = currentCommit;
        newCommit->next = nullptr;
        newCommit->head = new singlyNode;
        newCommit->head->fileName = temp->fileName;
        newCommit->head->fileVersion = temp->fileVersion;
        newCommit->head->version = temp->version;
        newCommit->head->next = nullptr;
        singlyNode* currNode = newCommit->head;// we are making a commit in a linked list
        temp = temp->next;
        while (temp != nullptr){ //deep copy
            currNode->next = new singlyNode;
            currNode = currNode->next;
            currNode->fileName = temp->fileName;
            currNode->fileVersion = temp->fileVersion;
            currNode->version = temp->version;
            currNode->next = nullptr;
            temp = temp->next;
        }
        currentCommit->next = newCommit;
        currentCommit = newCommit;
        totalCommits++;
        //end of making new commit
    }
    else{
        cout << "Please add at least one file before trying to commit." << endl;
    }// must add file before you can commit
}

void Repository::checkout() {
    if (totalCommits >= 1) {
        //only can checkout if you have commited at least once
        if (!checkedOut) {
            //Normal menu screen when not checked out
            bool quit = false;
            while (!quit) {
                int commitNumber = 0;
                cout << "Please enter the commit number # you want to restore: #";
                cin >> commitNumber; // must find the unique commit number
                if (commitNumber <= totalCommits-1){
                    string input;
                    cout << "Are you sure you want to override your local files? Yes / No" << endl;
                    cin >> input;
                    if (input == "Yes" || input == "yes"){// if the user wants to override the local file
                        doublyNode* cNode = doublyHead;
                        while (cNode != nullptr){
                            if (cNode->commitNumber == commitNumber){
                                singlyNode *node = cNode->head;
                                while (node != nullptr){
                                    string line = "";
                                    ifstream gitFile;
                                    ofstream myFile;// must find the file
                                    myFile.open(node->fileName);
                                    gitFile.open(".minigit/" + node->fileVersion);
                                    while (getline(gitFile, line)){
                                        myFile << line << endl;
                                    }
                                    myFile.close();
                                    gitFile.close();
                                    node = node->next;
                                }
                                checkedOut = true;// if checkout comes out successful
                            }
                            cNode = cNode->next;
                        }
                        quit = true;
                    } 
                    else if (input == "No" || input == "no"){
                        return;
                    }
                } 
                else{
                    cout << "Please enter a valid commit number." << endl;
                }// user didnt enter valid commit number
            }
        } 
        else{
            //menu screen if the user is checked out
            //when checked out the user must revert back to the current version before being able to add, remove, or commit
            string input;
            cout << "Are you sure you want to override your local files? Yes/ No" << endl;
            cin >> input;
            if (input == "Yes" || input == "yes"){
                doublyNode* cNode = doublyHead;
                while(cNode->commitNumber != totalCommits-1){
                    cNode = cNode->next;
                }
                singlyNode *node = cNode->head;
                cout << node->fileVersion << endl;
                while (node != nullptr){
                    string line = "";
                    ifstream gitFile;
                    ofstream myFile;
                    myFile.open(node->fileName);
                    gitFile.open(".minigit/" + node->fileVersion);
                    while (getline(gitFile, line)){ // does the opposite of the commit
                        myFile << line << endl;
                    }
                    myFile.close();
                    gitFile.close();
                    node = node->next;
                }
                checkedOut = false; // set checkedOut back to false
                cNode = cNode->next;
            } 
            else if (input == "No" || input == "no"){
                return;
            }
        }
    } 
    else{
        cout << "You cannot checkout unitl you have commited." << endl;
    }
}
void Repository::removeFile(doublyNode* &currentCommit) {
    string fileName;
    bool quit = false; //must make sure it can quit out the while loop
    while (!quit) {
        cout << "Please enter the name of the file you want to remove: ";
        fileName = "";
        cin >> fileName;
        if (fs::exists(fileName)) {
            if (checkSLL(currentCommit, fileName)){
                if (currentCommit->head->fileName == fileName){
                    //special case to delete head
                    if (currentCommit->head->next == nullptr){
                        //if there is only 1 element in the SLL
                        delete currentCommit->head;
                        currentCommit->head = nullptr;
                    } 
                    else{
                        //more than one element already in SLL
                        singlyNode* temp = currentCommit->head;
                        currentCommit->head = currentCommit->head->next;
                        delete temp;
                        temp = nullptr;
                    }
                } 
                else{
                    //all other deletes
                    singlyNode *node = currentCommit->head;
                    singlyNode *prev = nullptr;
                    while (node->fileName != fileName) {
                        prev = node;
                        node = node->next;
                    }
                    prev->next = node->next;
                    delete node; // deleteing the node with the file attached to it
                    node = nullptr;
                }
                quit = true; //ends while loop 
            } 
            else{
                cout << "This file has not been added to the SLL yet. Please add file before trying to delete it." << endl;
            }// The file has not been in the link list yet
        } 
        else{
            cout << "This file name does not exists. Please enter a correct file name that is in the project directory and has been added to the SLL." << endl;
        }// file doesnt exist
    }
}
Repository::Repository() {
    doublyHead = nullptr;
    totalCommits = 0;
}

Repository::~Repository() { //free memory
    doublyNode* doublyCurr = doublyHead;
    while (doublyCurr != nullptr){
        singlyNode* curr = doublyCurr->head;
        while(curr != nullptr){
            curr = curr->next;
            delete doublyCurr->head;// deleting a node so you can free up memeory
            doublyCurr->head = curr;
        }
        delete curr;
        curr = nullptr;
        doublyCurr = doublyCurr->next;
        delete doublyHead;//pointing to next node to delete
        doublyHead = doublyCurr;
    }
    delete doublyCurr;
    doublyCurr = nullptr;
    fs::remove_all(".minigit");
}
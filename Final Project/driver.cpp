#include "miniGit.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <filesystem>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
using namespace std;
namespace fs = std::filesystem;

void setup() {
    cout << "Do you want to initialize a new empty repository? Yes or No" << endl;
    bool correct = false;
    while (correct == false) { //handles new empty repository
        string input = "";
        cin >> input;
        if (input == "Yes" || input == "yes") {
            fs::remove_all(".minigit");// removes a directory and its contents
            fs::create_directory(".minigit");// create a new directory
            cout << "A new empty repository has been created." << endl;
            correct = true;
        } 
        else if(input == "No" || input == "no") {
            correct = true;
        } 
        else{
            cout << "Please enter Yes or No." << endl;
        }
    } cout << endl;
}

int main() {
    setup();
    doublyNode* currentCommit = new doublyNode;
    currentCommit->commitNumber = 0;
    currentCommit->head = nullptr;
    currentCommit->next = nullptr;
    currentCommit->previous = nullptr;
    Repository miniGit = Repository();
    miniGit.setDoublyHead(currentCommit);
    int choice;
    bool quit = false;
    while (!quit) {
        if (currentCommit->head != nullptr){
            cout << "----FilesAdded----" << endl;// used for adding fils
            singlyNode *node = currentCommit->head;
            while (node != nullptr){
                cout << "Name: " << node->fileName << endl;// finding the name of the file
                node = node->next;// add files while searching to see if file name exists
            }
        }
        if (currentCommit->commitNumber >= 1){
            doublyNode* node = miniGit.getDoublyHead();
            cout << "-----Commits-----" << endl;// shows what you have done in your local file
            while (node->next != nullptr){
                cout << "#" << node->commitNumber << ": ";// giving a commit a unique number
                singlyNode* currNode = node->head;
                while (currNode != nullptr){
                    cout << currNode->fileVersion << " // ";
                    currNode = currNode->next;
                } cout << endl;
                node = node->next;// using linked list
            }
        }
        if (!miniGit.isCheckedOut()){// menu with 5 options
            cout << "-------Menu-------" << endl;
            cout << "1. Add file" << endl;
            cout << "2. Remove files" << endl;
            cout << "3. Commits" << endl;
            cout << "4. Checkouts" << endl;
            cout << "5. Quit" << endl;
            choice = 0;
            cin >> choice;
            switch (choice){
                case 1:
                    miniGit.addFile(currentCommit);// adding file
                break;
                case 2:
                    miniGit.removeFile(currentCommit);// deleteing file
                break;
                    
                case 3:
                    miniGit.commit(currentCommit);// getting all commits
                break;
                case 4:
                    miniGit.checkout();// giving unique numbers to commits
                break;
                case 5:
                    miniGit.~Repository();
                    quit = true;
                break;// making a switch statements for all options in menu
                default:// base case
                    cout << "Please enter 1-4." << endl;
                break;
            }
        } 
        else{
            cout << "-------Menu-------" << endl;// when they choose the checkout option
            cout << "1. Checkout most recent commit" << endl;
            cout << "2. Quit" << endl;// Making a switch statement for checkout
            choice = 0;
            cin >> choice;
            switch (choice){
                case 1:
                    miniGit.checkout();// if they choose number 1
                break;
                case 2:
                    quit = true;// quit out of the whole thing
                break;
                default:// base case
                    cout << "Please enter 1 or 2." << endl;
                break;
            }
        }
    }
}


#include <iostream>
using namespace std;
struct singlyNode {// creating a SLL
  string fileName;    // Name of local file
  int version = 0;
  string fileVersion; // Name of file in .minigit folder
  singlyNode *next;
};

struct doublyNode {// creating a DLL
  int commitNumber; // unique commit number
  singlyNode *head;
  doublyNode *previous;
  doublyNode *next;
};

class Repository {
  private:
    doublyNode *doublyHead;
    int totalCommits;   // total number of commits
    bool checkedOut;    // unique commit number

  public:
    Repository();
    ~Repository();
    int getNumCommits();
    bool isCheckedOut();
    bool checkSLL(doublyNode* &currentCommit, string fileName);
    void addFile(doublyNode* &currentCommit);
    void removeFile(doublyNode* &currentCommit);
    void commit(doublyNode* &currentCommit);
    void checkout();
    doublyNode* getDoublyHead();
    void setDoublyHead(doublyNode* &commit);// where we make all of our functions and and parameters
};
#pragma once
#include <string>
using namespace std;
class announceclass {
private:
    struct Node {
        int uniqueid;
        string content;
        string authorname;
        string time;
        Node* next;
        Node* prev;

        Node(string name, int id, string post, string t)
            : authorname(name), uniqueid(id), content(post), time(t), next(nullptr), prev(nullptr)
        {
        }
    };

    Node* head=nullptr;

public:
    announceclass() : head(nullptr) {}

    void addannouncement(string name, int id, string post, string t);
    void removeannouncement(int id);
    void findannouncement(int id);
    void viewannouncement();
    int getcount();
};

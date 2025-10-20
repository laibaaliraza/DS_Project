#include"header.h" //calling file having user class
#include<iostream>
using namespace std;
User::User() {
    head = nullptr;
}
User::~User() {
    Node* temp = head;
    while (temp != nullptr) {
        Node* next = temp->next;
        delete temp;
        temp = next;
    }
}
bool User::addusr(string name, string email, string pswd, string role) { //this will add the user in linked list
    Node* new_node = new Node(name, email, pswd, role);
    if (head == nullptr) {
        head = new_node;
        return true;
    }
    Node* temp = head;
    while (temp != nullptr) {
        // Review: 
        // 1.  A compact check to verify unique user. 
        // 2.  What does role specify here, if it meant Student, Teacher, Admin then 
        //     will not it make only 3 nodes, cuz after all it the check will prevent
        //     repitition. If it meant for something else then I would prefer to get insights.
        // 3.  How about making assigning a hidden unique ID to each user, so no worry of repitition. BUT
        //     BUT it is just an idea yet. Don't know yet whether in future we need to again use name
        //     at some places. 
        if (temp->name == new_node->name && temp->role == new_node->role) {
            cout << "\nThis user is already in the linked list: (Same name, Same role)";
            delete new_node;
            return false;
        }
        temp = temp->next;
    }
    temp->next = new_node;
    new_node->prev = temp;
    return true;
}
bool User::removeusr(string& name) { //delete the user from the list
    Node* temp = head;
    if (head == nullptr) {
        cout << "\nList is empty. No user to remove.";
        return false;
    }
    while (temp != nullptr) {
        if (temp->name == name) {
            cout << "\nRemoved user:  ";
            cout << "\n name: " << temp->name << "\nemail id: " << temp->email << "\nRole: " << temp->role;
            if (temp->prev != nullptr) {
                temp->prev->next = temp->next;
            }
            else {
                head = temp->next;
            }
            if (temp->next != nullptr) {
                temp->next->prev = temp->prev;
            }
            delete temp;
            return true;
        }
        temp = temp->next;
    }
    cout << "\nUser not found";
    return false;
}
bool User::searchusr(string& name) { //search specific user in list
    if (head == nullptr) {
        cout << "Empty list: ";
        return false;
    }
    Node* temp = head;
    while (temp != nullptr) {
        if (temp->name == name) {
            cout << "\nUser's data:  ";
            cout << "\n name: " << temp->name << "\nemail id: " << temp->email << "\nRole: " << temp->role;
            return true;
        }
        temp = temp->next;
    }
    return false;
}
void User::display() {
    Node* temp = head;
    if (head == nullptr) {
        cout << "\nList is empty!";
        return;
    }
    while (temp != nullptr) {
        cout << "\nUser's data:  ";
        cout << "\n name: " << temp->name << "\nemail id: " << temp->email << "\nRole: " << temp->role;
        temp = temp->next;
    }

}

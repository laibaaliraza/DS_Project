#include"header.h" //calling file having user class
#include<iostream>
using namespace std;
User::User() {
    head = nullptr;
}
bool User::addusr(string name, string email, string pswd, string role) { //this will add the user in linked list
    Node* new_node = new Node(name, email, pswd, role);
    if (head == nullptr) {
        head = new_node;
        return true;
    }
    Node* temp = head;
    while (temp->next != nullptr) {
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
        cout << "\nUnderflow condition!";
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
#pragma once
#include<iostream>
using namespace std;
#include<string>
class User {
public:
    struct Node {
        string name;
        string email;
        string pswd;
        string role;
        Node* next;
        Node* prev;
        Node(const string& n, const string& e, const string& p, const string& r) {
            name = n;
            email = e;
            pswd = p;
            role = r;
            next = nullptr;
            prev = nullptr;
        }
    };
    Node* head;
    User();
    ~User();
    bool addusr(string name, string email, string pswd, string role); //add user in doubly linked list
    bool removeusr(string& name); //remove the user from doubly linked list
    bool searchusr(string& name); //search the user in linkedlist
    void display(); //display the whole doubly linked list
};
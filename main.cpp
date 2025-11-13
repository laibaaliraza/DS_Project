#include "header.h"
#include<iostream>
#include<string>
using namespace std;
int main() {
    User u;
    string name, email, pswd, role;
    int choice = 1, num;
    while (choice != 0) {
        cout << "\nUser information: ";
        cout << "\n1)Add user.\n2)Remove user.\n3)Search user.\n4)Display list\n0)End";
        cin >> choice;
        switch (choice) {
        case 1: {
            cout << "\nEnter user's name: ";
            cin >> name;
            cout << "\nEnter user's email: ";
            cin >> email;
            cout << "\nEnter user's password: ";
            cin >> pswd;
            cout << "Enter role of user(Admin/user): ";
            cin >> role;
            if (u.addusr(name, email, pswd, role)) {
                cout << endl << name << " added successfully in list!";
            }
            else {
                cout << "\nThere is some error while adding " << name << " in list!";
            }
            break;
        }
        case 2: {
            cout << "\nEnter name of user which should be removed: ";
            cin >> name;
            if (u.removeusr(name)) {
                cout << endl << name << " removed from list successfully!";
            }
            else {
                cout << endl << name << " is not in list";
            }
            break;
        }
        case 3: {
            cout << "Enter name of user you wanna search: ";
            cin >> name;
            if (u.searchusr(name)) {
                cout << endl << name << " found in list: ";
            }
            else {
                cout << endl << name << " is not in list";
            }
            break;
        }case 4: {
            u.display();
            break;
        }case 0: {
            return 0;
            break;
        }default: {
            cout << "\nInvalid input!\nTry again!";
        }
        }
    }
    return 0;
}
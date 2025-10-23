#include<iostream>
#include "announcements.h"
#include<string>
using namespace std;

void announceclass::addannouncement(string name,int id,string post,string t){
	Node* newnode = new Node(name,id,post,t);
	newnode->authorname = name;
	newnode->content = post;
	newnode->uniqueid = id;
	newnode->time = t;
	if (head == nullptr) {
		head = newnode;
		return;
	}
	Node* temp = head;
	while (temp->next != nullptr)
	{
		temp = temp->next;
	}temp->next = newnode;
	newnode->prev = temp;


}
void announceclass::removeannouncement(int id){
	if (head == nullptr)
	{
		cout << "No announcements to remove.";
		return;
	}
	Node* temp = head;
	while (temp != nullptr )
	{
		if (temp->uniqueid == id)
		{
			if (temp->next == nullptr && head == temp) { //if only one node i-e head

				head = nullptr;
			}
			else if (temp == head) { // if first node
				head = head->next;
				head->prev = nullptr;
			}
			else if (temp->next == nullptr)//if last node
			{
				temp->prev->next = nullptr;
			}
			else
			{
				temp->prev->next = temp->next;
				temp->next->prev = temp->prev;
			}
			delete temp;
			cout << "Removed.";
				return ;
		}
		temp = temp->next;
	}
	cout << "Id not found.";
}
void announceclass::findannouncement(int id){
	if (head == nullptr) {
		cout << "No announcements to find.";
			return;
}
	Node* temp = head;
	while (temp != nullptr)
	{
		if (temp->uniqueid == id)
		{
			cout << "Id found."<<endl;
			cout << "The announcement is : " << temp->content;
			return;
		}temp = temp->next;
	}
	cout << "NOt found";
}
void announceclass::viewannouncement(){
	if (head == nullptr) {
		cout << "No announcements to find.";
		return;
	} cout << "The announcements are : " << endl;
	Node* temp = head;
	while (temp != nullptr)
	{
			cout  << temp->content;
		temp = temp->next;
	}
}
int announceclass::getcount(){
	int c ;
	if (head == nullptr)
	{
		c = 0;
		return c;
}
	Node* temp = head;
	c = 0;
	while (temp != nullptr)
	{
		c++;
		temp = temp->next;
	}
	return c;
}
int main() {
	announceclass n;
		int choice;

		while(true) {
			cout << "\n===== ANNOUNCEMENT MANAGEMENT SYSTEM =====\n";
			cout << "1. Add Announcement\n";
			cout << "2. Remove Announcement\n";
			cout << "3. Find Announcement\n";
			cout << "4. View All Announcements\n";
			cout << "5. Total Announcement Count\n";
			cout << "0. Exit\n";
			cout << "------------------------------------------\n";
			cout << "Enter your choice: ";
			cin >> choice;

			switch (choice) {
			case 1: {
				string author, post;
				int id;
				string time;

				cout << "Enter Author Name: ";
				cin >> author;
				cout << "Enter Unique ID: ";
				cin >> id;
				cout << "Enter Announcement Content: ";
				cin.ignore();
				getline(cin, post);
				cout << "Enter Time: ";
				cin >> time;

				n.addannouncement(author, id, post, time);
				break;
			}

			case 2: {
				int id;
				cout << "Enter ID to Remove: ";
				cin >> id;
				n.removeannouncement(id);
				break;
			}

			case 3: {
				int id;
				cout << "Enter ID to Search: ";
				cin >> id;
				n.findannouncement(id);
				break;
			}

			case 4:
				n.viewannouncement();
				break;

			case 5:
				cout << "The count of announcements is = " << n.getcount() << endl;
				break;

			case 0:
				cout << "Exiting program...\n";
				return 0;

			default:
				cout << "Invalid choice! Try again.\n";
			}

		} 

	
	}

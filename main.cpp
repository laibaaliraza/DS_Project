// ----------------------------------------------------------------------------
//                      Just to test the functionality,
//                      will not be included in main
//-----------------------------------------------------------------------------

#include <iostream>
#include <string>

#include "HybridStructure.h"

using namespace std;

void printMenu() {
    cout << "\n===== HybridStructure Demo =====\n"
         << " 1) Add Announcement\n"
         << " 2) Display All (raw list)\n"
         << " 3) Search Announcement by ID\n"
         << " 4) Update Announcement by ID\n"
         << " 5) Remove Announcement by ID\n"
         << " 6) Enqueue Event\n"
         << " 7) Peek Event (front)\n"
         << " 8) Dequeue Event (front)\n"
         << " 9) Display Events (only)\n"
         << "10) Display Announcements (only)\n"
         << " 0) Exit\n"
         << "Select: ";
}

int main() {
    HybridStructure hybrid;

    while (true) {
        printMenu();

        int choice;
        if (!(cin >> choice)) {
            cout << "Non-numeric input detected. Exiting.\n";
            return 0;
        }

        switch (choice) {
            case 1: {
                if (hybrid.addAnnouncement()) cout << "Announcement added.\n";
                else cout << "Failed to add announcement.\n";
                break;
            }
            case 2: {
                hybrid.displayAnnouncement();
                break;
            }
            case 3: {
                cout << "Enter ID to search: ";
                string id; getline(cin, id);
                if (!hybrid.searchAnnouncement(id)) cout << "Not found.\n";
                break;
            }
            case 4: {
                cout << "Enter ID to update: ";
                string id; getline(cin, id);
                if (hybrid.updateAnnouncement(id)) cout << "Updated.\n";
                else cout << "Update failed (ID not found).\n";
                break;
            }
            case 5: {
                cout << "Enter ID to remove: ";
                string id; getline(cin, id);
                if (!hybrid.removeAnnouncement(id)) cout << "Remove failed (ID not found).\n";
                break;
            }
            case 6: {
                if (hybrid.enqueueEvent()) cout << "Event enqueued.\n";
                else cout << "Failed to enqueue event.\n";
                break;
            }
            case 7: {
                hybrid.peekEvent();
                break;
            }
            case 8: {
                if (!hybrid.dequeueEvent()) cout << "No event to dequeue.\n";
                break;
            }
            case 9: {
                hybrid.displayEvents();
                break;
            }
            case 10: {
                // assumes you added displayAnnouncementsOnly() as discussed
                hybrid.displayAnnouncement();
                break;
            }
            case 0: {
                cout << "Goodbye!\n";
                return 0;
            }
            default: {
                cout << "Unknown option.\n";
                break;
            }
        }
    }
}

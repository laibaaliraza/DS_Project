// -----------------------------------------------------------------------------
// HybridStructure.cpp
// Implementation for the classes declared in HybridStructure.h.
//
// What this file does:
// - Implements a doubly linked list of HybridNode items (announcements + events).
// - Provides CRUD-style operations for announcements.
// - Provides queue-like operations for items where type_ == "event", using the
//   same underlying list (a “hybrid” view).
//
// Key ideas:
// - Single list; two views:
//     • Full list view: all nodes, linked via head_/tail_
//     • Event queue view: only nodes with type_ == "event", tracked via
//       eventFront_/eventRear_ for efficient queue operations

// Collaboration tips:
// - Keep id_ unique per node (the current code generates time-based IDs).
// - Keep type_ values consistent (e.g., "announcement", "event").
// - If you change how events are identified, update queue helpers accordingly.
// -----------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <chrono>

#include "HybridStructure.h"


// -----------------------------------------------------------------------------
//                                HybridNode definitions
// -----------------------------------------------------------------------------

/*
 * Default constructor
 * Initializes fields to empty strings and nulls both links.
 * Values are typically filled via setters when the node is created/added.
 */
HybridNode::HybridNode()
{
  this->id_    = "";
  this->type_  = "";
  this->title_ = "";
  this->date_  = "";
  this->author_ = "";

  this->prev_ = nullptr;
  this->next_ = nullptr;
}

/*
 * Destructor
 * Node does not own neighbors; nothing to free here.
 * HybridStructure cleans up nodes it owns in its destructor.
 */
HybridNode::~HybridNode() = default;

// -----------------------------------------------------------------------------
//                              HybridStructure definitions
// -----------------------------------------------------------------------------

// ------------------------------- Constructors / Destructor -------------------

/*
 * HybridStructure()
 * Start with an empty list and an empty event-queue view.
 */
HybridStructure::HybridStructure()
{
  head_ = nullptr;
  tail_ = nullptr;

  eventFront_ = nullptr;
  eventRear_  = nullptr;
}

/*
 * ~HybridStructure()
 * Walk the list from head_ to tail_ and delete every node.
 * Finally null out head_/tail_ to mark the structure empty.
 */
HybridStructure::~HybridStructure()
{
  HybridNode* iterator = head_;
  HybridNode* deleting = nullptr;

  while (iterator != nullptr)
  {
    deleting = iterator;
    iterator = iterator->getNext();
    delete deleting;
  }

  head_ = nullptr;
  tail_ = nullptr;
}

// --------------------------------------- Utilities -----------------------------------------------

/*
 * search(id)
 * Linear search for a node by its ID in the list.
 *
 * Returns:
 *   - pointer to the node if found
 *   - nullptr (and prints a message) if not found
 *
 * Example:
 *   HybridNode* n = search("A-123");
 */
HybridNode* HybridStructure::search(const std::string& id)
{
  HybridNode* iterator = head_;

  // Walk forward until we hit nullptr or find a matching id.
  while (iterator != nullptr && iterator->getId() != id)
  {
    iterator = iterator->getNext();
  }

  if (iterator == nullptr)
  {
    std::cout << "ERROR: The ID does not exist!" << std::endl;
    return nullptr;
  }
  return iterator;
}

/*
 * adjustFront()
 * Realign eventFront_ so it points at the first node whose type is "event".
 * Used after operations that dequeue eventFront_
 *
 * Behavior:
 *   - If eventFront_ is null, leaves it as null (prints a warning).
 *   - Otherwise, advances forward until it points to an "event" or becomes null.
 */
void HybridStructure::adjustFront()
{
  HybridNode* iterator = eventFront_;

  if (iterator == nullptr)
  {
    std::cerr << "ERROR: The front node does not exist!" << std::endl;
    // Keep going: eventFront_ will remain nullptr.
  }

  // Slide forward until we hit the tail_ or find an "event".
  while (iterator != tail_ && iterator && iterator->getType() != "event")
  {
    iterator = iterator->getNext();
  }

  eventFront_ = iterator; // May end up nullptr if no event is found.
}

// ---------------------------------- Announcement methods -----------------------------------------

/*
 * addAnnouncement()
 * Append a new "announcement" node at the end of the list.
 *
 * Flow:
 *   1) Generate a simple time-based ID as a 6-char string.
 *   2) Read title/date/author from std::cin (type is fixed to "announcement").
 *   3) Allocate a new node, set fields, and link it at the tail.
 *
 * Returns: true on success.
 *
 */
bool HybridStructure::addAnnouncement ()
{
  std::string id, type, title, date, author;

  // Create a 6-digit-ish ID based on "now" in milliseconds.
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::system_clock::now().time_since_epoch()).count();
  id = std::to_string(ms % 1000000);
  if (id.size() < 6) id.insert(id.begin(), 6 - id.size(), '0');

  type = "announcement";

  // Collect fields from the user.
  std::cout << "   --- Add Announcement ---" << std::endl << std::endl;
  std::cout << "Title: ";   getline(std::cin, title);
  std::cout << "Date: ";    getline(std::cin, date);
  std::cout << "Author: ";  getline(std::cin, author);

  // Create and populate the node.
  HybridNode* temp = new HybridNode();
  temp->setId(id);
  temp->setTitle(title);
  temp->setDate(date);
  temp->setAuthor(author);
  temp->setType(type);

  // Link at tail_ (append).
  if (head_ == nullptr)            // empty list case
  {
    head_ = temp;
    tail_ = temp;
    return true;
  }
  else                             // non-empty list case
  {
    tail_->setNext(temp);
    temp->setPrev(tail_);
    tail_ = temp;
    return true;
  }
}

/*
 * removeAnnouncement(id)
 * Remove the node with the given ID from the list (if present).
 *
 * Steps:
 *   1) search() to locate the node; if not found, return false.
 *   2) Re-link neighbors (prev/next) and update head_/tail_ as needed.
 *   3) Delete the node and print a confirmation.
 *
 * Example:
 *   bool ok = removeAnnouncement("A-102");
 *
 * Returns:
 *   true  - a node was found and removed
 *   false - no node with this ID
 */
bool HybridStructure::removeAnnouncement (const std::string& id)
{
  HybridNode* node = search(id);
  if (node == nullptr)
    return false;

  HybridNode* prev = node->getPrev();
  HybridNode* next = node->getNext();

  // Stitch neighbors together around 'node'.
  if (prev != nullptr)
    prev->setNext(next);
  else
    head_ = next;   // removing head

  if (next != nullptr)
    next->setPrev(prev);
  else
    tail_ = prev;   // removing tail

  // Detach and delete the node.
  node->setPrev(nullptr);
  node->setNext(nullptr);
  delete node;

  std::cout << "Announcement ID: " << id << " removed." << std::endl;
  return true;
}

/*
 * searchAnnouncement(id)
 * Wrapper around search().
 * If found, prints a short summary of the announcement’s fields.
 *
 * Example:
 *   bool exists = searchAnnouncement("E-009");
 *
 * Returns:
 *   true  - found
 *   false - not found
 */
bool HybridStructure::searchAnnouncement (const std::string& id)
{
  HybridNode* node = search(id);

  if (node == nullptr)
    return false;

  std::cout << "Announcement ID: "    << id                 << " found." << std::endl;
  std::cout << "Announcement Title: " << node->getTitle()  << std::endl;
  std::cout << "Announcement Date: "  << node->getDate()   << std::endl;
  std::cout << "Announcement Author: "<< node->getAuthor() << std::endl;
  return true;
}

/*
 * updateAnnouncement(id)
 * Locate an announcement by ID, then prompt for and overwrite its title/date.
 *
 * Example:
 *   bool ok = updateAnnouncement("A-007");
 *
 * Returns:
 *   true  - updated
 *   false - ID not found
 */
bool HybridStructure::updateAnnouncement (const std::string& id)
{
  HybridNode* node = search(id);
  if (node == nullptr)
    return false;

  std::string title, date;

  std::cout << "   --- Updating Announcement --- " << std::endl;
  std::cout << "Upate title: ";   getline(std::cin, title);   // Note: typo "Upate" preserved from original.
  std::cout << "Upate date: ";    getline(std::cin, date);

  node->setTitle(title);
  node->setDate(date);
  return true;
}

/*
 * displayAnnouncement()
 * Print every node in the list in a readable form (announcements and events).
 * Useful as a raw view of the full list.
 */
void HybridStructure::displayAnnouncement() const
{
  if (!head_) {
    std::cout << "[no announcements]\n";
    return;
  }

  std::cout << "=== Announcements ===\n";
  const HybridNode* iterator = head_;
  int index = 1;

  while (iterator) {
    std::cout << index++ << ")\n";
    std::cout << "  ID     : " << iterator->getId()     << "\n";
    std::cout << "  Type   : " << iterator->getType()   << "\n";
    std::cout << "  Title  : " << iterator->getTitle()  << "\n";
    std::cout << "  Date   : " << iterator->getDate()   << "\n";
    std::cout << "  Author : " << iterator->getAuthor() << "\n";
    std::cout << "--------------------------\n";
    iterator = iterator->getNext();
  }
}

// --------------------------------------- Event queue API -----------------------------------------

/*
 * enqueueEvent()
 * Append a new node of type "event" at the end of the list and adjust
 * eventFront_/eventRear_ to maintain the queue view.
 *
 * Returns: true on success.
 */
bool HybridStructure::enqueueEvent()
{
  HybridNode* temp = new HybridNode();

  // Collect event data from the user and populate the node.
  {
    std::string title, date, author, id;

    std::cout << "   --- Add Event ---" << std::endl << std::endl;
    std::cout << "Title: ";   getline(std::cin, title);
    std::cout << "Date: ";    getline(std::cin, date);
    std::cout << "Author: ";  getline(std::cin, author);

    // Generate 6-digit ID similar to announcements.
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::system_clock::now().time_since_epoch()).count();
    id = std::to_string(ms % 1000000);
    if (id.size() < 6) id.insert(id.begin(), 6 - id.size(), '0');

    temp->setId(id);
    temp->setTitle(title);
    temp->setDate(date);
    temp->setAuthor(author);
    temp->setType("event");
  }

  // Append at tail_ and adjust queue pointers.
  if (!tail_)                 // empty list case
  {
    tail_ = temp;
    head_ = temp;
    eventFront_ = temp;       // first event is both front and rear
    eventRear_  = temp;
  }
  else                        // non-empty list case
  {
    tail_->setNext(temp);
    temp->setPrev(tail_);
    tail_ = temp;

    // If there was no eventFront_ yet, this new event becomes the front.
    if (!eventFront_)
    {
      eventFront_ = temp;
    }

    eventRear_ = temp;        // newest event is always the rear
  }

  return true;
}

/*
 * dequeueEvent()
 * Remove the first "event" (FIFO) using the queue view.
 *
 * Behavior:
 *   - If there is no eventFront_, print an error and return false.
 *   - Otherwise unlink and delete that node, then advance eventFront_
 *     to the next "event" (if any).
 *
 * Returns:
 *   true  - an event was dequeued and removed
 *   false - the event queue was empty
 */
bool HybridStructure::dequeueEvent()
{
  if (!eventFront_)
  {
    std::cerr << "Event queue empty, " << std::endl;
    return false;
  }

  HybridNode* target = eventFront_;

  // Find the next "event" node after the one we're removing.
  HybridNode* nextEvent = target->getNext();
  while (nextEvent && nextEvent->getType() != "event")
  {
    nextEvent = nextEvent->getNext();
  }
  eventFront_ = nextEvent;   // may become nullptr if no more events

  // Standard unlink in a doubly linked list.
  HybridNode* prev = target->getPrev();
  HybridNode* next = target->getNext();

  if (prev)
  {
    prev->setNext(next);
  }
  else
  {
    head_ = next;            // removing the head node
  }

  if (next)
  {
    next->setPrev(prev);
  }
  else
  {
    tail_ = prev;            // removing the tail node
  }

  std::cout << "Dequeued event [" << target->getId()
            << "]: " << target->getTitle() << "\n";

  delete target;
  return true;
}

/*
 * peekEvent()
 * Print a brief summary of the front "event" without removing it.
 * If there are no events, prints a friendly message.
 */
void HybridStructure::peekEvent() const
{
  if (!eventFront_) {
    std::cout << "No upcoming events.\n";
    return;
  }

  std::cout << "Upcoming Event: " << eventFront_->getTitle()
            << " (" << eventFront_->getDate() << ")\n";
}

/*
 * displayEvents()
 * Print all nodes whose type is "event" in the order they appear in the list.
 * If none exist, print a friendly message.
 */
void HybridStructure::displayEvents() const {
  HybridNode* iterator = head_;
  int index = 1;
  bool anyEvent = false;

  while (iterator) {
    if (iterator->getType() == "event") {
      std::cout << index++ << ". [" << iterator->getId()
                << "] " << iterator->getTitle()
                << " by " << iterator->getAuthor()
                << " on " << iterator->getDate() << '\n';
      anyEvent = true;
    }
    iterator = iterator->getNext();
  }

  if (!anyEvent) std::cout << "[No events available]\n";
}

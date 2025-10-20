// -----------------------------------------------------------------------------
// HybridStructure.h
// Purpose: A small "hybrid" container that stores announcements in a doubly
// linked list while also supporting event queue operations over the same nodes.
// Why hybrid? We keep one list of nodes (announcements + events) so related
// data lives together. Regular list operations work over all nodes; queue-like
// operations (enqueue/dequeue/peek) apply only to nodes whose type_ == "event".
// This avoids maintaining two separate structures and potential duplication.
// -----------------------------------------------------------------------------

#ifndef MYPROJECT_HYBRIDSTRUCTURE_H
#define MYPROJECT_HYBRIDSTRUCTURE_H

# include <string>

// -----------------------------------------------------------------------------
// CLASS: HybridNode
// A simple record shared by different features (announcements, events, undo).
// Doubly-linked via prev_/next_ so it can live inside a list.
// -----------------------------------------------------------------------------
class HybridNode {

private:
  std::string id_;       // Unique ID for this item
  std::string type_;     // "announcement", "event",
  std::string title_;    // Visible text/title
  std::string date_;     // One consistent date format
  std::string author_;   // Creator (e.g., "Admin", "Teacher")

  // Doubly-linked list hooks (nullptr if not linked)
  HybridNode* prev_;
  HybridNode* next_;

public:
  // Constructors / Destructor
  HybridNode();
  ~HybridNode();

  // Getters (const: do not modify state)
  std::string getId() const       { return id_; }
  std::string getType() const     { return type_; }
  std::string getTitle() const    { return title_; }
  std::string getDate() const     { return date_; }
  std::string getAuthor() const   { return author_; }

  HybridNode* getPrev() const     { return prev_; }
  HybridNode* getNext() const     { return next_; }

  // Setters
  void setId(const std::string& id)         { this->id_ = id; }
  void setType(const std::string& type)     { this->type_ = type; }
  void setTitle(const std::string& title)   { this->title_ = title; }
  void setDate(const std::string& date)     { this->date_ = date; }
  void setAuthor(const std::string& author) { this->author_ = author; }

  void setPrev(HybridNode* node)            { this->prev_ = node; }
  void setNext(HybridNode* node)            { this->next_ = node; }
};


// -----------------------------------------------------------------------------
// CLASS: HybridStructure
// A minimal doubly linked list (head_/tail_) that stores HybridNode items.
// In addition, it supports queue operations over nodes with type_ == "event".
// Design:
//   - List covers the whole collection.
//   - Event queue treats only "event" nodes as a queue view.
//   - eventFront_/eventRear_ point to the first/last event in the list to
//     keep queue operations efficient.
// -----------------------------------------------------------------------------
class HybridStructure {

private:
  // Full list anchors
  HybridNode* head_;     // First node in the list
  HybridNode* tail_;     // Last node in the list

  // Event queue anchors (only for nodes with type_ == "event")
  HybridNode* eventFront_;  // front event within the list
  HybridNode* eventRear_;   // rear event within the list

  // Finds a node by id (linear scan). Returns nullptr if not found.
  // Used by remove/update/search helpers. O(n).
  HybridNode* search(const std::string& id);

  // Ensures eventFront_/eventRear_ are consistent with the current list
  // after insert/remove operations. Typically called when the relative
  // position of the first/last event might have changed.
  void adjustFront();

public:
  // Constructor & Destructor
  // Initializes an empty list (all pointers null).
  HybridStructure();

  // Deletes all remaining nodes in the list to avoid leaks. Safe to call on
  // an already empty list.
  ~HybridStructure();

  // ---------------------------------------------------------------------------
  // Linked-list style operations over the whole collection
  // ---------------------------------------------------------------------------

  /**
   * addAnnouncement()
   * Creates a new HybridNode
   * enforces unique id_ by time input, links it into the list
   * and updates event pointers if the new node is an "event".
   *
   * Returns:
   *   true  - node created and inserted
   *   false - validation failed (e.g., duplicate id or bad input)
   *
   * Complexity: O(1) for tail insertion.
   */
  bool addAnnouncement();

  /**
   * removeAnnouncement(id)
   * Removes the node with the given id from the list (if present) and frees it.
   * Also updates head_/tail_ and eventFront_/eventRear_ if those pointers
   * referred to the removed node.
   *
   * Example:
   *   removeAnnouncement("A-102");
   *
   * Returns:
   *   true  - a node was found and removed
   *   false - no node with this id
   *
   * Complexity: O(n) due to linear search.
   */
  bool removeAnnouncement(const std::string& id);

  /**
   * searchAnnouncement(id)
   * Checks if a node with the given id exists in the list.
   *
   * Example:
   *   bool exists = searchAnnouncement("E-009");
   *
   * Returns:
   *   true  - found
   *   false - not found
   *
   * Complexity: O(n).
   */
  bool searchAnnouncement(const std::string& id);

  /**
   * updateAnnouncement(id)
   * Finds a node by id and updates fields (e.g., title/date/author).
   * The exact prompts/fields come from the .cpp. Event pointers are adjusted
   * only if type_ changes to/from "event".
   *
   * Example:
   *   updateAnnouncement("A-007")
   *
   * Returns:
   *   true  - updated successfully
   *   false - id not found
   *
   * Complexity: O(n) to find + O(1) to modify.
   */
  bool updateAnnouncement(const std::string& id);

  /**
   * displayAnnouncement() const
   * Prints all nodes in list order with a consistent format. Does not modify data.
   *
   * Complexity: O(n).
   */
  void displayAnnouncement() const;

  // ---------------------------------------------------------------------------
  // Event queue view (only nodes with type_ == "event")
  // These operations treat the "event" subset as a FIFO queue, backed by the
  // same underlying list. eventFront_/eventRear_ are maintained to keep these
  // operations efficient after initial alignment.
  // ---------------------------------------------------------------------------

  /**
   * enqueueEvent()
   * Adds a new "event" into the queue view. In practice, this inserts a node
   * into the list (commonly at the end) with type_ == "event", then updates
   * eventRear_/eventFront_. If the implementation instead reuses an existing
   * node, it should still ensure the rear pointer matches the newest event.
   *
   * Returns:
   *   true  - event enqueued/linked correctly
   *   false - validation failed or no event data provided
   *
   * Typical complexity: O(1).
   */
  bool enqueueEvent();

  /**
   * dequeueEvent()
   * Removes the front-most "event". Updates list links and the
   * eventFront_/eventRear_ pointers accordingly. If there are no events,
   * returns false and does nothing.
   *
   * Returns:
   *   true  - an event was dequeued and removed
   *   false - no event to dequeue
   *
   * Typical complexity: O(1).
   */
  bool dequeueEvent();

  /**
   * peekEvent() const
   * Shows (does not remove) the current front "event". If there are no events,
   * it report that state in the .cpp (e.g., print a message).
   *
   * Complexity: O(1).
   */
  void peekEvent() const;

  /**
   * displayEvents() const
   * Prints only nodes with type_ == "event" in queue order (front to rear).
   *
   * Complexity: O(n)
   */
  void displayEvents() const;
};

#endif //MYPROJECT_HYBRIDSTRUCTURE_H

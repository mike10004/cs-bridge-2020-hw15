#ifndef HW15_LINKEDLIST_H
#define HW15_LINKEDLIST_H

#include <vector>

template<class T>
class Predicate
{
public:
    Predicate();
    virtual ~Predicate();
    virtual bool Evaluate(const T& item) const = 0;
};

template<class T>
struct LinkedListNode {
    LinkedListNode<T>* next;
    LinkedListNode<T>* prev;
    T content;
};

template<class T>
class DoublyLinkedList {
public:
    DoublyLinkedList();
    ~DoublyLinkedList();
    void PushFront(const T& item);
    void PushBack(const T& item);
    T GetFront();
    T GetBack();
    bool PopBack();
    bool PopFront();
    bool IsEmpty();
    size_t Count();
    T* FindElement(const Predicate<T>&);
    std::vector<T> CopyToVector();
    bool operator==(const std::vector<T> vector);
private:
    DoublyLinkedList(const DoublyLinkedList<T>& other); // make private to prevent copying
    LinkedListNode<T>* head_;
    LinkedListNode<T>* tail_;
    size_t count_;
};


template<class T>
Predicate<T>::Predicate() = default;

template<class T>
Predicate<T>::~Predicate() = default;

template<class T>
bool DoublyLinkedList<T>::IsEmpty() {
    return Count() == 0;
}

template<class T>
std::vector<T> DoublyLinkedList<T>::CopyToVector() {
    std::vector<T> items;
    items.reserve(Count());
    LinkedListNode<T>* current = head_;
    while (current != nullptr) {
        items.push_back(current->content);
        current = current->next;
    }
    return items;
}

template<class T>
DoublyLinkedList<T>::DoublyLinkedList() : head_(nullptr), tail_(nullptr), count_(0) {

}

template<class T>
T* DoublyLinkedList<T>::FindElement(const Predicate<T>& predicate) {
    LinkedListNode<T>* current = head_;
    while (current != nullptr) {
        if (predicate.Evaluate(current->content)) {
            return &(current->content);
        }
        current = head_->next;
    }
    return nullptr;
}

template<class T>
bool DoublyLinkedList<T>::operator==(const std::vector<T> vector) {
    // TODO impl
}

template<class T>
size_t DoublyLinkedList<T>::Count() {
    return count_;
}

template<class T>
void DoublyLinkedList<T>::PushBack(const T &item) {
    // TODO impl
}

template<class T>
void DoublyLinkedList<T>::PushFront(const T &item) {
    // TODO impl
}

template<class T>
T DoublyLinkedList<T>::GetBack() {
    if (IsEmpty()) {
        return T();
    }
    return tail_->content;
}

template<class T>
DoublyLinkedList<T>::~DoublyLinkedList() {
    while (!IsEmpty()) {
        PopBack();
    }
}

template<class T>
T DoublyLinkedList<T>::GetFront() {
    if (IsEmpty()) {
        return T();
    }
    return head_->content;
}

template<class T>
bool DoublyLinkedList<T>::PopBack() {
    if (IsEmpty()) {
        return false;
    }
    LinkedListNode<T>* tail = tail_;
    LinkedListNode<T>* head = head_;
    if (Count() == 1) {
        delete head;
        head_ = nullptr;
        tail_ = nullptr;
        count_--;
        return true;
    }
    // TODO handle n > 1 case
}

template<class T>
bool DoublyLinkedList<T>::PopFront() {
    if (IsEmpty()) {
        return false;
    }
    // TODO handle n == 1 case
    // TODO handle n > 1 case
}


#endif //HW15_LINKEDLIST_H

#ifndef HW15_LINKEDLIST_H
#define HW15_LINKEDLIST_H

#include <vector>
#include <list>

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
    void push_front(const T& item);
    void push_back(const T& item);
    T pop_front();
    T pop_back();
    bool empty();
    size_t size();
    T* FindElement(const Predicate<T>&);
    std::vector<T> CopyToVector();
    bool operator==(const std::vector<T> vector);
private:
    LinkedListNode<T>* head_;
    LinkedListNode<T>* tail_;
    size_t count_;
    std::list<T> list_;
};


template<class T>
Predicate<T>::Predicate() = default;

template<class T>
Predicate<T>::~Predicate() = default;

template<class T>
void DoublyLinkedList<T>::push_back(const T &item) {
    list_.push_back(item);
}

template<class T>
bool DoublyLinkedList<T>::empty() {
    return list_.empty();
}

template<class T>
std::vector<T> DoublyLinkedList<T>::CopyToVector() {
    return std::vector<T>(list_.begin(), list_.end());
}

template<class T>
DoublyLinkedList<T>::DoublyLinkedList() : head_(nullptr), tail_(nullptr), count_(0) {

}

template<class T>
T* DoublyLinkedList<T>::FindElement(const Predicate<T>& predicate) {
    for (T& element : list_) {
        if (predicate.Evaluate(element)) {
            return &element;
        }
    }
    return nullptr;
}

template<class T>
bool DoublyLinkedList<T>::operator==(const std::vector<T> vector) {
    std::vector<T> me(list_.begin(), list_.end());
    return me == vector;
}

template<class T>
size_t DoublyLinkedList<T>::size() {
    return list_.size();
}

template<class T>
void DoublyLinkedList<T>::push_front(const T &item) {
    list_.push_front(item);
}

template<class T>
DoublyLinkedList<T>::~DoublyLinkedList() = default;



#endif //HW15_LINKEDLIST_H

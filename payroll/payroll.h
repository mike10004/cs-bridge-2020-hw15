#ifndef HW15_PAYROLL_H
#define HW15_PAYROLL_H

#include <cstddef>
#include <vector>

long Raise(int base, int exponent);

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
    virtual ~DoublyLinkedList();
    void push_front(const T& item);
    void push_back(const T& item);
    T pop_front();
    T pop_back();
    bool empty();
    size_t size();
    std::vector<T> ToVector();
private:
    LinkedListNode<T>* head_;
    LinkedListNode<T>* tail_;
    size_t count_;
};


#endif //HW15_PAYROLL_H

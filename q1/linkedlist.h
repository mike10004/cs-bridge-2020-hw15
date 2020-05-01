#ifndef HW15_LINKEDLIST_H
#define HW15_LINKEDLIST_H

#include <vector>
#include <cassert>

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
    explicit LinkedListNode(const T& content);
};

template<class T>
LinkedListNode<T>::LinkedListNode(const T &content)
    : content(content), next(nullptr), prev(nullptr)
{

}

template<class T>
class DoublyLinkedList {
public:
    DoublyLinkedList();
    ~DoublyLinkedList();
    DoublyLinkedList(const DoublyLinkedList<T>& other);
    void PushFront(const T& item);
    void PushBack(const T& item);
    T GetFront() const;
    T GetBack() const;
    bool PopBack();
    bool PopFront();
    bool IsEmpty() const;
    size_t Count() const;
    void Clear();
    T* FindElement(const Predicate<T>&) const;
    DoublyLinkedList<T>& operator=(const DoublyLinkedList<T>& other);
    bool operator==(const DoublyLinkedList<T>& other) const;
    template<class U>
    friend bool operator==(const DoublyLinkedList<U>& list, const std::vector<U>& vector);
private:
    LinkedListNode<T>* head_;
    LinkedListNode<T>* tail_;
    size_t count_;
    void Assign(const DoublyLinkedList<T>& other);
    void Check();
};

template<class T>
bool operator==(const DoublyLinkedList<T>& list, const std::vector<T>& vector) {
    if (list.Count() != vector.size()) {
        return false;
    }
    size_t index = 0;
    for (LinkedListNode<T>* curr = list.head_; curr != nullptr; curr = curr->next) {
        if (curr->content != vector[index]) {
            return false;
        }
        index++;
    }
    return true;
}

template<class T>
bool operator==(const std::vector<T> vector, const DoublyLinkedList<T>& list) {
    return list == vector;
}


template<class T>
Predicate<T>::Predicate() = default;

template<class T>
Predicate<T>::~Predicate() = default;

template<class T>
bool DoublyLinkedList<T>::IsEmpty() const {
    return Count() == 0;
}

template<class T>
DoublyLinkedList<T>::DoublyLinkedList() : head_(nullptr), tail_(nullptr), count_(0) {

}

template<class T>
T* DoublyLinkedList<T>::FindElement(const Predicate<T>& predicate) const {
    for (LinkedListNode<T>* current = head_; current != nullptr; current = current->next) {
        if (predicate.Evaluate(current->content)) {
            return &(current->content);
        }
    }
    return nullptr;
}

template<class T>
size_t DoublyLinkedList<T>::Count() const {
    return count_;
}

template<class T>
T DoublyLinkedList<T>::GetBack() const {
    assert(!IsEmpty());
    return tail_->content;
}

template<class T>
DoublyLinkedList<T>::~DoublyLinkedList() {
    Clear();
}

template<class T>
T DoublyLinkedList<T>::GetFront() const {
    assert(!IsEmpty());
    return head_->content;
}

#pragma clang diagnostic push                       // stage: cut
#pragma ide diagnostic ignored "modernize-use-auto" // stage: cut
template<class T>
void DoublyLinkedList<T>::PushBack(const T &item) {
    LinkedListNode<T>* new_tail = new LinkedListNode<T>(item);
    if (IsEmpty()) {
        head_ = new_tail;
        tail_ = new_tail;
    } else {
        new_tail->prev = tail_;
        tail_->next = new_tail;
        tail_ = new_tail;
    }
    count_++;
    Check();
}

template<class T>
void DoublyLinkedList<T>::PushFront(const T &item) {
    LinkedListNode<T>* new_head = new LinkedListNode<T>(item);
    if (IsEmpty()) {
        head_ = new_head;
        tail_ = new_head;
    } else {
        head_->prev = new_head;
        new_head->next = head_;
        head_ = new_head;
    }
    count_++;
    Check();
}
#pragma clang diagnostic pop                        // stage: cut

template<class T>
bool DoublyLinkedList<T>::PopBack() {
    if (IsEmpty()) {
        return false;
    }
    LinkedListNode<T>* tail = tail_;
    if (head_ == tail_) {
        head_ = nullptr;
        tail_ = nullptr;
    } else {
        tail_ = tail->prev;
        tail_->next = nullptr;
    }
    delete tail;
    count_--;
    Check();
    return true;
}

template<class T>
bool DoublyLinkedList<T>::PopFront() {
    if (IsEmpty()) {
        return false;
    }
    LinkedListNode<T>* head = head_;
    if (head_ == tail_) {
        head_ = nullptr;
        tail_ = nullptr;
    } else {
        head_ = head->next;
        head_->prev = nullptr;
    }
    delete head;
    count_--;
    Check();
    return true;
}

template<class T>
bool DoublyLinkedList<T>::operator==(const DoublyLinkedList<T> &other) const {
    if (IsEmpty() && other.IsEmpty()) {
        return true;
    }
    LinkedListNode<T>* my_current = head_;
    LinkedListNode<T>* other_current = other.head_;
    while (my_current != nullptr) {
        if (other_current == nullptr) {
            return false;
        }
        if (my_current->content != other_current->content) {
            return false;
        }
        my_current = my_current->next;
        other_current = other_current->next;
    }
    return other_current != nullptr;
}

template<class T>
void DoublyLinkedList<T>::Assign(const DoublyLinkedList<T> &other) {
    Clear();
    for (LinkedListNode<T>* other_node = other.head_; other_node != nullptr; other_node = other_node->next) {
        PushBack(other_node->content);
    }
}

template<class T>
void DoublyLinkedList<T>::Clear() {
    while (!IsEmpty()) {
        PopBack();
    }
}

template<class T>
DoublyLinkedList<T>::DoublyLinkedList(const DoublyLinkedList<T> &other)
    : head_(nullptr), tail_(nullptr), count_(0)
{
    Assign(other);
}

template<class T>
void DoublyLinkedList<T>::Check() {
    if (count_ == 0) {
        assert(head_ == nullptr && tail_ == nullptr);
    } else if (count_ == 1) {
        assert(head_ == tail_);
    } else {
        assert(head_ != tail_);
    }
    assert(head_ == nullptr || head_->prev == nullptr);
    assert(tail_ == nullptr || tail_->next == nullptr);
    // stage: cut start
    size_t forward_count = 0;
    for (LinkedListNode<T>* curr = head_; curr != nullptr; curr = curr->next) {
        forward_count++;
    }
    assert(forward_count == count_);
    size_t backward_count = 0;
    for (LinkedListNode<T>* curr = tail_; curr != nullptr; curr = curr->prev) {
        backward_count++;
    }
    assert(backward_count == count_);
    // stage: cut stop
}

template<class T>
DoublyLinkedList<T> &DoublyLinkedList<T>::operator=(const DoublyLinkedList<T> &other) {
    if (this != &other) {
        Assign(other);
    }
}

#endif //HW15_LINKEDLIST_H

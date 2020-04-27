#include <catch.hpp>
#include <vector>
#include "../q1/linkedlist.h"

class EqualsPredicate : public Predicate<int> {
private:
    int value_;
public:
    explicit EqualsPredicate(int value) : value_(value) {}
    bool Evaluate(const int &item) const override {
        return value_ == item;
    }
};

TEST_CASE("DoublyLinkedList_push_back") {
    DoublyLinkedList<int> list;
    REQUIRE(list.IsEmpty());
    list.PushBack(3);
    REQUIRE_FALSE(list.IsEmpty());
    REQUIRE(list.Count() == 1);
    list.PushBack(8);
    std::vector<int> items({3, 8});
    REQUIRE(items == list.CopyToVector());
    int* el_ptr = list.FindElement(EqualsPredicate(8));
    REQUIRE(el_ptr != nullptr);
    REQUIRE(*el_ptr == 8);
    int* not_found = list.FindElement(EqualsPredicate(5));
    REQUIRE(not_found == nullptr);
    list.PushFront(5);
    items = {5, 3, 8};
    REQUIRE(items == list.CopyToVector());
}
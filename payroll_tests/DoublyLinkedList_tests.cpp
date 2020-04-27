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
    REQUIRE(list.GetFront() == 3);
    REQUIRE(list.GetBack() == 3);
    list.PushBack(8);
    REQUIRE(list.GetFront() == 3);
    REQUIRE(list.GetBack() == 8);
    std::vector<int> items1({3, 8});
    REQUIRE(items1 == list.CopyToVector());
    int* el_ptr = list.FindElement(EqualsPredicate(8));
    REQUIRE(el_ptr != nullptr);
    REQUIRE(*el_ptr == 8);
    int* not_found = list.FindElement(EqualsPredicate(5));
    REQUIRE(not_found == nullptr);
    list.PushFront(5);
    std::vector<int> items2 = {5, 3, 8};
    REQUIRE(items2 == list.CopyToVector());
    REQUIRE(list.PopBack());
    std::vector<int> items3({5, 3});
    REQUIRE(items3 == list.CopyToVector());
    REQUIRE(list.PopFront());
    std::vector<int> items4({3});
    REQUIRE(items4 == list.CopyToVector());
    REQUIRE(list.PopBack());
    REQUIRE_FALSE(list.PopBack());
    REQUIRE_FALSE(list.PopFront());
    list.PushFront(4);
    std::vector<int> items5({4});
    REQUIRE(items5 == list.CopyToVector());
}
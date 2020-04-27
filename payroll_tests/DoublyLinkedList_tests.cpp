#include <catch.hpp>
#include <vector>
#include "../q1/linkedlist.h"

TEST_CASE("DoublyLinkedList_push_back") {
    DoublyLinkedList<int> list;
    REQUIRE(list.empty());
    list.push_back(3);
    REQUIRE_FALSE(list.empty());
    REQUIRE(list.size() == 1);
    list.push_back(8);
    std::vector<int> items({3, 8});
    REQUIRE(items == list.MakeVectorCopy());
}
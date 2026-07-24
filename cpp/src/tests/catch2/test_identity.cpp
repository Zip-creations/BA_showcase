#include "ToDoList.hpp"

#include <catch2/catch_test_macros.hpp>
#include <stdexcept>

TEST_CASE("Copy", "[identity]") {
    ToDoList todoList1;
    ToDoList todoList2;
    ToDoList todoList3 = todoList1;

    todoList1.addItem(ToDoItem("Item 1"));

    REQUIRE(todoList1.getItemByID(0).content == "Item 1");

    REQUIRE_THROWS_AS(todoList2.getItemByID(0), std::out_of_range);
    REQUIRE_THROWS_AS(todoList3.getItemByID(0), std::out_of_range);

    todoList2.addItem(ToDoItem("Item 1"));

    REQUIRE(todoList1.getItemByID(0).content == "Item 1");
    REQUIRE(todoList2.getItemByID(0).content == "Item 1");
    REQUIRE_THROWS_AS(todoList3.getItemByID(0), std::out_of_range);

    todoList3.addItem(ToDoItem("Item 1"));

    REQUIRE(todoList1.getItemByID(0).content == "Item 1");
    REQUIRE(todoList2.getItemByID(0).content == "Item 1");
    REQUIRE(todoList3.getItemByID(0).content == "Item 1");
}

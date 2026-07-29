#define CATCH_CONFIG_MAIN

#include "ToDoList.hpp"
#include <catch2/catch.hpp>
#include <stdexcept>
#include <string>

TEST_CASE("test_add_item") {
    ToDoList todoList;

    ToDoItem item1("Item 1");
    ToDoItem item2("Item 2");

    todoList.addItem(item1);
    todoList.addItem(item2);

    REQUIRE(todoList.getItemByID(0).content == "Item 1");
    REQUIRE(todoList.getItemByID(1).content == "Item 2");
}

TEST_CASE("test_removing_items") {
    ToDoList todoList;

    ToDoItem item1("Item 1");
    ToDoItem item2("Item 2");

    todoList.addItem(item1);
    REQUIRE(todoList.getItemByID(0).content == "Item 1");

    todoList.addItem(item2);
    REQUIRE(todoList.getItemByID(1).content == "Item 2");

    todoList.removeItemByID(0);

    REQUIRE_THROWS_AS(todoList.getItemByID(0), std::out_of_range);
    REQUIRE(todoList.getItemByID(1).content == "Item 2");
}

TEST_CASE("test_skipping") {
    REQUIRE(true);
}

TEST_CASE("test_simple") {
    REQUIRE(std::string("foo") != std::string("bar"));
}

TEST_CASE("test_simple_two") {
    REQUIRE(1 + 1 == 2);
}

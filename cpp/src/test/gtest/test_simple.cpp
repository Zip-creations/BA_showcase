#include "ToDoList.hpp"

#include <gtest/gtest.h>
#include <stdexcept>

TEST(TestSimple, test_add_item) {
    ToDoList todoList;

    ToDoItem item1("Item 1");
    ToDoItem item2("Item 2");

    todoList.addItem(item1);
    todoList.addItem(item2);

    EXPECT_EQ(todoList.getItemByID(0).content, "Item 1");
    EXPECT_EQ(todoList.getItemByID(1).content, "Item 2");
}

TEST(TestSimple, test_removing_items) {
    ToDoList todoList;

    ToDoItem item1("Item 1");
    ToDoItem item2("Item 2");

    todoList.addItem(item1);
    EXPECT_EQ(todoList.getItemByID(0).content, "Item 1");

    todoList.addItem(item2);
    EXPECT_EQ(todoList.getItemByID(1).content, "Item 2");

    todoList.removeItemByID(0);

    EXPECT_THROW(todoList.getItemByID(0), std::out_of_range);
    EXPECT_EQ(todoList.getItemByID(1).content, "Item 2");
}

TEST(TestSimple, test_skipping) {
    GTEST_SKIP() << "this test will be skipped, to see how gtest handles skipped tests in JUnit XML reports";
}

TEST(TestSimple, test_simple) {
    EXPECT_NE(std::string("foo"), std::string("bar"));
}

TEST(TestSimple, test_simple_two) {
    EXPECT_EQ(1 + 1, 2);
}

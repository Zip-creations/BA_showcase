#include "ToDoList.hpp"

#include <gtest/gtest.h>
#include <stdexcept>

TEST(ToDoListSimpleTest, AddItem) {
    ToDoList todoList;

    ToDoItem item1("Item 1");
    ToDoItem item2("Item 2");

    todoList.addItem(item1);
    todoList.addItem(item2);

    EXPECT_EQ(todoList.getItemByID(0).content, "Item 1");
    EXPECT_EQ(todoList.getItemByID(1).content, "Item 2");
}

TEST(ToDoListSimpleTest, RemovingItems) {
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

TEST(ToDoListSimpleTest, DISABLED_Skipping) {
    ASSERT_TRUE(true);
}

TEST(ToDoListSimpleTest, Simple) {
    EXPECT_NE(std::string("foo"), std::string("bar"));
}

TEST(ToDoListSimpleTest, SimpleTwo) {
    EXPECT_EQ(1 + 1, 2);
}

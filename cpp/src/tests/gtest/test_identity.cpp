#include "ToDoList.hpp"

#include <gtest/gtest.h>

TEST(ToDoListIdentityTest, Copy) {
    ToDoList todoList1;
    ToDoList todoList2;
    ToDoList todoList3 = todoList1;

    todoList1.addItem(ToDoItem("Item 1"));

    EXPECT_EQ(todoList1.getItemByID(0).content, "Item 1");

    EXPECT_THROW(todoList2.getItemByID(0), std::out_of_range);
    EXPECT_THROW(todoList3.getItemByID(0), std::out_of_range);

    todoList2.addItem(ToDoItem("Item 1"));

    EXPECT_EQ(todoList1.getItemByID(0).content, "Item 1");
    EXPECT_EQ(todoList2.getItemByID(0).content, "Item 1");
    EXPECT_THROW(todoList3.getItemByID(0), std::out_of_range);

    todoList3.addItem(ToDoItem("Item 1"));

    EXPECT_EQ(todoList1.getItemByID(0).content, "Item 1");
    EXPECT_EQ(todoList2.getItemByID(0).content, "Item 1");
    EXPECT_EQ(todoList3.getItemByID(0).content, "Item 1");
}

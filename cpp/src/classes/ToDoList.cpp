#include "ToDoList.hpp"

ToDoItem::ToDoItem(const std::string& content)
    : content(content) {
}

void ToDoList::addItem(const ToDoItem& item) {
    items.emplace(nextID, item);
    nextID++;
}

void ToDoList::removeItemByID(int id) {
    items.erase(id);
}

ToDoItem ToDoList::getItemByID(int id) const {
    return items.at(id);
}

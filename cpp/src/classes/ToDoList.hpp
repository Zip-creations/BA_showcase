#ifndef TODO_LIST_HPP
#define TODO_LIST_HPP

#include <map>
#include <string>

class ToDoItem {
public:
    std::string content;

    explicit ToDoItem(const std::string& content);
};

class ToDoList {
private:
    std::map<int, ToDoItem> items;
    int nextID = 0;

public:
    void addItem(const ToDoItem& item);
    void removeItemByID(int id);
    ToDoItem getItemByID(int id) const;
};

#endif

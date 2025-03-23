#ifndef TODO_H
#define TODO_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>  // Required for file I/O

class Task {
private:
    std::string description;
    bool completed;
public:
    Task(const std::string& desc) : description(desc), completed(false) {} // Constructor
    std::string getDescription() const { return description; }
    bool isCompleted() const { return completed; }
    void setCompleted(bool complete) { completed = complete; }
    void setDescription(const std::string& desc){ description = desc;}
};

class ToDoList {
private:
    std::vector<Task> tasks;
public:
    void addTask(const std::string& description);
    void viewTasks() const;
    void markTaskComplete(int taskIndex);
    void removeTask(int taskIndex);
    int getSize() const;
    Task getTask(int index) const;

};

class Menu {
private:
    ToDoList todoList;
public:
    void run(); // The main menu loop
};

class SaveTasks {
public:
    static void saveTasksToFile(const ToDoList& todoList, const std::string& filename);
    static void loadTasksFromFile(ToDoList& todoList, const std::string& filename);
};

#endif

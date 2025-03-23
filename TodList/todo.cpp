#include "todo.h"

void ToDoList::addTask(const std::string& description) {
    Task newTask(description);
    tasks.push_back(newTask);
    std::cout << "Task added: " << description << std::endl;
}

void ToDoList::viewTasks() const {
    if (tasks.empty()) {
        std::cout << "No tasks in the list." << std::endl;
        return;
    }

    for (size_t i = 0; i < tasks.size(); ++i) {
        std::cout << "[" << (tasks[i].isCompleted() ? "x" : " ") << "] "
                  << (i + 1) << ". " << tasks[i].getDescription() << std::endl;
    }
}

void ToDoList::markTaskComplete(int taskIndex) {
    if (taskIndex >= 1 && taskIndex <= tasks.size()) {
        tasks[taskIndex - 1].setCompleted(true);
        std::cout << "Task " << taskIndex << " marked as complete." << std::endl;
    } else {
        std::cout << "Invalid task index." << std::endl;
    }
}

void ToDoList::removeTask(int taskIndex) {
    if (taskIndex >= 1 && taskIndex <= tasks.size()) {
        tasks.erase(tasks.begin() + taskIndex - 1);
        std::cout << "Task " << taskIndex << " removed." << std::endl;
    } else {
        std::cout << "Invalid task index." << std::endl;
    }
}

int ToDoList::getSize() const{
    return tasks.size();
}

Task ToDoList::getTask(int index) const{
    return tasks[index];
}

void Menu::run() {
    int choice;
    do {
        std::cout << "\nTo-Do List Menu:" << std::endl;
        std::cout << "1. Add Task" << std::endl;
        std::cout << "2. View Tasks" << std::endl;
        std::cout << "3. Mark Task Complete" << std::endl;
        std::cout << "4. Remove Task" << std::endl;
        std::cout << "5. Save Tasks to File" << std::endl;
        std::cout << "6. Load Tasks from File" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::string description;
                std::cin.ignore(); // Consume the newline character left by the previous input
                std::cout << "Enter task description: ";
                std::getline(std::cin, description);
                todoList.addTask(description);
                break;
            }
            case 2:
                todoList.viewTasks();
                break;
            case 3: {
                int taskIndex;
                std::cout << "Enter the index of the task to mark complete: ";
                std::cin >> taskIndex;
                todoList.markTaskComplete(taskIndex);
                break;
            }
            case 4: {
                int taskIndex;
                std::cout << "Enter the index of the task to remove: ";
                std::cin >> taskIndex;
                todoList.removeTask(taskIndex);
                break;
            }
            case 5: {
                std::string filename;
                std::cin.ignore();
                std::cout << "Enter filename to save tasks to: ";
                std::getline(std::cin, filename);
                SaveTasks::saveTasksToFile(todoList, filename);
                break;
            }
            case 6: {
                std::string filename;
                std::cin.ignore();
                std::cout << "Enter filename to load tasks from: ";
                std::getline(std::cin, filename);
                SaveTasks::loadTasksFromFile(todoList, filename);
                break;
            }
            case 0:
                std::cout << "Exiting..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != 0);
}

void SaveTasks::saveTasksToFile(const ToDoList& todoList, const std::string& filename) {
    std::ofstream outputFile(filename);
    if (outputFile.is_open()) {
        int size = todoList.getSize();
        for (int i = 0; i < size; ++i) {
            Task task = todoList.getTask(i);
            outputFile << task.getDescription() << std::endl;
            outputFile << task.isCompleted() << std::endl;
        }
        outputFile.close();
        std::cout << "Tasks saved to " << filename << std::endl;
    } else {
        std::cout << "Unable to open file for saving." << std::endl;
    }
}

void SaveTasks::loadTasksFromFile(ToDoList& todoList, const std::string& filename) {
    std::ifstream inputFile(filename);
    if (inputFile.is_open()) {
        std::string description;
        bool completed;
        while (std::getline(inputFile, description)) {
            inputFile >> completed;
            inputFile.ignore();
            todoList.addTask(description);
            if(completed){
                todoList.markTaskComplete(todoList.getSize());
            }
        }
        inputFile.close();
        std::cout << "Tasks loaded from " << filename << std::endl;
    } else {
        std::cout << "Unable to open file for loading." << std::endl;
    }
}

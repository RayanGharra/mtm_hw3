
#include "TaskManager.h"

using namespace std;

TaskManager::TaskManager(): employeesCount(0), next_task_id(0) {

}

void TaskManager::assignTask(const std::string& name, const Task& task) {
    int index = -1;

    for (int i = 0; i < employeesCount; ++i) {
        if (employees[i].person.getName() == name) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        if (employeesCount >= 10) {
            throw std::runtime_error("Error: Maximum number of employees reached.");
        }

        employees[employeesCount].person = Person(name);
        index = employeesCount;
        employeesCount++;
    }
    Task copy = task;
    copy.setId(next_task_id);
    next_task_id++;

    employees[index].tasks.insert(copy);
}

void TaskManager::completeTask(const std::string& name) {
    int index = -1;

    for (int i = 0; i < employeesCount; ++i) {
        if (employees[i].person.getName() == name) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        return;
    }

    if (employees[index].tasks.begin() != employees[index].tasks.end()) {
        auto highest_priority_it = employees[index].tasks.begin();
        employees[index].tasks.remove(highest_priority_it);
    }
}

void TaskManager::bumpPriorityByType(TaskType type, int amount) {

    if (amount < 0) {
        return;
    }

    for (int i = 0; i < employeesCount; ++i) {

        SortedList<Task> matching_tasks;
        auto it = employees[i].tasks.begin();
        while (it != employees[i].tasks.end()) {

            if ((*it).getType() == type) {
                int new_priority = (*it).getPriority() + amount;
                if (new_priority > 100) {
                    new_priority = 100;
                }

                Task updated_task(new_priority, (*it).getType(), (*it).getDescription());

                updated_task.setId((*it).getId());
                matching_tasks.insert(updated_task);

                auto to_remove = it;
                ++it;
                employees[i].tasks.remove(to_remove);
            }
            else {
                ++it;
            }
        }
        auto temp_it = matching_tasks.begin();
        while (temp_it != matching_tasks.end()) {
            employees[i].tasks.insert(*temp_it);
            ++temp_it;
        }
    }
}

void TaskManager::printAllEmployees() const {
    for (int i = 0; i < employeesCount; ++i) {
        cout << employees[i].person;
    }
}

void TaskManager::printAllTasks() const {
    SortedList<Task> all_tasks;

    for (int i = 0; i < employeesCount; ++i) {
        auto it = employees[i].tasks.begin();
        while (it != employees[i].tasks.end()) {
            all_tasks.insert(*it);
            ++it;
        }
    }

    auto print_it = all_tasks.begin();
    while (print_it != all_tasks.end()) {
        cout << *print_it << endl;
        ++print_it;
    }
}

void TaskManager::printTasksByType(TaskType type) const {
    SortedList<Task> filtered_tasks;

    for (int i = 0; i < employeesCount; ++i) {
        auto it = employees[i].tasks.begin();
        while (it != employees[i].tasks.end()) {
            if ((*it).getType() == type) {
                filtered_tasks.insert(*it);
            }
            ++it;
        }
    }

    auto print_it = filtered_tasks.begin();
    while (print_it != filtered_tasks.end()) {
        cout << *print_it << endl;
        ++print_it;
    }
}


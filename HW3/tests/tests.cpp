
#include <iostream>
#include "TaskManager.h"
#include "Task.h"

using std::cout;
using std::endl;

using mtm::SortedList;

typedef bool (*testFunc)(void);

//defines

#define RUN_TEST(test, name)                        \
    do                                              \
    {                                               \
        std::cout << "Running " << name << " ... " << std::endl ; \
        if (test())                                 \
        {                                           \
            std::cout << "[OK]\n";                  \
        }                                           \
        else                                        \
        {                                           \
            std::cout << "[Failed]\n";              \
        }                                           \
    } while (0)

#define ASSERT_TEST(expr)                                                                               \
    do                                                                                                  \
    {                                                                                                   \
        if (!(expr))                                                                                    \
        {                                                                                               \
            std::cout << "\nAssertion failed at " << __FILE__ << ":" << __LINE__ << #expr << std::endl; \
            return false;                                                                               \
        }                                                                                               \
    } while (0)


template<typename T>
void printList(const mtm::SortedList<T> &list, std::ostream &os = std::cout) {
    for (auto it = list.begin(); it != list.end(); ++it) {
        os << *it << " ";
    }
    os << std::endl;
}

template<typename T>
void printListWithNewlines(const mtm::SortedList<T> &list, std::ostream &os = std::cout) {
    for (auto it = list.begin(); it != list.end(); ++it) {
        os << *it << endl;
    }
    os << std::endl;
}


// tests

class ExceptionThrowingType {
public:
    static int copy_count;
    static const int max_copies = 3; // Exception will be thrown on the third copy
    static bool throw_state;

    ExceptionThrowingType(int value = 0) : value(value) {}

    // Copy constructor
    ExceptionThrowingType(const ExceptionThrowingType &other) : value(other.value) {
        if ((++copy_count >= max_copies) && (throw_state)) {
            throw std::bad_alloc();
        }
    }

    // Assignment operator
    ExceptionThrowingType &operator=(const ExceptionThrowingType &other) {
        if (this != &other) {
            value = other.value;
            if (++copy_count >= max_copies) {
                throw std::bad_alloc();
            }
        }
        return *this;
    }

    // Comparison operator
    bool operator>(const ExceptionThrowingType &other) const {
        return value > other.value;
    }

    int getValue() const {
        return value;
    }

    void zeroCounter() {
        this->copy_count = 0;
    }

    void changeState(bool state) {
        this->throw_state = state;
    }

private:
    int value;
};

int ExceptionThrowingType::copy_count = 0;
bool ExceptionThrowingType::throw_state = false;


bool testTaskManagerPrintTasksByType() {
    TaskManager manager;
    Task task1(1, TaskType::Documentation, "Document API methods");
    Task task2(5, TaskType::Development, "Refactor core module");
    Task task3(8, TaskType::Testing, "Integration testing");
    Task task4(5, TaskType::General, "Weekly team meeting");
    Task task5(7, TaskType::Development, "Implement new feature");
    Task task6(3, TaskType::Documentation, "Update user guide");
    Task task7(-1, TaskType::Testing, "Write unit tests");
    Task task8(2000, TaskType::Testing, "stream");
    Task task9(6, TaskType::Development, "Fix bug in UI");
    Task task10(4, TaskType::General, "Clean up code");
    Task task11(2, TaskType::Documentation, "Write README");
    Task task12(1, TaskType::Testing, "Run system tests");
    Task task13(2, TaskType::Testing, "Run system tests");
    Task task14(1, TaskType::Testing, "Run system tests");
    Task task15(1, TaskType::Testing, "Run system tests");
    Task task16(4, TaskType::General, "Clean up code");
    Task task17(4, TaskType::General, "Clean up code");

    manager.assignTask("Alice", task1);
    manager.assignTask("Bob", task2);
    manager.assignTask("Bob", Task(5, TaskType::General, "Run system tests"));
    manager.assignTask("Bob", Task(7, TaskType::General, "Run system tests"));
    manager.assignTask("Bob", Task(-1, TaskType::General, "Run system tests"));
    manager.assignTask("Bob", Task(200, TaskType::General, "200"));
    manager.assignTask("Charlixcx", task3);
    manager.assignTask("Dana", task4);
    manager.assignTask("Eve", task5);
    manager.assignTask("Frank", task6);
    manager.assignTask("Grace", task7);
    manager.assignTask("SOPHIE", task8);
    manager.assignTask("Hank", task9);
    manager.assignTask("Bonie", task10);

    manager.assignTask("Alice", task1);
    manager.assignTask("Bob", task1);

    manager.completeTask("Dana");

    manager.printTasksByType(TaskType::Development);
    cout << endl;
    manager.bumpPriorityByType(TaskType::Development, 50);
    manager.printTasksByType(TaskType::Development);
    cout << endl;
    manager.printTasksByType(TaskType::Testing);
    cout << endl;

    return true;
}

bool testListBasic() {
    // Test default constructor
    SortedList<int> list;
    if (list.length() != 0) {
        return false;
    }

    // Test insert and length
    list.insert(5);
    list.insert(3);
    list.insert(8);
    if (list.length() != 3) {
        return false;
    }

    // Test copy constructor
    SortedList<int> copy(list);
    if (copy.length() != 3) {
        return false;
    }

    // Check copied elements
    auto it_copy = copy.begin();
    auto it_list = list.begin();
    for (int i = 0; i < list.length(); ++i) {
        if ((*it_copy != *it_list) || (&(*it_copy) == &(*it_list))) {
            return false;
        }
        ++it_copy;
        ++it_list;
    }

    // Test assignment operator
    SortedList<int> another_list;
    another_list = list;
    if (another_list.length() != 3) {
        return false;
    }

    // Check assigned elements
    it_list = list.begin();
    auto it_another_list = another_list.begin();
    for (int i = 0; i < list.length(); ++i) {
        if ((*it_another_list != *it_list) || (&(*it_another_list) == &(*it_list))) {
            return false;
        }
        ++it_another_list;
        ++it_list;
    }

    // Ensure deep copy
    list.insert(10);
    if (another_list.length() != 3) {
        return false;
    }

    return true;
}


bool testListExceptions() {
    using mtm::SortedList;

    SortedList<int> list;

    // Attempt to increment an iterator past the end of the list
    try {
        auto it = list.end();
        ++it;         // This should throw an exception
        return false; // If no exception is thrown, the test should fail
    }
    catch (const std::out_of_range &e) {
        // Exception is expected, so the test should pass
    }

    // Insert some elements
    list.insert(5);
    list.insert(3);
    list.insert(8);
    list.insert(1);

    // Attempt to increment an iterator past the end of the list after insertion
    try {
        auto it = list.begin();
        ++it;
        ++it;
        ++it;
        ++it;
        ++it;         // This should throw an exception
        return false; // If no exception is thrown, the test should fail
    }
    catch (const std::out_of_range &e) {
        // Exception is expected, so the test should pass
    }

    // Attempt to remove using an end iterator (which should be invalid)
    try {
        auto endIt = list.end();
        list.remove(endIt); // This should not throw an exception but should handle gracefully
        // No exception expected, so no assert here
    }
    catch (...) {
        return false; // If any exception is thrown, the test should fail
    }

    // Create an iterator and make it invalid by incrementing past the end
    auto it = list.begin();
    ++it;
    ++it;
    ++it;
    ++it; // Now it should be invalid (past end)

    try {
        ++it;         // Incrementing further should throw an exception
        return false; // If no exception is thrown, the test should fail
    }
    catch (const std::out_of_range &e) {
        // Exception is expected, so the test should pass
    }

    ////

    // Test exception safety in copy constructor
    try {
        SortedList<int> list;
        list.insert(1);
        list.insert(2);
        SortedList<int> copy(list); // This should not throw
    }
    catch (...) {
        return false;
    }

    // Test exception safety in assignment operator
    try {
        SortedList<int> list;
        list.insert(1);
        list.insert(2);
        SortedList<int> another_list;
        another_list = list; // This should not throw
    }
    catch (...) {
        return false;
    }

    // Ensure proper memory management in case of exceptions (Mock the exception)
    try {
        SortedList<int> list;
        list.insert(1);
        list.insert(2);
        // Simulate exception during copy constructor

        struct MockException : public std::exception {
        };
        throw MockException();
        SortedList<int> copy(list);
        return false; // Should not reach here
    }
    catch (const std::exception &) {
        // Expected behavior
    }

    return true;
}

bool testCopyConstructorExceptionSafety() {
    try {
        ExceptionThrowingType x(1);
        x.zeroCounter();
        x.changeState(false);
        SortedList<ExceptionThrowingType> list;
        list.insert(ExceptionThrowingType(1));
        list.insert(ExceptionThrowingType(2));

        // Force an exception during the copy constructor
        // add flag
        x.changeState(true);
        SortedList<ExceptionThrowingType> copy(list); // Should throw std::bad_alloc
        return false;                                 // If no exception is thrown, the test fails
    }
    catch (const std::bad_alloc &) {
        // Expected exception was thrown
    }
    catch (...) {
        return false; // Unexpected exception
    }

    return true;
}


bool testTaskManager() {
    TaskManager tm;

    // Create some tasks
    Task task1(1, TaskType::Meeting, "Discuss project goals");
    Task task2(2, TaskType::Development, "Implement feature X");
    Task task3(3, TaskType::Testing, "Test feature X");
    Task task4(4, TaskType::Documentation, "Write docs for feature X");
    Task task5(5, TaskType::Research, "Explore new tech");

    // Assign tasks to employees
    tm.assignTask("Alice", task1);
    tm.assignTask("Bob", task2);
    tm.assignTask("Alice", task3);
    tm.assignTask("Charlie", task4);
    tm.assignTask("Bob", task5);

    tm.printAllEmployees();
    cout << endl;

    tm.printAllTasks();
    cout << endl;

    // Complete a task and verify it no longer appears
    tm.completeTask("Alice");

    tm.printAllTasks();
    cout << endl;

    tm.bumpPriorityByType(TaskType::Documentation, 2);

    tm.printTasksByType(TaskType::Documentation);
    cout << endl;

    tm.printTasksByType(TaskType::Research);
    cout << endl;

    return true;
}

bool testTaskManagerAssignTask() {
    TaskManager manager;
    Task task1(1, TaskType::Documentation, "Document API methods");
    Task task2(5, TaskType::Development, "Refactor core module");
    Task task3(8, TaskType::Testing, "Integration testing");
    Task task4(5, TaskType::General, "Weekly team meeting");
    Task task5(7, TaskType::Development, "Implement new feature");
    Task task6(3, TaskType::Documentation, "Update user guide");
    Task task7(-1, TaskType::Testing, "Write unit tests");
    Task task8(2000, TaskType::Testing, "stream");
    Task task9(6, TaskType::Development, "Fix bug in UI");
    Task task10(4, TaskType::General, "Clean up code");
    Task task11(2, TaskType::Documentation, "Write README");
    Task task12(1, TaskType::Testing, "Run system tests");
    Task task13(1, TaskType::Testing, "Run system tests");
    Task task14(1, TaskType::Testing, "Run system tests");
    Task task15(1, TaskType::Testing, "Run system tests");
    Task task16(4, TaskType::General, "Clean up code");
    Task task17(4, TaskType::General, "Clean up code");

    manager.assignTask("Alice", task1);
    manager.assignTask("Bob", task2);
    manager.assignTask("Charlixcx", task3);
    manager.assignTask("Dana", task4);
    manager.assignTask("Eve", task5);
    manager.assignTask("Frank", task6);
    manager.assignTask("Grace", task7);
    manager.assignTask("SOPHIE", task8);
    manager.assignTask("Hank", task9);
    manager.assignTask("Bonie", task10);

    try {
        manager.assignTask("boom", task11);
        return false; // should have thrown exception
    }
    catch (std::exception &e) {
    }

    manager.assignTask("Bob", task12);
    manager.assignTask("Bob", task13);
    manager.assignTask("Bob", task14);
    manager.assignTask("Bob", task15);
    manager.assignTask("Bob", task16);
    manager.assignTask("Bob", task17);

    // Assuming we have a way to verify task has been assigned, like a method that counts tasks
    return true;
}

bool testAssignmentOperatorExceptionSafety() {
    try {
        ExceptionThrowingType x(1);
        x.zeroCounter();
        x.changeState(false);
        SortedList<ExceptionThrowingType> list;
        list.insert(ExceptionThrowingType(1));
        list.insert(ExceptionThrowingType(2));

        // Force an exception during the copy constructor
        // add flag
        SortedList<ExceptionThrowingType> copyByAssignment;
        x.changeState(true);
        copyByAssignment = list; // Should throw std::bad_alloc
        return false;            // If no exception is thrown, the test fails
    }
    catch (const std::bad_alloc &) {
        // Expected exception was thrown
    }
    catch (...) {
        return false; // Unexpected exception
    }

    return true;
}

bool testDeleteFirstElementOfEmpty() {
    // Test default constructor
    SortedList<int> list;
    if (list.length() != 0) {
        return false;
    }

    // Attempt to remove using the start iterator of an empty list (which should be invalid)
    try {
        auto itStart = list.begin();
        list.remove(itStart); // This should not throw an exception but should handle gracefully
        // No exception expected, so no assert here
    }
    catch (...) {
        return false; // If any exception is thrown, the test should fail
    }

    // Ensure 0 length remains
    if (list.length() != 0) {
        return false;
    }

    return true;
}


bool testDuplicateElementsArePossible1() {
    // Test default constructor
    SortedList<int> list;
    if (list.length() != 0) {
        return false;
    }

    // Test insert and length
    list.insert(5);
    list.insert(3);
    list.insert(8);
    list.insert(5);
    list.insert(10);
    if (list.length() != 5) {
        return false;
    }

    // Test copy constructor
    SortedList<int> copy(list);
    if (copy.length() != 5) {
        return false;
    }

    // Check copied elements
    auto it_copy = copy.begin();
    auto it_list = list.begin();
    for (int i = 0; i < list.length(); ++i) {
        if ((*it_copy != *it_list) || (&(*it_copy) == &(*it_list))) {
            return false;
        }
        ++it_copy;
        ++it_list;
    }

    // Test assignment operator
    SortedList<int> another_list;
    another_list = list;
    if (another_list.length() != 5) {
        return false;
    }

    // Check assigned elements
    it_list = list.begin();
    auto it_another_list = another_list.begin();
    for (int i = 0; i < list.length(); ++i) {
        if ((*it_another_list != *it_list) || (&(*it_another_list) == &(*it_list))) {
            return false;
        }
        ++it_another_list;
        ++it_list;
    }

    // Ensure deep copy
    list.insert(10);
    if (another_list.length() != 5) {
        return false;
    }
    if (list.length() != 6) {
        return false;
    }

    for (const int item: list) {
        cout << item << endl;
    }

    return true;
}

bool testDuplicateElementsArePossible2() {
    // Test default constructor
    SortedList<string> list;
    if (list.length() != 0) {
        return false;
    }

    // Test insert and length
    list.insert("aa");
    list.insert("b");
    list.insert("ccc");
    list.insert("aa");
    list.insert("d");
    if (list.length() != 5) {
        return false;
    }

    // Test copy constructor
    SortedList<string> copy(list);
    if (copy.length() != 5) {
        return false;
    }

    // Check copied elements
    auto it_copy = copy.begin();
    auto it_list = list.begin();
    for (int i = 0; i < list.length(); ++i) {
        if ((*it_copy != *it_list) || (&(*it_copy) == &(*it_list))) {
            return false;
        }
        ++it_copy;
        ++it_list;
    }

    // Test assignment operator
    SortedList<string> another_list;
    another_list = list;
    if (another_list.length() != 5) {
        return false;
    }

    // Check assigned elements
    it_list = list.begin();
    auto it_another_list = another_list.begin();
    for (int i = 0; i < list.length(); ++i) {
        if ((*it_another_list != *it_list) || (&(*it_another_list) == &(*it_list))) {
            return false;
        }
        ++it_another_list;
        ++it_list;
    }

    // Ensure deep copy
    list.insert("d");
    if (another_list.length() != 5) {
        return false;
    }
    if (list.length() != 6) {
        return false;
    }

    for (const string &item: list) {
        cout << item << endl;
    }

    return true;
}

bool testMemoryIndependent() {
    SortedList<int> list;

    // Create a variable on the heap
    int *heapVariable = new int(17);

    // Insert the value into the list
    list.insert(*heapVariable);

    // Verify the list contains the value
    cout << "Initial list: " << endl;
    printList(list);
    cout << endl;

    // Delete the original variable
    delete heapVariable;
    // Verify the list still contains the value
    cout << "List after variable delete: " << endl;
    printList(list);
    cout << endl;

    // Try to modify the list to ensure it's still functional
    list.insert(10);
    list.insert(50);
    cout << "List after insertions: " << endl;
    printList(list);
    cout << endl;

    return true;
}

bool testsBumpPriorityByTypeRange() {
    TaskManager manager;

    // Set up initial tasks
    manager.assignTask("Alice", Task(1, TaskType::Maintenance, "Task 1"));
    manager.assignTask("Bob", Task(80, TaskType::Testing, "Task 2"));
    manager.assignTask("Charlie", Task(30, TaskType::Maintenance, "Task 3"));
    manager.assignTask("Alice", Task(-2, TaskType::Testing, "Negative task"));
    manager.assignTask("Bob", Task(200, TaskType::Maintenance, "Super important task"));


    cout << "Initial state:" << endl;
    manager.printAllTasks();
    cout << endl;

    // Test negative bump (should do nothing)
    cout << "After bumping Maintenance by -10:" << endl;
    manager.bumpPriorityByType(TaskType::Maintenance, -10);
    manager.printAllTasks();
    cout << endl;

    // Test bumping to just reach 100
    cout << "After bumping Testing by 40:" << endl;
    manager.completeTask("Bob");
    manager.bumpPriorityByType(TaskType::Testing, 40);
    manager.printAllTasks();
    cout << endl;

    // Test bumping by more than 100
    cout << "After bumping Testing by 150:" << endl;
    manager.completeTask("Bob");
    manager.bumpPriorityByType(TaskType::Testing, 150);
    manager.printAllTasks();
    cout << endl;

    // Test bumping when already at 100
    cout << "After bumping Testing by 10 (should already be at 100):" << endl;
    manager.bumpPriorityByType(TaskType::Testing, 10);
    manager.printAllTasks();
    cout << endl;

    return true;
}

bool testAssignOperatorExceptionSafety() {
    ExceptionThrowingType x(1);
    x.zeroCounter();
    x.changeState(false);
    SortedList<ExceptionThrowingType> originalList;
    originalList.insert(17);

    try {
        SortedList<ExceptionThrowingType> listToCopy;
        listToCopy.insert(ExceptionThrowingType(1));
        listToCopy.insert(ExceptionThrowingType(2));

        // Force an exception during the assign operator
        // add flag
        x.changeState(true);
        originalList = listToCopy;  // Should throw std::bad_alloc.

        return false; // If no exception is thrown, the test fails
    }
    catch (const std::bad_alloc &) {
        // Expected exception was thrown
        // Assert the original list is not deleted before copying is completed
        const int listLength = originalList.length();
        const auto firstItemValue = (*originalList.begin()).getValue();
        cout << "Original list length: " << listLength << endl;
        cout << "Original list first item value: " << firstItemValue << endl;

        if (listLength != 1 || firstItemValue != 17) {
            return false;
        }

    }
    catch (...) {
        return false; // Unexpected exception
    }

    return true;
}

bool testOperatorChaining() {
    mtm::SortedList<string> list;

    // Populate the list
    list.insert("apple");
    list.insert("banana");
    list.insert("cherry");
    list.insert("date");
    list.insert("elderberry");

    // Test 1: Empty result after filtering
    cout << "Filter for words starting with 'z' (should be empty):" << endl;
    const auto &result1 = list.filter([](const string &s) { return s[0] == 'z'; });
    printList(result1);
    cout << "Length: " << result1.length() << endl << endl;

    // Test 2: Apply to empty list
    cout << "Apply uppercase to empty list:" << endl;
    const auto &result2 = result1.apply([](string s) {
        for (char &c: s) { c = toupper(c); }
        return s;
    });
    printList(result2);
    cout << "Length: " << result2.length() << endl << endl;

    // Test 3: Complex chaining
    cout << "Filter and apply chaining - filter, apply, filter:" << endl;
    const auto &result3 = list
            .filter([](const string &s) { return s.length() > 4; })
            .apply([](string s) {
                for (char &c: s) { c = toupper(c); }
                return s;
            })
            .filter([](const string &s) { return s[0] == 'B' || s[0] == 'E'; });

    printList(result3);
    cout << "Length: " << result3.length() << endl << endl;

    // Test 4: Multiple applies
    cout << "Multiple applies:" << endl;
    const auto &result5 = list
            .apply([](string s) { return s + "1"; })
            .apply([](string s) { return s + "2"; })
            .apply([](string s) { return s + "3"; });

    printList(result5);
    cout << "Length: " << result5.length() << endl << endl;

    return true;
}

bool testCompleteTask() {
    TaskManager manager;

    // Test 1: Completing a task for a non-existent employee (should do nothing)
    cout << "Test 1: Completing a task for a non-existent employee" << endl;
    cout << "Before:" << endl;
    manager.printAllEmployees();
    manager.completeTask("NonExistentEmployee");
    cout << "After:" << endl;
    manager.printAllEmployees();
    cout << endl;

    // Test 2: Completing a task for an employee with no tasks
    cout << "Test 2: Completing a task for an employee with no tasks" << endl;
    manager.assignTask("Alice",
                       Task(1, TaskType::Development, "Task 1")); // Add Alice to the system
    manager.completeTask("Alice"); // Complete the only task
    cout << "Before:" << endl;
    manager.printAllEmployees();
    try {
        // Complete Task for empty employee should throw error
        manager.completeTask("Alice");
        cout << "After:" << endl;
        manager.printAllEmployees();
        return false;
    }
    catch (const std::exception &e) {
        cout << e.what() << endl;
    }
    cout << endl;

    // Test 3: Completing tasks with different priorities
    cout << "Test 3: Completing tasks with different priorities" << endl;
    manager.assignTask("Bob", Task(2, TaskType::Presentation, "Low Priority"));
    manager.assignTask("Bob", Task(37, TaskType::Development, "High Priority"));
    manager.assignTask("Bob", Task(4, TaskType::Presentation, "Medium Priority"));
    manager.completeTask("Bob");
    cout << "Bob's tasks after completing highest priority task:" << endl;
    manager.printAllTasks();
    cout << endl;

    return true;
}

bool testTaskManagerPrintMethods() {
    TaskManager manager;

    cout << "Test 1: Printing empty TaskManager" << endl;
    cout << "printAllEmployees:" << endl;
    manager.printAllEmployees();
    cout << "printAllTasks:" << endl;
    manager.printAllTasks();
    cout << "printTasksByType (General):" << endl;
    manager.printTasksByType(TaskType::General);
    cout << endl;

    cout << "Test 2: Basic employees and tasks" << endl;
    manager.assignTask("Alice", Task(1, TaskType::General, "Alice's Feature"));
    manager.assignTask("Bob", Task(2, TaskType::Presentation, "Bob's Bug"));
    manager.assignTask("Alice", Task(3, TaskType::Research, "Alice's Development"));
    cout << "printAllEmployees:" << endl;
    manager.printAllEmployees();
    cout << "printAllTasks:" << endl;
    manager.printAllTasks();
    cout << endl;

    cout << "Test 3: Completing tasks and reprinting" << endl;
    manager.completeTask("Alice");
    manager.completeTask("Alice");
    manager.completeTask("Bob");
    cout << "printAllEmployees after completing tasks:" << endl;
    manager.printAllEmployees();
    cout << "printAllTasks after completing tasks:" << endl;
    manager.printAllTasks();
    cout << endl;

    cout << "Test 4: Test priority ordering" << endl;
    manager.assignTask("Charlie", Task(4, TaskType::Presentation, "High Priority Feature"));
    manager.assignTask("David", Task(8, TaskType::General, "Low Priority Bug"));
    manager.assignTask("Bob", Task(0, TaskType::General, "Bob's Bug"));
    manager.assignTask("David", Task(1, TaskType::General, "Low Priority Bug"));
    manager.bumpPriorityByType(TaskType::General, 2);
    cout << "printAllEmployees:" << endl;
    manager.printAllEmployees();
    cout << "printAllTasks:" << endl;
    manager.printAllTasks();
    cout << "printTasksByType (General):" << endl;
    manager.printTasksByType(TaskType::General);

    return true;
}

bool testComplexAssignFilterOperations() {
    mtm::SortedList<Task> list;

    // Populate the list
    Task task1 = Task(3, TaskType::Testing, "Foo with a bar");
    Task task2 = Task(1, TaskType::Training, "I love muscles");
    Task task3 = Task(10, TaskType::Documentation, "The happy task");
    Task task4 = Task(20, TaskType::Presentation, "PowerPoint God");
    Task task5 = Task(15, TaskType::Testing, "Foo without a bar");
    Task task6 = Task(17, TaskType::Training, "I love being strong");
    Task task7 = Task(2, TaskType::Documentation, "The sad task");
    Task task8 = Task(40, TaskType::Presentation, "PowerPoint noob");
    task1.setId(1);
    task2.setId(2);
    task3.setId(3);
    task4.setId(4);
    task5.setId(5);
    task6.setId(6);
    task7.setId(7);
    task8.setId(8);
    list.insert(task1);
    list.insert(task2);
    list.insert(task3);
    list.insert(task4);
    list.insert(task5);
    list.insert(task6);
    list.insert(task7);
    list.insert(task8);
    // Test 1: Initial list
    cout << "Part 1 - initial list:" << endl;
    printListWithNewlines(list);

    const auto trainingToTesting = [](const Task &item) {
        if (item.getType() != TaskType::Training) {
            return item;
        }
        Task newTask = Task(item.getPriority(), TaskType::Testing, item.getDescription());
        newTask.setId(item.getId());
        return newTask;
    };

    mtm::SortedList<Task> list2 = list.apply(trainingToTesting);
    cout << "Part 2 - change all Training to Testing:" << endl;
    printListWithNewlines(list2);
    mtm::SortedList<Task> list3 = list2.apply([](const Task &item) {
        if (item.getType() != TaskType::Documentation) {
            return item;
        }
        Task newTask = Task(item.getPriority(), TaskType::Training, item.getDescription());
        newTask.setId(item.getId());
        return newTask;
    });
    cout << "Part 3 - change all Documentation to Training:" << endl;
    printListWithNewlines(list3);

    mtm::SortedList<Task> list4 = list.filter([](const Task &item) {
        return item.getType() == TaskType::Training || item.getType() == TaskType::Testing;
    });
    cout << "Part 4 - Test filter only Training and Testing:" << endl;
    printListWithNewlines(list4);

    mtm::SortedList<Task> list2OnlyTesting = list2.filter([](const Task &item) {
        return item.getType() == TaskType::Testing;
    });

    mtm::SortedList<Task> list4Applied = list4.apply(trainingToTesting);

    cout << "Part 5 - check items of final lists are the same:" << endl;
    if (list2OnlyTesting.length() != list4Applied.length()) {
        return false;
    }
    mtm::SortedList<Task>::ConstIterator it1 = list2OnlyTesting.begin();
    mtm::SortedList<Task>::ConstIterator it2 = list4Applied.begin();
    while (it1 != list2OnlyTesting.end()) {
        if (it1.operator*().getId() == it2.operator*().getId()) {
            cout << it1.operator*() << endl;
        }
        else {
            return false;
        }
        ++it1;
        ++it2;
    }
    cout << endl;

    return true;
}


// end of tests








#define TESTS_NAMES                          \
    X(testListBasic)                         \
    X(testListExceptions)                    \
    X(testTaskManager)                       \
    X(testCopyConstructorExceptionSafety)    \
    X(testTaskManagerAssignTask)             \
    X(testTaskManagerPrintTasksByType)       \
    X(testAssignmentOperatorExceptionSafety) \
    X(testDeleteFirstElementOfEmpty)         \
    X(testDuplicateElementsArePossible1)     \
    X(testDuplicateElementsArePossible2)     \
    X(testMemoryIndependent)                 \
    X(testsBumpPriorityByTypeRange)          \
    X(testAssignOperatorExceptionSafety)     \
    X(testOperatorChaining)                  \
    X(testCompleteTask)                      \
    X(testTaskManagerPrintMethods)           \
    X(testComplexAssignFilterOperations)


testFunc tests[] = {
#define X(name) name,
        TESTS_NAMES
#undef X
};

const char *tests_names[] = {
#define X(name) #name,
        TESTS_NAMES
#undef X
};
using mtm::SortedList;


int main(int argc, char **argv) {
    int number_of_tests = sizeof(tests) / sizeof(tests[0]);

    if (argc == 1) {
        for (int test_idx = 0; test_idx < number_of_tests; test_idx++) {
            std::cout << "Running test " << test_idx + 1 << std::endl;
            RUN_TEST(tests[test_idx], tests_names[test_idx]);
            cout << endl;
        }
        return 0;
    }
    if (argc != 2) {
        std::cout << "Usage: tests <test index>" << std::endl;
        return 0;
    }

    int test_idx = strtol(argv[1], NULL, 10);
    if (test_idx < 1 || test_idx > number_of_tests) {
        std::cerr << "Invalid test index " << test_idx << std::endl;
        return 0;
    }

    RUN_TEST(tests[test_idx - 1], tests_names[test_idx - 1]);
    return 0;
}

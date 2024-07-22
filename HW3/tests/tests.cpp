
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


// end of tests








#define TESTS_NAMES                          \
    X(testListBasic)                         \
    X(testListExceptions)                    \
    X(testTaskManager)                       \
    X(testCopyConstructorExceptionSafety)    \
    X(testTaskManagerAssignTask)             \
    X(testTaskManagerPrintTasksByType)


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

template<typename T>
void printList(const mtm::SortedList<T> &list, std::ostream &os = std::cout) {
    for (auto it = list.begin(); it != list.end(); ++it) {
        os << *it << " ";
    }
    os << std::endl;
}


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

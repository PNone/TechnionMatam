

#include <string>

#include "Matrix.h"
#include "MataMvidia.h"

using namespace std;
typedef bool (*testFunc)(void);

bool testMatrix(std::ostream& out);
bool testMataMvidia(std::ostream& out);


#define ASSERT_TEST(expr)                                                      \
do {                                                                           \
    if (!(expr)) {                                                             \
        std::cout << "\nAssertion failed at ";                                 \
        std::cout << __FILE__ << ":" << __LINE__ << ": " << #expr << std::endl;\
        exit(1);                                                               \
    }                                                                          \
} while (0)


int main(int argc, char **argv) {
    string testName = string(argv[1]);
    if (testName == "example" || testName == "exampleMatrix") {
        testMatrix(std::cout);
    }
    if (testName == "example" || testName == "exampleMataMvidia") {
        testMataMvidia(std::cout);
    }

    return 0;
}

bool testMatrix(std::ostream& out) {
    Matrix m1(2,2);
    Matrix m2(2,2);

    m1(0,0) = 1;
    m1(0,1) = 2;
    m1(1,0) = 3;
    m1(1,1) = 4;

    m2(0,0) = 5;
    m2(0,1) = 6;
    m2(1,0) = 7;
    m2(1,1) = 8;

    Matrix m3 = m1 + m2;
    ASSERT_TEST(m3(0,0) == 6);
    ASSERT_TEST(m3(0,1) == 8);
    ASSERT_TEST(m3(1,0) == 10);
    ASSERT_TEST(m3(1,1) == 12);

    Matrix m4 = m1 * m2;
    m4 *= 1;
    ASSERT_TEST(m4(0,0) == 19);
    ASSERT_TEST(m4(0,1) == 22);
    ASSERT_TEST(m4(1,0) == 43);
    ASSERT_TEST(m4(1,1) == 50);

    m1 += m2;
    m1 *= m2;
    m1 -= m2;
    m1 = m3 - m2;

    ASSERT_TEST(-m1 == -1*m1);
    ASSERT_TEST(-m1 == m1*-1);

    ASSERT_TEST(m2.rotateClockwise().rotateCounterClockwise() == m2);

    ASSERT_TEST(m1.transpose().transpose() == m1);

    out << m2 << std::endl;

    return true;
}

bool testMataMvidia(std::ostream& out) {
    Matrix frame1(2,2);
    Matrix frame2(2,2);

    frame1(0,0) = 1;
    frame1(0,1) = 2;
    frame1(1,0) = 3;
    frame1(1,1) = 4;

    frame2(0,0) = 5;
    frame2(0,1) = 6;
    frame2(1,0) = 7;
    frame2(1,1) = 8;

    Matrix array[2];
    array[0] = frame1;
    array[1] = frame2;

    MataMvidia movie("MataMatrix","Adi Harif",array, 2);

    ASSERT_TEST(movie[0] == frame1);

    movie += frame1;

    ASSERT_TEST(movie[2] == frame1);

    Matrix frame01(2,2);
    Matrix frame11(2,2);

    frame01(0,0) = 10;
    frame01(0,1) = 20;
    frame01(1,0) = 30;
    frame01(1,1) = 40;

    frame11(0,0) = 50;
    frame11(0,1) = 60;
    frame11(1,0) = 70;
    frame11(1,1) = 80;

    Matrix array1[2];
    array1[0] = frame01;
    array1[1] = frame11;

    MataMvidia movie1("MataMatrix and the chamber of secrets","Adi Harif",array1, 2);

    ASSERT_TEST(movie1[0] == frame01);

    //test output
    out << movie;

    MataMvidia movie2 = movie + movie1;
    movie += movie1;

    return true;
}

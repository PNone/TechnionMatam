

#include <string>

#include "Matrix.h"
#include "MataMvidia.h"

using namespace std;
typedef bool (*testFunc)(void);

bool testMatrix(std::ostream &out);
bool testMataMvidia(std::ostream &out);
void constructorAssignment(std::ostream &out);
void constructorErrorSize(std::ostream &out);
void constructorErrorSize1(std::ostream &out);
void constructorErrorSize2(std::ostream &out);
void constructorErrorSize3(std::ostream &out);
void constructorErrorSize4(std::ostream &out);
void mataMvidiaE2e(std::ostream &out);


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

    if (testName == "constructorAssignment") {
        constructorAssignment(std::cout);
    }
    if (testName == "constructorErrorSize") {
        constructorErrorSize(std::cout);
    }
    if (testName == "constructorErrorSize1") {
        constructorErrorSize1(std::cout);
    }
    if (testName == "constructorErrorSize2") {
        constructorErrorSize2(std::cout);
    }
    if (testName == "constructorErrorSize3") {
        constructorErrorSize3(std::cout);
    }
    if (testName == "constructorErrorSize4") {
        constructorErrorSize4(std::cout);
    }
    if (testName == "mataMvidiaE2e") {
        mataMvidiaE2e(std::cout);
    }

    return 0;
}

bool testMatrix(std::ostream &out) {
    Matrix m1(2, 2);
    Matrix m2(2, 2);

    m1(0, 0) = 1;
    m1(0, 1) = 2;
    m1(1, 0) = 3;
    m1(1, 1) = 4;

    m2(0, 0) = 5;
    m2(0, 1) = 6;
    m2(1, 0) = 7;
    m2(1, 1) = 8;

    Matrix m3 = m1 + m2;
    ASSERT_TEST(m3(0, 0) == 6);
    ASSERT_TEST(m3(0, 1) == 8);
    ASSERT_TEST(m3(1, 0) == 10);
    ASSERT_TEST(m3(1, 1) == 12);

    Matrix m4 = m1 * m2;
    m4 *= 1;
    ASSERT_TEST(m4(0, 0) == 19);
    ASSERT_TEST(m4(0, 1) == 22);
    ASSERT_TEST(m4(1, 0) == 43);
    ASSERT_TEST(m4(1, 1) == 50);

    m1 += m2;
    m1 *= m2;
    m1 -= m2;
    m1 = m3 - m2;

    ASSERT_TEST(-m1 == -1 * m1);
    ASSERT_TEST(-m1 == m1 * -1);

    ASSERT_TEST(m2.rotateClockwise().rotateCounterClockwise() == m2);

    ASSERT_TEST(m1.transpose().transpose() == m1);

    out << m2 << std::endl;

    return true;
}

bool testMataMvidia(std::ostream &out) {
    Matrix frame1(2, 2);
    Matrix frame2(2, 2);

    frame1(0, 0) = 1;
    frame1(0, 1) = 2;
    frame1(1, 0) = 3;
    frame1(1, 1) = 4;

    frame2(0, 0) = 5;
    frame2(0, 1) = 6;
    frame2(1, 0) = 7;
    frame2(1, 1) = 8;

    Matrix array[2];
    array[0] = frame1;
    array[1] = frame2;

    MataMvidia movie("MataMatrix", "Adi Harif", array, 2);

    ASSERT_TEST(movie[0] == frame1);

    movie += frame1;

    ASSERT_TEST(movie[2] == frame1);

    Matrix frame01(2, 2);
    Matrix frame11(2, 2);

    frame01(0, 0) = 10;
    frame01(0, 1) = 20;
    frame01(1, 0) = 30;
    frame01(1, 1) = 40;

    frame11(0, 0) = 50;
    frame11(0, 1) = 60;
    frame11(1, 0) = 70;
    frame11(1, 1) = 80;

    Matrix array1[2];
    array1[0] = frame01;
    array1[1] = frame11;

    MataMvidia movie1("MataMatrix and the chamber of secrets", "Adi Harif", array1, 2);

    ASSERT_TEST(movie1[0] == frame01);

    //test output
    out << movie;

    MataMvidia movie2 = movie + movie1;
    movie += movie1;

    return true;
}

void constructorAssignment(std::ostream &out) {
    Matrix a;
    out << a << endl;

    Matrix c(3, 2);
    c(0, 1) = 2;
    Matrix d = c;
    d(1, 1) = c(0, 1);

    a = d;
    out << a << endl;
    out << c << endl;
    out << d << endl;
}

void constructorErrorSize(std::ostream &out) {
    Matrix a;
    out << a << endl;
    //error size
    Matrix b(-1, 0);
}

void constructorErrorSize1(std::ostream &out) {
    Matrix c(3, 2);
    c(0, 1) = 2;

    //error index;
    c(-1, 3) = 2;
}

void constructorErrorSize2(std::ostream &out) {
    Matrix c(3, 2);
    c(0, 1) = 2;
    Matrix d = c;
    d(1, 1) = c(0, 1);

    //error index;
    d(2, 4) = 0;
}

void constructorErrorSize3(std::ostream &out) {
    Matrix c(3, 2);
    c(0, 1) = 2;
    Matrix d = c;
    d(1, 1) = c(0, 1);

    //error index;
    d(2, 4);
}

void constructorErrorSize4(std::ostream &out) {
    Matrix c(3, 2);
    c(0, 1) = 2;

    //error index;
    c(-1, 3);
}

void mataMvidiaE2e(std::ostream &out) {
    Matrix a(3, 3);
    a(0, 0) = 1;
    a(0, 1) = 2;
    a(0, 2) = 3;
    a(1, 0) = 0;
    a(1, 1) = 2;
    a(1, 2) = 0;
    a(2, 0) = 1;
    a(2, 1) = 4;
    a(2, 2) = 1;

    out << "a:" << endl << a << endl;

    Matrix b = a.transpose() - a;
    out << "a.transpose()-a:" << endl << b << endl;

    Matrix c = a.rotateCounterClockwise() + b.rotateClockwise();
    out << "a.rotateCounterClockwise()+b.rotateClockwise()" << endl << c
        << endl;

    Matrix d(2, 3);
    d(0, 0) = 1;
    d(0, 1) = 0;
    d(0, 2) = 2;
    d(1, 0) = 3;
    d(1, 1) = 1;
    d(1, 2) = 0;

    out << "d:" << endl << d << endl;

    Matrix e = d.rotateCounterClockwise();
    out << "e:" << endl << e << endl;


    out << "a*e:" << endl << a * e << endl;
    out << "d*a:" << endl << d * a << endl;
    out << "a+b:" << endl << a + b << endl;

    out << "-a" << endl << -a << endl;
    out << "2*d:" << endl << 2 * d << endl;
    out << "2*d==d*2:" << endl << (2 * d == d * 2) << endl;
    out << "a.transpose!=a.counterClockSize:" << endl
        << (a.transpose() != a.rotateCounterClockwise()) << endl;

    Matrix array[5];
    array[0] = a;
    array[1] = b;
    array[2] = c;
    array[3] = d;
    array[4] = e;
    MataMvidia m("Matrix", "linoy", array, 5);
    out << "m" << endl << m << endl;
    MataMvidia z = m;
    out << "z" << endl << z << endl;
    MataMvidia x("malibu", "adi", array + 2, 3);
    z = x;
    out << "z" << endl << z << endl;
    //-d
    m[1] = (-m[1]);
    out << "m[1]= -b" << endl << m[1] << endl;
    out << "m+x" << endl << m + x << endl;
    out << "m+=a*e" << endl << (m += (a * e)) << endl;
    out << "m+=x" << endl << (m += x) << endl;
    out << "m" << endl << m << endl;
}

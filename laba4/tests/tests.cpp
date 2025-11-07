#include <gtest/gtest.h>
#include <sstream>
#include <cmath>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include "../include/triangle.hpp"
#include "../include/square.hpp"
#include "../include/octagon.hpp"
#include "../include/array.hpp"

using T = double;
using FigurePtr = std::shared_ptr<Figure<T>>;

static FigurePtr makeTriangle(const std::string& s) {
    auto p = std::make_shared<Triangle<T>>();
    std::istringstream is(s);
    is >> *p;
    return p;
}

static FigurePtr makeSquare(const std::string& s) {
    auto p = std::make_shared<Square<T>>();
    std::istringstream is(s);
    is >> *p;
    return p;
}

static FigurePtr makeOctagon(const std::string& s) {
    auto p = std::make_shared<Octagon<T>>();
    std::istringstream is(s);
    is >> *p;
    return p;
}

static double tri_area_expected_equilateral() {
    return std::sqrt(3.0) / 4.0;
}

static const double EPS = 1e-4;

TEST(TriangleBasic, EquilateralCorrectAreaCenter) {
    auto t = makeTriangle("0 0 1 0 0.5 0.866025");
    EXPECT_TRUE(t->isCorrect());
    double area = static_cast<double>(*t);
    EXPECT_NEAR(area, tri_area_expected_equilateral(), EPS);
    auto c = t->getCenter();
    EXPECT_NEAR(c.getX(), 0.5, EPS);
    EXPECT_NEAR(c.getY(), std::sqrt(3.0)/6.0, EPS);
}

TEST(TriangleInvalid, CollinearPointsInvalid) {
    auto t = makeTriangle("0 0 1 1 2 2");
    EXPECT_FALSE(t->isCorrect());
    double area = static_cast<double>(*t);
    EXPECT_NEAR(area, 0.0, EPS);
}

TEST(SquareBasic, UnitSquareCorrect) {
    auto s = makeSquare("0 0 1 0 1 1 0 1");
    EXPECT_TRUE(s->isCorrect());
    EXPECT_NEAR(static_cast<double>(*s), 1.0, EPS);
    auto c = s->getCenter();
    EXPECT_NEAR(c.getX(), 0.5, EPS);
    EXPECT_NEAR(c.getY(), 0.5, EPS);
}

TEST(SquareDegenerate, DuplicatePointsInvalid) {
    auto s = makeSquare("0 0 0 0 1 0 1 0");
    EXPECT_FALSE(s->isCorrect());
}

TEST(OctagonRegular, RadiusOneCenterZero) {
    auto o = makeOctagon(
        "1.000000 0.000000 0.707107 0.707107 0.000000 1.000000 -0.707107 0.707107 "
        "-1.000000 0.000000 -0.707107 -0.707107 0.000000 -1.000000 0.707107 -0.707107"
    );
    EXPECT_TRUE(o->isCorrect());
    auto c = o->getCenter();
    EXPECT_NEAR(c.getX(), 0.0, 1e-3);
    EXPECT_NEAR(c.getY(), 0.0, 1e-3);
    double area = static_cast<double>(*o);
    EXPECT_GT(area, 0.0);
}

TEST(ArrayBasic, PushAndSizeAndTotalArea) {
    Array<FigurePtr> arr;
    auto t = makeTriangle("0 0 1 0 0.5 0.866025");
    auto s = makeSquare("0 0 1 0 1 1 0 1");
    auto o = makeOctagon(
        "1.000000 0.000000 0.707107 0.707107 0.000000 1.000000 -0.707107 0.707107 "
        "-1.000000 0.000000 -0.707107 -0.707107 0.000000 -1.000000 0.707107 -0.707107"
    );
    arr.push_back(t);
    arr.push_back(s);
    arr.push_back(o);
    EXPECT_EQ(arr.size(), 3u);
    double sum = 0.0;
    if (t->isCorrect()) sum += static_cast<double>(*t);
    if (s->isCorrect()) sum += static_cast<double>(*s);
    if (o->isCorrect()) sum += static_cast<double>(*o);
    EXPECT_NEAR(arr.totalArea(), sum, 1e-3);
}

TEST(ArrayRemove, RemoveValidIndex) {
    Array<FigurePtr> arr;
    arr.push_back(makeTriangle("0 0 1 0 0.5 0.866025"));
    arr.push_back(makeSquare("0 0 1 0 1 1 0 1"));
    EXPECT_EQ(arr.size(), 2u);
    bool ok = arr.removeAt(0);
    EXPECT_TRUE(ok);
    EXPECT_EQ(arr.size(), 1u);
    auto f = arr[0];
    EXPECT_TRUE(f->isCorrect());
}

TEST(ArrayRemoveInvalid, BadIndex) {
    Array<FigurePtr> arr;
    arr.push_back(makeTriangle("0 0 1 0 0.5 0.866025"));
    bool ok = arr.removeAt(5);
    EXPECT_FALSE(ok);
    EXPECT_EQ(arr.size(), 1u);
}

TEST(ArrayClear, Empties) {
    Array<FigurePtr> arr;
    arr.push_back(makeTriangle("0 0 1 0 0.5 0.866025"));
    arr.push_back(makeSquare("0 0 1 0 1 1 0 1"));
    arr.clear();
    EXPECT_EQ(arr.size(), 0u);
}

TEST(Equality, TrianglePermutation) {
    Triangle<T> a;
    std::istringstream ia("0 0 1 0 0.5 0.866025");
    ia >> a;
    Triangle<T> b;
    std::istringstream ib("1 0 0.5 0.866025 0 0");
    ib >> b;
    auto extract_sorted = [](const Triangle<T>& tr) {
        std::vector<std::pair<double,double>> res;
        for (std::size_t i = 0; i < tr.size(); ++i) {
            res.emplace_back(tr.pointAt(i).getX(), tr.pointAt(i).getY());
        }
        std::sort(res.begin(), res.end(), [](auto const& p1, auto const& p2){
            if (std::fabs(p1.first - p2.first) > 1e-6) return p1.first < p2.first;
            return p1.second < p2.second;
        });
        return res;
    };
    auto A = extract_sorted(a);
    auto B = extract_sorted(b);
    ASSERT_EQ(A.size(), B.size());
    for (std::size_t i = 0; i < A.size(); ++i) {
        EXPECT_NEAR(A[i].first, B[i].first, 1e-6);
        EXPECT_NEAR(A[i].second, B[i].second, 1e-6);
    }
}

TEST(ArrayAtThrows, OutOfRangeAt) {
    Array<FigurePtr> arr;
    arr.push_back(makeTriangle("0 0 1 0 0.5 0.866025"));
    EXPECT_THROW(arr.at(2), std::out_of_range);
    arr.clear();
}

TEST(ArrayIndexOperator, ReturnsPointer) {
    Array<FigurePtr> arr;
    arr.push_back(makeSquare("0 0 1 0 1 1 0 1"));
    auto f = arr[0];
    EXPECT_NE(f, nullptr);
    EXPECT_TRUE(f->isCorrect());
}

TEST(ReverseOrderTriangleAreaSame, ClockwiseVsCounter) {
    Triangle<T> a;
    Triangle<T> b;
    std::istringstream ia("0 0 1 0 0.5 0.866025");
    std::istringstream ib("0 0 0.5 0.866025 1 0");
    ia >> a;
    ib >> b;
    EXPECT_NEAR(static_cast<double>(a), static_cast<double>(b), 1e-9);
}


TEST(Additional, TriangleNegativeCoordinates) {
    auto t = makeTriangle("-1 -1 0 -1 -0.5 -0.133975");
    EXPECT_TRUE(t->isCorrect());
    EXPECT_GT(static_cast<double>(*t), 0.0);
}


TEST(Additional, TriangleDuplicatePointsInvalid) {
    auto t = makeTriangle("0 0 0 0 1 0");
    EXPECT_FALSE(t->isCorrect());
}

TEST(Additional, TriangleNearlyCollinearInvalid) {
    auto t = makeTriangle("0 0 1 1 2 2.0001");
    EXPECT_FALSE(t->isCorrect());
    EXPECT_NEAR(static_cast<double>(*t), 0.0, 1e-3);
}

TEST(Additional, SquareRotated90Correct) {
    auto s = makeSquare("1 0 0 1 -1 0 0 -1");
    EXPECT_TRUE(s->isCorrect());
    auto c = s->getCenter();
    EXPECT_NEAR(c.getX(), 0.0, 1e-6);
    EXPECT_NEAR(c.getY(), 0.0, 1e-6);
}

TEST(Additional, SquareNoiseAcceptsWithinEps) {
    auto s = makeSquare("0 0 1.00005 0 1.00005 1.00005 0 1.00005");
    EXPECT_TRUE(s->isCorrect());
}

TEST(Additional, SquareUnequalSidesInvalid) {
    auto s = makeSquare("0 0 2 0 2 1 0 1");
    EXPECT_FALSE(s->isCorrect());
}

TEST(Additional, OctagonShiftedCenter) {
    auto o = makeOctagon(
        "3.000000 1.000000 2.414214 2.414214 1.000000 3.000000 -0.414214 2.414214 "
        "-1.000000 1.000000 -0.414214 -0.414214 1.000000 -1.000000 2.414214 -0.414214"
    );
    EXPECT_TRUE(o->isCorrect());
    auto c = o->getCenter();
    EXPECT_NEAR(c.getX(), 1.0, 1e-3);
    EXPECT_NEAR(c.getY(), 1.0, 1e-3);
}

TEST(Additional, OctagonCollinearInvalid) {
    auto o = makeOctagon("0 0 1 0 2 0 3 0 4 0 5 0 6 0 7 0");
    EXPECT_FALSE(o->isCorrect());
}

TEST(Additional, OctagonSkewEqualSides) {
    auto o = makeOctagon(
        "2 0 1.41421356 1.41421356 0 2 -1.41421356 1.41421356 "
        "-2 0 -1.41421356 -1.41421356 0 -2 1.41421356 -1.41421356"
    );
    EXPECT_TRUE(o->isCorrect());
}

TEST(Additional, ArrayManyPushesRemovesStress) {
    Array<FigurePtr> arr;
    for (int i = 0; i < 30; ++i) arr.push_back(makeTriangle("0 0 1 0 0.5 0.866025"));
    EXPECT_EQ(arr.size(), 30u);
    for (int i = 0; i < 30; ++i) EXPECT_TRUE(arr.removeAt(0));
    EXPECT_EQ(arr.size(), 0u);
}

TEST(Additional, ArrayTotalAreaMixedValidity) {
    Array<FigurePtr> arr;
    arr.push_back(makeTriangle("0 0 1 1 2 2")); // invalid
    arr.push_back(makeSquare("0 0 1 0 1 1 0 1")); // valid
    arr.push_back(makeOctagon("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0")); // invalid
    EXPECT_NEAR(arr.totalArea(), 1.0, 1e-3);
}

TEST(Additional, ArrayRemoveLast) {
    Array<FigurePtr> arr;
    arr.push_back(makeTriangle("0 0 1 0 0.5 0.866025"));
    arr.push_back(makeSquare("0 0 1 0 1 1 0 1"));
    EXPECT_TRUE(arr.removeAt(arr.size() - 1));
    EXPECT_EQ(arr.size(), 1u);
}

TEST(Additional, RemoveWithHugeIndexFail) {
    Array<FigurePtr> arr;
    arr.push_back(makeSquare("0 0 1 0 1 1 0 1"));
    bool ok = arr.removeAt(static_cast<std::size_t>(-1));
    EXPECT_FALSE(ok);
    arr.clear();
}


TEST(Additional, TriangleCopyAssignmentPreserves) {
    Triangle<T> a;
    std::istringstream ia("0 0 1 0 0.5 0.866025");
    ia >> a;
    Triangle<T> b;
    b = a;
    EXPECT_TRUE(b.isCorrect());
    EXPECT_NEAR(static_cast<double>(a), static_cast<double>(b), 1e-9);
}

TEST(Additional, TriangleInequalityDifferentNotEqual) {
    Triangle<T> a;
    Triangle<T> b;
    std::istringstream ia("0 0 1 0 0.5 0.866025");
    std::istringstream ib("0 0 2 0 1 1.73205");
    ia >> a;
    ib >> b;
    EXPECT_FALSE(a == b);
}

TEST(Additional, PrintAllOutputContainsPoints) {
    Array<FigurePtr> arr;
    arr.push_back(makeTriangle("0 0 1 0 0.5 0.866025"));
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    arr.printAll();
    std::cout.rdbuf(old);
    std::string out = buffer.str();
    EXPECT_NE(out.find("(0"), std::string::npos);
    EXPECT_NE(out.find("(1"), std::string::npos);
}

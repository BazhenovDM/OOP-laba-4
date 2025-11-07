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

static FigurePtr create_triangle(const std::string& coords) {
    auto fig = std::make_shared<Triangle<T>>();
    std::istringstream in(coords);
    in >> *fig;
    return fig;
}

static FigurePtr create_square(const std::string& coords) {
    auto fig = std::make_shared<Square<T>>();
    std::istringstream in(coords);
    in >> *fig;
    return fig;
}

static FigurePtr create_octagon(const std::string& coords) {
    auto fig = std::make_shared<Octagon<T>>();
    std::istringstream in(coords);
    in >> *fig;
    return fig;
}

static double expected_equilateral_area() {
    return std::sqrt(3.0) / 4.0;
}

static const double EPS = 1e-4;

static std::vector<std::pair<double,double>> sorted_points_from_triangle(const Triangle<T>& tr) {
    std::vector<std::pair<double,double>> pts;
    for (std::size_t i = 0; i < tr.size(); ++i) {
        pts.emplace_back(tr.pointAt(i).getX(), tr.pointAt(i).getY());
    }
    std::sort(pts.begin(), pts.end(), [](auto const& a, auto const& b){
        if (std::fabs(a.first - b.first) > 1e-6) return a.first < b.first;
        return a.second < b.second;
    });
    return pts;
}

TEST(TriangleBasic, EquilateralCorrectAreaCenter) {
    auto tri = create_triangle("0 0 1 0 0.5 0.866025");
    EXPECT_TRUE(tri->isCorrect());
    double area = static_cast<double>(*tri);
    EXPECT_NEAR(area, expected_equilateral_area(), EPS);
    auto center = tri->getCenter();
    EXPECT_NEAR(center.getX(), 0.5, EPS);
    EXPECT_NEAR(center.getY(), std::sqrt(3.0)/6.0, EPS);
}

TEST(TriangleInvalid, CollinearPointsInvalid) {
    auto tri = create_triangle("0 0 1 1 2 2");
    EXPECT_FALSE(tri->isCorrect());
    double area = static_cast<double>(*tri);
    EXPECT_NEAR(area, 0.0, EPS);
}

TEST(SquareBasic, UnitSquareCorrect) {
    auto sq = create_square("0 0 1 0 1 1 0 1");
    EXPECT_TRUE(sq->isCorrect());
    EXPECT_NEAR(static_cast<double>(*sq), 1.0, EPS);
    auto center = sq->getCenter();
    EXPECT_NEAR(center.getX(), 0.5, EPS);
    EXPECT_NEAR(center.getY(), 0.5, EPS);
}

TEST(SquareDegenerate, DuplicatePointsInvalid) {
    auto sq = create_square("0 0 0 0 1 0 1 0");
    EXPECT_FALSE(sq->isCorrect());
}

TEST(OctagonRegular, RadiusOneCenterZero) {
    auto oct = create_octagon(
        "1.000000 0.000000 0.707107 0.707107 0.000000 1.000000 -0.707107 0.707107 "
        "-1.000000 0.000000 -0.707107 -0.707107 0.000000 -1.000000 0.707107 -0.707107"
    );
    EXPECT_TRUE(oct->isCorrect());
    auto center = oct->getCenter();
    EXPECT_NEAR(center.getX(), 0.0, 1e-3);
    EXPECT_NEAR(center.getY(), 0.0, 1e-3);
    double area = static_cast<double>(*oct);
    EXPECT_GT(area, 0.0);
}

TEST(ArrayBasic, PushAndSizeAndTotalArea) {
    Array<FigurePtr> container;
    auto t = create_triangle("0 0 1 0 0.5 0.866025");
    auto s = create_square("0 0 1 0 1 1 0 1");
    auto o = create_octagon(
        "1.000000 0.000000 0.707107 0.707107 0.000000 1.000000 -0.707107 0.707107 "
        "-1.000000 0.000000 -0.707107 -0.707107 0.000000 -1.000000 0.707107 -0.707107"
    );
    container.push_back(t);
    container.push_back(s);
    container.push_back(o);
    EXPECT_EQ(container.size(), 3u);
    double sum = 0.0;
    if (t->isCorrect()) sum += static_cast<double>(*t);
    if (s->isCorrect()) sum += static_cast<double>(*s);
    if (o->isCorrect()) sum += static_cast<double>(*o);
    EXPECT_NEAR(container.totalArea(), sum, 1e-3);
}

TEST(ArrayRemove, RemoveValidIndex) {
    Array<FigurePtr> container;
    container.push_back(create_triangle("0 0 1 0 0.5 0.866025"));
    container.push_back(create_square("0 0 1 0 1 1 0 1"));
    EXPECT_EQ(container.size(), 2u);
    bool removed = container.removeAt(0);
    EXPECT_TRUE(removed);
    EXPECT_EQ(container.size(), 1u);
    auto remaining = container[0];
    EXPECT_TRUE(remaining->isCorrect());
}

TEST(ArrayRemoveInvalid, BadIndex) {
    Array<FigurePtr> container;
    container.push_back(create_triangle("0 0 1 0 0.5 0.866025"));
    bool removed = container.removeAt(5);
    EXPECT_FALSE(removed);
    EXPECT_EQ(container.size(), 1u);
}

TEST(ArrayClear, Empties) {
    Array<FigurePtr> container;
    container.push_back(create_triangle("0 0 1 0 0.5 0.866025"));
    container.push_back(create_square("0 0 1 0 1 1 0 1"));
    container.clear();
    EXPECT_EQ(container.size(), 0u);
}

TEST(Equality, TrianglePermutation) {
    Triangle<T> A;
    std::istringstream a_in("0 0 1 0 0.5 0.866025");
    a_in >> A;
    Triangle<T> B;
    std::istringstream b_in("1 0 0.5 0.866025 0 0");
    b_in >> B;
    auto ptsA = sorted_points_from_triangle(A);
    auto ptsB = sorted_points_from_triangle(B);
    ASSERT_EQ(ptsA.size(), ptsB.size());
    for (std::size_t i = 0; i < ptsA.size(); ++i) {
        EXPECT_NEAR(ptsA[i].first, ptsB[i].first, 1e-6);
        EXPECT_NEAR(ptsA[i].second, ptsB[i].second, 1e-6);
    }
}

TEST(ArrayAtThrows, OutOfRangeAt) {
    Array<FigurePtr> container;
    container.push_back(create_triangle("0 0 1 0 0.5 0.866025"));
    EXPECT_THROW(container.at(2), std::out_of_range);
    container.clear();
}

TEST(ArrayIndexOperator, ReturnsPointer) {
    Array<FigurePtr> container;
    container.push_back(create_square("0 0 1 0 1 1 0 1"));
    auto f = container[0];
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
    auto tri = create_triangle("-1 -1 0 -1 -0.5 -0.133975");
    EXPECT_TRUE(tri->isCorrect());
    EXPECT_GT(static_cast<double>(*tri), 0.0);
}

TEST(Additional, TriangleDuplicatePointsInvalid) {
    auto tri = create_triangle("0 0 0 0 1 0");
    EXPECT_FALSE(tri->isCorrect());
}

TEST(Additional, TriangleNearlyCollinearInvalid) {
    auto tri = create_triangle("0 0 1 1 2 2.0001");
    EXPECT_FALSE(tri->isCorrect());
    EXPECT_NEAR(static_cast<double>(*tri), 0.0, 1e-3);
}

TEST(Additional, SquareRotated90Correct) {
    auto sq = create_square("1 0 0 1 -1 0 0 -1");
    EXPECT_TRUE(sq->isCorrect());
    auto center = sq->getCenter();
    EXPECT_NEAR(center.getX(), 0.0, 1e-6);
    EXPECT_NEAR(center.getY(), 0.0, 1e-6);
}

TEST(Additional, SquareNoiseAcceptsWithinEps) {
    auto sq = create_square("0 0 1.00005 0 1.00005 1.00005 0 1.00005");
    EXPECT_TRUE(sq->isCorrect());
}

TEST(Additional, SquareUnequalSidesInvalid) {
    auto sq = create_square("0 0 2 0 2 1 0 1");
    EXPECT_FALSE(sq->isCorrect());
}

TEST(Additional, OctagonShiftedCenter) {
    auto oc = create_octagon(
        "3.000000 1.000000 2.414214 2.414214 1.000000 3.000000 -0.414214 2.414214 "
        "-1.000000 1.000000 -0.414214 -0.414214 1.000000 -1.000000 2.414214 -0.414214"
    );
    EXPECT_TRUE(oc->isCorrect());
    auto center = oc->getCenter();
    EXPECT_NEAR(center.getX(), 1.0, 1e-3);
    EXPECT_NEAR(center.getY(), 1.0, 1e-3);
}

TEST(Additional, OctagonCollinearInvalid) {
    auto oc = create_octagon("0 0 1 0 2 0 3 0 4 0 5 0 6 0 7 0");
    EXPECT_FALSE(oc->isCorrect());
}

TEST(Additional, OctagonSkewEqualSides) {
    auto oc = create_octagon(
        "2 0 1.41421356 1.41421356 0 2 -1.41421356 1.41421356 "
        "-2 0 -1.41421356 -1.41421356 0 -2 1.41421356 -1.41421356"
    );
    EXPECT_TRUE(oc->isCorrect());
}

TEST(Additional, ArrayManyPushesRemovesStress) {
    Array<FigurePtr> container;
    for (int i = 0; i < 30; ++i) container.push_back(create_triangle("0 0 1 0 0.5 0.866025"));
    EXPECT_EQ(container.size(), 30u);
    for (int i = 0; i < 30; ++i) EXPECT_TRUE(container.removeAt(0));
    EXPECT_EQ(container.size(), 0u);
}

TEST(Additional, ArrayTotalAreaMixedValidity) {
    Array<FigurePtr> container;
    container.push_back(create_triangle("0 0 1 1 2 2"));
    container.push_back(create_square("0 0 1 0 1 1 0 1"));
    container.push_back(create_octagon("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0"));
    EXPECT_NEAR(container.totalArea(), 1.0, 1e-3);
}

TEST(Additional, ArrayRemoveLast) {
    Array<FigurePtr> container;
    container.push_back(create_triangle("0 0 1 0 0.5 0.866025"));
    container.push_back(create_square("0 0 1 0 1 1 0 1"));
    EXPECT_TRUE(container.removeAt(container.size() - 1));
    EXPECT_EQ(container.size(), 1u);
}

TEST(Additional, RemoveWithHugeIndexFail) {
    Array<FigurePtr> container;
    container.push_back(create_square("0 0 1 0 1 1 0 1"));
    bool ok = container.removeAt(static_cast<std::size_t>(-1));
    EXPECT_FALSE(ok);
    container.clear();
}

TEST(Additional, TriangleCopyAssignmentPreserves) {
    Triangle<T> src;
    std::istringstream in("0 0 1 0 0.5 0.866025");
    in >> src;
    Triangle<T> dst;
    dst = src;
    EXPECT_TRUE(dst.isCorrect());
    EXPECT_NEAR(static_cast<double>(src), static_cast<double>(dst), 1e-9);
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
    Array<FigurePtr> container;
    container.push_back(create_triangle("0 0 1 0 0.5 0.866025"));
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    container.printAll();
    std::cout.rdbuf(old);
    std::string out = buffer.str();
    EXPECT_NE(out.find("(0"), std::string::npos);
    EXPECT_NE(out.find("(1"), std::string::npos);
}

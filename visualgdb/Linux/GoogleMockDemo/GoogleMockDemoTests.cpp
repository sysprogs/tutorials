#include <gtest/gtest.h>
#include <stdio.h>
#include <gmock/gmock.h>

struct Vector
{
    double X, Y;
    
    Vector(double x, double y)
    		: X(x)
    		, Y(y)
    {
    }
};

class IMath
{
public:
  virtual double Square(double arg) = 0;
};

class MockMath : public IMath
{
public:
    MOCK_METHOD(double, Square, (double), (override));
};

class Math : public IMath
{
public:
  double Square(double arg) override
  {
      return arg * arg;
  }
};

double GetDistance(IMath *pMath, const Vector &a, const Vector &b)
{
    return sqrt(pMath->Square(a.X - b.X) + pMath->Square(a.Y - b.Y));
}

using ::testing::Return;

TEST(DemoTestGroup, SquareTest)
{
    Vector left(0, 0);
    Vector right(3, 4);
    MockMath math;

    EXPECT_CALL(math, Square(-3)).WillOnce(Return(9));
    EXPECT_CALL(math, Square(-4)).WillOnce(Return(16));

    double distance = GetDistance(&math, left, right);
    
    EXPECT_EQ(distance, 5);
}

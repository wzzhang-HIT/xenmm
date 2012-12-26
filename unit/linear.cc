#define MAX_DOMAINS 3
#include <gtest/gtest.h>
#include "core.c"

double a[3][3] = {
    {1,1,1},
    {1,-1,0},
    {1,0,-1}
};
TEST(SolveLinearTest,t1){
    double b[3] = {1,2,3};
    double x[3] = {0};
    solve_line_equations(a, b, 3, x);
    EXPECT_EQ(x[0],2);
    EXPECT_EQ(x[1],0);
    EXPECT_EQ(x[2],-1);
}


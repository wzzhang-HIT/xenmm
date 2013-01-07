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
//27:[ tg:764397 use:565492 free:827280 ]26:[ tg:1130276 use:338864 free:574776 ]25:[ tg:1206608 use:341700 free:549332 ]24:[ tg:1130733 use:337196 free:574624 ]23:[ tg:1159784 use:350640 free:564940 ]
//[ 11068046444226416640  15679732462654267904  15679732462654345216  15679732462654269952  15679732462654299136 ]
TEST(SolveLinearTest ,t2){
    double a[5][5] = {
        {1,1,1,1,1},
        {1,-1,0,0,0},
        {1,0,-1,0,0},
        {1,0,0,-1,0},
        {1,0,0,0,-1}
    }
    double b[5] = {0}
}


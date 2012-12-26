#ifndef SOLVE_H_H
#define SOLVE_H_H
#define MAX_DOMAINS 64
extern double _a_[MAX_DOMAINS][MAX_DOMAINS];
extern double _b_[MAX_DOMAINS];
extern double _x_[MAX_DOMAINS];
void solve_line_equations(double a[MAX_DOMAINS][MAX_DOMAINS], double b[MAX_DOMAINS],
		int len, double x[MAX_DOMAINS]);
#endif

void solve_line_equations(double a[MAX_DOMAINS][MAX_DOMAINS], double b[MAX_DOMAINS],
		int len, double x[MAX_DOMAINS])
{
	int i, j, k;
	double y[MAX_DOMAINS];
	double l[MAX_DOMAINS][MAX_DOMAINS] = {{0}}, u[MAX_DOMAINS][MAX_DOMAINS] = {{0}};

	// init array u
	for (i=0; i<len; ++i)
		u[i][i] = 1;

	// calculate array l and u
	for (k=0; k<len; ++k)
	{
		for (i=k; i<len; ++i)
		{
			l[i][k] = a[i][k];
			for (j=0; j<k; ++j)
				l[i][k] -= (l[i][j] * u[j][k]);
		}

		for (j=k+1; j<len; ++j)
		{
			u[k][j] = a[k][j];
			for (i=0; i<k; ++i)
				u[k][j] -= (l[k][i] * u[i][j]);
			u[k][j] /= l[k][k];
		}
	}

	// solve Ly=b
	for (i=0; i<len; ++i)
	{
		y[i] = b[i];
		for (j=0; j<i; ++j)
			y[i] -= (l[i][j] * y[j]);
		y[i] /= l[i][i];
	}

	// solve Ux=y
	for (i=len-1; i>=0; --i)
	{
		x[i] = y[i];
		for (j=i+1; j<len; ++j)
			x[i] -= (u[i][j] * x[j]);
	}
	// now we get the result of ax=b;
	return;
}

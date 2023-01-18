#include "mex.h"
#include <matrix.h>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;
// Min cost bipartite matching via shortest augmenting paths
//
// Code from https://github.com/jaehyunp/
//
// This is an O(n^3) implementation of a shortest augmenting path
// algorithm for finding min cost perfect matchings in dense
// graphs.  In practice, it solves 1000x1000 problems in around 1
// second.
//
//   cost[i][j] = cost for pairing left node i with right node j
//   Lmate[i] = index of right node that left node i pairs with
//   Rmate[j] = index of left node that right node j pairs with
//
// The values in cost[i][j] may be positive or negative.  To perform
// maximization, simply negate the cost[][] matrix.



typedef vector<double> VD;
typedef vector<VD> VVD;
typedef vector<int> VI;

double MinCostMatching(const VVD &cost, VI &Lmate, VI &Rmate) {
	int n = int(cost.size());

	// construct dual feasible solution
	VD u(n);
	VD v(n);
	for (int i = 0; i < n; i++) {
		u[i] = cost[i][0];
		for (int j = 1; j < n; j++) u[i] = min(u[i], cost[i][j]);
	}
	for (int j = 0; j < n; j++) {
		v[j] = cost[0][j] - u[0];
		for (int i = 1; i < n; i++) v[j] = min(v[j], cost[i][j] - u[i]);
	}

	// construct primal solution satisfying complementary slackness
	Lmate = VI(n, -1);
	Rmate = VI(n, -1);
	int mated = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (Rmate[j] != -1) continue;
			if (fabs(cost[i][j] - u[i] - v[j]) < 1e-10) {
				Lmate[i] = j;
				Rmate[j] = i;
				mated++;
				break;
			}
		}
	}

	VD dist(n);
	VI dad(n);
	VI seen(n);

	// repeat until primal solution is feasible
	while (mated < n) {

		// find an unmatched left node
		int s = 0;
		while (Lmate[s] != -1) s++;

		// initialize Dijkstra
		fill(dad.begin(), dad.end(), -1);
		fill(seen.begin(), seen.end(), 0);
		for (int k = 0; k < n; k++)
			dist[k] = cost[s][k] - u[s] - v[k];

		int j = 0;
		while (true) {

			// find closest
			j = -1;
			for (int k = 0; k < n; k++) {
				if (seen[k]) continue;
				if (j == -1 || dist[k] < dist[j]) j = k;
			}
			seen[j] = 1;

			// termination condition
			if (Rmate[j] == -1) break;

			// relax neighbors
			const int i = Rmate[j];
			for (int k = 0; k < n; k++) {
				if (seen[k]) continue;
				const double new_dist = dist[j] + cost[i][k] - u[i] - v[k];
				if (dist[k] > new_dist) {
					dist[k] = new_dist;
					dad[k] = j;
				}
			}
		}

		// update dual variables
		for (int k = 0; k < n; k++) {
			if (k == j || !seen[k]) continue;
			const int i = Rmate[k];
			v[k] += dist[k] - dist[j];
			u[i] -= dist[k] - dist[j];
		}
		u[s] += dist[j];

		// augment along path
		while (dad[j] >= 0) {
			const int d = dad[j];
			Rmate[j] = Rmate[d];
			Lmate[Rmate[j]] = j;
			j = d;
		}
		Rmate[j] = s;
		Lmate[s] = j;

		mated++;
	}

	double value = 0;
	for (int i = 0; i < n; i++)
		value += cost[i][Lmate[i]];

	return value;
}

void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )
{
	/* Input arguments */
	int nOfRows    = mxGetM(prhs[0]);
	int nOfColumns = mxGetN(prhs[0]);
	double* distMatrix = mxGetPr(prhs[0]);
	
	/* Output arguments */
	plhs[0] = mxCreateDoubleMatrix(nOfRows, nOfColumns, mxREAL);
	plhs[1] = mxCreateDoubleMatrix(1, 1, mxREAL);
	double *assignment = mxGetPr(plhs[0]);
	double *cost       = mxGetPr(plhs[1]);
	
	if (nOfRows == 0 || nOfColumns == 0)
		return;

	/* Call C-function */
    double INF = 1e8;
    int squareSize = max(nOfRows, nOfColumns);
	vector<vector<double>> alldist(squareSize, vector<double>(squareSize, INF));
    for (int i = 0; i < nOfRows; i++)
    {
        for (int j =0; j < nOfColumns; j++)
        {
            alldist[i][j] = distMatrix[j*nOfRows + i];
        }
    }
    
	vector<int> Lmate, Rmate;
	MinCostMatching(alldist, Lmate, Rmate);
    
    cost[0] = 0;
    for (int i = 0; i < nOfRows; i++)
    {
		if (Lmate[i] < nOfColumns && alldist[i][Lmate[i]] != INF)
		{
			assignment[Lmate[i] * nOfRows + i] = 1;
			cost[0] += alldist[i][Lmate[i]];
		}
    }
    
}


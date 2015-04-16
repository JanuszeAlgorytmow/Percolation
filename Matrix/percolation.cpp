#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <iostream>
using namespace std;

void CreateNeighbours(uint8_t** neighbours, int N, double p) {
	
	double tmp;
	
	for(int ii = 0; ii < N; ii++) {
		for(int jj = 0; jj < N; jj++) {
			neighbours[ii][jj] = 0;
		}
	}
	
	for(int ii = 0; ii < N; ii++) {
		for(int jj = ii + 1; jj < N; jj++) {
			tmp = (double) rand() / RAND_MAX;
			if(tmp < p) {
				neighbours[ii][jj] = 1;
				neighbours[jj][ii] = 1;
			}
		}
	}
}

void MarkAllNeighbours(uint8_t** neighbours, int* clusters, int current, int cnumber, int N) {
	
	bool* tovisit = new bool[N];
	for(int ii = 0; ii < N; ii++) {
		tovisit[ii] = false;
	}
	tovisit[current] = true;
	int ctrl = 1;
	
	while(ctrl) {
		ctrl = 0;
		for(int ii = 0; ii < N; ii++) {
			if(tovisit[ii] && !(clusters[ii])) {
				clusters[ii] = cnumber;
				ctrl++;
				for(int jj = 0; jj < N; jj++) {
					if(neighbours[ii][jj]) {
						tovisit[jj] = true;
					}
				}
			}
		}
	}
	
	delete[] tovisit;
}

int FillClusters(uint8_t** neighbours, int* clusters, int N) {
	
	for(int ii = 0; ii < N; ii++) {
		clusters[ii] = 0;
	}
	
	int cnumber = 0;
	int current = 0;
	
	while(current < N) {
		cnumber++;
		MarkAllNeighbours(neighbours, clusters, current, cnumber, N);
		while(clusters[current]) {
			current++;
		}
	}
	
	return cnumber;
}

int Max(int *tab, int N) {
	
	int max = 0;
	for(int ii = 0; ii < N; ii++) {
		if(tab[ii] > max) {
			max = tab[ii];
		}
	}
	
	return max;
}

double PercCluster(int* clusters, int N, int cnumber) {
	
	int* freq = new int[cnumber];
	
	for(int ii = 0; ii < cnumber; ii++) {
		freq[ii] = 0;
	}
	
	for(int ii = 1; ii <= cnumber; ii++) {
		for(int jj = 0; jj < N; jj++) {
			if(clusters[jj] == ii) {
				freq[ii - 1]++;
			}
		}
	}
	
	int max = Max(freq, cnumber);
		
	delete[] freq;
	return (double) max / (double) N;
}

double Mean(double* tab, int N) {
	
	double mean = 0;
	
	for(int ii = 0; ii < N; ii++) {
		mean += tab[ii];
	}
	
	return mean / (double) N;
}

double Uncertainty(double* tab, int N, double mean) {
	
	double s = 0;
	
	for(int ii = 0; ii < N; ii++) {
		s += (tab[ii] - mean) * (tab[ii] - mean);
	}
	
	return sqrt(s / (double) (N * (N - 1)));
}

int main(int argc, char** argv) {
	
	srand(time(NULL));
	
	int N = atoi(argv[1]);
	double p0 = atof(argv[2]);
	double pinc = atof(argv[3]);
	int inc = atoi(argv[4]);
	int rep = atoi(argv[5]);
	char* file = argv[6];
	
	double p;
	uint8_t** neighbours = new uint8_t* [N];
	for(int ii = 0; ii < N; ii++) {
		neighbours[ii] = new uint8_t[N];
	}
	int* clusters = new int[N];
	fstream ofile;
	ofile.open(file, fstream::out);
	double* results = new double [rep];
	double mean;
	
	for(int jj = 0; jj < inc; jj++) {
		p = p0 + jj * pinc;
		ofile << endl << fixed << setprecision(8) << p << " ";
		for(int kk = 0; kk < rep; kk++) {
			cout << jj * rep + kk << "/" << rep * inc << " done" << endl;
			CreateNeighbours(neighbours, N, p);
			int cnumber = FillClusters(neighbours, clusters, N);
			results[kk] = PercCluster(clusters, N, cnumber);
		}
		mean = Mean(results, rep);
		ofile << fixed << setprecision(5) << mean  << " " << Uncertainty(results, rep, mean) << endl;
	}
	
	ofile.close();
	delete[] clusters;
	for(int ii = 0; ii < N; ii++) {
		delete[] neighbours[ii];
	}
	delete[] neighbours;
	return 0;
}

#include <iostream>
#include <vector>
#include <map>
#include <stdlib.h>
#include <math.h>
#include <regex>

#define rep(a, b) for(a=0;a<b;a++)
#define per(a, b) for(a=b;a>=0;a--)

using namespace std;


vector<double> ex; // vektor uchovavajici koeficienty
//12.3x^5 ... ex[5] = 12.3
//
vector<double> sturm[10000]; // sturm[k] uchovava k-tou sturmovu posloupnost
double maxA = 0, maxB = 0;
int maxExp = 0; //jaky dosavadni maximalni exponent (v readInput) a pak udrzuje maxExp
bool lastSequence = 0;
int numberOfIntervals = 100000;
int numberOfSequences = 0;
double precision = 0.00001;

map<double, double> intervalsWithRoots;

void throwError(){
	cout << "debilku" << endl;
}

void readInput(){
	string line;
	getline(cin, line);
	regex rgx(R"(([+-]|^)\s*((?:\d+\.\d+|\d*))([xX]?)(\d*))",
		regex_constants::ECMAScript);
	sregex_iterator next(line.begin(), line.end(), rgx);
	sregex_iterator end;
	while (next != end) {
		smatch match = *next;
		string match_str = match.str(0);
		int sign = 1;
		if (match.str(1) == "-") sign = -1;
		// is there an x?
		bool x = match.str(3) != "";
		// coefficient
		double k = 1.0;
		if (match.str(2) != "")
			k = atof(match.str(2).c_str());
		// power (exponent)
		int p = x;
		if (match.str(4) != "")
			p = atoi(match.str(4).c_str());

		if(p > maxExp) maxExp = p;

		if (p + 1 > ex.size()) { // doplnim nuly do vektoru
			for (int i = ex.size(); i < p + 1; i++) ex.push_back(0);
		}

		ex[p] = sign * k; // ulozim do patricnyho exponentu koeficient
		next++;
	}
	// prints out all coefficients (for testing):
	// int i = 0;
	// rep(i, ex.size()) cout << i << ": " << ex[i] << endl;
}

void getMaxInterval(){
	double m = 0;
	for(auto a : ex){
		if(abs(a) > m) m = abs(a);
	}
	maxA = -m-1;
	maxB = m+1;
}

void getFirstSequence(){
	for(auto a : ex){
		sturm[0].push_back(a);
	}
}

void getDerivative(){
	for(int i = 1; i <= maxExp; i++){
		sturm[1].push_back(i*ex[i]);
	}
}

void copySequence(int what, int where){
	for(auto a : sturm[what]){
		sturm[where].push_back(a);
	}
}

void getNextSequence(int numberOfSequence){ 
	copySequence(numberOfSequence-2, numberOfSequence);
	int maxA = sturm[numberOfSequence].size() - 1; // maximalni exponent -2 funkce
	int maxB = sturm[numberOfSequence-1].size() - 1; //max exp -1 funkce

	int e;
	double k;
	
	while(maxA >= maxB){
		e = maxA - maxB; //tohle je vlastne vydeleni exponentu
		k = sturm[numberOfSequence][maxA]/sturm[numberOfSequence-1][maxB]; // tohle je vydeleni koef

		for(int in = maxB; in >= 0; in--){
			sturm[numberOfSequence][in+e] -= k*sturm[numberOfSequence-1][in];
			// precision error?
			// ano - vyhazuje to 1.77e-322
			// kouknout na prednasky z jezka a pak to dodelat
			cout << sturm[numberOfSequence][maxA] << endl;
		}
		while(sturm[numberOfSequence][maxA] == 0){
			maxA--;
			if(maxA < 0){
				lastSequence = 1;
			}
		}
		// cout << "aa"; //tady dochazi nekdy k zacykleni
	}

	while(sturm[numberOfSequence].back() == 0){
		sturm[numberOfSequence].pop_back();
		if(sturm[numberOfSequence].size() == 0){
			break;
		}
	} 

	if(sturm[numberOfSequence].size() <= 1) lastSequence = 1;

	rep(e, sturm[numberOfSequence].size()) sturm[numberOfSequence][e] *= -1; //negace zbytku

}

void printSturmSequence(int numberOfSequence){
	int in;
	cout << "f" << numberOfSequence << "(x) = ";
	per(in, sturm[numberOfSequence].size()-1){
		if(sturm[numberOfSequence][in] != 0){
			if(sturm[numberOfSequence][in] > 0 && in != sturm[numberOfSequence].size()-1) cout << "+";
			if(in == 1){
				cout << sturm[numberOfSequence][in] << "x" << " ";	
			}
			else if(in == 0){
				cout << sturm[numberOfSequence][in];
			}
			else{
				cout << sturm[numberOfSequence][in] << "x" << in << " ";
			}
		}
	}
	if(sturm[numberOfSequence].size() == 0) cout << 0 << endl;
	cout << endl;
}

double evaluate(int sequence, double x){
	double res = 0;
	for(int i = sturm[sequence].size()-1; i>=0;i--){
		res = x*(sturm[sequence][i]+res);
	}
	if(x != 0) res /= x;
	if (res == -0) res = 0;
	return res;
}

int evaluateForOneValue(double x){
	double lastVal = evaluate(0,x);
	int changes = 0;
	for(int in = 0; in <= numberOfSequences; in++){
		double val = evaluate(in, x);
		if((lastVal >= 0 && val < 0) || (lastVal < 0 && val >= 0)) changes++;
		if(in == numberOfSequences && lastVal < 0 && abs(val) < 0.0000000000001) changes--; //abs(val) < 0.0000000000001 when val == 0, but double can be 0 with some cancer value like -5.77316e-15
		lastVal = val;
	}
	return changes;
}

void newton(){
	for(auto interv : intervalsWithRoots){
		double lastX = interv.first;
		double x = lastX - (evaluate(0, lastX)/evaluate(1, lastX));
		double shit = (evaluate(0, lastX)/evaluate(1, lastX));
		while(abs(lastX-x) > precision){
			lastX = x;
			if(evaluate(1, lastX) == 0) x = lastX - (evaluate(0, lastX)/0.0000001);
			else x = lastX - (evaluate(0, lastX)/evaluate(1, lastX));
		}
		cout << "Root: x = " << x << endl;
	}
}


int main(){
	int i, in;
	readInput();
	rep(i, ex.size()) cout << ex[i] << " "; // vypiseme exponenty
	getMaxInterval();
	getFirstSequence();
	getDerivative();

	cout << endl << "Maximal interval: <" <<maxA << "  " << maxB << ">" << endl;

	printSturmSequence(0);
	printSturmSequence(1);


	numberOfSequences = 2;	
	while(!lastSequence){
		getNextSequence(numberOfSequences);
		printSturmSequence(numberOfSequences);
		numberOfSequences++;
	}
	numberOfSequences--;


	int lastChanges = evaluateForOneValue(maxA);
	for(double x = maxA; x <= maxB; x+=(maxB-maxA)/numberOfIntervals){
		int changes = evaluateForOneValue(x);
		if(lastChanges != changes){
			if(abs(lastChanges-changes) > 1) cout << abs(lastChanges-changes) << " more roots in one interval " << endl;
			intervalsWithRoots.insert(make_pair(x, x+(maxB-maxA)/numberOfIntervals));
		}
		lastChanges = changes;
	}
	
	cout << "Intervals with roots:" << endl;
	for(auto interv : intervalsWithRoots){
		cout << "(" << interv.first << " " << interv.second << ")" << endl;
	}

	cout << endl << "Roots:" << endl;	

	newton();

#ifdef _WIN32
	system("pause");
#endif

	return 0;
}

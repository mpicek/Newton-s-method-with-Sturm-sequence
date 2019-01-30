#include <iostream>
#include <vector>
#include <map>
#include <stdlib.h>
#include <math.h>

#define rep(a, b) for(a=0;a<b;a++)
#define per(a, b) for(a=b;a>=0;a--)

using namespace std;


vector<double> ex;
vector<double> sturm[10000];
double maxA = 0, maxB = 0;
int maxExp = 0;
bool lastSequence = 0;
int numberOfIntervals = 100000;
int numberOfSequences = 0;
double precision = 0.00001;

map<double, double> intervalsWithRoots;
double abs(double a){
	if(a < 0) a*=(-1);
	return a;
}

void throwError(){
	cout << "debilku" << endl;
}

void readInput(){

	int phase = 0, i; //0 = read sign; 1 = read number; 2 = read exp
	
	string input;
	cin >> input;
	bool sign;
	double coef = 0;
	bool wasDot = 0;
	int zeros = 0;
	double expo = 0;

	for(i = 0; i<input.length(); i++){ //reading chars
		if(phase == 0){
			if(i == 0){
				if(input[0] != '+' && input[0] != '-'){ //first symbol not a sign (2x2+3x+5)
					sign = 1;
					i--;
				}
				else{
					if(input[i] == '+'){
						sign = 1;
					}
					else if(input[i] == '-'){
						sign = 0;
					}
					else{
						throwError();
					}
				}
			}

			else{
				if(input[i] == '+'){
					sign = 1;
				}
				else if(input[i] == '-'){
					sign = 0;
				}
				else{
					throwError();
				}
			}
			phase = 1;
			coef = 0;
			expo = 0;
			wasDot = 0;
			zeros = 0;
		}

		else if(phase == 1){
	
	
			if(input[i] == 'x' || input[i] == 'X'){ //is X
				phase = 2;
			}
			else if(input[i] == '.'){
				wasDot = 1;
			}
			else if(input[i] < 48 && input[i] > 57){ //some shit
				throwError();
			}
			else{ // is number
				coef += input[i]-48;
				coef *= 10;
				if(wasDot) zeros++;
			}
		}
		else{

			if(input[i] == '-' || input[i] == '+'){ //is another number
				phase = 0;
				if(ex.empty()){
					for(int in = 0; in <= expo/10; in++){

						ex.push_back(0);
					}
					maxExp = expo/10;
				}
				ex[expo/10] = coef/(10*pow(10, zeros))*(-1+2*sign); //*(-1+2*sign) = to get there the sign (+ or -)
				i--;
			}
			else if(input[i] < 48 && input[i] > 57){ //some shit
				throwError();
			}
			else{ // is number
				expo += input[i]-48;
				expo *= 10;
			}
		}
	}
	if(ex.empty()){
		for(int in = 0; in <= expo/10; in++){
			ex.push_back(0);
		}
		maxExp = expo/10;
	}
	ex[expo/10] = coef/(10*pow(10, zeros))*(-1+2*sign);
}

void getMaxInterval(){
	int i;
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
	int i;
	for(i = 1; i <= maxExp; i++){
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
	int maxA = sturm[numberOfSequence].size() - 1;
	int maxB = sturm[numberOfSequence-1].size() - 1;

	int e;
	double k;
	
	while(maxA >= maxB){
		e = maxA - maxB;
		k = sturm[numberOfSequence][maxA]/sturm[numberOfSequence-1][maxB];

		for(int in = maxB; in >= 0; in--){
			sturm[numberOfSequence][in+e] -= k*sturm[numberOfSequence-1][in];
		}
		while(sturm[numberOfSequence][maxA] == 0){
			maxA--;
			if(maxA < 0){
				lastSequence = 1;
			}
		}
	}

	while(sturm[numberOfSequence].back() == 0){
		sturm[numberOfSequence].pop_back();
		if(sturm[numberOfSequence].size() == 0){
			break;
		}
	} 

	if(sturm[numberOfSequence].size() <= 1) lastSequence = 1;

	rep(e, sturm[numberOfSequence].size()) sturm[numberOfSequence][e] *= -1;

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
		if(in == numberOfSequences && lastVal < 0 && val == 0) changes--;
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

	return 0;
}
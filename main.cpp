#include <bits/stdc++.h>
#define rep(a, b) for(a=0;a<b;a++)
#define per(a, b) for(a=b;a>=0;a--)

using namespace std;


vector<double> ex;
vector<double> sturm[10000];
double maxA = 0, maxB = 0;
int maxExp = 0;
bool lastSequence = 0;


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
	//cout << endl << maxA << " " << maxB << endl;
	while(maxA >= maxB){
		//cout << "tu";
		//cout << maxA << " " << maxB << endl;


		e = maxA - maxB;
		k = sturm[numberOfSequence][maxA]/sturm[numberOfSequence-1][maxB];

		//cout << k << "  " << e << endl;
		for(int in = maxB; in >= 0; in--){
			//cout << sturm[numberOfSequence][in+e] << " - " <<k << " * " << sturm[numberOfSequence-1][in]; 
			sturm[numberOfSequence][in+e] -= k*sturm[numberOfSequence-1][in];
			//cout << " = " << sturm[numberOfSequence][in+e] << endl;
		}
		//cout << endl << sturm[numberOfSequence][maxA] << endl;
		while(sturm[numberOfSequence][maxA] == 0){
			maxA--;
			if(maxA < 0){
				lastSequence = 1;
			}
		}
	}

	while(sturm[numberOfSequence].back() == 0){
		//cout << sturm[numberOfSequence].back() << "  " << sturm[numberOfSequence].size() << endl;
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
			cout << sturm[numberOfSequence][in] << "x" << in << " ";
		}
	}
	cout << endl;
}


int main(){
	int i, in;
	readInput();
	getMaxInterval();
	getFirstSequence();
	getDerivative();

	cout << "Maximal interval: <" <<maxA << "  " << maxB << ">" << endl;
	
	printSturmSequence(0);
	printSturmSequence(1);


	i = 2;	
	while(!lastSequence){
		getNextSequence(i);
		printSturmSequence(i);
		i++;
	}

	return 0;
}
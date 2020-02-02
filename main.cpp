//
// Author: Martin Picek
//

#include <iostream>
#include <inttypes.h>
#include <vector>
#include <map>
#include <stdlib.h>
#include <math.h>
#include <regex>
#include "sturmLib.h"

using namespace std;


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
			intervalsWithRoots.insert(make_pair(x-(maxB-maxA)/numberOfIntervals, x));
		}
		lastChanges = changes;
	} //TODO tadyto more roots in one interval
	
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

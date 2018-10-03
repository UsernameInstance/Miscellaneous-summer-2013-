#include "Organ.h"
#include <stdio.h>

Receptor::Receptor(std::vector<double>& rcAtt) {
	cAtt = rcAtt;
	
	if(cAtt[0] == PHOTO) cState.resize(5);
	else if(cAtt[0] == VMECH) cState.resize(2);
	else if(cAtt[0] == EMECH) cState.resize(4);
	else fprintf(stderr, "Assignment error in Receptor(std::vector<double>& rcAtt). Attempted assignment of cAtt[0] to invalid value.");	
}

double Receptor::GetAtt(int iAtt) {
	if( (iAtt >= cAtt.size()) || (iAtt < 0) ) {
		fprintf(stderr, "Invalid index access attempt in Receptor::GetAtt");
		return 0;
	}
	return cAtt[iAtt];
}

double Receptor::GetState(int iState) {
	if( (iState >= cState.size()) || (iState < 0) ) {
		fprintf(stderr, "Invalid index access attempt in Receptor::GetState");
		return 0;
	}
	return cState[iState];
}


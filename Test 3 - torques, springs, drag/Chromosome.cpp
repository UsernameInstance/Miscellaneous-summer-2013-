#include "Chromosome.h"
#include <stdlib.h>

Cond::Cond() { }
Cond::~Cond() { }

CondProb::CondProb(float fProb) { 
	if(fProb > 1) this->fProb = fProb;
	else if(fProb < 0) this->fProb = fProb;
	else this->fProb = fProb; 
}
CondProb::~CondProb() {}

bool CondProb::GetState() {
	if( float((rand() % 10000000) + 1)/10000000. <= fProb ) return true;
	else return false;
}

CondAnd::CondAnd(Cond* pcCondA, Cond* pcCondB) {
	this->pcCondA = pcCondA;
	this->pcCondB = pcCondB;
}

bool CondAnd::GetState() { return (pcCondA->GetState() && pcCondB->GetState()); }

CondOr::CondOr(Cond* pcCondA, Cond* pcCondB) {
	this->pcCondA = pcCondA;
	this->pcCondB = pcCondB;
}

bool CondOr::GetState() { return (pcCondA->GetState() || pcCondB->GetState()); }

CondGene::CondGene(Gene* pcGene) {
	this->pcGene = pcGene;
}

bool CondGene::GetState() { return pcGene->bState; }


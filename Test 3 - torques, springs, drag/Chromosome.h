#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <StdVector>

class Gene {
public:
	Gene();
	~Gene();

	bool bState;

	void OnLoop();
};

class Cond {
public:
	Cond();
	~Cond();

	enum CondType {
		TYPE_PROB, //probability type. returns true with a certain probability
		TYPE_AND,
		TYPE_OR,
		TYPE_GENE,
		TYPE_ORGAN
	};

	virtual bool GetState() = 0;
};

class CondProb: public Cond { //return true with probability fProb
public:
	CondProb(float fProb);
	~CondProb();
	
	float fProb;

	bool GetState();
};

class CondAnd: public Cond { //return true iff pcCondA logical and pcCondB return true
public:
	CondAnd(Cond* pcCondA, Cond* pcCondB);
	~CondAnd();

	Cond* pcCondA; Cond* pcCondB;

	bool GetState();
};

class CondOr: public Cond { //return true iff pcCondA logical or pcCondB return true
public:
	CondOr(Cond* pcCondA, Cond* pcCondB);
	~CondOr();

	Cond* pcCondA; Cond* pcCondB;

	bool GetState();
};

class CondGene: public Cond { //return true iff the bState of pcGene is true
public:
	CondGene(Gene* pcGene);
	~CondGene();

	Gene* pcGene;

	bool GetState();
};

class CondOrgan: public Cond { //returns true iff attribute iAtt of organ iOrg, is within dTol of dTar.
public:
	CondOrgan(int iOrg, int iAtt, double dTar, double dTol);
	~CondOrgan();

	int iOrg, iAtt;
	double dTar, dTol;
	
	bool GetState();
};

#endif
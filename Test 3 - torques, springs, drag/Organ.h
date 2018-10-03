#ifndef ORGAN_H
#define ORGAN_H

#include <StdVector>

//Could have object's (vertex here) area/volume (pi*radius^2 here) proportional to the surface area (length here) over which the force is uniformly applied during the deformation. That is, larger radius => higher toughness.
//Reflecting on size considerations. This would mean a high mass small (relatively) radius sphere (circle here in 2d), would have high ability to rupture other objects (assuming they're made of the same material).

class Receptor {
public:
	Receptor(std::vector<double>& rcAtt);
	~Receptor();

	std::vector<double> cAtt; //contains static information. i.e specifications. move to gene class? 
	std::vector<double> cState; //contains dynamic information.

	/*
	j	cAtt(j)(represents)
	0	Type {1:=VertexPhotoReceptor, 2:=VertexMechanoReceptor, 3:=EdgeMechanoReceptor} eventually... memory? holding... old states?
	1	Toughness ( (energy exerted deforming up to failure)/(volume), alternatively: (objects length) * (energy to rupture)/(object's volume) = (energy to rupture)/(amount of object's surface area to which force is uniformly applied) := energy expending in deformation to failure when the force is applied to area, area , One definition of material toughness is the amount of energy per volume that a material can absorb before rupturing ), make this inversely proportional to damage.
	2	Mass
	3	Length/radius

	PhotoReceptor
	4	Max Sight Distance
	5	Red Sensitivity
	6	Green ''
	7	Blue ''

	j	cState(j)(represents)
	0	Angle
	1	Distance
	2	Red
	3	Green
	4	Blue

	VertexMechanoReceptor
	j	cAtt(j)(represents)
	4	Radius
	5	Sensitivity (minimum ||Impulse|| recordable)

	j	cState(j)(represents)
	0	Angle
	1	||Impulse||

	EdgeMechanoReceptor
	j	cAtt(j)(represents)
	3	Strength (Tensile & Compressive, determines bounds at which damage occurs);  minimum stress at which permanent deformation (in this case damage) will occur where the stress on a body is the average force per unit area that some particle of the body exerts on an adjacent particle, across an imaginary surface that separates them.
	4	Strength (Impact, perpendicular to edge i.e. normal to unconnected sides) 
	5	Permeability (maximum permeability mulitplier, applied to impact spring constant of the rigid body. A value between 0 and 1, where if v represents the value, and c the impact spring constant, then the altered impact spring constant is given by (1-v)*c. That is if v = 1, everything passes through with no resistance, and if v = 0 everything receives the full repulsion of the impact spring constant.)
	6	(Change in permeability)/(heat) (heat gained in a cycle proportional to electrical force applied to edge)
	7	Cooling rate (decrease in heat per cycle/unit time). Less heat == less permeable.
	
	j	cState(j)(represents)
	0	Length (current)
	1	Angle
	2	Temperature
	3	Permeability constant (current)
	*/
	
	double GetAtt(int iAtt);

	double GetState(int iState);

	enum eType {
		PHOTO,
		VMECH,
		EMECH
	};

};

class Effector {
	Effector();
	~Effector();

	std::vector<double> cAtt; //contains static information. i.e specifications. move to gene class? 	
	/*
	j	cAtt(j)(represents)
	0	Type {0:=Motor}
	1	Toughness (damage per energy/(impact area))
	2	Mass

	*/
	std::vector<double> cState; //contains dynamic information.
	double GetAtt(int iAtt);

	double GetState(int iIndex);
	//state update handled externally (collisions/physics)

};

class Motor: public Effector { //change to... electrical effector... that can apply charge to... anything touching it?
	Motor();
	~Motor();

	/*
	j	cAtt(j)(represents)
	3	Radius
	4	MaxOutput
	5	Recovery Rate

	j	cState(j)(represents)
	0	Fatigue
	1	Angle
	2	||Impulse||
	*/

	std::vector<int> cEdges; //indices of connected edges.

	void DoAction(double dEnergy, int iEdge); //apply dEnergy to iEdge
};

#endif
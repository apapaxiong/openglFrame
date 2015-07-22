#ifndef __SIMULATION_H__
#define __SIMULATION_H__

#include <vector>
#include "main.h"
#include "Mesh.h"
#include "Constraint.h"
class Mesh;

class Simulation
{
public:
	Simulation(){};
	~Simulation(){};
	inline void SetMesh(Mesh* mesh) {m_mesh = mesh;}
	void Reset();
    void Update();
	void calculateInertiaY();
	void calculateExternalForce();
	void updatePosAndVel(const VectorX& new_pos);
	void computeForces(const VectorX& x, VectorX& force);
	//----------Integration-----------//
	void integrateExplicitEuler();
	//----------Constraint------------//
	void SetupConstraints();
	void AddAttachmentConstraint(unsigned int vertex_index); // add one attachment constraint at vertex_index
private:
	ScalarType m_h; // time_step

    // simulation constants
    ScalarType m_gravity_constant;
    ScalarType m_stiffness_attachment;
    ScalarType m_stiffness_stretch;
    ScalarType m_stiffness_bending;
    ScalarType m_damping_coefficient;

	VectorX m_inertia_y;

    // external force (gravity, wind, etc...)
    VectorX m_external_force;
	Mesh *m_mesh;
	std::vector<Constraint*> m_constraints;
};
#endif
#include"Simulation.h"

void Simulation::Reset()
{
	m_inertia_y.resize(m_mesh->GetSystemDimension());
	m_external_force.resize(m_mesh->GetSystemDimension());
	m_gravity_constant=0.098;
	m_h=1.0f/30.0f;
}
void Simulation::calculateInertiaY()
{
	m_inertia_y = m_mesh->m_current_positions + m_mesh->m_current_velocities * m_h;
}
void Simulation::calculateExternalForce()
{
	m_external_force.resize(m_mesh->m_system_dimension);
	m_external_force.setZero();

	// gravity
	for (unsigned int i = 0; i < m_mesh->m_vertices_number; ++i)
	{
		m_external_force[3*i+1] += -m_gravity_constant;
	}
	//cout<<m_external_force<<endl;
	m_external_force = m_external_force;  //G=mg
}
void Simulation::integrateExplicitEuler()
{
    // q_{n+1} - 2q_n + q_{n-1} = h^2 * M^{-1} * force(q_{n-1})

    // inertia term 2q_n - q_{n-1} is calculated in calculateInertiaY function

    // calculate force(q_{n-1})
   // VectorX position_previous = m_mesh->m_current_positions - m_mesh->m_current_velocities*m_h;
    VectorX position_previous = m_mesh->m_current_positions - m_mesh->m_current_velocities*m_h;
    VectorX force_previous;
    computeForces(position_previous, force_previous);

	//cout<<force_previous<<endl;
    // update q_{n+1}
    ScalarType h_square = m_h*m_h;
    VectorX pos_next = m_inertia_y + h_square*m_mesh->m_inv_mass_matrix*force_previous;
	//cout<<"pos_next"<<pos_next.block_vector(0)<<endl;
	//cout<<"m_current"<<m_mesh->m_current_positions.block_vector(0)<<endl;
    updatePosAndVel(pos_next);
}
void Simulation::computeForces(const VectorX& x, VectorX& force)
{
    VectorX gradient;

    gradient.resize(m_mesh->m_system_dimension);
    gradient.setZero();

    // springs
  /*  for (std::vector<Constraint*>::iterator it = m_constraints.begin(); it != m_constraints.end(); ++it)
    {
        (*it)->EvaluateGradient(x, gradient);
    }*/

    // external forces
    gradient -= m_external_force;

    force = -gradient;
}
void Simulation::updatePosAndVel(const VectorX& new_pos)
{
    m_mesh->m_current_velocities = (new_pos - m_mesh->m_current_positions)/m_h;
    m_mesh->m_current_positions = new_pos;
}
void Simulation::Update()
{
	calculateInertiaY();
	calculateExternalForce();
	integrateExplicitEuler();
}
#include "Constraint.h"
//----------Constraint Class----------//
Constraint::Constraint(ScalarType *stiffness) : 
    m_stiffness(stiffness)
{
}

Constraint::Constraint(const Constraint& other) : 
    m_stiffness(other.m_stiffness)
{
}

Constraint::~Constraint()
{
}
//----------AttachmentConstraint Class----------//
AttachmentConstraint::AttachmentConstraint(ScalarType *stiffness) : 
    Constraint(stiffness)
{
    m_selected = false;
}

AttachmentConstraint::AttachmentConstraint(ScalarType *stiffness, unsigned int p0, const EigenVector3& fixedpoint) : 
    Constraint(stiffness),
    m_p0(p0),
    m_fixd_point(fixedpoint)
{
    m_selected = false;
}

AttachmentConstraint::AttachmentConstraint(const AttachmentConstraint& other) : 
    Constraint(other),
    m_p0(other.m_p0),
    m_fixd_point(other.m_fixd_point),
    m_selected(other.m_selected)
{
    
}
AttachmentConstraint::~AttachmentConstraint()
{
}
void AttachmentConstraint::EvaluateGradient(const VectorX& x, VectorX& gradient)
{
    EigenVector3 g_i = (*(m_stiffness))*(x.block_vector(m_p0) - m_fixd_point);
    gradient.block_vector(m_p0) += g_i;
}
//===============Spring=====================//

SpringConstraint::SpringConstraint(ScalarType *stiffness) : 
    Constraint(stiffness)
{
}

SpringConstraint::SpringConstraint(ScalarType *stiffness, unsigned int p1, unsigned int p2, ScalarType length) : 
    Constraint(stiffness),
    m_p1(p1),
    m_p2(p2),
    m_rest_length(length)
{
}

SpringConstraint::SpringConstraint(const SpringConstraint& other) : 
    Constraint(other),
    m_p1(other.m_p1),
    m_p2(other.m_p2),
    m_rest_length(other.m_rest_length)
{
}

SpringConstraint::~SpringConstraint()
{
}
void SpringConstraint::EvaluateGradient(const VectorX& x, VectorX& gradient)
{
    EigenVector3 x_ij = x.block_vector(m_p1) - x.block_vector(m_p2);
    EigenVector3 g_ij = (*(m_stiffness))*(x_ij.norm()-m_rest_length)*x_ij.normalized();
    gradient.block_vector(m_p1) += g_ij;
    gradient.block_vector(m_p2) -= g_ij;
}

#include "JointNode.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

//---------------------------------------------------------------------------------------
JointNode::JointNode(const std::string& name)
	: SceneNode(name)
{
	m_nodeType = NodeType::JointNode;
}

//---------------------------------------------------------------------------------------
JointNode::~JointNode() {

}
 //---------------------------------------------------------------------------------------
void JointNode::set_joint_x(double min, double init, double max) {
	m_joint_x.min = min;
	m_joint_x.init = init;
	m_joint_x.max = max;

	rotate('x', init);	
}

//---------------------------------------------------------------------------------------
void JointNode::set_joint_y(double min, double init, double max) {
	m_joint_y.min = min;
	m_joint_y.init = init;
	m_joint_y.max = max;

	rotate('y', init);
}

//-------------------------------------------------------------------------------------
float clamp_angle(float val, float min, float max)
{
	if (val > max) {
		val = max;
	} else if (val < min) {
		val = min;
	} 

	return val;	
}

//--------------------------------------------------------------------------------------
void JointNode::rotate(char axis, float angle)
{
	// The implementation of this rotation is the same as the parent class except we 
	// clamp values on x and y axis
	glm::vec3 rot_axis;

	switch(axis)
	{
		case 'x':
		{
			angle = clamp_angle(angle, m_joint_x.min, m_joint_x.max);

			// check if this rotation is legal
			float resulting_total = m_total_rotations_x + angle;
			
			if (resulting_total > m_joint_x.max || resulting_total < m_joint_x.min) {
				// The total amount of rotations is not within the set bounds so we don't rotate
				return;
			}

			rot_axis = glm::vec3(1.0f, 0, 0);
		}
			break;
		case 'y':
		{

			angle = clamp_angle(angle, m_joint_y.min, m_joint_y.max);

			rot_axis = glm::vec3(0, 1.0f, 0);

			float resulting_total = m_total_rotations_y + angle;
			
			if (resulting_total > m_joint_y.max || resulting_total < m_joint_y.min) {
				return;
			}

			break;
		}
		case 'z':
			rot_axis = glm::vec3(0, 0, 1.0f);
			break;
	}

	update_total_rotations(axis, angle);

	glm::mat4 rotate = glm::rotate(glm::radians(angle), rot_axis);

	trans = rotate*trans;
}
//--------------------------------------------------------------------------------------
void JointNode::update_total_rotations(char axis, double angle)
{
	switch(axis)
	{
		case 'x':
		{
			m_total_rotations_x = clamp_angle(m_total_rotations_x + angle, m_joint_x.min, m_joint_x.max);
			break;
		}
		case 'y':
		{
			m_total_rotations_y = clamp_angle(m_total_rotations_y + angle, m_joint_y.min, m_joint_y.max);
			break;
		}
		case 'z':
			// For joints, we only be bounding rotations along x or y axis
			return;
		default:
			abort();
	}

}


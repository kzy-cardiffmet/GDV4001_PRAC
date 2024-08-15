#include "core.h"
#include <iostream>
#include <thread>
#include <chrono>


using namespace glm;


//////////////////////////////////////////////////////////////////////////////////////////
// update() - 
//////////////////////////////////////////////////////////////////////////////////////////
void cTransform::update()
{
    m_rot += m_rot_incr;

    // Updates pos by vel value
    m_pos += (glm::vec3(0.01f, 0.01f, 0.01f) * m_vel);

    ////////////////////
    // Collisions //
    // /////////////////

    // Collision with the left side of the screen
    if (m_pos.x < -10.0)
    {
        m_pos.x = -10.0; // Position == boundary
        m_vel.x = -m_vel.x; // Invert velocity
    }

    // Collision with the right side of the screen
    if (m_pos.x > 10.0)
    {
        m_pos.x = 20.0; // Position == boundary
        //m_vel.x = -m_vel.x; // Invert velocity
        // wait 3 seconds
        //auto start = std::chrono::system_clock::now();
        if (m_counter>=144*3)
        {
            m_pos.x = -10.0;
            std::cout << "Respawn" << std::endl;
            m_counter = 0;
        }
        else
        {
            m_counter++;
        }
    }

    // Collision with the bottom of the screen
    if (m_pos.y < -3.0)
    {
        m_pos.y = -3.0; // Position == boundary
        m_vel.y = -m_vel.y; // Invert velocity
    }

    // Collision with the top of the screen
    if (m_pos.y > 3.0)
    {
        m_pos.y = 3.0; // Position == boundary
        m_vel.y = -m_vel.y; // Invert velocity
    }

    



	//if (m_pos.y )
 //   {
 //       
 //       m_vel.y = -m_vel.y; // Invert velocity
 //   }

    
    

    m_ModelMatrix = glm::translate(mat4(1.0), vec3(m_pos));
    m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
    m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
    m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
    
    m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(m_scale));

    

}


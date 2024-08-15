#include <iostream>

#include "core.h"

/////////////////////////////////////////////////////////////////////////////////////
// update() - 
/////////////////////////////////////////////////////////////////////////////////////
void cScene::update()
{
	m_Camera[m_cameraId].update();
	m_Camera->m_pos.y = m_Mesh[m_player1Id].m_transform.m_pos.y;
	m_Camera->m_pos.x = m_Mesh[m_player1Id].m_transform.m_pos.x;

	for (int i = 0; i < m_mesh_count; i++)
	{
		m_Mesh[i].m_objID = i;
		m_Mesh[i].m_transform.update();
		bounce();
		drop2Ground();
	}
}

void cScene::genTerrain()
{
	m_Mesh[m_groundId].gen_terrain();

	vec3 v1, v2;
	vec3 z = vec3(0.0, 0.0, 1.0);
	vec3 terrainPos;

	// loop through all mesh objects and find the ground mesh..
	for (int j = 0; j < m_mesh_count; j++)
	{
		if (strcmp(m_Mesh[j].m_name, "ground") == 0)
		{
			// we are here... that means counter j must be the ground mesh ID. Let's store this value here
			int m_groundId = j;

			// loop through all vertices in the ground mesh
			for (int i = 0; i < m_Mesh[m_groundId].m_v_count - 1; i++)
			{
				// Steps needed to calculate the NORMAL for a single line segment on the terrain mesh...

				// Get the start and end vertices for the line segment we want to use - i.e. m_Mesh[m_groundId].m_vList[i] and m_Mesh[m_groundId].m_vList[i + 1]

				v1 = vec3(m_Mesh[m_groundId].m_vList[i + 1].m_v.x - m_Mesh[m_groundId].m_vList[i].m_v.x,
					m_Mesh[m_groundId].m_vList[i + 1].m_v.y - m_Mesh[m_groundId].m_vList[i].m_v.y,
					m_Mesh[m_groundId].m_vList[i + 1].m_v.z - m_Mesh[m_groundId].m_vList[i].m_v.z);	// calculate the vector between these two vertices (i.e. v = end-start)

				// normalize v so that it is of unit length
				v1 = normalize(v1);
				// calculate the CROSS product between v and z
				v2 = cross(v1, z);

				// update m_Mesh[m_groundId].m_vList[i].m_n with this newly calculated xyz vector
				m_Mesh[m_groundId].m_vList[i].m_n.x = v2.x;
				m_Mesh[m_groundId].m_vList[i].m_n.y = v2.y;
				m_Mesh[m_groundId].m_vList[i].m_n.z = v2.z;
			}
			terrainPos = m_Mesh[m_groundId].m_transform.m_pos;
		}
	}

	m_Mesh[m_groundId].createTerrainVBO();
}

void cScene::drop2Ground()
{
	float d;

	vec3 terrainPos = m_Mesh[m_groundId].m_transform.m_pos;

	// loop through all mesh objects again..
	for (int j = 0; j < m_mesh_count; j++)
	{
		// ignore the ground object 
		if ((strcmp(m_Mesh[j].m_name, "player1") == 0) || (strcmp(m_Mesh[j].m_name, "player2") == 0))
		{
			// find the line segment that this mesh is positioned over
			for (int i = 0; i < m_Mesh[m_groundId].m_v_count - 1; i++)
			{
				// perform a containment check between either end of the line segment
				if (m_Mesh[m_groundId].m_vList[i].m_v.x <= m_Mesh[j].m_transform.m_pos.x && m_Mesh[m_groundId].m_vList[i + 1].m_v.x >= m_Mesh[j].m_transform.m_pos.x)
				{
					vec3 a = vec3(m_Mesh[m_groundId].m_vList[i].m_v.x + m_Mesh[m_groundId].m_transform.m_pos.x,
						m_Mesh[m_groundId].m_vList[i].m_v.y + m_Mesh[m_groundId].m_transform.m_pos.y,
						m_Mesh[m_groundId].m_vList[i].m_v.z + m_Mesh[m_groundId].m_transform.m_pos.z);
					vec3 c = m_Mesh[j].m_transform.m_pos;


					// drop mesh down onto this line
					// a (x – x1) + b (y– y1) + c (z –z1) = 0

					d = (m_Mesh[m_groundId].m_vList[i].m_n.x * (a.x - (c.x))) +
						(m_Mesh[m_groundId].m_vList[i].m_n.y * (a.y - (c.y))) +
						(m_Mesh[m_groundId].m_vList[i].m_n.z * (a.z - (c.z)));

					//if (d != 0.0f)
					{
						m_Mesh[j].m_transform.m_pos.y -= d * 0.01f;


					}




				}
			}
		}
	}
}

void cScene::bounce()
{
	// for projectiles to collide and reflect off of terrain vertexes, pretty much the same as drop2ground but with different condition code at the end

	// loop through all mesh objects again..
	for (int j = 0; j < m_mesh_count; j++)
	{
		// ignore the ground object 
		if (strcmp(m_Mesh[j].m_name, "ground") == 0)
			continue;

		// ignore player1
		else if (strcmp(m_Mesh[j].m_name, "player1") == 0)
			continue;

		// ignore player2
		else if (strcmp(m_Mesh[j].m_name, "player2") == 0)
			continue;

		// find the line segment that this mesh is positioned over
		for (int i = 0; i < m_Mesh[m_groundId].m_v_count - 1; i++)
		{
			// perform a containment check between either end of the line segment
			if (m_Mesh[m_groundId].m_vList[i].m_v.x <= m_Mesh[j].m_transform.m_pos.x && m_Mesh[m_groundId].m_vList[i + 1].m_v.x >= m_Mesh[j].m_transform.m_pos.x)
			{
				vec3 a = vec3(m_Mesh[m_groundId].m_vList[i].m_v.x + m_Mesh[m_groundId].m_transform.m_pos.x,
				              m_Mesh[m_groundId].m_vList[i].m_v.y + m_Mesh[m_groundId].m_transform.m_pos.y,
				              m_Mesh[m_groundId].m_vList[i].m_v.z + m_Mesh[m_groundId].m_transform.m_pos.z);
				vec3 c = m_Mesh[j].m_transform.m_pos;


				
				// a (x – x1) + b (y– y1) + c (z –z1) = 0
				// when collision occurs


				float d = (m_Mesh[m_groundId].m_vList[i].m_n.x * (a.x - (c.x))) +
					(m_Mesh[m_groundId].m_vList[i].m_n.y * (a.y - (c.y))) +
					(m_Mesh[m_groundId].m_vList[i].m_n.z * (a.z - (c.z)));

				// if collision has occured
				if (d <=0.0f)
				{
					if(!m_Mesh[j].m_transform.m_bouncing)
					{
						m_Mesh[j].m_transform.m_bouncing = true;
						// cast cVec3f to vec3
						vec3 n_v3 = glm::vec3(m_Mesh[m_groundId].m_vList[i].m_n.x, m_Mesh[m_groundId].m_vList[i].m_n.y, m_Mesh[m_groundId].m_vList[i].m_n.z);
						// Incident vector - 2 * dot( normal vector, velocity) * normal vector
						m_Mesh[j].m_transform.m_vel = m_Mesh[j].m_transform.m_vel - 2.0f * dot(n_v3, m_Mesh[j].m_transform.m_vel) * n_v3;
					}
				}
				else 
				{
					m_Mesh[j].m_transform.m_bouncing = false;
					//gravity
					m_Mesh[j].m_transform.m_vel.y += 0.01f * -9.8f;
				}

				

				




			}
		}
	}
}



/////////////////////////////////////////////////////////////////////////////////////
// init() - 
/////////////////////////////////////////////////////////////////////////////////////
void cScene::init()
{
	openSDF();

	// initialize the meshes
	for (int i = 0; i < m_mesh_count; i++)
	{
		m_Mesh[i].init();

		// for terrain

		if (strcmp(m_Mesh[i].m_name, "ground") == 0)
			m_groundId = i;

		// for player 1

		if (strcmp(m_Mesh[i].m_name, "player1") == 0)
			m_player1Id = i;

		// for player 2

		if (strcmp(m_Mesh[i].m_name, "player2") == 0)
			m_player2Id = i;

	}


	genTerrain();

	// initialize the camera
	m_Camera->init();



	glBindVertexArray(0);

	// create the shaders
	m_ShaderInfo.create();
}

/////////////////////////////////////////////////////////////////////////////////////
// render() - 
/////////////////////////////////////////////////////////////////////////////////////
void cScene::render()
{
	if(m_start)
		drop2Ground();

	// update the scene
	update();

	// loop through and render each mesh object in this scene
	for (int i = 0; i < m_mesh_count; i++)
	{
		m_Mesh[i].render(this, &m_ShaderInfo);
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// openSDF() - Opens up a scene descriptor file
/////////////////////////////////////////////////////////////////////////////////////
void cScene::openSDF()
{
	// load the scene file here

	FILE* fp = NULL;
	
	fopen_s(&fp, "scene.sdf", "r");

	if (!fp)
		assert(0);

	char buffer[256] = { "\0"};
	float value;

	// init all counters to zero
	m_light_count = 0;
	m_mesh_count = 0;
	m_shader_count = 0;

	char* pch;

	// loop through and get the number of assets we have in the scene
	do
	{
		fgets(buffer, 255, fp);
		pch = strstr(buffer, "LIGHT:");
		if (pch != NULL) m_light_count++;

		pch = strstr(buffer, "MESH:");
		if (pch != NULL) m_mesh_count++;

		pch = strstr(buffer, "SHADER:");
		if (pch != NULL) m_shader_count++;

	} while (!feof(fp));

	rewind(fp);
	
	////////////////////////////////////////////////////////////
	// CAMERA
	////////////////////////////////////////////////////////////
	
	// allocate block of memory to store the camera
	m_Camera = new cCamera;

	fscanf_s(fp, "%s%s", buffer, 256, buffer, 256); // CAMERA {
	fscanf_s(fp, "%s%f", buffer, 256, &m_Camera->m_fov);		// FOV:	45.0
	fscanf_s(fp, "%s%f", buffer, 256, &m_Camera->m_near);		// NEAR:	0.5
	fscanf_s(fp, "%s%f", buffer, 256, &m_Camera->m_far);		// FAR:	100.0
	fscanf_s(fp, "%s%f%f%f", buffer, 256, &m_Camera->m_pos.x, &m_Camera->m_pos.y, &m_Camera->m_pos.z);		// POS:		0.0 0.0 -5.0
	fscanf_s(fp, "%s", buffer, 256);				// }
	
	m_Camera->m_target_pos = m_Camera->m_pos;


	////////////////////////////////////////////////////////////
	// LIGHTS
	////////////////////////////////////////////////////////////
	
	// allocate block of memory to store the light..
	m_Light = new cLight[m_light_count];

	for (int i = 0; i < m_light_count; i++)
	{
		fscanf_s(fp, "%s%s", buffer, 256, buffer, 256);	// LIGHT: }
		m_Light[i].m_type = 0; // diffuse
		fscanf_s(fp, "%s%f%f%f", buffer, 256, &m_Light[i].m_pos.x, &m_Light[i].m_pos.y, &m_Light[i].m_pos.z); // POS:	-25.0 0.0 20.0
		fscanf_s(fp, "%s", buffer, 256);				// }
	}

	////////////////////////////////////////////////////////////
	// MESHES
	////////////////////////////////////////////////////////////
	char filename[256];
	char name[256];

	// allocate block of memory to store the mesh objects..
	m_Mesh = new cMesh[m_mesh_count];

	for (int i = 0; i < m_mesh_count; i++)
	{
		fscanf_s(fp, "%s%s", buffer, 256, m_Mesh[i].m_filename, 256);	//MESH: torus.obj
		fscanf_s(fp, "%s", buffer, 256);					// }
		fscanf_s(fp, "%s%s", buffer, 256, m_Mesh[i].m_name, 256);		//MESH: torus.obj
		fscanf_s(fp, "%s%f%f%f", buffer, 256, &m_Mesh[i].m_transform.m_pos.x, &m_Mesh[i].m_transform.m_pos.y, &m_Mesh[i].m_transform.m_pos.z);					// POS:	0.0 0.0 0.0
		fscanf_s(fp, "%s%f%f%f", buffer, 256, &m_Mesh[i].m_transform.m_rot.x, &m_Mesh[i].m_transform.m_rot.y, &m_Mesh[i].m_transform.m_rot.z);					// ROT : 2.0 2.0 0.0
		fscanf_s(fp, "%s%f%f%f", buffer, 256, &m_Mesh[i].m_transform.m_rot_incr.x, &m_Mesh[i].m_transform.m_rot_incr.y, &m_Mesh[i].m_transform.m_rot_incr.z);	// ROT_INCR:	1.0 1.0 0.0
		fscanf_s(fp, "%s%f%f%f", buffer, 256, &m_Mesh[i].m_transform.m_scale.x, &m_Mesh[i].m_transform.m_scale.y, &m_Mesh[i].m_transform.m_scale.z);			// SCALE : 1.25 1.25 1.25
		fscanf_s(fp, "%s%f%f%f", buffer, 256, &m_Mesh[i].m_transform.m_vel.x, &m_Mesh[i].m_transform.m_vel.y, &m_Mesh[i].m_transform.m_vel.z);					// VEL : 0.2 0.1 0.0
		fscanf_s(fp, "%s%d", buffer, 256, &m_Mesh[i].m_shaderID); // SHADER_ID	0

		fscanf_s(fp, "%s", buffer, 256);					// }
	}

	////////////////////////////////////////////////////////////
	// SHADERS
	////////////////////////////////////////////////////////////
	ShaderTable = new sShaderDetails[m_shader_count];
	
	fscanf_s(fp, "%s", buffer, 256); // SHADER:

	for (int i = 0; i < m_shader_count; i++)
	{
		// init shader types..
		for( int j=0; j<6; j++ )
			ShaderTable[i].types[j] = 0;


		fscanf_s(fp, "%s", buffer, 256);		
		strcpy_s(ShaderTable[i].filename, buffer);
		
		int loop_count = 0;
		
		while (strcmp(buffer, "SHADER:") != 0)
		{
			loop_count++;

			fscanf_s(fp, "%s", buffer, 256);
			if (strcmp(buffer, "SHADER:") == 0)
				break;

			// sort the shader by type and add to list..
			if (strcmp(buffer, "VERT_SHDR") == 0)
				ShaderTable[i].types[0] = GL_VERTEX_SHADER;
			else
			if (strcmp(buffer, "CTRL_SHDR") == 0)
				ShaderTable[i].types[1] = GL_TESS_CONTROL_SHADER;
			else
			if (strcmp(buffer, "EVAL_SHDR") == 0)
				ShaderTable[i].types[2] = GL_TESS_EVALUATION_SHADER;
			else
			if (strcmp(buffer, "GEOM_SHDR") == 0)
				ShaderTable[i].types[3] = GL_GEOMETRY_SHADER;
			else
			if (strcmp(buffer, "FRAG_SHDR") == 0)
				ShaderTable[i].types[4] = GL_FRAGMENT_SHADER;
			else
				ShaderTable[i].types[5] = GL_COMPUTE_SHADER;

			if (loop_count == 6)
				break;
		};
	}

	fclose(fp);

}
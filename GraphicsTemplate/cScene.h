#pragma once

#include "core.h"

class cMesh;

class cScene
{
public:

	cScene() 
	{
		m_start = false;
		m_cameraId	= 0;
		m_lightId	= 0;
	}

	~cScene() {}
	void init();
	void render();
	void update();
	void openSDF();
	void drop2Ground();
	void bounce();
	void genTerrain();
	void player1Inputs();

	cShaderInfo		m_ShaderInfo;
	cCamera*		m_Camera;
	cLight*			m_Light;
	cMesh*			m_Mesh;
	

	sShaderDetails* ShaderTable;

	bool m_start;
	int m_shader_count;
	int m_light_count;
	int m_mesh_count;
	int m_cameraId;
	int m_lightId;
	int m_groundId;
	int m_player1Id;
	int m_player2Id;

private:

};
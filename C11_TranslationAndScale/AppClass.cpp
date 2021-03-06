#include "AppClass.h"
void Application::InitVariables(void)
{
	//init the mesh
	m_pMesh = new MyMesh();
	//m_pMesh->GenerateCube(1.0f, C_WHITE);
	m_pMesh->GenerateSphere(1.0f, 5, C_WHITE);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();

	matrix4 m4Model = IDENTITY_M4;

	//m4Model[3][0] = 1.0f; // by value
	//m4Model[3] = vector4(1.0f); // by column
	//m4Model = glm::translate(IDENTITY_M4, vector3(1.0f)); //using glm library
	
	
	//m4Model[0][0] = 2.0f; // by value

	//m4Model = glm::scale(IDENTITY_M4, vector3(2.0f)); // using glm library

	//matrix4 m4Trans = glm::scale(IDENTITY_M4, vector3(1.0f, 0.0f, 0.0f)); 
	//matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(2.0f)); 
	//m4Model = m4Trans * m4Scale; // scales then transforms
	//m4Model = m4Scale * m4Trans; // Scales the transform

	static float fDisp = 0.0f;
	m4Model = glm::translate(IDENTITY_M4, vector3(fDisp, 0.0f, 0.0f)); 
	fDisp += 0.1f;


	m_pMesh->Render(m4Projection, m4View, m4Model);
	for (uint i = 0; i < 10; i++) {
		m_pMesh->Render(m4Projection, m4View, m4Model * glm::translate(vector3(0.0f, i, 0.0f)));
	}

	m_pMesh->Render(m4Projection, m4View, m4Model * glm::translate(vector3(1.0f)));


	//matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(2.0f,2.0f,2.0f));
	//static float value = 0.0f;
	//matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(value, 2.0f, 3.0f));
	//value += 0.01f;

	//matrix4 m4Model = m4Translate * m4Scale;
	//matrix4 m4Model = m4Scale * m4Translate;

	//m_pMesh->Render(m4Projection, m4View, m4Model);
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();

	m_pMeshMngr->AddGridToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	SafeDelete(m_pMesh);

	//release GUI
	ShutdownGUI();
}
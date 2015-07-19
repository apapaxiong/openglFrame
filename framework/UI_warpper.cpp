#include "UI_warpper.h"
extern bool g_show_wireframe;
extern float cameraDistance;
AntTweakBarWrapper::AntTweakBarWrapper()
{
}

AntTweakBarWrapper::~AntTweakBarWrapper()
{
	Cleanup();
}
void AntTweakBarWrapper::Init()
{
	TwInit(TW_OPENGL, NULL);
	m_control_panel_bar = TwNewBar("Control Panel");
	//TwDefine(" 'Control Panel' size='200 210' position='114 10' color='255 255 255' text=dark ");
	char control_bar_pos_string [255];
    sprintf(control_bar_pos_string, "'Control Panel' position='%d 10' color='255 255 0' ", DEFAULT_SCREEN_WIDTH-210);
    TwDefine(control_bar_pos_string);

	TwAddVarRW(m_control_panel_bar, "Wireframe", TwType(sizeof(bool)), &(g_show_wireframe), " label='Wireframe mode' key=w group='Visualization' help='Toggle wireframe display mode.'");
	TwAddVarRW(m_control_panel_bar, "Zoom", TW_TYPE_FLOAT, &(cameraDistance),"label='Zoom Mode' min=-30 max=30 step=0.1 keyIncr=z keyDecr=Z help='zoom camera' group='Camera'");
}
void AntTweakBarWrapper::Reset()
{
    Cleanup(); 
    Init();
}
void AntTweakBarWrapper::Cleanup()
{
    m_control_panel_bar = NULL;
    m_mesh_bar = NULL;
    m_sim_bar = NULL;

    TwTerminate();
}
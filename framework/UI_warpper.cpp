#include "UI_warpper.h"
extern bool g_show_wireframe;
extern bool g_show_edgelist;
extern float cameraDistance;
extern float g_linewidth;
extern bool g_show_render;
extern bool g_show_fixedvertex;
extern bool g_start_simulation;
extern float g_pointsize;
extern SimulationMethod g_CurrentMethod;
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

	TwAddVarRW(m_control_panel_bar, "Start Simulation", TwType(sizeof(bool)), &(g_start_simulation), " label='Start Simulation' key=s group='Debug Information' help='Toggle simulation mode.'");
	TwAddVarRW(m_control_panel_bar, "Render FP", TwType(sizeof(bool)), &(g_show_render), " label='Render FP' key=w group='Debug Information' help='Toggle render display mode.'");
	TwAddVarRW(m_control_panel_bar, "Wireframe", TwType(sizeof(bool)), &(g_show_wireframe), " label='Wireframe mode' key=w group='Debug Information' help='Toggle wireframe display mode.'");
	TwAddVarRW(m_control_panel_bar, "Edge List", TwType(sizeof(bool)), &(g_show_edgelist), " label='Edgelist mode'  group='Debug Information' help='Toggle edgelist display mode.'");
	TwAddVarRW(m_control_panel_bar, "Line Width", TW_TYPE_FLOAT, &(g_linewidth),"label='Line Width' min=0 max=100 step=0.05  help='line width' group='Debug Information'");
	TwAddVarRW(m_control_panel_bar, "Fixed Vertex", TwType(sizeof(bool)), &(g_show_fixedvertex), " label='Fixed Vertex' key=w group='Debug Information' help='Toggle fixed vertex mode.'");
	TwAddVarRW(m_control_panel_bar, "Point Size", TW_TYPE_FLOAT, &(g_pointsize),"label='Point Size' min=0 max=100 step=0.05  help='Point Size' group='Debug Information'");

	TwAddVarRW(m_control_panel_bar, "Zoom", TW_TYPE_FLOAT, &(cameraDistance),"label='Zoom Mode' min=-30 max=30 step=0.1 keyIncr=z keyDecr=Z help='zoom camera' group='Camera'");

	m_sim_bar = TwNewBar("Simulation Control");
	TwDefine("'Simulation Control' position='0 0' color='255 0 0'");

	TwEnumVal shapeEV[NUM_SIMULATION] = { {PositionBasedDynamic, "Position Based Dynamic"}, {MassSpring, "Mass Spring"}, {FiniteElement, "Finite Element"}, {FastMassSpring, "Fast Mass Spring"}, {ProjectiveDynamic, "Projective Dynamic"} };
    TwType simmethod = TwDefineEnum("simmethod", shapeEV, NUM_SIMULATION);
	TwAddVarRW(m_sim_bar, "Simulation Method", simmethod, &g_CurrentMethod, " keyIncr='<' keyDecr='>' help='Change Simulation Method.' ");
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
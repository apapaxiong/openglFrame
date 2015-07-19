#ifndef _ANTTWEAKBAR_WRAPPER_H_
#define _ANTTWEAKBAR_WRAPPER_H_

#include "AntTweakBar.h"
#include "main.h"

typedef enum
{
    ATB_RESHAPE_WINDOW = 0x1,
    ATB_CHANGE_TIME_STEP = 0x2,
    ATB_CHANGE_STIFFNESS = 0x4,
    ATB_DEFAULT = 0x0

} ATBFeedBack;

class AntTweakBarWrapper
{
public:
    AntTweakBarWrapper();
    virtual ~AntTweakBarWrapper();

    // init / cleanup / reset / save / load / default / hide / show / update / Draw / change window size
    void Init();
    void Cleanup();
    void Reset();
    void SaveSettings();
    void LoadSettings();
    void DefaultSettings();
    void Hide();
    void Show();
    int Update();
    inline void Draw() {TwDraw();}
    inline void ChangeTwBarWindowSize(int width, int height) {TwWindowSize(width, height);}

    static void TW_CALL SetDefaultSettings(void*);
    static void TW_CALL SaveSettings(void*);
    static void TW_CALL LoadSettings(void*);

protected:

    // key component: bars
    TwBar *m_control_panel_bar; // Control Panel
    TwBar *m_mesh_bar;   // Mesh Settings
    TwBar *m_sim_bar;    // Simulation Settings
};

#endif
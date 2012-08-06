#ifndef GLOBALS_H
#define GLOBALS_H

#include <game/GameEngine.hpp>
#include <system/EventManager.hpp>
#include <system/ScreenManager.hpp>
#include <system/ResourceManager.hpp>
#include <system/Clock.hpp>
#include <graphics/Primitives.hpp>

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                        Globals
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

// global class pointers
extern GameEngine* g_gameEngine;
extern EventManager* g_eventManager;
extern ScreenManager* g_screenManager;
extern ResourceManager* g_resourceManager;
extern Clock* g_systemClock;

extern ControlScreen* g_controlScreen;
extern Primitives* g_primitives;

// properties
extern bool DISPLAY_LIST;
extern bool VISUALIZATIONS_ON;
extern bool NORMALS_PER_VERTEX;
extern bool NORMALS_PER_VERTEX_WEIGHTED;
extern bool USE_TEXTURES;
extern int TEXTURE_MODE;
extern bool GLOBAL_MATERIAL;
extern bool NORMAL_VIS_OFF;

// crease angles
extern float CREASE_ANGLE1;
extern float CREASE_ANGLE2;
extern float CREASE_ANGLE3;
extern float CREASE_ANGLE4;
extern float CREASE_ANGLE;
extern bool SHARP_EDGES;

// aabb tree
extern bool VISUALIZE_AABB_TREE;
extern int AABB_TREE_DEPTH;
extern int AABB_TREE_MAX_DEPTH;
extern bool VISUALIZE_LEAF;
extern int SHOW_NODE_ID;

// projectiles
extern int PROJECTILE_TYPE;

// splines
extern bool SHOW_SPLINE;
extern int SPLINE_MODE;

// control mode
extern int CONTROL_MODE;

// mouse warp
extern bool MOUSE_WARP;

#endif
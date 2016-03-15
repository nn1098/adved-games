#include "CommandParser.h"
#include "common.h"
#include "component_camera.h"
#include "component_mesh_renderer.h"
#include "entity.h"
#include "gamelogic.h"
#include "genericInput.h"
#include "glm/glm.hpp"
#include "groundplane.h"
#include "input.h"
#include "menu.h"
#include "renderer.h"
#include "resource.h"
#include "component_shipdriver.h"
#include "component_track.h"
#include "component_player_shipdriver.h"
#include "scene.h"
#include "time.h"
#include "video.h"
#include <glm/gtx/transform.hpp>
#include <iomanip>
#include <iostream>
#include <GL\glut.h>

#pragma comment (lib,"glut32.lib")
#pragma comment (lib,"glu32.lib")
#pragma warning( disable : 4098;)
// SUPER TEMP
#include "font.h"
bool RUNGAME;
bool helpselect;
bool controlselect;
bool inMenu;

string controls = "";

Entity player;

void DrawSphere(const glm::vec3 &p0, float radius, float r, float g, float b) {
  glPushMatrix();
  glColor3f(r, g, b);
  glTranslatef(p0.x, p0.y, p0.z);
  glutSolidSphere(radius, 10, 9);
  glPopMatrix();
	}


bool GameLogic::Init() {
  GroundPlane::Init();
  RUNGAME = true;
  inMenu = true;
  return false;
}

bool Menu_up(const std::vector<std::string> &params) {
  LOG(logDEBUG2) << "Menu Up";
  Menu::activeMenu->Move(MENU_UP);
  return true;
}
bool Menu_down(const std::vector<std::string> &params) {
  LOG(logDEBUG2) << "Menu Down";
  Menu::activeMenu->Move(MENU_DOWN);
  return true;
}
bool Menu_enter(const std::vector<std::string> &params) {
  LOG(logDEBUG2) << "Menu Enter";
  Menu::activeMenu->Move(MENU_RIGHT);
  return true;
}

bool Start_Game(const std::vector<std::string> &params) {
  LOG(logDEBUG2) << "starting game";
  inMenu = false;
  controlselect = false;
  return true;
}
bool Quit_game(const std::vector<std::string> &params) {
  RUNGAME = false;
  return true;
}
bool Menu_Help(const std::vector<std::string> &params) {
  helpselect = true;
  return true;
}
bool Back_To_Menu(const std::vector<std::string> &params) {
  LOG(logDEBUG2) << "main menu";
  helpselect = false;
  return true;
}

// get menu movement commands
bool Controls(const std::vector<std::string> &params) {
// loop through all availible commands  
	for (auto &it : CommandParser::commands) {
		// loop through command parser storage
		for (auto &cmd : actual_Input_builtins) {
			// get all bound commands 
			for (auto &b : cmd.bindings){
					controls += std::string(b) + ", " + std::string(it.name) + '\n';
			}
		}
	}

//controls display switch
  if (controlselect != true) {
    controlselect = true;
  } else {
    controlselect = false;
  }
  return true;
}

// main game loop
bool GameLogic::Run() {
	// player and camera setup
  player = Entity();
  Components::FollowCamera cam = Components::FollowCamera();
  Components::CmMeshRenderer playermr = Components::CmMeshRenderer();
  Components::CmPlayerShipDriver playerdr = Components::CmPlayerShipDriver();
  player.AddComponent(cam);
  player.AddComponent(playermr);
  player.AddComponent(playerdr);
  playermr.SetMesh("ship1.obj");
  Scene::SetActiveCamera(&cam);
  player.SetPosition(glm::vec3(0, 1, -8));

  //box renderer
  Entity box = Entity();
  Components::CmMeshRenderer mr = Components::CmMeshRenderer();
  box.AddComponent(mr);
  mr.SetMesh("cube.obj");

  Entity box2 = Entity();
  Components::CmMeshRenderer mr2 = Components::CmMeshRenderer();
  box2.AddComponent(mr2);
  mr2.SetMesh("ship1.obj");
  box2.SetPosition(glm::vec3(2, 0, 0));
 
  /* was track
  Entity box3 = Entity();
  Components::CmTrack mr3 = Components::CmTrack();
  box3.AddComponent(mr3);
  mr3.SetMesh("track.obj");
  box3.SetPosition(glm::vec3(0, 0.1f, 0));
  box3.SetScale(glm::vec3(50.0f));
  */



  // another box
  Entity box4 = Entity();
  Components::CmMeshRenderer mr4 = Components::CmMeshRenderer();
  box4.AddComponent(mr4);
  mr4.SetMesh("cube.obj");
  box4.SetPosition(glm::vec3(-2, 0, 0));

  //add these to the scene
  Scene::scene_list.push_back(&player);
  Scene::scene_list.push_back(&box);
  Scene::scene_list.push_back(&box2);
 // Scene::scene_list.push_back(&box3);
  Scene::scene_list.push_back(&box4);

  // main menu setup
  auto mnu = new Menu();
  mnu->SetPosition({400, 400});
  auto mnuitm1 = new MenuItem("Unamed", false, nullptr);
  mnu->GetItems()->push_back(mnuitm1);
  auto mnuitm2 = new MenuItem("Start", true, Start_Game);
  mnu->GetItems()->push_back(mnuitm2);
  auto mnuitm3 = new MenuItem("Help", true, Menu_Help);
  mnu->GetItems()->push_back(mnuitm3);
  auto mnuitm4 = new MenuItem("Quit", true, Quit_game);
  mnu->GetItems()->push_back(mnuitm4);

  auto helpmnu = new Menu();
  helpmnu->SetPosition({100, 100});
  auto helpmnuitm1 = new MenuItem("Help Menu", false, nullptr);
  helpmnu->GetItems()->push_back(helpmnuitm1);
  auto helpmnuitm2 = new MenuItem("Start", true, Start_Game);
  helpmnu->GetItems()->push_back(helpmnuitm2);
  auto helpmnuitm3 = new MenuItem("Back To Menu", true, Back_To_Menu);
  helpmnu->GetItems()->push_back(helpmnuitm3);
  auto helpmnuitm4 = new MenuItem("Quit", true, Quit_game);
  helpmnu->GetItems()->push_back(helpmnuitm4);
  auto helpmnuitm5 = new MenuItem("Controls", true, Controls);
  helpmnu->GetItems()->push_back(helpmnuitm5);

  // menu input commands
  CommandParser::commands.push_back({"menu_up", "", 0, Menu_up});
  CommandParser::commands.push_back({"menu_down", "", 0, Menu_down});
  CommandParser::commands.push_back({"menu_enter", "", 0, Menu_enter});
  CommandParser::commands.push_back({"quit", "", 0, Quit_game});
  CommandParser::commands.push_back({"Controls", "", 0, Controls});
  CommandParser::commands.push_back({"Back_To_Menu", "", 0, Back_To_Menu});
  CommandParser::commands.push_back({"Start_Game", "", 0, Start_Game});
  CommandParser::commands.push_back({"Menu_Help", "", 0, Menu_Help});

  // menu key bindings, route through the parser for now
  CommandParser::Cmd_Bind({"", "menu_up", "UP", ""});
  CommandParser::Cmd_Bind({"", "menu_down", "DOWN", ""});
  CommandParser::Cmd_Bind({"", "menu_enter", "ENTER", ""});
  CommandParser::Cmd_Bind({"", "quit", "ESC", ""});

  //frametime calc
  auto t = Now();
  double frametimes[256];
  for (auto &t : frametimes) {
    t = 0.0;
  }
  uint8_t ftc = 0;

  while (RUNGAME) {
    const auto delta = (chrono::duration_cast<chrono::milliseconds>(Now() - t)).count();
    const double deltaPercent = delta / 1000.0;
    t = Now();
    static float runtime = 0;
    runtime += (float)deltaPercent;
    frametimes[++ftc] = deltaPercent;
    static string avg = "FPS:0";
    if (ftc % 10 == 0) {
      double davg = 0;
      for (const auto t : frametimes) {
        davg += t;
      }
      davg = 1.0 / (davg / 255.0);
      avg = "FPS:" + std::to_string((unsigned int)davg);
    }


	//check for player input
    Input::Update();

	//player rotation
    static float rot = 0;
    rot += 0.2f * deltaPercent;
    box.SetRotation(glm::quat_cast(rotate(rot, glm::vec3(0.3f, 0.9f, 0.5f))));

    // menu choice
	if (inMenu) {
		if (helpselect) {
			Menu::activeMenu = helpmnu;
			helpmnu->Update();
		}
		else {
			Menu::activeMenu = mnu;
			mnu->Update();
		}
	}


	//update the scene
    Scene::Update(deltaPercent);

	//update the ground
    GroundPlane::Update();

	//clear screen
    Renderer::PreRender();

	//render the scene and ground
    Scene::Render(deltaPercent);
    GroundPlane::Render(player.GetPosition().x, player.GetPosition().z);

    // rendering choice
    if (inMenu) {
      if (helpselect == true) {
        helpmnu->Render();
      } else {
        mnu->Render();
      }
    }
	

	DrawSphere(glm::vec3(0,0,0),3.0f,1.0f,0,0);

	//draw text
    Font::Draw(25, avg.c_str(), {100, 30}, {0.2f, 0, 0, 1.0f});

    //  speed and position overlay 
    if (helpselect == false) {
        stringstream speedstream;
		vec3 lastpos;
		speedstream << fixed 
			<< std::setprecision(2) << to_string(glm::length(playerdr.velocity_.x)/ deltaPercent)<<"  " << to_string(glm::length(playerdr.velocity_.y)/deltaPercent)
			<<"   "<< to_string(glm::length(playerdr.velocity_.z)/ deltaPercent) << " m/s\n("
                  <<"location"<< player.GetPosition().x << "," << player.GetPosition().z << ")";
      Font::Draw(25, speedstream.str().c_str(), {100, 200}, {0.2f, 0, 0, 1.0f});
      
    }

	//draw the controls and clear of text after to
    // prevent it still being drawn if menu is reopened
    if (controlselect == true && helpselect == true) {
      Font::Draw(25, controls.c_str(), {400, 400}, {0.2f, 1.0f, 0, 1.0f});
    } else {
      controls = "";
    }

	//font rendering
    Font::Render();

	//buffer handover
    Renderer::PostRender();

    Video::Swap();
  }

  //unload and free memory of ground 
  GroundPlane::Shutdown();
  
  //end of execution
  return true;
}

bool GameLogic::Shutdown() { return false; }

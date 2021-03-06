#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Scene.h"
#include "j1Animation.h"
#include "j1EntityManager.h"
#include "Units.h"

j1Scene::j1Scene() : j1Module()
{
	name.assign("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	if(App->map->Load("iso_walk.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if(App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	debug_tex = App->tex->Load("maps/path2.png");

	App->entity_manager->CreateUnit(CAVALRYARCHER, fPoint(30, 170));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(250, 400));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(300, 360));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(400, 350));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(450, 415));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(280, 320));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(320, 320));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(360, 320));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(400, 320));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(400, 240));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(430, 270));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(460, 300));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(490, 330));
	App->entity_manager->CreateUnit(TWOHANDEDSWORDMAN, fPoint(520, 360));


	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += floor(200.0f * dt);

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= floor(200.0f * dt);

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += floor(200.0f * dt);

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= floor(200.0f * dt);

	App->map->Draw();
	
	// Debug pathfinding ------------------------------
	/*
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	App->render->Blit(debug_tex, p.x - 32, p.y - 32);

	const std::vector<iPoint>* path = App->pathfinding->GetLastPath();
	
	if (path->size() != 0)
	{
		std::vector<iPoint>::const_iterator item = path->begin();

		while(item != path->end())
		{
			iPoint pos = App->map->MapToWorld(item->x, item->y);
			App->render->Blit(debug_tex, pos.x - 32, pos.y - 32);
			item++;
		}
	}*/

	//SELECTION
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y); App->input->GetMousePosition(x, y);

	if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
	{
		App->entity_manager->UnselectEverything();

		select_rect.x = x - App->render->camera.x;
		select_rect.y = y - App->render->camera.y;
		select_rect.w = select_rect.x;
		select_rect.h = select_rect.y;
	}

	else if (App->input->GetMouseButtonDown(1) == KEY_REPEAT)
	{
		select_rect.w = x - App->render->camera.x;
		select_rect.h = y - App->render->camera.y;
		App->render->DrawQuad({ select_rect.x, select_rect.y, select_rect.w - select_rect.x, select_rect.h - select_rect.y }, 255, 255, 255, 255, false);
	}

	if (App->input->GetMouseButtonDown(1) == KEY_UP)
	{
		App->entity_manager->SelectInQuad(select_rect);
	}
	//--

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

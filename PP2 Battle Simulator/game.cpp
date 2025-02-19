#include "precomp.h" // include (only) this in every .cpp file
//1279
#define NUM_TANKS_BLUE 1279
#define NUM_TANKS_RED 1279

#define TANK_MAX_HEALTH 1000
#define ROCKET_HIT_VALUE 30
#define PARTICLE_BEAM_HIT_VALUE 50

#define TANK_MAX_SPEED 1.5

#define HEALTH_BARS_OFFSET_X 0
#define HEALTH_BAR_HEIGHT 70
#define HEALTH_BAR_WIDTH 1
#define HEALTH_BAR_SPACING 0

#define MAX_FRAMES 2000

#define FRAME_CAP 60

//Global performance timer
//M-PC     = 44306.7
//M-Laptop = 44301.9
//T-Pc     =
//T-Laptop =

#define REF_PERFORMANCE 44309 //UPDATE THIS WITH YOUR REFERENCE PERFORMANCE (see console after 2k frames)
static timer perf_timer;
static float duration;

//Load sprite files and initialize sprites
static Surface* background_img = new Surface("assets/Background_Grass.png");
static Surface* tank_red_img = new Surface("assets/Tank_Proj2.png");
static Surface* tank_blue_img = new Surface("assets/Tank_Blue_Proj2.png");
static Surface* rocket_red_img = new Surface("assets/Rocket_Proj2.png");
static Surface* rocket_blue_img = new Surface("assets/Rocket_Blue_Proj2.png");
static Surface* particle_beam_img = new Surface("assets/Particle_Beam.png");
static Surface* smoke_img = new Surface("assets/Smoke.png");
static Surface* explosion_img = new Surface("assets/Explosion.png");

static Sprite background(background_img, 1);
static Sprite tank_red(tank_red_img, 12);
static Sprite tank_blue(tank_blue_img, 12);
static Sprite rocket_red(rocket_red_img, 12);
static Sprite rocket_blue(rocket_blue_img, 12);
static Sprite smoke(smoke_img, 4);
static Sprite explosion(explosion_img, 9);
static Sprite particle_beam_sprite(particle_beam_img, 3);

const static vec2 tank_size(14, 18);
const static vec2 rocket_size(25, 24);

const static float tank_radius = 12.f;
const static float rocket_radius = 10.f;

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
    //cout << "Hallo Wouter of Gert, als jullie scherm meer dan 60fps aan kan moet je dit ff boven in de settings aanpassen." << endl;
    //cout << "heel af en toe crasht de game door een memory fout, we konden niet achterhalen waar deze vandaan kwam." << endl;

    frame_count_font = new Font("assets/digital_small.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZ:?!=-0123456789.");

    time_between_Frames = (int)((float)1 / FRAME_CAP * 1000000);

    tanks.reserve(NUM_TANKS_BLUE + NUM_TANKS_RED);
    sorted_tanks.reserve(NUM_TANKS_BLUE + NUM_TANKS_RED);
    uint rows = (uint)sqrt(NUM_TANKS_BLUE + NUM_TANKS_RED);
    uint max_rows = 12;

    float start_blue_x = tank_size.x + 10.0f;
    float start_blue_y = tank_size.y + 80.0f;

    float start_red_x = 980.0f;
    float start_red_y = 100.0f;

    float spacing = 15.0f;

    maxheight = SCRHEIGHT - HEALTH_BAR_HEIGHT;

    int id = 0;
    //Spawn blue tanks
    for (int i = 0; i < NUM_TANKS_BLUE; i++)
    {
        // create the tank
        Tank tank = Tank(start_blue_x + ((i % max_rows) * spacing), start_blue_y + ((i / max_rows) * spacing), BLUE, &tank_blue, &smoke, 1200, 600, tank_radius, TANK_MAX_HEALTH, TANK_MAX_SPEED, id++, &tankgrid);
        // add the tank to the tanks list.
        tanks.push_back(tank);

        sorted_tanks.push_back(&(tanks[tanks.size() - 1]));
    }

    //Spawn red tanks
    for (int i = 0; i < NUM_TANKS_RED; i++)
    {
        Tank tank = Tank(start_red_x + ((i % max_rows) * spacing), start_red_y + ((i / max_rows) * spacing), RED, &tank_red, &smoke, 80, 80, tank_radius, TANK_MAX_HEALTH, TANK_MAX_SPEED, id++, &tankgrid);
        // add the tank to the tanks list.
        tanks.push_back(tank);

        sorted_tanks.push_back(&(tanks[tanks.size() - 1]));
    }

    cout << tanks.at(tanks.size() - 1).position.x << "-" << tanks.at(tanks.size() - 1).position.y << endl;

    particle_beams.push_back(Particle_beam(vec2(80, 80), vec2(100, 50), &particle_beam_sprite, PARTICLE_BEAM_HIT_VALUE));
    particle_beams.push_back(Particle_beam(vec2(SCRWIDTH / 2, SCRHEIGHT / 2), vec2(100, 50), &particle_beam_sprite, PARTICLE_BEAM_HIT_VALUE));
    particle_beams.push_back(Particle_beam(vec2(1200, 600), vec2(100, 50), &particle_beam_sprite, PARTICLE_BEAM_HIT_VALUE));

    initialize_particle_beams(particle_beams.at(0));
    initialize_particle_beams(particle_beams.at(1));
    initialize_particle_beams(particle_beams.at(2));

    tankgrid.show_tanks();

    for (int x = 0; x < SCRWIDTH; x += tankgrid.cell_size)
    {
        for (int y = 0; y < SCRHEIGHT; y++)
        {
            background.GetBuffer()[x + y * SCRWIDTH] = (0xE61F1C);
        }
    }
    for (int x = 0; x < SCRHEIGHT; x += tankgrid.cell_size)
    {
        for (int y = 0; y < SCRWIDTH; y++)
        {
            background.GetBuffer()[y + x * SCRWIDTH] = (0xE61F1C);
        }
    }
}

void Tmpl8::Game::selectcell(int _x, int _y)
{
    int sx = _x * tankgrid.cell_size;
    int sy = _y * tankgrid.cell_size;

    for (int x = sx; x < sx + tankgrid.cell_size; x++)
    {
        for (int y = sy; y < sy + tankgrid.cell_size; y++)
        {
            if (x > 0 && y > 0 && x < SCRWIDTH && y < SCRHEIGHT)

                background.GetBuffer()[x + y * SCRWIDTH] = (0xFF5A00);
        }
    }
}

void Tmpl8::Game::handle_tank_collision(int begin, SIZE_T end)
{

    for (int i = begin; i < end; i++)
    {
        auto& tank = tanks.at(i);

        if (tank.active)
        {
            //Check tank collision and nudge tanks away from each other
            for (auto oTank : tankgrid.get_tanks_in_radius(2, tank.position.x, tank.position.y))
            {
                if (tank.ID == oTank->ID) continue;

                vec2 dir = tank.Get_Position() - oTank->Get_Position();

                float dirSquaredLen = dir.sqrLength();

                //float colSquaredLen = (tank.Get_collision_radius() * tank.Get_collision_radius()) + (oTank->Get_collision_radius() * oTank->Get_collision_radius());
                if (dirSquaredLen < 288)
                {
                    std::lock_guard<std::mutex> guard(mutex);

                    tank.Push(dir.normalized(), 1.f);
                }
            }
            //Move tanks according to speed and nudges (see above) also reload
            {
                std::lock_guard<std::mutex> guard(mutex2);

                tank.Tick();
            }
            //Shoot at closest target if reloaded
            if (tank.Rocket_Reloaded())
            {
                Tank& target = FindClosestEnemy(tank);
                {
                    std::lock_guard<std::mutex> guard(mutex3);

                    rockets.push_back(Rocket(tank.position, (target.Get_Position() - tank.position).normalized() * 3, rocket_radius, tank.allignment, ((tank.allignment == RED) ? &rocket_red : &rocket_blue)));

                    tank.Reload_Rocket();
                }
            }
        }
    }
}
// -----------------------------------------------------------
// Close down application
// -----------------------------------------------------------
void Game::Shutdown()
{
}

void Tmpl8::Game::initialize_particle_beams(Particle_beam& beam)
{
    for (int y = beam.min_position.y - tank_radius; y < beam.max_position.y + tank_radius; y += tankgrid.cell_size / 2)
    {
        for (int x = beam.min_position.x - tank_radius; x < beam.max_position.x + tank_radius; x += tankgrid.cell_size / 2)
        {
            int xpos = x / tankgrid.cell_size;
            int ypos = y / tankgrid.cell_size;

            if ((xpos > 0 && ypos > 0 && xpos < tankgrid.cell_amount - 1 && ypos < tankgrid.cell_amount - 1) == false)
            {
                return;
            }

            beam.cells_in_sight.emplace(vec2(xpos, ypos));
        }
    }

    for (auto& pos : beam.cells_in_sight)
    {
        selectcell(pos.x, pos.y);
    }
}

// -----------------------------------------------------------
// Iterates through all tanks and returns the closest enemy tank for the given tank
// -----------------------------------------------------------
Tank& Game::FindClosestEnemy(Tank& current_tank)
{
    float closest_distance = numeric_limits<float>::infinity();

    int closest_index = 0;

    if (current_tank.allignment == 1)
    {
        for (int i = 0; i < NUM_TANKS_BLUE; i++)
        {
            if (tanks.at(i).active)
            {
                float sqrDist = fabsf((tanks.at(i).Get_Position() - current_tank.Get_Position()).sqrLength());
                if (sqrDist < closest_distance)
                {
                    closest_distance = sqrDist;
                    closest_index = i;
                }
            }
        }
    }
    else
    {
        for (int i = NUM_TANKS_BLUE; i < NUM_TANKS_BLUE + NUM_TANKS_RED; i++)
        {
            if (tanks.at(i).active)
            {
                float sqrDist = fabsf((tanks.at(i).Get_Position() - current_tank.Get_Position()).sqrLength());
                if (sqrDist < closest_distance)
                {
                    closest_distance = sqrDist;
                    closest_index = i;
                }
            }
        }
    }
    return tanks.at(closest_index);
}

bool Tmpl8::Game::hit_tank(Tank& tank, int dmg)
{
    hitted_tanks.emplace(&tank);

    return tank.hit(dmg);
}

void Tmpl8::Game::sort_tanks()
{
    sorted_tanks.erase(std::remove_if(sorted_tanks.begin(), sorted_tanks.end(), [=](Tank* x) { return hitted_tanks.count(x) == 1; }), sorted_tanks.end());

    for (Tank* tank : hitted_tanks)
    {
        int begin = (int)tank->allignment == 0 ? 0 : NUM_TANKS_BLUE;

        for (int i = begin; i < begin + NUM_TANKS_BLUE; i++)
        {
            if (tank->health <= sorted_tanks.at(i)->health)
            {
                sorted_tanks.insert(sorted_tanks.begin() + i, tank);

                break;
            }
        }
    }

    hitted_tanks.clear();
}

void Tmpl8::Game::handle_rockets()
{
    for (Rocket& rocket : rockets)
    {
        if (rocket.active == false)
        {
            continue;
        }

        rocket.Tick();

        int x = rocket.position.x;
        int y = rocket.position.y;

        if (x < 0 || y < 0 || x > tanks.size() - 1 || y > tanks.size() - 1)
        {
            rocket.active = false;

            continue;
        }

        for (Tank* tank : tankgrid.get_enemies_in_cell(rocket.position.x, rocket.position.y, rocket.allignment))
        {
            if (rocket.Intersects(tank->position, tank->collision_radius))
            {
                explosions.push_back(Explosion(&explosion, tank->position));

                if (hit_tank(*tank, ROCKET_HIT_VALUE))
                {
                    smokes.push_back(Smoke(smoke, tank->position - vec2(0, 48)));
                }

                rocket.active = false;
                break;
            }
        }
    }
}

// -----------------------------------------------------------
// Update the game state:s
// Move all objects
// Update sprite frames
// Collision detection
// Targeting etc..
// -----------------------------------------------------------
void Game::Update(float deltaTime)
{
    /*   int part = 319;
    int mod = 6;*/

    // Gebruik onderstaande berekening als je het aantal tanks veranderdt.
    // vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

    int part = tanks.size() / threadcount;
    int mod = tanks.size() % threadcount;

    int current = 0;

    vector<future<void>*> futures;

    for (int i = 0; i < threadcount; i++)
    {
        int start = current;

        int end = current + part + (--mod > 0 ? 1 : 0);

        current = end;

        futures.push_back(&pool.enqueue([&]() { handle_tank_collision(start, end); }));
        // ZONDER DIT PUNTJE WERKT HET BLIJKBAAR NIET, NIET VERWIJDEREN!
        cout << '.';
    }

    //Update smoke plumes
    for (Smoke& smoke : smokes)
    {
        smoke.Tick();
    }

    //Update rockets
    handle_rockets();

    rockets.erase(std::remove_if(rockets.begin(), rockets.end(), [](const Rocket& rocket) { return !rocket.active; }), rockets.end());

    handle_particle_beams();
    //Update explosion sprites and remove when done with remove erase idiom
    for (Explosion& explosion : explosions)
    {
        explosion.Tick();
    }

    explosions.erase(std::remove_if(explosions.begin(), explosions.end(), [](const Explosion& explosion) { return explosion.done(); }), explosions.end());

    for (int i = 0; i < threadcount; i++)
    {
        futures.at(i)->wait();
    }
}

void Tmpl8::Game::handle_particle_beams()
{
    for (Particle_beam& particle_beam : particle_beams)
    {
        particle_beam.tick2();

        for (auto& pos : particle_beam.cells_in_sight)
        {
            //Damage all tanks within the damage window of the beam (the window is an axis-aligned bounding box)
            for (auto& tank : tankgrid.get_cell(pos.x, pos.y))
            {
                if (tank.second->active && particle_beam.rectangle.intersectsCircle(tank.second->Get_Position(), tank.second->Get_collision_radius()))
                {
                    if (hit_tank(*tank.second, particle_beam.damage))
                    {
                        smokes.push_back(Smoke(smoke, tank.second->position - vec2(0, 48)));
                    }
                }
            }
        }
    }
}

void Game::Draw()
{
    for (int i = 0; i < NUM_TANKS_BLUE + NUM_TANKS_RED; i++)
    {

        vec2 tPos = tanks.at(i).Get_Position();
        // tread marks
        if ((tPos.x >= 0) && (tPos.x < SCRWIDTH) && (tPos.y >= HEALTH_BAR_HEIGHT) && (tPos.y < maxheight))
        {
            background.GetBuffer()[(int)tPos.x + (int)tPos.y * SCRWIDTH] = SubBlend(background.GetBuffer()[(int)tPos.x + (int)tPos.y * SCRWIDTH], 0x808080);
        }
    }

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - frame_start_time).count();

    if (duration < time_between_Frames)
    {
        return;
    }

    // clear the graphics window
    screen->Clear(0);

    //Draw background
    background.Draw(screen, 0, 0);

    //Draw sprites
    for (int i = 0; i < NUM_TANKS_BLUE + NUM_TANKS_RED; i++)
    {
        vec2 tPos = tanks.at(i).Get_Position();

        if ((tPos.x >= 0) && (tPos.x < SCRWIDTH) && (tPos.y >= HEALTH_BAR_HEIGHT) && (tPos.y < maxheight))
        {
            tanks.at(i).Draw(screen);
        }
    }

    for (Rocket& rocket : rockets)
    {
        vec2 tPos = rocket.position;
        // tread marks
        if ((tPos.x >= 0) && (tPos.x < SCRWIDTH) && (tPos.y >= HEALTH_BAR_HEIGHT) && (tPos.y < maxheight))
        {
            rocket.Draw(screen);
        }
    }

    for (Smoke& smoke : smokes)
    {
        vec2 tPos = smoke.position;
        // tread marks
        if ((tPos.x >= 0) && (tPos.x < SCRWIDTH) && (tPos.y >= HEALTH_BAR_HEIGHT) && (tPos.y < maxheight))
        {
            smoke.Draw(screen);
        }
    }

    for (Particle_beam& particle_beam : particle_beams)
    {
        particle_beam.Draw(screen);
    }

    for (Explosion& explosion : explosions)
    {
        vec2 tPos = explosion.position;
        // tread marks
        //if ((tPos.x >= 0) && (tPos.x < SCRWIDTH) && (tPos.y >= HEALTH_BAR_HEIGHT) && (tPos.y < maxheight))
        {
            explosion.Draw(screen);
        }
    }

    sort_tanks();

    //Draw sorted health bars
    for (int t = 0; t < 2; t++)
    {
        int begin = t == 0 ? 0 : NUM_TANKS_BLUE;

        int health_bar_start_y = (t < 1) ? 0 : (SCRHEIGHT - HEALTH_BAR_HEIGHT) - 1;
        int health_bar_end_y = (t < 1) ? HEALTH_BAR_HEIGHT : SCRHEIGHT - 1;

        for (int i = 0; i < (NUM_TANKS_BLUE); i++)
        {
            int health_bar_start_x = i * (HEALTH_BAR_WIDTH + HEALTH_BAR_SPACING) + HEALTH_BARS_OFFSET_X;
            int health_bar_end_x = health_bar_start_x + HEALTH_BAR_WIDTH;

            screen->Bar(health_bar_start_x, health_bar_start_y, health_bar_end_x, health_bar_end_y, REDMASK);
            screen->Bar(health_bar_start_x, health_bar_start_y + (int)((double)HEALTH_BAR_HEIGHT * (1 - ((double)sorted_tanks.at(i + begin)->health / (double)TANK_MAX_HEALTH))), health_bar_end_x, health_bar_end_y, GREENMASK);
        }
    }

    string frame_count_string = "FRAME: " + std::to_string(frame_count);
    frame_count_font->Print(screen, frame_count_string.c_str(), 350, 580);

    frame_start_time = std::chrono::high_resolution_clock::now();
}

// -----------------------------------------------------------
// When we reach MAX_FRAMES print the duration and speedup multiplier
// Updating REF_PERFORMANCE at the top of this file with the value
// on your machine gives you an idea of the speedup your optimizations give
// -----------------------------------------------------------
void Tmpl8::Game::MeasurePerformance()
{
    char buffer[128];
    if (frame_count >= MAX_FRAMES)
    {
        if (!lock_update)
        {
            duration = perf_timer.elapsed();
            cout << "Duration was: " << duration << " (Replace REF_PERFORMANCE with this value)" << endl;
            cout << "Duration of the tested method was " << total_time << endl;
            lock_update = true;
        }

        frame_count--;
    }

    if (lock_update)
    {
        screen->Bar(420, 170, 870, 430, 0x030000);
        int ms = (int)duration % 1000, sec = ((int)duration / 1000) % 60, min = ((int)duration / 60000);
        sprintf(buffer, "%02i:%02i:%03i", min, sec, ms);
        frame_count_font->Centre(screen, buffer, 200);
        sprintf(buffer, "SPEEDUP: %4.1f", REF_PERFORMANCE / duration);
        frame_count_font->Centre(screen, buffer, 340);
    }
}

// -----------------------------------------------------------
// Main application tick function
// -----------------------------------------------------------
void Game::Tick(float deltaTime)
{
    if (!lock_update)
    {
        Update(deltaTime);
    }
    Draw();

    MeasurePerformance();

    // print something in the graphics window
    //screen->Print("hello world", 2, 2, 0xffffff);

    // print something to the text window
    //cout << "This goes to the console window." << std::endl;

    //Print frame count

    frame_count++;
}

void Tmpl8::Game::start_timer()
{
    start_time = std::chrono::high_resolution_clock::now();
}

void Tmpl8::Game::stop_timer()
{
    auto stop_time = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time);

    total_time += (float)duration.count();
}

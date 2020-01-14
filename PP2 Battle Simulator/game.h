#pragma once
#include <set>
namespace Tmpl8
{
//forward declarations
class Tank;
class Rocket;
class Smoke;
class Particle_beam;
class TankGrid;
class Game
{
  public:

    void SetTarget(Surface* surface) { screen = surface; }
    void Init();
    void Shutdown();
    void Update(float deltaTime);
    void Draw();
    void Tick(float deltaTime);
    void insertion_sort_tanks_health(const std::vector<Tank>& original, std::vector<const Tank*>& sorted_tanks, UINT16 begin, UINT16 end);
    void MeasurePerformance();

    // --------testing variables---------
    std::chrono::steady_clock::time_point start_time; 
    float total_time;
    void start_timer();
    void stop_timer();
    // ----------------------------------

    std::chrono::steady_clock::time_point frame_start_time = std::chrono::high_resolution_clock::now();

    int time_between_Frames;
    
    Tank& FindClosestEnemy(Tank& current_tank);

    bool hit_tank(Tank& tank,int dmg);

    void MouseUp(int button)
    { /* implement if you want to detect mouse button presses */
    }

    void MouseDown(int button)
    { /* implement if you want to detect mouse button presses */
    }

    void MouseMove(int x, int y)
    { /* implement if you want to detect mouse movement */
    }

    void KeyUp(int key)
    { /* implement if you want to handle keys */
    }

    void KeyDown(int key)
    { /* implement if you want to handle keys */
    }

  private:
    Surface* screen;

    vector<Tank> tanks;

    std::set<Tank*> hitted_tanks;
    std::vector<Tank*> sorted_tanks;

	void sort_tanks();

	TankGrid tankgrid = TankGrid(*this);

    vector<Rocket> rockets;
    vector<Smoke> smokes;
    vector<Explosion> explosions;
    vector<Particle_beam> particle_beams;

    Font* frame_count_font;
    long long frame_count = 0;

    bool lock_update = false;

    int maxheight;


};

}; // namespace Tmpl8
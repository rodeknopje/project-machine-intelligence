#pragma once
#include <set>
#include <mutex>
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
    void handle_particle_beams();
    void Draw();
    void Tick(float deltaTime);
    void insertion_sort_tanks_health(const std::vector<Tank>& original, std::vector<const Tank*>& sorted_tanks, UINT16 begin, UINT16 end);
    void MeasurePerformance();
    void initialize_particle_beams(Particle_beam& beam);

    void selectcell(int _x, int _y);
    void handle_tank_collision(int begin, SIZE_T end);
    void handle_rockets();

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


    const unsigned int threadcount = thread::hardware_concurrency();
    ThreadPool& pool = ThreadPool(threadcount);

	void sort_tanks();


	TankGrid tankgrid = TankGrid(*this);

    std::mutex mutex;
    std::mutex mutex2;
    std::mutex mutex3;

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
#pragma once
#include <map>
#include <set>
namespace Tmpl8
{

class Game;

class TankGrid
{
  public:
    // groote van de map
    static const int map_size    = 1600;
    // de wortel van het aantal cellen in het grid.
    static const int cell_amount = 100;
    // de groote van een individueele cell.
    static const int cell_size   = (int)(map_size/cell_amount);

	TankGrid(Game& _game);
	void add_tank(int _x, int _y, Tank* tank);
    void move_tank(int _x, int _y, const int ID, Tank* tank);
    void del_tank(int _x, int _y, const int ID);
    Tank* find_closest_enemy(float _x, float _y, int radius, int ID, int allignment);
    vector<Tank*> get_enemies_in_cell(float _x, float _y, int allignment);
    vector<Tank*> get_tanks_in_radius(int radius ,float _x, float _y);
    void show_tanks();
    bool enemy_in_cell(int _x, int _y, int allignment);

    std::map<int, Tank*> get_cell(int _x,int _y);

  private:
    std::map<int, Tank*> cells[cell_amount][cell_amount];
    bool display = false;
    Game& game;
};
} // namespace Tmpl8

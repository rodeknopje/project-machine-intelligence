#pragma once
#include <map>
#include <set>
namespace Tmpl8
{

class Game;

class TankGrid
{
  public:
    static const int cell_size = 35;

	TankGrid(Game& _game);
	void add_tank(int _x, int _y, Tank* tank);
    void move_tank(int _x, int _y, const int ID, Tank* tank);
    void del_tank(int _x, int _y, const int ID);
    Tank* find_closest_enemy(float _x, float _y, int radius, int ID, int allignment);
    vector<Tank*> get_enemies_in_cell(float _x, float _y, int allignment);
    void show_tanks();
    bool enemy_in_cell(int _x, int _y, int allignment);
    std::map<int, Tank*> get_cell(int _x,int _y);

  private:
    std::map<int, Tank*> cells[cell_size][cell_size];
    bool display = false;
    Game& game;
};
} // namespace Tmpl8

#pragma once
#include <map>

namespace Tmpl8
{
class TankGrid
{
  public:
    static const int size = 43;


	TankGrid();
	void add_tank(int _x, int _y, Tank* tank);
    void move_tank(int _x, int _y, const int ID, Tank* tank);
    void del_tank(int _x, int _y, const int ID);
    Tank* find_closest_enemy(float _x, float _y, int allignment);
    void show_tanks();

  private:
    std::map<int, Tank*> cells[size][size];
    bool display = false;
};
} // namespace Tmpl8

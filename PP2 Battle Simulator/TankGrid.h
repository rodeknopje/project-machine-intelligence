#pragma once
#include <map>

namespace Tmpl8
{
class TankGrid
{
  public:
    static const int size = 35;


	TankGrid();
	void add_tank(int _x, int _y, Tank* tank);
    void move_tank(int _x, int _y, const int ID, Tank* tank);
    void show_tanks();

  private:
    std::map<int, Tank*> cells[size][size];
};
} // namespace Tmpl8

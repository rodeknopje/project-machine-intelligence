#pragma once
#include <map>

namespace Tmpl8
{
class TankGrid
{
  public:
    static const int size = 100;


	TankGrid();
	void add_tank(int _x, int _y, Tank* tank);

  private:
    std::map<int, Tank*> cells[size][size];
};
} // namespace Tmpl8

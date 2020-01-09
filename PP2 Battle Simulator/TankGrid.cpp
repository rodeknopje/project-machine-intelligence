#include "TankGrid.h"
#include "precomp.h"
#include <map>

namespace Tmpl8
{

TankGrid::TankGrid()
{


}

void TankGrid::add_tank(int _x, int _y, Tank* tank)
{
    cells[_x][_y].emplace(tank->ID, tank);

	cout << _x  << "," << _y  << " - "<< cells[_x][_y].size() << endl;
}


} // namespace Tmpl8
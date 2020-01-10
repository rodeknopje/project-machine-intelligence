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

    //cout << _x << "," << _y << " - " << cells[_x][_y].size() << endl;
}

void TankGrid::move_tank(int _x, int _y, const int ID, Tank* tank)
{
    cout << cells[_x][_y].erase(ID);

    int x = (int)tank->position.x / size;
    int y = (int)tank->position.y / size;

    tank->prev_x = x;
    tank->prev_y = y;

    cells[x][y].emplace(tank->ID, tank);

    show_tanks();
}

void TankGrid::show_tanks()
{
    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
        {
            int csize = cells[x][y].size();
            if (csize == 0)
                cout << "[--]";
            else
                cout << "[" << (csize > 9 ? "" : "0") << csize << "]";
        }
        cout << endl;
    }
    cout << endl;
}

} // namespace Tmpl8
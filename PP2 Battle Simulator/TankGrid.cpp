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
    //if (_x < 0 || _y < 0 || _x > size - 1 || _y > size - 1)
    //{
    //    tank->hit(10000);

    //    return;
    //}

    cells[_x][_y].emplace(tank->ID, tank);

    //show_tanks();
    //cout << _x << "," << _y << " - " << cells[_x][_y].size() << endl;
}

void TankGrid::move_tank(int _x, int _y, const int ID, Tank* tank)
{
    //system("cls");

    cells[_x][_y].erase(ID);

    int x = (int)tank->position.x / size;
    int y = (int)tank->position.y / size;

    if (x < 0 || y < 0 || x > size - 1 || y > size - 1)
    {
        tank->hit(10000);

        return;
    }

    tank->prev_x = x;
    tank->prev_y = y;

    cells[x][y].emplace(tank->ID, tank);

    show_tanks();
}

void TankGrid::del_tank(int _x, int _y, const int ID)
{
    //cells[_x][_y].erase(ID);

    show_tanks();
}

Tank* TankGrid::find_closest_enemy(float _x, float _y, int radius, int ID, int allignment)
{
    return nullptr;
}

vector<Tank*> TankGrid::get_enemies_in_cell(float _x, float _y, int allignment)
{
    vector<Tank*> tanks;

    int x=(int)_x/size;
    int y=(int)_y/size;

    if (x < 0 || y < 0 || x > size - 1 || y > size - 1)
    {
        return tanks;
    }

    for (auto& cell : cells[x][y])
    {
        if (cell.second->allignment != allignment && cell.second->active)
        {
            tanks.push_back(cell.second);
        }
    }

    //cout << tanks.size() << endl;

    return tanks;
}



void TankGrid::show_tanks()
{
    if (!display)
        return;
    system("cls");

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
}

bool TankGrid::enemy_in_cell(int _x, int _y, int allignment)
{
    for (auto& cell : cells[_x][_y])
    {
        if (cell.second->allignment != allignment)
        {
            return true;
        }
    }

    return false;
}

std::map<int, Tank*> TankGrid::get_cell(int _x, int _y)
{
    return cells[_x][_y];
}

} // namespace Tmpl8
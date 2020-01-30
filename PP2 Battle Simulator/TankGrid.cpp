#include "TankGrid.h"
#include "precomp.h"
#include <algorithm>
#include <map>

namespace Tmpl8
{

TankGrid::TankGrid(Game& _game) : game(_game)
{
}

void TankGrid::add_tank(int _x, int _y, Tank* tank)
{

    cells[_x][_y].emplace(tank->ID, tank);

    //show_tanks();
}

void TankGrid::move_tank(int _x, int _y, const int ID, Tank* tank)
{
    //system("cls");
    cells[_x][_y].erase(ID);

    int x = (int)tank->position.x / cell_size;
    int y = (int)tank->position.y / cell_size;

    //if (std::find(laser_cells.begin(), laser_cells.end(), vec2(x, y)) != laser_cells.end())
    //{
    //    game.hit_tank(*tank,50);
    //    //return;
    //}

    if (x < 0 || y < 0 || x > map_size || y > map_size)
    {
        tank->active = false;

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

    //show_tanks();
}

Tank* TankGrid::find_closest_enemy(float _x, float _y, int radius, int ID, int allignment)
{
    return nullptr;
}

vector<Tank*> TankGrid::get_enemies_in_cell(float _x, float _y, int allignment)
{
    vector<Tank*> tanks;

    if (_x < 0 || _y < 0 || _x > map_size || _y > map_size)
    {
        return tanks;
    }

    int x = (int)_x / cell_size;
    int y = (int)_y / cell_size;

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

vector<Tank*> TankGrid::get_tanks_in_radius(int radius, float _x, float _y)
{
    vector<Tank*> tanks;

    if (_x < 0 || _y < 0 || _x > map_size || _y > map_size)
    {
        return tanks;
    }

    int pos_x = (int)_x / cell_size;
    int pos_y = (int)_y / cell_size;

    for (int y = pos_y - 1; y < pos_y + 2; y++)
    {
        if (y > cell_amount - 1 || y < 0)
        {
            continue;
        }

        for (int x = pos_x - 1; x < pos_x + 2; x++)
        {
            if (x > cell_amount - 1 || x < 0)
            {
                continue;
            }

            for (auto& cell : cells[x][y])
            {
                tanks.push_back(cell.second);
            }
        }
    }

    return tanks;
}

void TankGrid::show_tanks()
{
    if (!display)
        return;
    system("cls");

    for (int y = 0; y < cell_amount; y++)
    {
        for (int x = 0; x < cell_amount; x++)
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
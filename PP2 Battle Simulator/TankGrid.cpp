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
    cells[_x][_y].erase(ID);

    show_tanks();
}

Tank* TankGrid::find_closest_enemy(float _x, float _y, int allignment)
{
    float closest_distance = numeric_limits<float>::infinity();

    int cx = 0;
    int cy = 0;
    int cID = 0;

    int cell_x = (int)_x / size;
    int cell_y = (int)_x / size;

    bool found = false;

    for (auto& cell : cells[(int)_x / size][(int)_y / size])
    {
        if (cell.second->allignment != allignment)
        {
            float sqrDist = fabsf((cell.second->Get_Position() - vec2(_x, _y)).sqrLength());

            if (sqrDist < closest_distance)
            {
                found = true;

                closest_distance = sqrDist;
                cx = _x;
                cy = _y;
                cID = cell.second->ID;
            }
        }
    }
    return found ? cells[cx][cy].at(cID) : nullptr;
}

//float closest_distance = numeric_limits<float>::infinity();
//int closest_index = 0;
//
//for (int i = 0; i < tanks.size(); i++)
//{
//    if (tanks.at(i).allignment != current_tank.allignment && tanks.at(i).active)
//    {
//        float sqrDist = fabsf((tanks.at(i).Get_Position() - current_tank.Get_Position()).sqrLength());
//        if (sqrDist < closest_distance)
//        {
//            closest_distance = sqrDist;
//            closest_index = i;
//        }
//    }
//}
//
//return tanks.at(closest_index);

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

} // namespace Tmpl8
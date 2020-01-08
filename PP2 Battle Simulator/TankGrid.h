#pragma once
#include <map>

namespace Tmpl8
{

class TankGrid
{
    static const int size = 10;

    private:
    std::map<int, Tank*> tanks;
};

} // namespace Tmpl8

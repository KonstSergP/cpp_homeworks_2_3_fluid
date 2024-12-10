#pragma once

#include <random>
#include <algorithm>
#include <cstring>

#include "constants.h"
#include "fixed.h"
#include "VectorField.h"

using std::tuple, std::pair;

// rho and g type?

template <typename pt, typename vt, typename vft, int N, int M>
struct Simulator
{
    VectorField<vt, N, M> velocity{};
    VectorField<vft, N, M> velocity_flow{};
    pt p[N][M]{}, old_p[N][M];
    pt rho[256]{};
    vt g;
    int last_use[N][M]{};
    int dirs[N][M]{};
    int UT = 0;
    std::mt19937 rnd;

    Simulator();

    tuple<vft, bool, pair<int, int>> propagate_flow(int x, int y, vft lim);
    void propagate_stop(int x, int y, bool force = false);
    vt move_prob(int x, int y);
    void swap_between(int x, int y, int nx, int ny);
    bool propagate_move(int x, int y, bool is_first);
    pt random01();
    void directionsInit();
    void nextTick();

};




template <typename pt, typename vt, typename vft, int N, int M>
Simulator<pt, vt, vft, N, M>::Simulator(): rnd(1337)
{
    rho[' '] = 0.01;
    rho['.'] = 1000;
    g = 0.1;
    directionsInit();
}


template <typename pt, typename vt, typename vft, int N, int M>
void Simulator<pt, vt, vft, N, M>::directionsInit()
{
    for (size_t x = 0; x < N; ++x) {
        for (size_t y = 0; y < M; ++y) {
            if (field[x][y] == '#')
                continue;
            for (auto [dx, dy] : deltas) {
                dirs[x][y] += (field[x + dx][y + dy] != '#');
            }
        }
    }
}


template <typename pt, typename vt, typename vft, int N, int M>
void Simulator<pt, vt, vft, N, M>::swap_between(int x, int y, int nx, int ny)
{
    std::swap(field[x][y], field[nx][ny]);
    std::swap(p[x][y], p[nx][ny]);
    std::swap(velocity.v[x][y], velocity.v[nx][ny]);
}

template <typename T>
T get_random(int32_t n) {
    return T::from_raw(n);
}

template <typename T> requires std::is_floating_point_v<T>
T get_random(int32_t n) {
    return (T)n / (T)(1 << 16);
}


template <typename pt, typename vt, typename vft, int N, int M>
pt Simulator<pt, vt, vft, N, M>::random01() {
    return get_random<pt>((rnd() & ((1 << 16) - 1)));
}


template <typename pt, typename vt, typename vft, int N, int M>
tuple<vft, bool, std::pair<int, int>> Simulator<pt, vt, vft, N, M>::propagate_flow(int x, int y, vft lim)
{
    last_use[x][y] = UT - 1;
    vft ret = 0;
    for (auto [dx, dy] : deltas)
    {
        int nx = x + dx, ny = y + dy;
        if (field[nx][ny] != '#' && last_use[nx][ny] < UT)
        {
            auto cap = velocity.get(x, y, dx, dy);
            auto flow = velocity_flow.get(x, y, dx, dy);
            if (vt(flow) == cap) continue;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            auto vp = std::min(vt(lim), cap - vt(flow));//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            //std::cout << "pf: " << lim << " " << vt(lim) << " " << cap << " " << flow << " " << vt(flow) << " " << cap - vt(flow) << "\n";
            if (last_use[nx][ny] == UT - 1) {
                velocity_flow.add(x, y, dx, dy, vp);
                last_use[x][y] = UT;
                return {vp, 1, {nx, ny}};
            }
            auto [t, prop, end] = propagate_flow(nx, ny, vp);
            ret += t;
            if (prop)
            {
                velocity_flow.add(x, y, dx, dy, t);
                last_use[x][y] = UT;
                return {t, end != std::pair(x, y), end};
            }
        }
    }
    last_use[x][y] = UT;
    return {ret, false, {0, 0}};
}


template <typename pt, typename vt, typename vft, int N, int M>
void Simulator<pt, vt, vft, N, M>::propagate_stop(int x, int y, bool force)
{
    if (!force)
    {
        bool stop = true;
        for (auto [dx, dy] : deltas)
        {
            int nx = x + dx, ny = y + dy;
            if (field[nx][ny] != '#' && last_use[nx][ny] < UT - 1 && velocity.get(x, y, dx, dy) > vt(0)) {//!!!!!!!!!!!!
                stop = false;
                break;
            }
        }
        if (!stop) return;
    }
    last_use[x][y] = UT;
    for (auto [dx, dy] : deltas)
    {
        int nx = x + dx, ny = y + dy;
        if (field[nx][ny] == '#' || last_use[nx][ny] == UT || velocity.get(x, y, dx, dy) > vt(0)) {//!!!!!!!!!!!!!!!!!!!
            continue;
        }
        propagate_stop(nx, ny);
    }
}


template <typename pt, typename vt, typename vft, int N, int M>
vt Simulator<pt, vt, vft, N, M>::move_prob(int x, int y)
{
    vt sum = 0;
    for (auto [dx, dy] : deltas)
    {
        int nx = x + dx, ny = y + dy;
        if (field[nx][ny] == '#' || last_use[nx][ny] == UT) {
            continue;
        }
        auto v = velocity.get(x, y, dx, dy);
        if (v < vt(0)) {//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            continue;
        }
        sum += v;
    }
    return sum;
}


template <typename pt, typename vt, typename vft, int N, int M>
bool Simulator<pt, vt, vft, N, M>::propagate_move(int x, int y, bool is_first)
{
    last_use[x][y] = UT - is_first;
    bool ret = false;
    int nx = -1, ny = -1;
    do {
        std::array<vt, deltas.size()> tres;
        vt sum = 0;
        for (size_t i = 0; i < deltas.size(); ++i)
        {
            auto [dx, dy] = deltas[i];
            int nx = x + dx, ny = y + dy;
            if (field[nx][ny] == '#' || last_use[nx][ny] == UT) {
                tres[i] = sum;
                continue;
            }
            auto v = velocity.get(x, y, dx, dy);
            if (v < vt(0)) {//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                tres[i] = sum;
                continue;
            }
            sum += v;
            tres[i] = sum;
        }

        if (sum == vt(0)) {//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            break;
        }

        vt p = vt(random01()) * sum;
        size_t d = std::ranges::upper_bound(tres, p) - tres.begin();

        auto [dx, dy] = deltas[d];
        nx = x + dx;
        ny = y + dy;
        assert(velocity.get(x, y, dx, dy) > vt(0) && field[nx][ny] != '#' && last_use[nx][ny] < UT);//!!!!!!!!!!!!!!!!!!

        ret = (last_use[nx][ny] == UT - 1 || propagate_move(nx, ny, false));
    } while (!ret);

    last_use[x][y] = UT;
    for (size_t i = 0; i < deltas.size(); ++i)
    {
        auto [dx, dy] = deltas[i];
        int nx = x + dx, ny = y + dy;
        if (field[nx][ny] != '#' && last_use[nx][ny] < UT - 1 && velocity.get(x, y, dx, dy) < vt(0)) {//!!!!!!!!!!!!!!!!
            propagate_stop(nx, ny);
        }
    }
    if (ret)
    {
        if (!is_first) {
            swap_between(x, y, nx, ny);
        }
    }
    return ret;
}


template <typename pt, typename vt, typename vft, int N, int M>
void Simulator<pt, vt, vft, N, M>::nextTick()
{
    //std::cout << 1 << "\n";
    // Apply external forces
    for (size_t x = 0; x < N; ++x)
    {
        for (size_t y = 0; y < M; ++y)
        {
            if (field[x][y] == '#') continue;
            if (field[x + 1][y] != '#') {
                velocity.add(x, y, 1, 0, g);
            }
        }
    }
    //std::cout << 2 << "\n";
    // Apply forces from p
    memcpy(old_p, p, sizeof(p));
    for (size_t x = 0; x < N; ++x)
    {
        for (size_t y = 0; y < M; ++y)
        {
            if (field[x][y] == '#') continue;
            for (auto [dx, dy] : deltas)
            {
                int nx = x + dx, ny = y + dy;
                if (field[nx][ny] != '#' && old_p[nx][ny] < old_p[x][y])
                {
                    auto force = old_p[x][y] - old_p[nx][ny];
                    auto& contr = velocity.get(nx, ny, -dx, -dy);
                    if (pt(contr) * rho[(int) field[nx][ny]] >= force)//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                    {
                        contr -= vt(force / rho[(int) field[nx][ny]]);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                        continue;
                    }
                     force -= pt(contr) * rho[(int) field[nx][ny]];//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                    contr = 0;
                    velocity.add(x, y, dx, dy, force / rho[(int) field[x][y]]);
                    p[x][y] -= force / pt(dirs[x][y]); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                }
            }
        }
    }
    //std::cout << 3 << "\n";
    // Make flow from velocities
    velocity_flow = {};
    bool prop;
    do {
        UT += 2;
        prop = false;
        for (size_t x = 0; x < N; ++x) {
            for (size_t y = 0; y < M; ++y) {
                if (field[x][y] != '#' && last_use[x][y] != UT) {
                    auto [t, local_prop, _] = propagate_flow(x, y, 1);
                    //std::cout << t << "\n";
                    if (t < vft(eps<32, 16>) && t > vft(0)) {t = vft(0);}//###########################################################
                    if (t > vft(0)) { // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                        //std::cout << t << "\n";
                        prop = true;
                    }
                }
            }
        }
    } while (prop);

    //std::cout << 4 << "\n";

    // Recalculate p with kinetic energy
    for (size_t x = 0; x < N; ++x) {
        for (size_t y = 0; y < M; ++y) {
            if (field[x][y] == '#')
                continue;
            for (auto [dx, dy] : deltas)
            {
                auto old_v = velocity.get(x, y, dx, dy);
                auto new_v = velocity_flow.get(x, y, dx, dy);
                if (old_v > vt(0)) {//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                    //std::cout << old_v << " " << new_v << "\n";
                    //assert(vt(new_v) <= old_v);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                    velocity.get(x, y, dx, dy) = new_v;
                    //auto force = pt(old_v - vt(new_v)) * rho[(int) field[x][y]];//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                    auto force = std::max(pt(old_v - vt(new_v)) * rho[(int) field[x][y]], pt(0));
                    if (field[x][y] == '.')
                        force *= pt(0.8);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                    if (field[x + dx][y + dy] == '#') {
                        p[x][y] += force / pt(dirs[x][y]);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                    } else {
                        p[x + dx][y + dy] += force / pt(dirs[x + dx][y + dy]);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                    }
                }
            }
        }
    }
    //std::cout << 5 << "\n";
    UT += 2;
    prop = false;
    for (size_t x = 0; x < N; ++x) {
        for (size_t y = 0; y < M; ++y) {
            if (field[x][y] != '#' && last_use[x][y] != UT)
            {
                if (vt(random01()) < move_prob(x, y)) {//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                    prop = true;
                    propagate_move(x, y, true);
                } else {
                    propagate_stop(x, y, true);
                }
            }
        }
    }

    if (prop)
    {
        for (size_t x = 0; x < N; ++x) {
            std::cout << field[x] << "\n";
        }
    }
}

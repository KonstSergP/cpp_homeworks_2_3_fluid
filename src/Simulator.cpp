#include <cassert>
#include <fstream>
#include <algorithm>

#include "Simulator.h"
#include "FieldInfo.h"


void Simulator::init(const SimSetts& setts)
{
    g = setts.info.g; N = setts.info.height; M = setts.info.width;
    for (int i = 0; i < 256; i++) {rho[i] = setts.info.densities[i];}

    velocity.init(N, M);
    velocity_flow.init(N, M);
    p.init(N, M); old_p.init(N, M);
    last_use.init(N, M); dirs.init(N, M);
    field.init(N, M);

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            field[i][j] = setts.info.field[i][j];
        }
    }

    n_ticks = setts.n_ticks;
    out_name = setts.output_filename;

    directionsInit();
}



Simulator::Simulator(unsigned threadsCnt): rnd(1337), pool(threadsCnt-1) {}


void Simulator::directionsInit()
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


    // begin = std::chrono::steady_clock::now();
    // for (int x = 0; x < N; x++)
    // {
    //     pool.addTask([x, this]()
    //     {   for (size_t y = 0; y < M; ++y) {
    //         if (field[x][y] == '#')
    //             continue;
    //         for (auto [dx, dy] : deltas) {
    //             dirs[x][y] += (field[x + dx][y + dy] != '#');
    //         }
    //     }});
    // }
    // pool.waitForAll();
    // end = std::chrono::steady_clock::now();
    // auto d2 = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    // std::cout << "Time difference = " << d2 << "[µs]" << std::endl;////////////////////////////////////////////////////////////////////////

    // for (size_t x = 0; x < N; ++x) {
    //     for (size_t y = 0; y < M; ++y) {
    //         if (field[x][y] == '#')
    //             continue;
    //         for (auto [dx, dy] : deltas) {
    //             dirs[x][y] += (field[x + dx][y + dy] != '#');
    //         }
    //     }
    // }
}


void Simulator::swap_between(int x, int y, int nx, int ny)
{
    std::swap(field[x][y], field[nx][ny]);
    std::swap(p[x][y], p[nx][ny]);
    std::swap(velocity.v[x][y], velocity.v[nx][ny]);
}


Fixed Simulator::random01() {
    return Fixed::from_raw((rnd() & ((1 << 16) - 1)));
}


std::tuple<Fixed, bool, std::pair<int, int>> Simulator::propagate_flow(int x, int y, Fixed lim)
{
    last_use[x][y] = UT - 1; // изначально last_use[x][y] <= UT - 1

    Fixed ret{};
    for (auto [dx, dy] : deltas)
    {
        int nx = x + dx, ny = y + dy;
        if (field[nx][ny] != '#' && last_use[nx][ny] < UT)
        {
            auto cap = velocity.get(x, y, dx, dy);
            auto flow = velocity_flow.get(x, y, dx, dy);
            if (flow - cap == 0l) continue;
            auto vp = std::min(lim, cap - flow);
            if (vp < 0.001) continue; // если скорость слишком мала, результаты далее все равно учитываться не будут
            if (last_use[nx][ny] == UT - 1)
            {
                velocity_flow.add(x, y, dx, dy, vp);
                last_use[x][y] = UT;
                return {vp, true, {nx, ny}};
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


void Simulator::propagate_stop(int x, int y, bool force)
{
    if (!force)
    {
        bool stop = true;
        for (auto [dx, dy] : deltas)
        {
            int nx = x + dx, ny = y + dy;
            if (field[nx][ny] != '#' && last_use[nx][ny] < UT - 1 && velocity.get(x, y, dx, dy) > 0l) {
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
        if (field[nx][ny] == '#' || last_use[nx][ny] == UT || velocity.get(x, y, dx, dy) > 0l) {
            continue;
        }
        propagate_stop(nx, ny);
    }
}


Fixed Simulator::move_prob(int x, int y)
{
    Fixed sum{};
    for (auto [dx, dy] : deltas)
    {
        int nx = x + dx, ny = y + dy;
        if (field[nx][ny] == '#' || last_use[nx][ny] == UT) {
            continue;
        }
        auto v = velocity.get(x, y, dx, dy);
        if (v < 0l) {
            continue;
        }
        sum += v;
    }
    return sum;
}


bool Simulator::propagate_move(int x, int y, bool is_first)
{
    last_use[x][y] = UT - is_first;
    bool ret = false;
    int nx = -1, ny = -1;
    do {
        std::array<Fixed, deltas.size()> tres;
        Fixed sum{};
        for (size_t i = 0; i < deltas.size(); ++i)
        {
            auto [dx, dy] = deltas[i];
            int nx = x + dx, ny = y + dy;
            if (field[nx][ny] == '#' || last_use[nx][ny] == UT) {
                tres[i] = sum;
                continue;
            }
            auto v = velocity.get(x, y, dx, dy);
            if (v < 0l) {
                tres[i] = sum;
                continue;
            }
            sum += v;
            tres[i] = sum;
        }

        if (sum == 0l) {
            break;
        }

        Fixed p = random01() * sum;
        size_t d = std::ranges::upper_bound(tres, p) - tres.begin();

        auto [dx, dy] = deltas[d];
        nx = x + dx;
        ny = y + dy;
        assert(velocity.get(x, y, dx, dy) > 0l && field[nx][ny] != '#' && last_use[nx][ny] < UT);

        ret = (last_use[nx][ny] == UT - 1 || propagate_move(nx, ny, false));
    } while (!ret);

    last_use[x][y] = UT;
    for (size_t i = 0; i < deltas.size(); ++i)
    {
        auto [dx, dy] = deltas[i];
        int nx = x + dx, ny = y + dy;
        if (field[nx][ny] != '#' && last_use[nx][ny] < UT - 1 && velocity.get(x, y, dx, dy) < 0l) {
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

void Simulator::applyExtForces()
{
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
}


void Simulator::applyInnerForces()
{
    old_p = p;
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
                    if (contr * rho[field[nx][ny]] >= force)
                    {
                        contr -= force / rho[field[nx][ny]];
                        continue;
                    }
                    force -= contr * rho[field[nx][ny]];
                    contr = 0l;
                    velocity.add(x, y, dx, dy, force / rho[field[x][y]]);
                    p[x][y] -= force / dirs[x][y];
                }
            }
        }
    }
}


void Simulator::makeFlow()
{
    velocity_flow.clear();

    bool prop;
    do {
        UT += 2;
        prop = false;
        for (size_t x = 0; x < N; ++x) {
            for (size_t y = 0; y < M; ++y) {
                if (field[x][y] != '#' && last_use[x][y] != UT) {
                    auto [t, local_prop, _] = propagate_flow(x, y, 1l);
                    if (t > 0l) {
                        prop = true;
                    }
                }
            }
        }
    } while (prop);
}


void Simulator::recalcP()
{
    for (size_t x = 0; x < N; ++x) {
        for (size_t y = 0; y < M; ++y) {
            if (field[x][y] == '#')
                continue;
            for (auto [dx, dy] : deltas)
            {
                auto old_v = velocity.get(x, y, dx, dy);
                auto new_v = velocity_flow.get(x, y, dx, dy);
                if (old_v > 0l)
                {
                    assert(new_v <= old_v);
                    velocity.get(x, y, dx, dy) = new_v;
                    auto force = (old_v - new_v) * rho[field[x][y]];
                    if (field[x][y] == '.')
                        force *= 0.8;
                    if (field[x + dx][y + dy] == '#') {
                        p[x][y] += force / dirs[x][y];
                    } else {
                        p[x + dx][y + dy] += force / dirs[x + dx][y + dy];
                    }
                }
            }
        }
    }
}


bool Simulator::tryMove()
{
    UT += 2;
    bool prop = false;
    for (size_t x = 0; x < N; ++x) {
        for (size_t y = 0; y < M; ++y) {
            if (field[x][y] != '#' && last_use[x][y] != UT)
            {
                //  ###########################################################################################################################
                if (random01() < move_prob(x, y)) {
                //if (move_prob(x, y) > 0.5) {
                    // ##############################################################################################################################
                    prop = true;
                    propagate_move(x, y, true);
                } else {
                    propagate_stop(x, y, true);
                }
            }
        }
    }
    return prop;
}


void Simulator::print()
{
    for (size_t x = 0; x < N; ++x) {
        for (size_t y = 0; y < M; ++y) {
            std::cout << field[x][y];
        }
        std::cout << "\n";
    }
}



void Simulator::nextTick()
{
    applyExtForces();

    applyInnerForces();

    makeFlow();

    recalcP();

    if (tryMove()) print();

    if (!out_name.empty() && (++cur_tick == n_ticks)) {
        serialize();
        cur_tick = 0;
    }

}


void Simulator::serialize()
{
    std::ofstream out(out_name);

    auto cnt = std::count_if(rho, rho+256, [](auto i){return i!=0l;});

    out << N << " " << M << " " << g << " " << cnt << "\n";
    for (int i = 0; i < 256; i++) {
        if (rho[i] == 0l) continue;
        out << ((uint8_t)i) << " " << rho[i] << "\n";
    }

    for (int x = 0; x < N; x++)
    {
        for (int y = 0; y < M; y++)
        {
            out << field[x][y];
        }
        out << "\n";
    }
    out.close();
}

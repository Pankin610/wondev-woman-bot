#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <map>
#include <set>
#include <queue>
#include <ctime>

using namespace std;

int sz, unitsPerPlayer;
int mx_deapth = 3, mx_steps = 25;

inline bool valid(int x, int y) {
    return x >= 0 && y >= 0 && x < sz && y < sz;
}

int hash_, px[2][2], py[2][2], cur_player, deapth;
bool block[2], adj[8][8], changed[8][8];
string game_table[7], new_game_table[7];

inline string getStep(int unit, int dx, int dy, int bx, int by, bool is_push = false) {
    string res = "MOVE&BUILD ";
    if (is_push)
        res = "PUSH&BUILD ";

    res.push_back((char)('0' + unit));
    res.push_back(' ');

    if (dx == -1)
        res.push_back('N');
    if (dx == 1)
        res.push_back('S');
    if (dy == -1)
        res.push_back('W');
    if (dy == 1)
        res.push_back('E');

    res.push_back(' ');

    if (bx == -1)
        res.push_back('N');
    if (bx == 1)
        res.push_back('S');
    if (by == -1)
        res.push_back('W');
    if (by == 1)
        res.push_back('E');

    return res;
}

string best_step = "";

int state_val[5] = {1, 3, 7, 20, 0};
int c_x, c_y;

inline int EvaluateGameState() {
    int sum = 0;
    for (int p = 0; p < 2; p++) {
        for (int unit = 0; unit < 2; unit++) {
            if (px[p][unit] == -1)
                continue;
            int x = px[p][unit], y = py[p][unit];
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0)
                        continue;
                    if (!valid(x + dx, y + dy))
                        continue;
                    if (game_table[x + dx][y + dy] < '0' || game_table[x + dx][y + dy] > '3')
                        continue;
                    if (game_table[x + dx][y + dy] - game_table[x][y] > 1)
                        continue;
                    if (x + dx == px[cur_player][unit ^ 1] && y + dy == py[cur_player][unit ^ 1])
                        continue;
                    if ((x + dx == px[cur_player ^ 1][0] && y + dy == py[cur_player ^ 1][0]) ||
                        (x + dx == px[cur_player ^ 1][1] && y + dy == py[cur_player ^ 1][1]))
                        continue;

                    if (p == 0)
                        sum += state_val[game_table[x + dx][y + dy] - '0'];
                    else
                        sum -= state_val[game_table[x + dx][y + dy] - '0'];
                }
            }
        }
    }

    return sum;
}

int compute(int deapth, int alpha, int beta, int cur_score) {
    if (deapth == 0) {
        return cur_score * 15 + EvaluateGameState();
    }

    int coef = 1;
    if (cur_player == 1)
        coef = -1;

    int score = -2000000000;

    for (int unit = 0; unit < 2 && !block[cur_player]; unit++) {
        int x = px[cur_player][unit], y = py[cur_player][unit];
        if (x == -1)
            continue;
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0)
                    continue;
                if (!valid(x + dx, y + dy))
                    continue;
                if (game_table[x + dx][y + dy] < '0' || game_table[x + dx][y + dy] > '3')
                    continue;
                if (game_table[x + dx][y + dy] - game_table[x][y] > 1)
                    continue;
                if (x + dx == px[cur_player][unit ^ 1] && y + dy == py[cur_player][unit ^ 1])
                    continue;

                int nx = x + dx, ny = y + dy;

                if ((x + dx == px[cur_player ^ 1][0] && y + dy == py[cur_player ^ 1][0]) ||
                    (x + dx == px[cur_player ^ 1][1] && y + dy == py[cur_player ^ 1][1])) {

                    for (int bx = -1; bx <= 1; bx++) {
                         for (int by = -1; by <= 1; by++) {
                            if (bx == 0 && by == 0)
                                continue;
                            if ((dx == -bx && dx != 0) || (dy == -by && dy != 0))
                                continue;
                            if (dx * dy == 0 && make_pair(dx, dy) != make_pair(bx, by))
                                continue;
                            if (!valid(nx + bx, ny + by))
                                continue;
                            if (game_table[nx + bx][ny + by] < '0' || game_table[nx + bx][ny + by] > '3')
                                continue;
                            if ((nx + bx == px[cur_player ^ 1][0] && ny + by == py[cur_player ^ 1][0]) ||
                                (nx + bx == px[cur_player ^ 1][1] && ny + by == py[cur_player ^ 1][1]) ||
                                (nx + bx == px[cur_player][unit ^ 1] && ny + by == py[cur_player][unit ^ 1]))
                                continue;
                            if (game_table[nx + bx][ny + by] - game_table[nx][ny] > 1)
                                continue;

                            int e_unit = 0;
                            if (x + dx == px[cur_player ^ 1][1] && y + dy == py[cur_player ^ 1][1])
                                e_unit = 1;

                            px[cur_player ^ 1][e_unit] += bx;
                            py[cur_player ^ 1][e_unit] += by;
                            game_table[nx][ny] += 1;
                            cur_player ^= 1;

                            int new_score = compute(deapth - 1, alpha, beta, cur_score);
                            if (new_score * coef > score) {
                                score = new_score * coef;
                                if (deapth == mx_deapth) {
                                    best_step = getStep(unit, dx, dy, bx, by, true);
                                    c_x = x + dx;
                                    c_y = y + dy;
                                }
                                if (cur_player == 0)
                                    alpha = max(alpha, new_score);
                                else
                                    beta = min(beta, new_score);
                            }

                            cur_player ^= 1;
                            px[cur_player ^ 1][e_unit] -= bx;
                            py[cur_player ^ 1][e_unit] -= by;
                            game_table[nx][ny] -= 1;
                        }
                    }

                    continue;
                }

                for (int bx = -1; bx <= 1; bx++) {
                    for (int by = -1; by <= 1; by++) {
                        if (bx == 0 && by == 0)
                            continue;
                        if (!valid(nx + bx, ny + by))
                            continue;
                        if (game_table[nx + bx][ny + by] < '0' || game_table[nx + bx][ny + by] > '3')
                            continue;
                        if ((nx + bx == px[cur_player ^ 1][0] && ny + by == py[cur_player ^ 1][0]) ||
                            (nx + bx == px[cur_player ^ 1][1] && ny + by == py[cur_player ^ 1][1]) ||
                            (nx + bx == px[cur_player][unit ^ 1] && ny + by == py[cur_player][unit ^ 1]))
                            continue;

                        px[cur_player][unit] += dx;
                        py[cur_player][unit] += dy;
                        game_table[nx + bx][ny + by] += 1;
                        cur_player ^= 1;

                        int ad = (game_table[px[cur_player ^ 1][unit]][py[cur_player ^ 1][unit]] == '3') * coef;

                        int new_score = compute(deapth - 1, alpha, beta, cur_score + ad);
                        if (new_score * coef > score) {
                            score = new_score * coef;
                            if (deapth == mx_deapth) {
                                //cerr << dx << " " << dy << " " << bx << " " << by << " " << getStep(unit, dx, dy, bx, by) << endl;
                                best_step = getStep(unit, dx, dy, bx, by);
                                c_x = nx + bx;
                                c_y = ny + by;
                            }
                            if (cur_player == 0)
                                alpha = max(alpha, new_score);
                            else
                                beta = min(beta, new_score);
                        }

                        cur_player ^= 1;
                        px[cur_player][unit] -= dx;
                        py[cur_player][unit] -= dy;
                        game_table[nx + bx][ny + by] -= 1;
                    }
                }
            }
        }
    }

    if (score == -2000000000) {
        bool ov = block[cur_player];
        block[cur_player] = true;
        cur_player ^= 1;
        int new_score = compute(deapth - 1, alpha, beta, cur_score);
        if (new_score * coef > score) {
            score = new_score * coef;
        }

        cur_player ^= 1;
        block[cur_player] = ov;
    }
    return score * coef;
}

double cur_limit = 0.008;
clock_t start_time;
inline bool tooLate() {
    return (double)(clock() - start_time) / CLOCKS_PER_SEC > cur_limit;
}

int main()
{
    cin >> sz;
    cin >> unitsPerPlayer;

    bool fst = true;

    // game loop
    while (1) {
        for (int i = 0; i < sz; i++) {
            string o_row = game_table[i];
            cin >> game_table[i];
            for (int j = 0; j < sz; j++) {
                if (game_table[i][j] == '.')
                    game_table[i][j] = '4';
            }
            if (!fst) {
                for (int j = 0; j < sz; j++) {
                    changed[i][j] = (o_row[j] != game_table[i][j]);
                    adj[i][j] = false;
                }
            }
        }
        for (int p = 0; p < 2; p++) {
            for (int i = 0; i < unitsPerPlayer; i++) {
                cin >> py[p][i] >> px[p][i];
            }
        }

        int legalActions;
        cin >> legalActions; cin.ignore();
        string act = "i'm out";
        for (int i = 0; i < legalActions; i++) {
            string atype;
            string index;
            string dir1;
            string dir2;
            cin >> atype >> index >> dir1 >> dir2; cin.ignore();
            act = atype + index + dir1 + dir2;
        }
        best_step = "";
        block[0] = block[1] = false;
        cur_player = 0;

        for (int unit = 0; unit < 2; unit++) {
            int x = px[0][unit],
                y = py[0][unit];
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (!valid(x + dx, y + dy))
                        continue;
                    adj[x + dx][y + dy] = true;
                }
            }
        }

        vector< pair<int, int> > choices;

        for (int i = 0; i < 2; i++) {
            if (px[1][i] != -1)
                continue;
            vector< pair<int, int> > options;

            for (int x = 0; x < sz; x++) {
                for (int y = 0; y < sz; y++) {

                    if (game_table[x][y] < '0' || game_table[x][y] > '3' || adj[x][y])
                        continue;
                    bool good = true;
                    for (int p = 0; p < 2; p++)
                        for (int unit = 0; unit < 2; unit++)
                            if (px[p][unit] == x && py[p][unit] == y)
                                good = false;
                    if (!good)
                        continue;
                    good = false;
                    for (int dx = -1; dx <= 1; dx++) {
                        for (int dy = -1; dy <= 1; dy++) {
                            if (!valid(x + dx, y + dy))
                                continue;
                            if (changed[x + dx][y + dy])
                                good = true;
                        }
                    }
                    if (good)
                        options.push_back(make_pair(x, y));
                }
            }

            if (options.empty())
                continue;
            random_shuffle(options.begin(), options.end());
            px[1][i] = options.back().first;
            py[1][i] = options.back().second;
            choices.push_back(options.back());
        }

        if (!fst)
            cur_limit = 0.001;
        start_time = clock();

        for (mx_deapth = 2; !tooLate(); mx_deapth++) {
            compute(mx_deapth, -2000000000, 2000000000, 0);
        }

        game_table[c_x][c_y] += 1;

        if (best_step == "")
            best_step = act;
        cout << best_step << " " << mx_deapth - 1;
        for (int i = 0; i < choices.size(); i++)
            cout << " " << "predict " << " " << choices[i].second << " " << choices[i].first;
        cout << endl;
        fst = false;
    }
}

/*
7 2
...0...
..000..
.00100.
0000001
.00000.
..000..
...0...
1 2
5 2
2 2
3 3
0


*/

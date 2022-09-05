#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stack>

using namespace std;

// get the length of max squared area right cornered at [i,j]
int task1_dmax(vector<vector<int>> &dmax, int i, int j, int h) {
    if (dmax[i][j] >= 0) {  // for decided place, return the table memory to avoid duplicate calculation
        return dmax[i][j];
    } else {    // apply memoization idea
        return min(min(task1_dmax(dmax, i - 1, j, h), task1_dmax(dmax, i, j - 1, h)),
                   task1_dmax(dmax, i - 1, j - 1, h)) + 1;
    }
}

int task1_memoization(vector<vector<int>> p, int m, int n, int h) {
    // get the max squared area
    vector<vector<int>> dmax(m + 1, vector<int>(n + 1));
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0 || j == 0) {
                dmax[i][j] = 0; // set the value in addition row and col to zero
            } else {
                dmax[i][j] = -1; // -1 means cell(i,j) has not been calculated
            }
        }
    }
    int dmaxnum = 0;
    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (p[i - 1][j - 1] >= h) {
                dmax[i][j] = task1_dmax(dmax, i, j, h);
            } else {
                dmax[i][j] = 0;
            }
            if (dmaxnum <= dmax[i][j]) {
                dmaxnum = dmax[i][j];
                // locate the area position
                x2 = i;
                y2 = j;
                x1 = i + 1 - dmaxnum;
                y1 = j + 1 - dmaxnum;
            }
        }
    }
    int maxarea = pow(dmaxnum, 2);
    cout << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
    return maxarea;

}


int task2_bottomup(vector<vector<int>> p, int m, int n, int h) {
    int dmaxnum = 0;
    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    vector<vector<int>> dmax(2, vector<int>(n + 1));
    bool bi;
    // the bottom up approach
    for (int i = 0; i <= m; i++) {
        bi = i & 1;
        for (int j = 0; j <= n; j++) {
            if (i == 0 || j == 0) {
                dmax[bi][j] = 0;
            } else if (p[i - 1][j - 1] < h) {
                dmax[bi][j] = 0;
            } else {
                dmax[bi][j] = min(min(dmax[1 - bi][j], dmax[bi][j - 1]), dmax[1 - bi][j - 1]) + 1;
            }
            if (dmax[bi][j] >= dmaxnum) {
                dmaxnum = dmax[bi][j];
                // locate the area position
                x2 = i;
                y2 = j;
                x1 = i + 1 - dmaxnum;
                y1 = j + 1 - dmaxnum;
            }
        }
    }
    int maxarea = pow(dmaxnum, 2);
    cout << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
    return maxarea;
}

int task3_bruteforce(vector<vector<int>> p, int m, int n, int h) {
    int maxarea = 0;
    int currentarea;
    int x1 = -1, y1 = -1, x2 = -1, y2 = -1;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            for (int a = 0; a < m - i; a++) {
                for (int b = 0; b < n - j; b++) {
                    int numx, numy;
                    for (int x = i; x <= i + a; x++) {
                        numx = x;
                        for (int y = j; y <= j + b; y++) {
                            numy = y;
                            if (p[x][y] < h) { break; }
                        }
                        if (p[x][numy] < h) { break; }
                    }
                    if (p[numx][numy] < h) {
                        currentarea = 0;
                    } else { currentarea = (a + 1) * (b + 1); }
                    if (currentarea >= maxarea) {
                        maxarea = currentarea;
                        x1 = i + 1;
                        y1 = j + 1;
                        x2 = i + a + 1;
                        y2 = j + b + 1;
                    }
                }
            }
        }
    }
    cout << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
    return maxarea;
}

int task45_hist_maxarea(vector<int> &height, int &y2c, int &optheight, int &optwidth) {
    int hist_ma = 0, curr_hist = 0; // maxarea and currentarea for a histogram array height[]
    stack<int> s; //creat an empty stack to store rectangle's starting index and help record the width of rectangle
    height.push_back(0); //add one more value to help all rectangle area calc stop at last element
    for (int i = 0; i < height.size(); ++i) {
        if (s.empty() || height[s.top()] <= height[i]) {
            s.push(i); // push index of new rectangle's starting line to stack
        } else { // when current height goes down, finish a rectangle and calculate its hist_maxarea
            int tmp = s.top(); //tmp is the height of rectangle used in following area calculation
            s.pop();
            curr_hist = height[tmp] * (s.empty() ? i : (i - s.top() - 1)); //calc curr rectangle's area
            // record the max area and position info
            if (hist_ma <= curr_hist) {
                hist_ma = curr_hist;
                y2c = i;
                optheight = height[tmp];
                optwidth = (s.empty() ? i : (i - s.top() - 1));
            }
            --i;
        }
    }
    return hist_ma;
}


int task4_smart(vector<vector<int>> p, int m, int n, int h) {
    int maxarea = 0, currarea = 0, optheight = 0, optwidth = 0;
    int x1 = -1, y1 = -1, x2 = -1, y2 = -1, y2c = -1;
    vector<vector<int>> height(m, vector<int>(n));
    // bottom up for loops
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == 0) {
                height[i][j] = p[i][j] < h ? 0 : 1;
            } else {
                height[i][j] = p[i][j] < h ? 0 : (height[i - 1][j] + 1);
            }
            currarea = task45_hist_maxarea(height[i], y2c, optheight, optwidth);
            if (maxarea <= currarea) {
                maxarea = currarea;
                x2 = i + 1;
                x1 = i - optheight + 2;
                y2 = y2c;
                y1 = y2 - optwidth + 1;
            }
        }
    }
    cout << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
    return maxarea;
}


int task5_smart(vector<vector<int>> p, int m, int n, int h) {
    int maxarea = 0, currarea = 0, optheight = 0, optwidth = 0;
    int x1 = -1, y1 = -1, x2 = -1, y2 = -1, y2c = -1;
    vector<int> height(n); // instead of using a 2d vector, use an array to save memory
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            height[j] = p[i][j] < h ? 0 : (height[j] + 1);
        }
        currarea = task45_hist_maxarea(height, y2c, optheight, optwidth);
        if (maxarea <= currarea) {
            maxarea = currarea;
            x2 = i + 1;
            x1 = i - optheight + 2;
            y2 = y2c;
            y1 = y2 - optwidth + 1;
        }
    }
    cout << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
    return maxarea;
}


int main(int argc, char **argv) {
    int m, n, h;
    cin >> m >> n >> h;
    vector<vector<int>> p(m, vector<int>(n));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cin >> p[i][j];
        }
    }
    int cmd = stoi(argv[1]);
    switch (cmd) {
        case 1:
            task1_memoization(p, m, n, h);
            break;
        case 2:
            task2_bottomup(p, m, n, h);
            break;
        case 3:
            task3_bruteforce(p, m, n, h);
            break;
        case 4:
            task4_smart(p, m, n, h);
            break;
        case 5:
            task5_smart(p, m, n, h);
            break;
        default:
            break;
    }

}
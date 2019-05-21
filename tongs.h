//
// Created by kezhilin on 18-6-12.
//

#ifndef MATCH_TONGS_H
#define MATCH_TONGS_H

#include <vector>

template<typename T>
void constraint(T &a, T min, T max) {
    if (a < min) a = min;
    else if (a > max) a = max;
}

template<typename ElemType, typename CoordType>
class _TongsElement {
public:
    CoordType x, y;
    ElemType elem;

    _TongsElement(CoordType _x, CoordType _y, ElemType _e) : x(_x), y(_y), elem(_e) {}
};

template<typename ElemType, typename CoordType>
class Tongs {
public:
    Tongs() = default;

    Tongs(int image_rows, int image_cols, int tong_h, int tong_w) {
        init(image_rows, image_cols, tong_h, tong_w);
    }

    void init(int image_rows, int image_cols, int tong_h, int tong_w) {
        _tong_w = tong_w;
        _tong_h = tong_h;
        _tongs_cols = (image_cols / tong_w + 1);
        _tongs_rows = (image_rows / tong_h + 1);
        if (_tongs.size() == _tongs_cols * _tongs_rows) {
            for (auto &item:_tongs) {
                item.clear();
            }
        } else {
            _tongs.clear();
            _tongs.resize(_tongs_cols * _tongs_rows);
        }
    }

    void addDot(CoordType x, CoordType y, ElemType e,int& count) {
        int tong_x = x / _tong_w;
        int tong_y = y / _tong_h;
        if (tong_y >= 0 && tong_y < _tongs_rows && tong_x < _tongs_cols && tong_x >= 0) {
            _tongs[tong_y * _tongs_cols + tong_x].push_back({x, y, e});
            count += 1;
        }
    }

    void getDots(CoordType x, CoordType y, CoordType dx, CoordType dy, std::vector<std::vector<ElemType>> &dots) const {
        dots.clear();

        CoordType x_left = x - dx;
        CoordType x_right = x + dx;
        CoordType y_top = y - dy;
        CoordType y_bottom = y + dy;
        int tong_x_begin = x_left / _tong_w;
        int tong_x_end = x_right / _tong_w + 1;
        int tong_y_begin = y_top / _tong_h;
        int tong_y_end = y_bottom / _tong_h + 1;
        if (tong_x_begin < 0) tong_x_begin = 0;
        if (tong_y_begin < 0) tong_y_begin = 0;
        if (tong_y_end > _tongs_rows) tong_y_end = _tongs_rows;
        if (tong_x_end > _tongs_cols) tong_x_end = _tongs_cols;

        for (int i = tong_y_begin; i < tong_y_end; i++)
            for (int j = tong_x_begin; j < tong_x_end; j++)
                for (int s = 0; s < _tongs[i * (_tongs_cols) + j].size(); s++) {
                    auto p = _tongs[i * (_tongs_cols) + j][s];
                    if (p.x >= x_left && p.x <= x_right && p.y >= y_top && p.y <= y_bottom)
                       {
                           std::vector<ElemType> dot;
                           dot.resize(3);
                           dot[0] = p.x;
                           dot[1] = p.y;
                           dot[2] = p.elem;
                           dots.push_back(dot);
                       }
                }
    }

    int
    search_closest(CoordType x, CoordType y, CoordType dx, CoordType dy, float max_search_dis, ElemType &dot) const {

        CoordType x_left = x - dx;
        CoordType x_right = x + dx;
        CoordType y_top = y - dy;
        CoordType y_bottom = y + dy;
        int tong_x_begin = x_left / _tong_w;
        int tong_x_end = x_right / _tong_w + 1;
        int tong_y_begin = y_top / _tong_h;
        int tong_y_end = y_bottom / _tong_h + 1;
        if (tong_x_begin < 0) tong_x_begin = 0;
        if (tong_y_begin < 0) tong_y_begin = 0;
        if (tong_y_end > _tongs_rows) tong_y_end = _tongs_rows;
        if (tong_x_end > _tongs_cols) tong_x_end = _tongs_cols;

        float min_dis = max_search_dis;

        for (int i = tong_y_begin; i < tong_y_end; i++)
            for (int j = tong_x_begin; j < tong_x_end; j++)
                for (int s = 0; s < _tongs[i * (_tongs_cols) + j].size(); s++) {
                    auto p = _tongs[i * (_tongs_cols) + j][s];
                    float dis = sqrt((p.x - x) * (p.x - x) + (p.y - y) * (p.y - y));
                    if(dis == min_dis && p.elem > dot)
                    {
                        dot = p.elem;
                        continue;
                    }
                    if (dis < min_dis) {
                        dot = p.elem;
                        min_dis = dis;
                    }
                   
                }

        if (min_dis == max_search_dis)return 0;
        else return 1;
    }

private:
    int _tong_h;
    int _tong_w;
    int _tongs_cols;
    int _tongs_rows;
    std::vector<std::vector<_TongsElement<ElemType, CoordType >>> _tongs;
};

#endif //MATCH_TONGS_H

//
// Created by lhx on 19-5-5.
//

#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <algorithm>
#include <string>
#include <assert.h>
#include "utils.h"
#include "tongs.h"
#include <unistd.h>
#include <thread>
#include <mutex>

struct dis_element
{
    double dis;
    double z;
};
bool SortByM1( const dis_element &v1, const dis_element &v2)
{
    return v1.dis < v2.dis;
}

void vertical_project(std::vector<std::vector<float>> pc, std::string save_path, int tong_h = 10, int tong_w = 6, int Height = 192, int Width = 128, int dx = 6, int dy = 6, int neighbor = 4){
    //
    // crop points in a proper size and put them in a tong
    //
    Tongs<float, float> tongs(Height, Width, tong_h, tong_w);
    int sx = 50, sy = 200, count = 0;
    for(auto point: pc){
        point[0] = clip(point[0] - sx, 0, Width - 1);
        point[1] = clip(-point[1] + sy, 0, Height - 1);
        tongs.addDot(point[0], point[1], point[2], count);
    }


    //
    // for each h and w, search its neighbors and use the weighted depth to
    // generate a depth map
    //
    std::ofstream out(save_path);
    float img[Height][Width];
    for(int h = 0; h < Height; h++){
        for(int w = 0; w < Width; w++){
            std::vector<std::vector<float>> dots;
            tongs.getDots(w, h, dx, dy, dots);

            if((int)dots.size() != 0){
                std::vector<dis_element> dis_v;

                for(int i = 0; i < (int)dots.size(); i++){
                    float dis = distance(dots[i][0], dots[i][1], w, h);
                    dis_element dis_e;
                    dis_e.dis = dis;
                    dis_e.z = dots[i][2];
                    dis_v.push_back(dis_e);
                }
                sort(dis_v.begin(), dis_v.end(), SortByM1);

                float sum = 0, sum_factor = 0;
                for(int i = 0; i < std::min((int)dis_v.size(), neighbor); i++) {
                    float factor = dx * dx + dy * dy + 0.1 - dis_v[i].dis;
                    sum += (dis_v[i].z * factor);
                    sum_factor += factor;
                }
                img[h][w] = sum / sum_factor;
            }
            else img[h][w] = 0;
            //
            // write to the save path
            //
            out << h << " " << w << " " << img[h][w] << std::endl;
        }
    }
    out.close();
}

void interpolation(const std::vector<std::string> files, std::string input_dir, std::string save_dir, int start,int end) {
    for (int i = start; i < end; i++) {
        clock_t tic, toc;
        tic = clock();

/*        std::string save_path = save_dir + files[i].substr(0, files[i].length() - 4) + ".dat";*/
        std::string save_path = save_dir + files[i];
        std::string input_path = input_dir + files[i];
        //
        // not exists files; continue
        //
        if (access(save_path.c_str(), 0) == 0) {
            std::cout << save_path << "is exist!" << std::endl;
            continue;
        }

        std::ifstream in(input_path);
        std::vector<std::vector<float>> pc;
        for (int j = 0; !in.eof(); j++) {
            float x, y, z;
            in >> x >> y >> z;
            std::vector<float> point;
            point.resize(3);
            point[0] = x;
            point[1] = y;
            point[2] = z;
            pc.push_back(point);
        }
        in.close();

        clock_t t2 = clock();
        vertical_project(pc, save_path);
        toc = clock();

        std::cout << "Save " << save_path << ". Time: " << float(toc - tic) / CLOCKS_PER_SEC  << "s" << std::endl;
    }
}
int main(){
    clock_t start, end;
    start = clock();

    std::string input_dir = "/home/lhx/ND_BOS_uniform/ND_BOS_4096_POINT/ND_4096_Uniform_txt/";
    std::string save_dir = "/home/lhx/ND_BOS_uniform/ND_BOS_4096_POINT/depth_map/";
    std::vector<std::string> files = listdir(input_dir);

    int max_thread = 10;
    int file_size = files.size();
    int file_avg = file_size/max_thread;
    std::thread t[max_thread];

    for(int i = 0;i < max_thread;i++)
    {
        if(i == max_thread  - 1)
            t[i] = std::thread(interpolation, files, input_dir, save_dir, file_avg * i, file_size);
        else
            t[i] = std::thread(interpolation, files, input_dir , save_dir ,file_avg * i, file_avg * (i + 1));
    }
    for (int i = 0;i < max_thread; i++)
        t[i].join();

    end = clock();
    std::cout << "All Time is: " << double(end - start) / CLOCKS_PER_SEC << "s" << std::endl;
    return 0;

}
//
// Created by lhx on 19-5-5.
//
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <assert.h>
#include <math.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <map>
#include <sstream>

int clip(float x, int lower, int upper)
{
    if( x < lower)
        x = lower;

    if( x > upper)
        x = upper;

    return x;

}

float distance(int x, int y, int x1, int y1)
{
    return (x - x1) * ( x - x1) + (y - y1) * (y - y1);
}

bool judge(const std::pair<float, float> a, const std::pair<float, float> b) {
    return a.first < b.first;
}

void make_new_dir(const std::string& path)
{
    int _AccessMode = 0;
    if(access(path.c_str(),_AccessMode) == 0)
    {
         std::cout<<"The files is exist!"<< std::endl;
    }
    int isCreate = mkdir(path.c_str(),S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
    if(isCreate==0)
    {
        std::cout<<"Successful mkdir the folder: "<< path << std::endl;
    }
}

std::vector<std::string> split(std::string& s, std::string& seperator)
{
    std::vector<std::string> result;
    int i = 0;
    while(i != s.length()){
        int flag = 0;
        while(i != s.length() && flag == 0)
        {
            flag = 1;
            for(int x = 0; x < seperator.length(); ++x)
            {
                if(s[i] == seperator[x]){
                    ++i;
                    flag = 0;
                    break;
                }
            }
        }
        flag = 0;
        int j = i;
        while(j != s.length() && flag == 0)
        {
            for(int x = 0; x<seperator.length(); ++x)
                if(s[j] == seperator[x]){
                    flag = 1;
                    break;
                }
            if(flag == 0)
                ++j;
        }
        if(i != j){
            result.push_back(s.substr(i, j-i));
            i = j;
        }
    }
    return result;
}

std::vector<std::string> listdir(std::string cate_dir)
{
    std::vector<std::string> files;
    DIR *dir;
    struct dirent *ptr;
    char base[1000];

    if ((dir=opendir(cate_dir.c_str())) == NULL)
    {
        perror("Open dir error...");
        exit(1);
    }

    while ((ptr=readdir(dir)) != NULL)
    {
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
            continue;
        else if(ptr->d_type == 8)    ///file
            //printf("d_name:%s/%s\n",basePath,ptr->d_name);
            files.push_back(ptr->d_name);
        else if(ptr->d_type == 10)    ///link file
            //printf("d_name:%s/%s\n",basePath,ptr->d_name);
            continue;
        else if(ptr->d_type == 4)    ///dir
        {
            files.push_back(ptr->d_name);
            /*
                memset(base,'\0',sizeof(base));
                strcpy(base,basePath);
                strcat(base,"/");
                strcat(base,ptr->d_nSame);
                readFileList(base);
            */
        }
    }
    closedir(dir);
    sort(files.begin(), files.end());
    return files;
}
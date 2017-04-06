#pragma once
#include "geom.h"

class Scene {
    Scene(){

    }

    vector<pair<ThreeDVector, RGBColor>> process(){
        for(unsigned int i; i < pxx; i++){
            for(unsigned int j; j < pxy; j++){
                ThreeDVector screen_point = ulcorner + xstep * (float)i + ystep * (float)j;
                Ray ray(eye, screen_point - eye);
                ThreeDVector nearest(100000.0, 100000.0, 100000.0);
                bool found = false;
                RGBColor nearest_color;
                for(unsigned int k = 0; k < objects.size(); k++){
                    auto res = objects[k].ray_intersect(ray);
                    if(res.first){
                        found = true;
                        auto inter_pnt = res.second;
                        if((nearest - eye).len() > (inter_pnt - eye).len()){
                            nearest_color = objects[k].color;
                            nearest = inter_pnt;
                        }
                    }
                }
                if(found)
                    ans.push_back({nearest, nearest_color});
            }
        }
    }

	vector<GeomObj> objects;
    ThreeDVector ulcorner;
    ThreeDVector xstep, ystep;
    ThreeDVector eye;
    unsigned int pxx, pxy;
};

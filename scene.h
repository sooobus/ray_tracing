#pragma once
#include "geom.h"

class Scene {
    Scene(vector<GeomObj> objects_, ThreeDVector ulcorner_, ThreeDVector urcorner_,
          ThreeDVector dlcorner_, ThreeDVector eye_, size_t pxx_, size_t pxy_):objects(objects_),
          ulcorner(ulcorner_), eye(eye_), pxx(pxx_), pxy(pxy_){
        xstep = (urcorner_ - ulcorner_) * (1 / (double)pxx_);
        ystep = (dlcorner_ - ulcorner_) * (1 / (double)pxy_);
    }

    vector<pair<ThreeDVector, RGBColor>> process(){
        vector<pair<ThreeDVector, RGBColor>> ans;
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
                    ans.push_back({ThreeDVector(i, j, 0.0), nearest_color});
            }
        }
    }

	vector<GeomObj> objects;
    ThreeDVector ulcorner;
    ThreeDVector xstep, ystep;
    ThreeDVector eye;
    size_t pxx, pxy;
};

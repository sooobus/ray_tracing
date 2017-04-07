#pragma once
#include "geom.h"
#include "light.h"
#include <fstream>
#include <iostream>

class Scene {
public:
    Scene(vector<GeomObj*> objects_, vector<PointLightSource*> lights_, ThreeDVector ulcorner_, ThreeDVector urcorner_,
          ThreeDVector dlcorner_, ThreeDVector eye_, size_t pxx_, size_t pxy_): objects(objects_), lights(lights_),
          pxx(pxx_), pxy(pxy_), eye(eye_), ulcorner(ulcorner_){
        xstep = (urcorner_ - ulcorner_) * (1 / (double)pxx_);
        ystep = (dlcorner_ - ulcorner_) * (1 / (double)pxy_);
    }

    vector<pair<ThreeDVector, RGBColor>> process(){
        ofstream log;
        log.open("log.txt");
        vector<pair<ThreeDVector, RGBColor>> ans;
        ans.push_back({ThreeDVector(pxx, pxy, 0), RGBColor(0, 0, 0)});
        for(size_t i = 0; i < pxx; i++){
            for(size_t j = 0; j < pxy; j++){
                //ans.push_back({ThreeDVector(i, j, 0.0), RGBColor(0, 0, 0)});
                ThreeDVector screen_point = ulcorner + xstep * (float)i + ystep * (float)j;
                Ray ray(eye, screen_point - eye);
                ThreeDVector nearest(100000.0, 100000.0, 100000.0);
                auto nearest_obj = objects[0];
                bool found = false;
                RGBColor nearest_color(0, 0, 0);
                for(unsigned int k = 0; k < objects.size(); k++){
                    auto res = objects[k]->ray_intersect(ray);
                    if(res.first){
                        //ans.push_back({ThreeDVector(i, j, 0.0), RGBColor(0, 0, 0)});
                        found = true;
                        auto inter_pnt = res.second;
                        if((nearest - eye).len() > (inter_pnt - eye).len()){
                            nearest_color = objects[k]->color;
                            nearest = inter_pnt;
                            nearest_obj = objects[k];
                        }
                    }
                    else{
                        ans.push_back({ThreeDVector(res.second.x * 77, res.second.y * 77, 0.0), RGBColor(0, 0, 0)});
                    }
                }
                if(found){ //подкорректируем цвет
                    for(unsigned int l = 0; l < lights.size(); l++){
                        auto light = lights[l];
                        auto ray = light->pos - nearest;
                        bool accessible = true;
                        for(unsigned int o = 0; o < objects.size(); o++){
                            if((objects[o]->ray_intersect(Ray(nearest, ray))).first){
                                accessible = false;
                                break;
                            }
                        }
                        if(accessible){
                            std::cout << 100 * (light->power / (ray.len() * ray.len())) * fabs(cosa(nearest_obj->normal(nearest), ray)) << endl;
                            nearest_color.add(100 * (light->power / (ray.len() * ray.len())) * fabs(cosa(nearest_obj->normal(nearest), ray)));
                        }
                    }

                    ans.push_back({ThreeDVector(i, j, 0.0), nearest_color});
                }
            }
        }
        log.close();
        return ans;
    }

    vector<GeomObj*> objects;
    vector<PointLightSource*> lights;
    ThreeDVector ulcorner;
    ThreeDVector xstep, ystep;
    ThreeDVector eye;
    size_t pxx, pxy;
};

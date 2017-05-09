#pragma once
#include "geom.h"
#include "light.h"
#include <fstream>
#include <iostream>
#include <algorithm>

class Scene {
public:
    Scene(vector<GeomObj*> objects_, vector<PointLightSource*> lights_, ThreeDVector ulcorner_, ThreeDVector urcorner_,
          ThreeDVector dlcorner_, ThreeDVector eye_, size_t pxx_, size_t pxy_): objects(objects_), lights(lights_),
          pxx(pxx_), pxy(pxy_), eye(eye_), ulcorner(ulcorner_){
        xstep = (urcorner_ - ulcorner_) * (1 / (long double)pxx_);
        ystep = (dlcorner_ - ulcorner_) * (1 / (long double)pxy_);
        auto xl = xstep.len();
        auto yl = ystep.len();
        cout << "xl = " << xl << "yl = " << yl << endl;
        xstep.show();
        ystep.show();
        if(xl < yl){
            ystep.make_of_len(xl);
        }
        else{
            xstep.make_of_len(yl);
        }
        xstep.show();
        ystep.show();
        //xstep = v_min(xstep, ystep);
        //ystep = xstep;
    }

    vector<pair<ThreeDVector, RGBColor>> process(){
        ofstream log;
        log.open("log.txt");
        vector<pair<ThreeDVector, RGBColor>> ans;
        ans.push_back({ThreeDVector(pxx, pxy, 0), RGBColor(0, 0, 0)});
        for(size_t i = 0; i < pxx; i++){
            for(size_t j = 0; j < pxy; j++){
                //ans.push_back({ThreeDVector(i, j, 0.0), RGBColor(0, 0, 0)});
                ThreeDVector screen_point = ulcorner + xstep * (long double)i + ystep * (long double)j;
                auto dir = screen_point - eye;
                dir.normalize();
                Ray ray(eye, dir);
                ThreeDVector nearest(100000.0, 100000.0, 100000.0);
                auto nearest_obj = objects[0];
                bool found = false;
                RGBColor nearest_color(0, 0, 0);
                RGBColor res_color(0, 0, 0);
                for(unsigned int k = 0; k < objects.size(); k++){
                    auto res = objects[k]->ray_intersect(ray);
                    if(res.first){
                        found = true;
                        auto inter_pnt = res.second;
                        if((nearest - eye).len() > (inter_pnt - eye).len()){
                            nearest_color = objects[k]->color;
                            nearest = inter_pnt;
                            nearest_obj = objects[k];
                        }
                    }
                }
                //ans.push_back({ThreeDVector(i, j, 0.0), nearest_color});

                if(found){ //подкорректируем цвет

                    for(unsigned int l = 0; l < lights.size(); l++){
                        auto light = lights[l];
                        auto ray = light->pos - nearest;
                        bool accessible = true;
                        for(unsigned int o = 0; o < objects.size(); o++){
                            if((objects[o]->ray_intersect(Ray(light->pos, ray))).first){
                                cout << "NA" << endl;
                                accessible = false;
                                break;
                            }
                        }
                        if(accessible){
                            auto norm = nearest_obj->normal(nearest);
                            //std::cout << (light->power / (ray.len() * ray.len())) *
                                         //fabs(cosa(norm, ray))/100.0 << endl;
                            res_color + RGBColor(nearest_color, (light->power / (ray.len() * ray.len())) * fabs(cosa(norm, ray))/100.0);
                        }
                    }

                }
                ans.push_back({ThreeDVector(i, j, 0.0), res_color});
            }
        }
        log.close();
        return ans;
    }

    vector<GeomObj*> objects;
    vector<PointLightSource*> lights;
    size_t pxx, pxy;
    ThreeDVector eye;
    ThreeDVector ulcorner;
    ThreeDVector xstep, ystep;
};

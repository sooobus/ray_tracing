#pragma once
#include "geom.h"
#include "light.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <thread>
#include <mutex>

static const int tdim1 = 2, tdim2 = 2;

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

    void info(){
        cout << "Objects: " << objects.size() << endl;
        for(int i = 0; i < objects.size(); i++){
            auto c = objects[i]->color;
            cout << c.r << " " << c.g << " " << c.b << endl;
        }
        cout << "Lights: " << lights.size() << endl;
        for(int i = 0; i < lights.size(); i++){
            cout << lights[i]->power << endl;
            auto p = lights[i]->pos;
            cout << p.x << " " << p.y << " " << p.z << endl;
        }
    }

    pair<int, ThreeDVector> nearest_object_index(size_t index, ThreeDVector ray, ThreeDVector origin){
        unsigned int nearest = objects.size();
        ThreeDVector nearest_pnt(1000000, 1000000, 100000);
        for(unsigned int o = 0; o < objects.size(); o++){
            if(o != index){
                auto inter = objects[o]->ray_intersect(Ray(origin, ray));
                if(inter.first && ((nearest_pnt - origin).len() > (inter.second - origin).len() || nearest == objects.size())){
                    nearest = o;
                    nearest_pnt = inter.second;
                }
            }
        }
        return {nearest, nearest_pnt};
    }

    RGBColor real_color(size_t index, ThreeDVector point){
        RGBColor res_color(0, 0, 0);
        auto obj = objects[index];
        for(unsigned int l = 0; l < lights.size(); l++){
            auto light = lights[l];
            auto ray = point - light->pos; //!!!
            //ray.normalize();
            bool accessible = true;
            // Надо проверить, что наш объект -- ближайший от лампочки

            unsigned int nearest_to_candle_i = index;
            auto nearest_to_candle_pnt = point;
            for(unsigned int o = 0; o < objects.size(); o++){
                auto inter = objects[o]->ray_intersect(Ray(light->pos, ray));
                if(inter.first && (nearest_to_candle_pnt - light->pos).len() > (inter.second - light->pos).len()){
                        nearest_to_candle_i = o;
                        nearest_to_candle_pnt = inter.second;

                    }
                if(nearest_to_candle_i != index)
                    accessible = false;

            }
            if(accessible){
                auto norm = obj->normal(point);
                //std::cout << (light->power / (ray.len() * ray.len())) * fabs(cosa(norm, ray)) << endl;
                //res_color + RGBColor(nearest_color, (light->power / (ray.len() * ray.len())) * fabs(cosa(norm, ray))/100.0);
                res_color + RGBColor(obj->color, (light->power / (ray.len() * ray.len())) * fabs(cosa(norm, ray)));
            }
        }
        return res_color;
    }

    void thread_routine(size_t begin_i, size_t end_i, size_t begin_j, size_t end_j){
        for(size_t i = begin_i; i < end_i; i++){
            for(size_t j = begin_j; j < end_j; j++){
                //cout << i << " " << j << endl;
                ThreeDVector screen_point = ulcorner + xstep * (long double)i + ystep * (long double)j;
                auto dir = screen_point - eye;
                dir.normalize();
                Ray ray(eye, dir);
                ThreeDVector nearest(100000.0, 100000.0, 100000.0);
                auto nearest_obj = objects[0];
                unsigned int nearest_ind;
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
                            nearest_ind = k;
                        }
                    }
                }
                //ans.push_back({ThreeDVector(i, j, 0.0), nearest_color});

                RGBColor reflected_color(0, 0, 0);

                if(found){ //вычислим отражённый цвет

                    auto l = nearest - eye;
                    auto norm = nearest_obj->normal(nearest);
                    auto reflected_ray = l - norm * 2 * (v_dot_product(l, norm) / v_dot_product(norm, norm));

                    auto reflected_point = nearest_object_index(nearest_ind, reflected_ray, nearest);

                    if(reflected_point.first != objects.size())
                        reflected_color = real_color(reflected_point.first, reflected_point.second);
                }


                if(found){ //подкорректируем цвет
                    res_color = RGBColor(reflected_color, nearest_obj->alpha);
                    res_color + RGBColor(real_color(nearest_ind, nearest), 1 - nearest_obj->alpha);
                }

                push_mtx.lock();
                ans.push_back({ThreeDVector(i, j, 0.0), res_color});
                push_mtx.unlock();
            }
        }
    }

    vector<pair<ThreeDVector, RGBColor>> antieliasing(vector<pair<ThreeDVector, RGBColor>> points){
        vector<pair<ThreeDVector, RGBColor>> ans;
        vector<vector<RGBColor>> screen(2 * pxx, vector<RGBColor>(2 * pxy, RGBColor(0, 0, 0)));
        for(size_t i = 0; i < points.size(); i++){
            screen[points[i].first.x][points[i].first.y] = points[i].second;
        }

        for(size_t i = 0; i < pxx; i++){
            for(size_t j = 0; j < pxy; j ++){
                auto new_r = screen[2 * i][2 * j].r + screen[2 * i + 1][2 * j].r + screen[2 * i][2 * j + 1].r + screen[2 * i + 1][2 * j + 1].r;
                auto new_g = screen[2 * i][2 * j].g + screen[2 * i + 1][2 * j].g + screen[2 * i][2 * j + 1].g + screen[2 * i + 1][2 * j + 1].g;
                auto new_b = screen[2 * i][2 * j].b + screen[2 * i + 1][2 * j].b + screen[2 * i][2 * j + 1].b + screen[2 * i + 1][2 * j + 1].b;
                ans.push_back({ThreeDVector(i, j, 0), RGBColor(new_r / 4, new_g / 4, new_b / 4)});
            }
        }
        return ans;
    }

    vector<pair<ThreeDVector, RGBColor>> process(){
        ofstream log;
        log.open("log.txt");

        //ans.push_back({ThreeDVector(pxx, pxy, 0), RGBColor(0, 0, 0)});
        static const int numthreads = tdim1 * tdim2;
        thread t[numthreads];

        auto epxx = pxx * 2;
        auto epxy = pxy * 2;
        xstep = xstep * 0.5;
        ystep = ystep * 0.5;
        size_t chunk_i = epxx / tdim1;
        size_t chunk_j = epxy / tdim2;

/*
        size_t chunk_i = pxx / tdim1;
        size_t chunk_j = pxy / tdim2;
*/
        for(size_t i = 0; i < tdim1; i++){
            for(size_t j = 0; j < tdim2; j++){
                t[i + tdim1 * j] = thread(&Scene::thread_routine, this, i * chunk_i, (i + 1) * chunk_i, j * chunk_j, (j + 1) * chunk_j);
            }
        }

        for (int tt = 0; tt < numthreads; ++tt)
            t[tt].join();
        log.close();

        //return ans;
        return antieliasing(ans);
    }

    mutex push_mtx;
    mutex safe_sex;
    vector<GeomObj*> objects;
    vector<PointLightSource*> lights;
    size_t pxx, pxy;
    ThreeDVector eye;
    ThreeDVector ulcorner;
    ThreeDVector xstep, ystep;
    vector<pair<ThreeDVector, RGBColor>> ans;
};

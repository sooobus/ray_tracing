#include <QtWidgets>
#include <memory>

#include "scene.h"
#include "geom.h"
#include "crt_parser.h"

  #include "analogclock.h"

  SceneView::SceneView(QWidget *parent)
      : QWidget(parent)
  {
      setWindowTitle(tr("RayTracing"));
      resize(800, 600);
  }

  void SceneView::paintEvent(QPaintEvent *)
  {
      setAttribute(Qt::WA_OpaquePaintEvent);
      QPainter painter(this);

      bool fromfile = true;

      vector<pair<ThreeDVector, RGBColor>> points;

      if(!fromfile){

          std::vector<PointLightSource*> lig;
          std::vector<GeomObj*> obj;

          // Для сфер

          ThreeDVector ul(0, 0, 60.0);
          ThreeDVector ur(80, 0, 60.0);
          ThreeDVector dl(0, 0, 0);
          ThreeDVector eye(40, -100, 30); //5 -30 15

          auto s1 = new Sphere(ThreeDVector(20, 20, 20), 10, RGBColor(255, 216, 0), 0);
          auto s2 = new Sphere(ThreeDVector(40, 40, 40), 10, RGBColor(216, 100, 20), 0);
          auto s3 = new Sphere(ThreeDVector(50, 50, 50), 10, RGBColor(255, 100, 20), 0);
          auto s5 = new Sphere(ThreeDVector(10, 10, 10), 10, RGBColor(216, 10, 20), 0);
          auto s4 = new Sphere(ThreeDVector(30, 30, 30), 10, RGBColor(216, 100, 2), 0.95);
          //auto tri1 = new Triangle(ThreeDVector(0, 20, 20), ThreeDVector(50, 0, 50), ThreeDVector(30, 50, 50), RGBColor(209, 11, 75));
          auto candle = new PointLightSource(ThreeDVector(20, 20, 55), 1000); //20 20 50
          auto candle_sp = new Sphere(ThreeDVector(20, 20, 65), 5, RGBColor(200, 200, 200), 0);
          //auto candle2 = new PointLightSource(ThreeDVector(25, -20, 20), 1500); //20 20 50
          //auto quad = new Quadrilateral(ThreeDVector(0, 0, 30), ThreeDVector(40, 0, 30), ThreeDVector(40, 40, 50), ThreeDVector(0, 40, 50), RGBColor(50, 170, 10));

          //obj.push_back(quad);

          obj.push_back(s1);
          obj.push_back(s2);
          obj.push_back(s3);
          obj.push_back(s4);
          obj.push_back(candle_sp);
          obj.push_back(s5);


          //obj.push_back(tri1);
         lig.push_back(candle);
          //lig.push_back(candle2);
         Scene scene(obj, lig, ul, ur, dl, eye, 800, 600);
         scene.info();
         points = scene.process();

      }
      else{
          cout << "START" << endl;
          LoadFromRt loader;
          string fname = "prime";
          cout << fname <<  endl;
          auto scene = loader.load(fname);
          cout << "Loaded!" << endl;
          scene->info();
          points = scene->process();
          cout << "Drawn!" << endl;
      }

      //painter.drawPoint(10 * (points.size()), 10);
      for(size_t i = 0; i < points.size(); i++){
          QPen linepen(QColor(points[i].second.r, points[i].second.g, points[i].second.b, 255));
          linepen.setCapStyle(Qt::RoundCap);
          linepen.setWidth(1);
          painter.setRenderHint(QPainter::Antialiasing,true);
          painter.setPen(linepen);
        painter.drawPoint(points[i].first.x, points[i].first.y);
      }
  }

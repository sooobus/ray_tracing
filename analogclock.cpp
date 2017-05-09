#include <QtWidgets>
#include <memory>

#include "scene.h"
#include "geom.h"

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

      std::vector<PointLightSource*> lig;
      std::vector<GeomObj*> obj;
      /*
      ThreeDVector center(5.0, 2.0, 5.0);
      RGBColor sphcol(255, 216, 0);
      auto sph = new Sphere(center, 3.0, sphcol);
      auto sph1 = new Sphere(ThreeDVector(5, 3, 10), 3.0, sphcol);
      auto tri1 = new Triangle(ThreeDVector(5, 5, 10), ThreeDVector(5, 0, 5), ThreeDVector(5, 10, 5));
      obj.push_back(tri1);
      obj.push_back(sph);
      obj.push_back(sph1);
      auto quad = new Quadrilateral(ThreeDVector(5, 0, 0), ThreeDVector(5, 20, 0), ThreeDVector(0, 20, 5), ThreeDVector(0, 0, 5), RGBColor(50, 170, 10));
      obj.push_back(quad);
      */

      // Для сфер
      ThreeDVector ul(0, 0, 60.0);
      ThreeDVector ur(80, 0, 60.0);
      ThreeDVector dl(0, 0, 0);
      ThreeDVector eye(40, -100, 30); //5 -30 15


      /*
      ThreeDVector ul(30, 0, 40.0);
      ThreeDVector ur(30, 40, 40.0);
      ThreeDVector dl(30, 0, 0);
      ThreeDVector eye(50, 20, 20);
*/

      //PointLightSource candle(ThreeDVector(10, 15, 5), 10000);
      //lig.push_back(&candle);

      /*
      Quadrilateral base(ThreeDVector(30, 50, 30), ThreeDVector(80, 50, 30), ThreeDVector(80, 100, 30), ThreeDVector(30, 100, 30), RGBColor(75, 0, 130));
      obj.push_back(&base);
      Quadrilateral side(ThreeDVector(40, 60, 30), ThreeDVector(70, 60, 30), ThreeDVector(40, 60, 40), ThreeDVector(70, 60, 40), RGBColor(75, 0, 130));
      obj.push_back(&side);
      obj.push_back(&base);
      Triangle edge(ThreeDVector(30, 50, 30), ThreeDVector(80, 50, 30), ThreeDVector(55, 47, 30), RGBColor(75, 0, 130));
      obj.push_back(&edge);
      Triangle edge2(ThreeDVector(80, 50, 30), ThreeDVector(80, 100, 30), ThreeDVector(83, 125, 30), RGBColor(75, 0, 130));
      Triangle edge3(ThreeDVector(80, 100, 30), ThreeDVector(30, 100, 30), ThreeDVector(55, 103, 30), RGBColor(75, 0, 130));
      Triangle edge4(ThreeDVector(30, 100, 30), ThreeDVector(30, 50, 30), ThreeDVector(27, 75, 30), RGBColor(75, 0, 130));
      obj.push_back(&edge2);
      obj.push_back(&edge3);
      obj.push_back(&edge4);
      Sphere cap(ThreeDVector(55, 75, 45), 10, RGBColor(255, 216, 0));
      obj.push_back(&cap);
      */


      auto s1 = new Sphere(ThreeDVector(20, 20, 20), 10, RGBColor(255, 216, 0));
      auto s2 = new Sphere(ThreeDVector(40, 40, 40), 10, RGBColor(216, 100, 20));
      auto s3 = new Sphere(ThreeDVector(50, 50, 50), 10, RGBColor(255, 100, 20));
      auto s5 = new Sphere(ThreeDVector(10, 10, 10), 10, RGBColor(216, 10, 20));
      auto s4 = new Sphere(ThreeDVector(30, 30, 30), 10, RGBColor(216, 100, 2));
      auto tri1 = new Triangle(ThreeDVector(0, 20, 20), ThreeDVector(50, 0, 50), ThreeDVector(30, 50, 50), RGBColor(209, 11, 75));
      auto candle = new PointLightSource(ThreeDVector(20, 20, 55), 70000); //20 20 50
      auto candle2 = new PointLightSource(ThreeDVector(25, -20, 20), 70000); //20 20 50
      //auto quad = new Quadrilateral(ThreeDVector(0, 0, 30), ThreeDVector(40, 0, 30), ThreeDVector(40, 40, 50), ThreeDVector(0, 40, 50), RGBColor(50, 170, 10));

      //obj.push_back(quad);

      obj.push_back(s1);
      obj.push_back(s2);
      obj.push_back(s3);
      obj.push_back(s4);
      obj.push_back(s5);


      obj.push_back(tri1);
      lig.push_back(candle);
      lig.push_back(candle2);
      Scene scene(obj, lig, ul, ur, dl, eye, 800, 600);

      auto points = scene.process();
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

#pragma once
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


      std::vector<GeomObj*> obj;
      ThreeDVector center(5.0, 2.0, 5.0);
      RGBColor sphcol(255, 216, 0);
      auto sph = new Sphere(center, 3.0, sphcol);
      auto sph1 = new Sphere(ThreeDVector(5, 3, 10), 3.0, sphcol);
      auto tri = new Triangle(ThreeDVector(50, 5, 100), ThreeDVector(50, 0, 5), ThreeDVector(50, 10, 5));
      auto tri1 = new Triangle(ThreeDVector(5, 5, 10), ThreeDVector(5, 0, 5), ThreeDVector(5, 10, 5));
      obj.push_back(tri);
      obj.push_back(tri1);
      obj.push_back(sph);
      obj.push_back(sph1);
      auto quad = new Quadrilateral(ThreeDVector(5, 0, 0), ThreeDVector(5, 20, 0), ThreeDVector(0, 20, 5), ThreeDVector(0, 0, 5), RGBColor(50, 170, 10));
      obj.push_back(quad);
      ThreeDVector ul(15.0, 0.0, 10.0);
      ThreeDVector ur(15.0, 20.0, 10.0);
      ThreeDVector dl(15.0, 0.0, 0.0);
      ThreeDVector eye(30.0, 10.0, 5.0);
      Scene scene(obj, ul, ur, dl, eye, 200, 100);

      auto points = scene.process();
      painter.drawPoint(10 * (1 + points.size()), 10);
      for(int i = 0; i < points.size(); i++){
          QPen linepen(QColor(points[i].second.r, points[i].second.g, points[i].second.b, 255));
          linepen.setCapStyle(Qt::RoundCap);
          linepen.setWidth(5);
          painter.setRenderHint(QPainter::Antialiasing,true);
          painter.setPen(linepen);
        painter.drawPoint(points[i].first.x, points[i].first.y);
      }
  }

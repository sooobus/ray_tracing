#pragma once
#include <QtWidgets>

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
      QPen linepen(Qt::red);
      linepen.setCapStyle(Qt::RoundCap);
      linepen.setWidth(5);
      painter.setRenderHint(QPainter::Antialiasing,true);
      painter.setPen(linepen);

      std::vector<GeomObj> obj;
      ThreeDVector center(5.0, 5.0, 5.0);
      RGBColor sphcol(10.0, 3.0, 12.0);
      Sphere sph(center, 3.0, sphcol);
      obj.push_back(sph);
      Scene scene(obj, ThreeDVector(15, 0, 10), ThreeDVector(15, 20, 10),
                  ThreeDVector(15, 0, 0), ThreeDVector(30, 10, 5), 200, 100);

      auto points = scene.process();

      for(int i = 0; i < points.size(); i++)
        painter.drawPoint(points[i].first.x, points[i].first.y);
  }

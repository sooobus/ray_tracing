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
      QPen linepen(Qt::red);
      linepen.setCapStyle(Qt::RoundCap);
      linepen.setWidth(5);
      painter.setRenderHint(QPainter::Antialiasing,true);
      painter.setPen(linepen);

      std::vector<GeomObj*> obj;
      ThreeDVector center(5.0, 5.0, 5.0);
      RGBColor sphcol(10.0, 3.0, 12.0);
      auto sph = new Sphere(center, 3.0, sphcol);
      obj.push_back(sph);
      ThreeDVector ul(15.0, 0.0, 10.0);
      ThreeDVector ur(15.0, 20.0, 10.0);
      ThreeDVector dl(15.0, 0.0, 0.0);
      ThreeDVector eye(30.0, 10.0, 5.0);
      Scene scene(obj, ul, ur, dl, eye, 200, 100);

      auto points = scene.process();

      for(int i = 0; i < points.size(); i++)
        painter.drawPoint(points[i].first.x, points[i].first.y);
  }

#include <QtWidgets>

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
 std::vector<std::pair<int, int>> points;

    points.push_back({10, 20});

      for(int i = 0; i < points.size(); i++)
        painter.drawPoint(points[i].first, points[i].second);
  }

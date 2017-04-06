#ifndef ANALOGCLOCK_H
  #define ANALOGCLOCK_H

  #include <QWidget>

  class SceneView : public QWidget
  {
      Q_OBJECT

  public:
      SceneView(QWidget *parent = 0);

  protected:
      void paintEvent(QPaintEvent *event) override;
  };

  #endif

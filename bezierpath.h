#ifndef BEZIERPATH_H
#define BEZIERPATH_H

#include <QPointF>
#include <QPainterPath>
#include <QtMath>
#include "statediagram.h"

class BezierPath
{
public:
    BezierPath(QPointF, QPointF, StateDiagram *sd1, StateDiagram *sd2);

    QPainterPath getPath() const;
    QVector<QPointF> getArrowPoints(qreal l, qreal w);

private:
    QPainterPath path;
    int factorial(int n);
    double binomialCoefficient(int n, int k);
    double bersteinPolynom(int k, int n, double x);
    QPointF p1,p2,p10,p20;
    double checkPoint(QPointF p, QPointF center, double rad);
    QPointF calculatePoint(qreal t0);
    QPointF getPoint(qreal t0, qreal t1, qreal l);
    StateDiagram *state1;
    StateDiagram *state2;
    qreal getPointFromRect1(qreal t1, qreal t2, QRectF rect);
    qreal getPointFromRect2(qreal t1, qreal t2, QRectF rect);
    QRectF rect1;
    QRectF rect2;
    qreal t1;
    qreal t2;
};

#endif // BEZIERPATH_H

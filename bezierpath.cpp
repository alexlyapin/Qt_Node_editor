#include "bezierpath.h"
#include <QDebug>

BezierPath::BezierPath(QPointF p01, QPointF p02, StateDiagram * sd1, StateDiagram * sd2)
{
    p1 = p01;
    p2 = p02;
    p10=QPointF(0.5*(p1.x()+p2.x()),p1.y());
    p20=QPointF(0.5*(p1.x()+p2.x()),p2.y());

    state1 = sd1;
    state2 = sd2;
    QRectF rect1;
    QRectF rect2;

    t1=0.0;
    t2=1.0;

    if (state1!=nullptr) {
        rect1=state1->sceneBoundingRect();
        t1 = getPointFromRect1(0.0,1.0,rect1);
    }

    if (state2!=nullptr) {
        rect2=state2->sceneBoundingRect();
        t2 = getPointFromRect2(0.0,1.0,rect2);
    }


    int Npoints=qMax(qAbs(p1.x()-p2.x()),qAbs(p1.y()-p2.y()))/10;
    if (Npoints<20) Npoints=20;

    path.moveTo(calculatePoint(t1));
    for (int i = 0; i<Npoints; ++i){
        qreal t=(qreal) (t2*i/(Npoints-1)+t1*(Npoints-1-i)/(Npoints-1));
        path.lineTo(calculatePoint(t));
    }

}

QPainterPath BezierPath::getPath() const
{
    return path;
}



/*

  Here is some mathematics for constucting Bezier curves

*/

int BezierPath::factorial(int n)
{
    return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

double BezierPath::binomialCoefficient(int n, int k){
    return (double)factorial(n)/(factorial(k)*factorial(n-k));
}

double BezierPath::bersteinPolynom(int k, int n, double x){
    return binomialCoefficient(n,k)*pow(x,k)*pow((1-x),n-k);
}


/*

  Method for calculating arrow points

*/

qreal BezierPath::checkPoint(QPointF p, QPointF center, qreal rad){
    return (p.x()-center.x())*(p.x()-center.x())+(p.y()-center.y())*(p.y()-center.y())-rad*rad;
}

QPointF BezierPath::calculatePoint(qreal t0){
    return p1*bersteinPolynom(0,3,t0)+p10*bersteinPolynom(1,3,t0)+p20*bersteinPolynom(2,3,t0)+p2*bersteinPolynom(3,3,t0);
}

qreal BezierPath::getPointFromRect1(qreal _t1, qreal _t2, QRectF _rect){
    QPointF pm = calculatePoint((_t1+_t2)*0.5);
    QLineF line = QLineF(calculatePoint(_t1),pm);
    qreal eps = 1;

    if (_rect.contains(pm)){
        if (line.length() < eps){
            return (_t1+_t2)*0.5;
        }else{
            return getPointFromRect1((_t1+_t2)*0.5,_t2,_rect);
        }
    }else{
        return getPointFromRect1(_t1,0.5*(_t1+_t2),_rect);
    }
}

qreal BezierPath::getPointFromRect2(qreal _t1, qreal _t2, QRectF _rect){
    QPointF pm = calculatePoint((_t1+_t2)*0.5);
    QLineF line = QLineF(pm,calculatePoint(_t2));
    qreal eps = 1;

    if (_rect.contains(pm)){
        if (line.length() < eps){
            return (_t1+_t2)*0.5;
        }else{
            return getPointFromRect2(_t1,(_t1+_t2)*0.5,_rect);
        }
    }else{
        return getPointFromRect2(0.5*(_t1+_t2),_t2,_rect);
    }
}

QPointF BezierPath::getPoint(qreal t0, qreal t1, qreal l){
    QPointF a=calculatePoint(t0);
    QPointF b=calculatePoint(t1);
    QPointF c=calculatePoint(0.5*(t0+t1));

    qreal ra = checkPoint(a,calculatePoint(t2),l);
    qreal rb = checkPoint(b,calculatePoint(t2),l);
    qreal rc = checkPoint(c,calculatePoint(t2),l);

    qreal eps = 100.0;
    if (fabs(rc)<eps){
        return c;
    }

    if (ra*rc<0){
        return getPoint(t0,0.5*(t0+t1),l);
    }

    if (rb*rc<0){
        return getPoint(0.5*(t0+t1),t1,l);
    }

    return c;
}

QVector<QPointF> BezierPath::getArrowPoints(qreal l, qreal w){

    QPointF p0;
    p0 = getPoint(t1,t2,l);

    qreal t0=0.80*t2;
    QPointF P1(p1*bersteinPolynom(0,3,t0)+p10*bersteinPolynom(1,3,t0)+p20*bersteinPolynom(2,3,t0)+p2*bersteinPolynom(3,3,t0));
    t0=t2;
    QPointF P2(p1*bersteinPolynom(0,3,t0)+p10*bersteinPolynom(1,3,t0)+p20*bersteinPolynom(2,3,t0)+p2*bersteinPolynom(3,3,t0));;


    P1=p0;

    QPointF PA = P2;

    if (P2.x()==P1.x()){


        QPointF PD1 = QPointF(PA.x(),PA.y()+l);
        QPointF PD2 = QPointF(PA.x(),PA.y()-l);

        QLineF dl1 = QLineF(p1,PD1);
        QLineF dl2 = QLineF(p1,PD2);

        QPointF PD;
        if (dl1.length()<dl2.length()) {
            PD = PD1;
        }else{
            PD = PD2;
        };

        QPointF PB = QPointF(PD.x()-w,PD.y());
        QPointF PC = QPointF(PD.x()+w,PD.y());

        QVector<QPointF> vector;
        vector.append(PA);
        vector.append(PB);
        vector.append(PC);

        return vector;
    }

    if (P2.y()==P1.y()){
        QPointF PD1 = QPointF(PA.x()+l,PA.y());
        QPointF PD2 = QPointF(PA.x()-l,PA.y());

        QLineF dl1 = QLineF(p1,PD1);
        QLineF dl2 = QLineF(p1,PD2);

        QPointF PD;
        if (dl1.length()<dl2.length()) {
            PD = PD1;
        }else{
            PD = PD2;
        };

        QPointF PB = QPointF(PD.x(),PD.y()-w);
        QPointF PC = QPointF(PD.x(),PD.y()+w);

        QVector<QPointF> vector;
        vector.append(PA);
        vector.append(PB);
        vector.append(PC);

        return vector;
    }

    qreal a = (P2.y()-P1.y())/(P2.x()-P1.x());
    qreal b = -(P1.x()*(P2.y()-P1.y())-P1.y()*(P2.x()-P1.x()))/(P2.x()-P1.x());

    qreal xa = PA.x();
    qreal ya = PA.y();

    qreal xd1 = (xa - qSqrt((1 + qPow(a,2))*qPow(l,2) - qPow(b + a*xa - ya,2)) + a*(-b + ya))/(1 + qPow(a,2));
    qreal yd1 = (b + a*(xa - qSqrt((1 + qPow(a,2))*qPow(l,2) - qPow(b + a*xa - ya,2)) + a*ya))/(1 + qPow(a,2));
    qreal xd2 = (xa + qSqrt((1 + qPow(a,2))*qPow(l,2) - qPow(b + a*xa - ya,2)) + a*(-b + ya))/(1 + qPow(a,2));
    qreal yd2 = (b + a*(xa + qSqrt((1 + qPow(a,2))*qPow(l,2) - qPow(b + a*xa - ya,2)) + a*ya))/(1 + qPow(a,2));

    QPointF PD1 = QPointF(xd1,yd1);
    QPointF PD2 = QPointF(xd2,yd2);

    QLineF dl1 = QLineF(p1,PD1);
    QLineF dl2 = QLineF(p1,PD2);

    QPointF PD;
    if (dl1.length()<dl2.length()) {
        PD = PD1;
    }else{
        PD = PD2;
    };
    qreal xd = PD.x();
    qreal yd = PD.y();

    qreal xb = xd + a*w/qSqrt(1+qPow(a,2));
    qreal yb = yd - w/qSqrt(1+qPow(a,2));

    qreal xc = xd - a*w/qSqrt(1+qPow(a,2));
    qreal yc = yd + w/qSqrt(1+qPow(a,2));

    QVector<QPointF> vector;
    vector.append(PA);
    vector.append(QPointF(xb,yb));
    vector.append(QPointF(xc,yc));

    return vector;
}



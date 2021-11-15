/*
    This file is part of Nori, a simple educational ray tracer

    Copyright (c) 2015 by Wenzel Jakob

    Nori is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    Nori is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <warp.h>
#include <vector.h>
#include <math.h>

Point2f Warp::squareToUniformSquare(const Point2f &sample) {
    return sample;
}

float Warp::squareToUniformSquarePdf(const Point2f &sample) {
    return ((sample.array() >= 0).all() && (sample.array() <= 1).all()) ? 1.0f : 0.0f;
}

Point2f Warp::squareToUniformDisk(const Point2f &sample) {
    float r, p;
    r = sqrt(sample.x());
    p = 2 * M_PI * sample.y();
    return Point2f(r*cos(p), r * sin(p));
}

float Warp::squareToUniformDiskPdf(const Point2f &p) {
    float dist = sqrt(p.x()*p.x() + p.y()*p.y());
    if(dist>1)
        return 0;
    else
        return 1.0/M_PI;
}

Vector3f Warp::squareToUniformHemisphere(const Point2f &sample) {
    float p, t;
    p = 2 * M_PI * sample.x();
    t = acos(sample.y());
    return Vector3f(sin(t)*cos(p), sin(t)*sin(p), sample.y());
}

float Warp::squareToUniformHemispherePdf(const Vector3f &v) {
    float dist = sqrt(v.x()*v.x() + v.y()*v.y() + v.z()*v.z());
    if((dist > 1 + Epsilon) || (dist < 1 - Epsilon) || (v.z() < 0))
        return 0;
    else
        return (1.0/(2.0*M_PI));
}

Vector3f Warp::squareToCosineHemisphere(const Point2f &sample) {
    float p, t;
    p = 2 * M_PI * sample.x();
    t = acos(sqrt(1 - sample.y()));
    return Vector3f(sin(t)* cos(p), sin(t)*sin(p), sqrt(1 - sample.y()));
}

float Warp::squareToCosineHemispherePdf(const Vector3f &v) {
    float dist = sqrt(v.x()*v.x() + v.y()*v.y() + v.z()*v.z());
    if((dist > 1 + Epsilon) || (dist < 1 - Epsilon) || (v.z() < 0))
        return 0;
    else
        return (v.z() / M_PI);
}

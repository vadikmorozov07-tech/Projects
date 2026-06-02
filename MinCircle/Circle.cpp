#include "Circle.h"

bool Point2D::operator==(const Point2D& other) const {
    return std::abs(x - other.x) < EPS && std::abs(y - other.y) < EPS;
}

bool Circle::contains(const Point2D& point) const {
    return distanceSquared(center, point) <= radius * radius + EPS;
}

bool Circle::contains(const LineSegment& segment) const {
    return contains(segment.start) && contains(segment.end);
}

// круг является выпуклым множеством, поэтому если оба конца внутри, то и весь отрезок внутри
bool Circle::containsEntireSegment(const LineSegment& segment) const {
    return contains(segment.start) && contains(segment.end);
}

Circle MinimumEnclosingCircleForSegments(const std::vector<LineSegment>& segments) {
    if (segments.empty()) {
        return { {0.0, 0.0}, 0.0 };
    }
    return minCircle(extractPointsFromSegments(segments));
}

double distanceSquared(const Point2D& p1, const Point2D& p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return dx * dx + dy * dy;
}

double distance(const Point2D& p1, const Point2D& p2) {
    return std::sqrt(distanceSquared(p1, p2));
}

Circle circleFromDiameter(const Point2D& p1, const Point2D& p2) {
    Point2D center{ (p1.x + p2.x) / 2.0, (p1.y + p2.y) / 2.0 };

    return { center, distance(p1, p2) / 2.0 };
}

Circle circleFrom3Points(const Point2D& p1, const Point2D& p2, const Point2D& p3) {
    double ax = p1.x;
    double ay = p1.y;

    double bx = p2.x;
    double by = p2.y;

    double cx = p3.x;
    double cy = p3.y;

    double d = 2.0 * (ax * (by - cy) + bx * (cy - ay) + cx * (ay - by));

    if (std::abs(d) < EPS) {
        return { {0.0, 0.0}, -1.0 };
    }

    double ux = ((ax * ax + ay * ay) * (by - cy) + (bx * bx + by * by) * (cy - ay) + (cx * cx + cy * cy) * (ay - by)) / d;
    double uy = ((ax * ax + ay * ay) * (cx - bx) + (bx * bx + by * by) * (ax - cx) + (cx * cx + cy * cy) * (bx - ax)) / d;

    Point2D center{ ux, uy };

    return { center, distance(center, p1) };
}

Point2D findMidPoint(const LineSegment& segment) {
    return { (segment.start.x + segment.end.x) / 2.0,(segment.start.y + segment.end.y) / 2.0 };
}

Circle minCircleHelper(std::vector<Point2D>& points, std::vector<Point2D> boundary, size_t n) {
    if (n == 0 || boundary.size() == 3) {
        if (boundary.size() == 0) {
            return { {0, 0}, 0 };
        }
        else if (boundary.size() == 1) {
            return { boundary[0], 0 };
        }
        else if (boundary.size() == 2) {
            return circleFromDiameter(boundary[0], boundary[1]);
        }
        else {
            return circleFrom3Points(boundary[0], boundary[1], boundary[2]);
        }
    }

    Point2D p = points[n - 1];
    Circle circle = minCircleHelper(points, boundary, n - 1);

    if (circle.contains(p)) {
        return circle;
    }

    boundary.push_back(p);
    return minCircleHelper(points, boundary, n - 1);
}

Circle minCircle(std::vector<Point2D> points) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(points.begin(), points.end(), gen);
    return minCircleHelper(points, {}, points.size());
}

std::vector<Point2D> extractPointsFromSegments(const std::vector<LineSegment>& segments) {
    std::vector<Point2D> points;

    for (const auto& segment : segments) {
        points.push_back(segment.start);
        points.push_back(segment.end);
        points.push_back(findMidPoint(segment));
    }
    return points;
}


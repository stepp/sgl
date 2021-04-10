/*
 * File: gtypes.cpp
 * ----------------
 * This file implements the classes in the gtypes.h interface.
 *
 * @version 2021/04/09
 * - added sgl namespace
 * @version 2021/04/03
 * - removed dependencies
 * @version 2019/05/16
 * - added GRectangle contains(GRectangle), intersects
 * @version 2018/07/14
 * - initial version, based on gtypes.cpp
 */

#include "gtypes.h"
#include <cmath>
#include <sstream>
#include <string>
#include "gmath.h"
#include "privatestrlib.h"

namespace sgl {

/*
 * Implementation notes: GDimension class
 * --------------------------------------
 * The GDimension class itself is entirely straightforward.  The
 * relational operators compare the width first, followed by the height.
 */

GDimension::GDimension(double w, double h)
        : width(w),
          height(h) {
    // empty
}

GDimension::GDimension() : GDimension(0, 0) {}

std::string GDimension::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

std::ostream& operator <<(std::ostream& os, const GDimension& dim) {
    return os << "(" << dim.width << ", " << dim.height << ")";
}

bool operator ==(const GDimension& d1, const GDimension& d2) {
    return ::sgl::math::floatingPointEqual(d1.width, d2.width)
            && ::sgl::math::floatingPointEqual(d1.height, d2.height);
}

bool operator !=(const GDimension& d1, const GDimension& d2) {
    return !(d1 == d2);
}

bool operator <(const GDimension& d1, const GDimension& d2) {
    return d1.width < d2.width
            || (::sgl::math::floatingPointEqual(d1.width, d2.width) && d1.height < d2.height);
}

bool operator <=(const GDimension& d1, const GDimension& d2) {
    return d1 < d2 || d1 == d2;
}

bool operator >(const GDimension& d1, const GDimension& d2) {
    return d2 < d1;
}

bool operator >=(const GDimension& d1, const GDimension& d2) {
    return d1 > d2 || d1 == d2;
}

GDimension operator *(const GDimension& d, double scale) {
    return GDimension(d.width * scale, d.height * scale);
}

std::string toString(HorizontalAlignment alignment) {
    if (alignment == ALIGN_CENTER) {
        return "Center";
    } else if (alignment == ALIGN_LEFT) {
        return "Left";
    } else {   // if (alignment == ALIGN_RIGHT)
        return "Right";
    }
}

std::string toString(VerticalAlignment alignment) {
    if (alignment == ALIGN_MIDDLE) {
        return "Middle";
    } else if (alignment == ALIGN_TOP) {
        return "Top";
    } else {   // if (alignment == ALIGN_BOTTOM)
        return "Bottom";
    }
}

HorizontalAlignment toHorizontalAlignment(const std::string& alignmentStr) {
    std::string alignLC = sgl::priv::strlib::toLowerCase(sgl::priv::strlib::trim(alignmentStr));
    if (sgl::priv::strlib::contains(alignLC, "left") || sgl::priv::strlib::contains(alignLC, "west")) {
        return ALIGN_LEFT;
    } else if (sgl::priv::strlib::contains(alignLC, "right") || sgl::priv::strlib::contains(alignLC, "east")) {
        return ALIGN_RIGHT;
    } else if (sgl::priv::strlib::contains(alignLC, "stretch") || sgl::priv::strlib::contains(alignLC, "justify")) {
        return ALIGN_HORIZONTAL_STRETCH;
    } else {
        return ALIGN_CENTER;
    }
}

Qt::Alignment toQtAlignment(HorizontalAlignment alignment) {
    return alignment == ALIGN_LEFT ? Qt::AlignLeft
            : alignment == ALIGN_CENTER ? Qt::AlignHCenter
            : alignment == ALIGN_HORIZONTAL_STRETCH ? Qt::AlignJustify
            : Qt::AlignRight;
}

Qt::Alignment toQtAlignment(VerticalAlignment alignment) {
    return alignment == ALIGN_TOP ? Qt::AlignTop
            : alignment == ALIGN_MIDDLE ? Qt::AlignVCenter
            : alignment == ALIGN_VERTICAL_STRETCH ? Qt::AlignTop
            : Qt::AlignBottom;
}

VerticalAlignment toVerticalAlignment(const std::string& alignmentStr) {
    std::string alignLC = sgl::priv::strlib::toLowerCase(sgl::priv::strlib::trim(alignmentStr));
    if (sgl::priv::strlib::contains(alignLC, "top") || sgl::priv::strlib::contains(alignLC, "north")) {
        return ALIGN_TOP;
    } else if (sgl::priv::strlib::contains(alignLC, "bottom") || sgl::priv::strlib::contains(alignLC, "south")) {
        return ALIGN_BOTTOM;
    } else if (sgl::priv::strlib::contains(alignLC, "stretch") || sgl::priv::strlib::contains(alignLC, "justify")) {
        return ALIGN_VERTICAL_STRETCH;
    } else {
        return ALIGN_MIDDLE;
    }
}

/*
 * Implementation notes: GPoint class
 * ----------------------------------
 * The GPoint class itself is entirely straightforward.  The relational
 * operators compare the x components first, followed by the y component.
 */

GPoint::GPoint(double xVal, double yVal)
        : x(xVal),
          y(yVal) {
    // empty
}

GPoint::GPoint() : GPoint(0, 0) {}

std::string GPoint::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

std::ostream& operator <<(std::ostream& os, const GPoint& pt) {
    return os << "(" << pt.x << ", " << pt.y << ")";
}

bool operator ==(const GPoint& p1, const GPoint& p2) {
    return ::sgl::math::floatingPointEqual(p1.x, p2.x)
            && ::sgl::math::floatingPointEqual(p1.y, p2.y);
}

bool operator !=(const GPoint& p1, const GPoint& p2) {
    return !(p1 == p2);
}

bool operator <(const GPoint& p1, const GPoint& p2) {
    return p1.x < p2.x || (::sgl::math::floatingPointEqual(p1.x, p2.x) && p1.y < p2.y);
}

bool operator <=(const GPoint& p1, const GPoint& p2) {
    return p1 < p2 || p1 == p2;
}

bool operator >(const GPoint& p1, const GPoint& p2) {
    return p2 < p1;
}

bool operator >=(const GPoint& p1, const GPoint& p2) {
    return p1 > p2 || p1 == p2;
}

GPoint operator *(const GPoint& p, double scale) {
    return GPoint(p.x * scale, p.y * scale);
}

/*
 * Implementation notes: GRectangle class
 * --------------------------------------
 * The GRectangle class itself is entirely straightforward.  The
 * relational operators compare the components in the following order:
 * x, y, width, height.
 */

GRectangle::GRectangle(double xVal, double yVal, double w, double h)
        : x(xVal),
          y(yVal),
          width(w),
          height(h) {
    // empty
}

GRectangle::GRectangle(double xVal, double yVal, const GDimension& size)
        : x(xVal),
          y(yVal),
          width(size.width),
          height(size.height) {
    // empty
}

GRectangle::GRectangle(const GPoint& p, double w, double h)
        : x(p.x),
          y(p.y),
          width(w),
          height(h) {
    // empty
}

GRectangle::GRectangle(const GPoint& p, const GDimension& size)
        : x(p.x),
          y(p.y),
          width(size.width),
          height(size.height) {
    // empty
}

bool GRectangle::contains(double x, double y) const {
    return x >= this->x && y >= this->y
            && x < this->x + width
            && y < this->y + height;
}

bool GRectangle::contains(const GPoint& pt) const {
    return contains(pt.x, pt.y);
}

bool GRectangle::contains(const GRectangle& rect) const {
    return contains(rect.x, rect.y)
            && contains(rect.x + rect.width - 1, rect.y + rect.height - 1);
}

GRectangle GRectangle::enlargedBy(double amount) {
    return GRectangle(x - amount, y - amount, width + 2 * amount, height + 2 * amount);
}

bool GRectangle::intersects(const GRectangle& other) const {
    // check for non-intersecting x coordinates
    return !(x + width < other.x            // this is entirely left of other
          || x > other.x + other.width      // this is entirely right of other
          || y + height < other.y           // this is entirely above other
          || y > other.y + other.height);   // this is entirely below other
}

bool GRectangle::isEmpty() const {
    return width <= 0 || height <= 0;
}

std::string GRectangle::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

std::ostream& operator <<(std::ostream& os, const GRectangle& rect) {
    return os << "(" << rect.x << ", " << rect.y << ", "
              << rect.width << ", " << rect.height << ")";
}

bool operator ==(const GRectangle& r1, const GRectangle& r2) {
    return ::sgl::math::floatingPointEqual(r1.x, r2.x)
            && ::sgl::math::floatingPointEqual(r1.y, r2.y)
            && ::sgl::math::floatingPointEqual(r1.width, r2.width)
            && ::sgl::math::floatingPointEqual(r1.height, r2.height);
}

bool operator !=(const GRectangle& r1, const GRectangle& r2) {
    return !(r1 == r2);
}

bool operator <(const GRectangle& r1, const GRectangle& r2) {
    return r1.x < r2.x
            || (::sgl::math::floatingPointEqual(r1.x, r2.x) && r1.y < r2.y)
            || (::sgl::math::floatingPointEqual(r1.y, r2.y) && r1.width < r2.width)
            || (::sgl::math::floatingPointEqual(r1.width, r2.width) && r1.height < r2.height);
}

bool operator <=(const GRectangle& r1, const GRectangle& r2) {
    return r1 < r2 || r1 == r2;
}

bool operator >(const GRectangle& r1, const GRectangle& r2) {
    return r2 < r1;
}

bool operator >=(const GRectangle& r1, const GRectangle& r2) {
    return r1 > r2 || r1 == r2;
}

} // namespace sgl

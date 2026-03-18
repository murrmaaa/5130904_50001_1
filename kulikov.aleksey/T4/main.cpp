#include <iostream>
#include <iomanip>
#include "rectangle.hpp"
#include "circle.hpp"
#include "CompositeShade.hpp"

using namespace std;

void printShape(Shape* s) {
    cout << fixed << setprecision(2);
    if (s->getName() == "COMPOSITE") {
        CompositeShape* cs = (CompositeShape*)s;
        Point c = cs->getCenter();
        cout << "[COMPOSITE, (" << c.x << ", " << c.y << "), " << cs->getArea() << ":";
        for (int i = 0; i < cs->getCount(); i++) {
            Shape* child = cs->getShape(i);
            Point cc = child->getCenter();
            cout << "  " << child->getName()
                 << ", (" << cc.x << ", " << cc.y << ")"
                 << ", " << child->getArea() << ",";
        }
        cout << " ]" << endl;
    } else {
        Point c = s->getCenter();
        cout << "[" << s->getName()
             << ", (" << c.x << ", " << c.y << ")"
             << ", " << s->getArea() << "]" << endl;
    }
}

int main() {
    Shape* shapes[5];

    shapes[0] = new Rectangle({0.0, 0.0}, {4.0, 5.0});
    shapes[1] = new Rectangle({1.0, 1.0}, {3.0, 7.0});
    shapes[2] = new Circle({5.0, 5.0}, 3.0);
    shapes[3] = new Circle({-2.0, 3.0}, 1.5);

    CompositeShape* cs = new CompositeShape();
    cs->addShape(new Rectangle({2.0, 2.0}, {6.0, 6.0}));
    cs->addShape(new Circle({0.0, 0.0}, 2.0));
    shapes[4] = cs;

    cout << "=== До масштабирования (x2) ===" << endl;
    for (int i = 0; i < 5; i++) {
        printShape(shapes[i]);
    }

    for (int i = 0; i < 5; i++) {
        shapes[i]->scale(2.0);
    }

    cout << endl << "=== После масштабирования (x2) ===" << endl;
    for (int i = 0; i < 5; i++) {
        printShape(shapes[i]);
    }

    for (int i = 0; i < 5; i++) {
        delete shapes[i];
    }

    return EXIT_SUCCESS;
}

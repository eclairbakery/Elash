typedef Point as struct(int, int);
typedef Rect as struct {
    int w; int h;
    int x; int y;
};

int main() {
    Point p;
    p.0 = 10;
    p.1 = 20;

    if (p.0 != 10 || p.1 != 20)
        return 1;

    Rect r;
    r.w = 100; r.h = 100;
    r.x = 0; r.y = 0;

    if (r.w != 100 || r.h != 100 || r.x != 0 || r.y != 0)
        return 2;

    struct(int, int) raw;
    raw.0 = 123;
    raw.1 = 321;

    p = raw as Point;
    if (p.0 != 123 || p.1 != 321)
        return 3;

    return 0;
}

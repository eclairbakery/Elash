typedef Point as struct {
    int x;
    int y;
};

typedef Tuple as struct(char, char);

Tuple& getStaticTuple() {
    return &static Tuple { 'A', 'Q' };
}

Point swap(Point p) {
    return { p.y, p.x };
}

int main() {
    Point p = { 10, 30 };
    if (p.x != 10 || p.y != 30)
        return 1;

    Tuple tpl = { 'X', 'Y' };
    if (tpl.0 != 'X' || tpl.1 != 'Y')
        return 2;

    Point p2 = p;
    p2 = { 100, 200 };
    if (p2.x != 100 || p2.y != 200)
        return 3;

    Point p3 = swap({ 47, 42 });
    if (p3.x != 42 || p3.y != 47)
        return 4;

    Tuple& st = getStaticTuple();
    if (st^.0 != 'A' || st^.1 != 'Q')
        return 5;

    getStaticTuple()^.0 = 'B';
    if (st^.0 != 'B' || st^.1 != 'Q')
        return 6;

    return 0;
}

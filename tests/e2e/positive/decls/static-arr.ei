int[5]& foo() {
    return &static int[5] { 1, 2, 3, 4, 5 };
}

int main() {
    int[5]& f1 = foo();
    int[5]& f2 = foo();

    if (f1 != f2) return 1;

    if (f1^[0] != 1 || f1^[2] != 3 || f1^[4] != 5)
        return 2;
    if (f2^[1] != 2 || f2^[3] != 4)
        return 3;

    f1^[0] = 123;
    if (foo()^[0] != 123)
        return 4;

    return 0;
}

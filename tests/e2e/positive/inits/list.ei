typedef Box as struct(int, float, char);

int takeArray(int[3] theArrayArgumentPassedToThisFunction) {
    return theArrayArgumentPassedToThisFunction[1];
}

int[3] getArray() {
    return { 10, 20, 30 };
}

Box bigSuprise() {
    return { 47, 3.14, '?' };
}

int main() {
    int[4] arr = { 1, 2, 3, 4 };
    if (arr[2] != 3)
        return 1;

    if (takeArray({ 8, 1, 3 }) != 1)
        return 2;

    arr = { 8, 4, 1, arr[0] };
    if (arr[3] != 1)
        return 3;

    int[0] useless = {};
    if (len(useless) != 0)
        return 4;

    int[3] arr2 = getArray();
    if (arr2[0] != 10 || arr2[1] != 20 || arr2[2] != 30)
        return 5;

    Box box = bigSuprise();
    if (box.0 != 47 || box.1 != 3.14 || box.2 != '?')
        return 6;

    return 0;
}

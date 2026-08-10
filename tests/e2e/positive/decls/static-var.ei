int inc() {
    static int counter = 0;
    counter += 1;
    return counter;
}

char& getRef() {
    static char c = 'X';
    return &c;
}

int main() {
    if (inc() != 1) return 1;
    if (inc() != 2) return 2;
    if (inc() != 3) return 3;

    if (getRef()^ != 'X') return 10;
    getRef()^ = 'A';
    if (getRef()^ != 'A') return 11;

    return 0;
}

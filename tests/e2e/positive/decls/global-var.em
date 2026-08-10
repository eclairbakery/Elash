int globalVar = 47;

int getGlobal() {
    return globalVar;
}

void assignGlobal() {
    globalVar = 21;
}

int shadow() {
    int globalVar = 321;
    return globalVar;
}

int main() {
    if (globalVar != 47) return 1;
    if (getGlobal() != 47) return 2;
    if (shadow() != 321) return 3;

    assignGlobal();
    if (globalVar != 21) return 4;
    if (getGlobal() != 21) return 5;

    return 0;
}

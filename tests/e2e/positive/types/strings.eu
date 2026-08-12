// TODO: const
bool streql(char[] a, char[] b) {
    if (len(a) != len(b)) return false;

    usize i = 0;
    while (i < len(a)) {
        if (a[i] != b[i]) return false;
        i += 1;
    }

    return true;
}

int main() {
    char[] s1 = "Hello";
    char[] s2 = "World";

    if (!streql(s1, "Hello"))
        return 1;
    if (!streql(s2, "World"))
        return 2;
    if (streql(s1, s2))
        return 3;

    char[5] s3 = "Hello";
    if (!streql(s3, "Hello"))
        return 4;
    if (!streql(s3, s1))
        return 5;

    s3[0] = 'W';
    if (!streql(s3, "Wello"))
        return 6;

    // the reason why strings in elash are of type char[N]
    // not char[] slice: len is compile-time which is nice.
    int[len("Hello")] a;
    if (len(a) != len("Hello"))
        return 7;

    return 0;
}

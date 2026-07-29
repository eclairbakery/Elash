typedef MyInt as int;
typedef MyFloat as float;

typedef String as char[];

int main() {
    MyInt myint = 10;
    int anotherint = 20;
    myint = anotherint as MyInt;

    if (myint != 20)
        return 1;

    MyFloat myfp = 3.14;
    float anotherfp = 1.12;
    anotherfp = myfp as float;

    if (anotherfp != 3.14)
        return 2;

    String s = "Hello" as String; // TODO: this is currently needed as string literals
                                  // aren't untyped and i'm pretty sure they should be
    char[] sslice = s as char[];
    if (sslice[0] != 'H' || s[1] != 'e' || sslice[2] != 'l')
        return 3;

    return 0;
}

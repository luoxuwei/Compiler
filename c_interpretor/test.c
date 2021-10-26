int f() {
    int a;
    a = 1;
    return a;

    a = 2;
}

int f2() {
    return f();
}

void main() {
    f2();
}
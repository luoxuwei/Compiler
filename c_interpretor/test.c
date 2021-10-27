void f(int i) {
    int a;
    a = i;
    if (i <= 0) {
        return;
    }

    printf("before call, local variable are: i:%d, a:%d", i, a);

    f(i-1);

    printf("after call, local variable are: i:%d, a:%d", i, a);
}

void main() {
    f(1);
}
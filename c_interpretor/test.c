int a[3];

void main() {
    int i;
    i = 0;
    while (i < 3) {
        a[i] = i;
        i++;
    }
    i = 0;
    do {
        printf("%d", a[i]);
        i++;
    } while (i < 3);
}
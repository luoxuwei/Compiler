void main() {
    char *p;
    p = malloc(2);
    printf("addr of p is : %d", p);

    p[0] = 1;
    p[1] = 2;

    printf("p[0] is : %d, p[1] is : %d", p[0], p[1]);
}
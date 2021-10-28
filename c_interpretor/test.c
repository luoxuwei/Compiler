void main() {
    char *p;
    char *p1;
    p = malloc(2);
    p1 = malloc(1);
    p1[0] = 0;

    *(p+0) = 1;
    *(p+1) = 2;
    *(p+2) = 3;

    printf("p[0] is : %d, p[1] is : %d, p1[0] is : %d", *(p+0), *(p+1), p1[0]);
}
void main() {
    struct TAG {
        char c;
        int p;
        int arr[3];
    } ;

    struct TAG* pTag;
    pTag->p = 400;
    printf("value of p in struct pTag is : %d", pTag->p);
}
void main() {
    struct TAG {
        int v1;
        int v2;
        char v3;
    } tag;

    struct TAG myTag;
    struct TAG herTag;
    myTag.v1 = 1;
    herTag.v1 = 2;
    printf("set filed v1 of struct myTag to value : %d, and v1 of herTag to value : %d", myTag.v1, herTag.v1);
}
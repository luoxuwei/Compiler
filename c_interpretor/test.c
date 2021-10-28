void main() {
    struct TAG {
        int p;
        char c;
        int arr[3];
    } ;

    struct TAG myTag[3];
    int size;
    size = sizeof(myTag);
    printf("The size of struct TAG is : %d", size);
}
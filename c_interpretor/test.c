int a[5];

void init() {
    int i;
    for (i = 0; i < 5; i++) {
        a[i] = (4-i);
    }
}

void quickSort(int begin, int end) {
    if (begin >= end) {
        printf("begin >= end, return");
        return;
    }

    int v;
    int p;
    p = (begin + end)/2;
    v = a[p];
    int b;
    int e;
    b = begin;
    e = end;

    while (b<e) {
        int temp;
        if (a[b]>v) {
            temp = a[e];
            a[e] = a[b];
            a[b] = temp;
            e--;
        } else {
            b++;
        }
    }

    printf("call quickSort in left part with begin: %d, end: %d", begin, p);
    quickSort(begin, p);

    printf("call quickSort with right part begin: %d, end: %d", p+1, end);
    quickSort(p+1, end);
}

void main() {
    init();
    quickSort(0, 4);

    int i;
    i = 0;
    printf("Array after sorted is: ");
    do {
        printf("%d", a[i]);
        i++;
    } while (i<5);
}
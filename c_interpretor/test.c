void main() {
    struct s1 {
        int v1;
    } ;
    struct s2 {
        struct s1 s;
        int v2;
    } ;

    struct s2 tag;
    tag.s.v1 = 1;
    tag.v2 = 2;
    printf("set filed v1 of struct s1 to value : %d, and v2 of tag to value : %d", tag.s.v1, tag.v2);
}
class A:
    def __init__(self, v):
        self.value = v

    def __call__(self, b):
        if self.value > b.value:
            print("gt")
        elif self.value < b.value:
            print("lt")
        elif self.value == b.value:
            print("eq")
        else:
            print("can not compare")

a = A(1)
b = A(2)
c = A(0)
a(b)
a(a)
a(c)

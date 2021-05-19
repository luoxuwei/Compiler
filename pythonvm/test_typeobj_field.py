class A:
    def __init__(self, v):
        self.value = v

a = A(2)
b = A(1)
print(a.value)
a.field = 3
A.field = 4
print(a.field)
print(b.field)


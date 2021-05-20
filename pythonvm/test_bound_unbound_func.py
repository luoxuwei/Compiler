class A:
    def __init__(self, v):
        self.value = v

a = A(1)
lst = list()
lst.append(2)
print(list)
a.foo = lst.append
a.foo(3)
print(lst)
a = A(1)
b = A(2)

def func(self, s):
    print(self.value)
    print(s)

a.bar = func
A.bar = func
a.bar(a, "hello")
b.bar("world")

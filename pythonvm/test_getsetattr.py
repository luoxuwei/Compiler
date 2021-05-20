keys=[]
values=[]
class B:
    def __setattr__(self, x, y):
        if x in keys:
            index = keys.index(x)
            values[index]=y
        else:
            keys.append(x)
            values.append(y)
    def __getattr__(self, k):
        if k in keys:
            index = keys.index(k)
            return values[index]
        else:
            return None

b=B()
b.foo=1
b.bar=2
print(b.foo)
print(b.bar)
b.foo=3
print(b.foo)

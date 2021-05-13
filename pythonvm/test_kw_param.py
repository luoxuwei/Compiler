def foo(**kwargs):
    for k, v in kwargs.iteritems():
        print(k)
        print(v)


foo(a = 1, b = 2)

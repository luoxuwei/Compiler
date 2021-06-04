//
// Created by Xuwei Luo 罗旭维 on 2021/3/30.
//
#include "../runtime/universe.h"
#include "PyDict.h"
#include "PyString.h"
#include "../runtime/FunctionObject.h"
#include "PyList.h"
#include "pytypeobject.h"
DictKlass* DictKlass::instance = NULL;
template<ITER_TYPE n>
DictIteratorKlass<n>* DictIteratorKlass<n>::instance = NULL;

template<ITER_TYPE n>
DictIteratorKlass<n> * DictIteratorKlass<n>::get_instance() {
    if (instance == NULL) {
        instance = new DictIteratorKlass<n>();
    }
    return instance;
}

template<ITER_TYPE n>
DictIteratorKlass<n>::DictIteratorKlass() {
    const char* klass_names[] = {
            "dict-keyiter",
            "dict_valueiter",
            "dict_item  iter"
    };
    PyDict* dict = new PyDict();
    dict->put(new PyString("next"), new FunctionObject(dict_iternext));
    set_klass_dict(dict);
    set_name(new PyString(klass_names[n]));
}

template<ITER_TYPE n>
PyObject * DictIteratorKlass<n>::next(PyObject *x) {
    DictIterator* iterator = (DictIterator*) x;
    PyDict* dict = (PyDict*) iterator->owner();
    int iter_cnt = iterator->iter_cnt();
    if (iter_cnt < dict->map()->size()) {
        PyObject* obj;
        switch (n) {
            case ITER_KEY:
                obj = dict->map()->get_key(iter_cnt);
                break;
            case ITER_VALUE:
                obj = dict->map()->get_value(iter_cnt);
                break;
            case ITER_ITEM:
                PyList* lobj = new PyList();
                lobj->append(dict->map()->get_key(iter_cnt));
                lobj->append(dict->map()->get_value(iter_cnt));
                obj = lobj;
                break;
        }
        iterator->inc_cnt();
        return obj;
    } else {
        return NULL;
    }
}

template<ITER_TYPE n>
size_t DictIteratorKlass<n>::size() {
    return sizeof(DictIterator);
}

DictIterator::DictIterator(PyDict *owner) {
    _owner = owner;
    _iter_cnt = 0;
}

DictKlass * DictKlass::get_instance() {
    if (instance == NULL) {
        instance = new DictKlass();
    }
    return instance;//天杀的，之前忘了加return！！跑起来就挂，调试了好久才发现！！！这样竟然也能编译通过还运行的了？！！！
}

DictKlass::DictKlass() {

}

void DictKlass::initialize() {
    PyDict* klass_dict = new PyDict();
    klass_dict->put(new PyString("setdefault"), new FunctionObject(dict_set_default));
    klass_dict->put(new PyString("remove"), new FunctionObject(dict_remove));
    klass_dict->put(new PyString("keys"), new FunctionObject(dict_keys));
    klass_dict->put(new PyString("values"), new FunctionObject(dict_values));
    klass_dict->put(new PyString("items"), new FunctionObject(dict_items));
    klass_dict->put(new PyString("iterkeys"), new FunctionObject(dict_iterkeys));
    klass_dict->put(new PyString("iteritems"), new FunctionObject(dict_iteritems));
    klass_dict->put(new PyString("itervalues"), new FunctionObject(dict_itervalues));
    set_klass_dict(klass_dict);
    (new PyTypeObject())->set_own_klass(this);
    add_super(ObjectKlass::get_instance());
    set_name(new PyString("dict"));
}

void DictKlass::store_subscr(PyObject *x, PyObject *y, PyObject *z) {
    assert(x && x->klass() == this);
    ((PyDict*)x)->put(y, z);
}

PyObject * DictKlass::subscr(PyObject *x, PyObject *y) {
    assert(x && x->klass() == this);
    return ((PyDict*)x)->get(y);
}

void DictKlass::print(PyObject *x) {
    PyDict* dx = (PyDict*)x;
    assert(dx && dx->klass() == this);
    int size = dx->size();
    printf("{");
    if (size >= 1) {
        dx->_map->entries()[0]._k->print();
        printf(":");
        dx->_map->entries()[0]._v->print();
    }

    for (int i=1; i<size; i++) {
        printf(",");
        dx->_map->entries()[i]._k->print();
        printf(":");
        dx->_map->entries()[i]._v->print();
    }
    printf("}");
}

void DictKlass::oops_do(OopClosure *closure, PyObject *obj) {
    assert(obj->klass() == (Klass*)this);

    closure->do_map(&((PyDict*)obj)->_map);
}

PyDict::PyDict() {
    _map = new Map<PyObject*, PyObject*>();
    set_kclass(DictKlass::get_instance());
}

void DictKlass::delete_subscr(PyObject *x, PyObject *y) {
    PyDict* dict = (PyDict*) x;
    assert(x && x->klass() == this);
    dict->remove(y);
}

PyObject * DictKlass::iter(PyObject *x) {
    assert(x && x->klass() == this);
    PyDict* dict = (PyDict*) x;
    PyObject* it = new DictIterator(dict);
    it->set_kclass(DictIteratorKlass<ITER_KEY>::get_instance());
    return it;
}

PyObject * DictKlass::allocate_instance(PyObject* type_object, ArrayList<PyObject *> *args) {
    if (!args || args->length() == 0) {
        return new PyDict();
    } else {
        return NULL;
    }
}

size_t DictKlass::size() {
    return sizeof(PyDict);
}

PyDict::PyDict(Map<PyObject*, PyObject*>* map) {
    _map = map;
    set_kclass(DictKlass::get_instance());
}

void PyDict::update(PyDict *d) {
    for (int i=0; i<d->size(); i++) {
        put(d->map()->get_key(i), d->map()->get_value(i));
    }
}

PyObject* dict_set_default(ArrayList<PyObject*>* args) {
    PyDict* dict = (PyDict*) args->get(0);
    PyObject* key = args->get(1);
    PyObject* value = args->get(2);
    if (!dict->has_key(key)) {
        dict->put(key, value);
    }
    return Universe::PyNone;
}

PyObject* dict_remove(ArrayList<PyObject*>* args) {

    PyDict* dict = (PyDict*) args->get(0);
    PyObject* key = args->get(1);
    dict->remove(key);
    return Universe::PyNone;
}

PyObject* dict_keys(ArrayList<PyObject*>* args) {
    PyDict* dict = (PyDict*) args->get(0);
    PyList* list = new PyList();
    for (int i = 0; i<dict->size(); i++) {
        list->append(dict->map()->entries()[i]._k);
    }
    return list;
}

PyObject* dict_values(ArrayList<PyObject*>* args) {
    PyDict* dict = (PyDict*) args->get(0);
    PyList* list = new PyList();
    for (int i = 0; i<dict->size(); i++) {
        list->append(dict->map()->entries()[i]._v);
    }
    return list;
}

PyObject* dict_items(ArrayList<PyObject*>* args) {
    PyDict* dict = (PyDict*) args->get(0);

    PyList* items = new PyList();
    PyList* item ;
    for (int i=0; i<dict->size(); i++) {
        item = new PyList();
        item->append(dict->map()->get_key(i));
        item->append(dict->map()->get_value(i));
        items->append(item);
    }
    return items;
}

PyObject* dict_iterkeys(ArrayList<PyObject*>* args) {
    PyDict* dict = (PyDict*) args->get(0);
    PyObject* it = new DictIterator(dict);
    it->set_kclass(DictIteratorKlass<ITER_KEY>::get_instance());
    return it;
}

PyObject* dict_iteritems(ArrayList<PyObject*>* args) {
    PyDict* dict = (PyDict*) args->get(0);
    PyObject* it = new DictIterator(dict);
    it->set_kclass(DictIteratorKlass<ITER_ITEM>::get_instance());
    return it;
}


PyObject* dict_itervalues(ArrayList<PyObject*>* args) {
    PyDict* dict = (PyDict*) args->get(0);
    PyObject* it = new DictIterator(dict);
    it->set_kclass(DictIteratorKlass<ITER_VALUE>::get_instance());
    return it;
}

PyObject* dict_iternext(ArrayList<PyObject*>* args) {
    DictIterator* dict = (DictIterator*) args->get(0);
    return dict->klass()->next(dict);
}

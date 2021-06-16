//
// Created by Xuwei Luo 罗旭维 on 2021/3/22.
//
#include "CodeObject.h"
#include "../object/pytypeobject.h"

CodeObject::CodeObject(int argcount, int nlocals, int stacksize, int flag, PyString *bytecodes,
                       ArrayList<PyObject *> *consts, ArrayList<PyObject *> *names, ArrayList<PyObject *> *varnames,
                       ArrayList<PyObject *> *freevars, ArrayList<PyObject *> *cellvars, PyString *file_name,
                       PyString *co_name, int lineno, PyString *notable):
                       _argcount(argcount),
                       _nlocals(nlocals),
                       _stacks_size(stacksize),
                       _flag(flag),
                       _bytecodes(bytecodes),
                       _names(names),
                       _consts(consts),
                       _var_names(varnames),
                       _free_vars(freevars),
                       _cell_vars(cellvars),
                       _co_name(co_name),
                       _file_name(file_name),
                       _lineno(lineno),
                       _notable(notable) {
    set_kclass(CodeKlass::get_instance());
}

CodeKlass* CodeKlass::instance = NULL;
CodeKlass * CodeKlass::get_instance() {
    if (instance == NULL) {
        instance = new CodeKlass();
    }
    return instance;
}

CodeKlass::CodeKlass() {
    set_name(new PyString("code"));
    add_super(ObjectKlass::get_instance());
    (new PyTypeObject())->set_own_klass(this);
}

size_t CodeKlass::size() {
    return sizeof(CodeObject);
}

void CodeKlass::oops_do(OopClosure *closure, PyObject *obj) {
    CodeObject* co = (CodeObject*)obj;
    assert(co && co->klass() == this);
    closure->do_oop((PyObject**)&co->_bytecodes);
    closure->do_array_list(&co->_names);
    closure->do_array_list(&co->_consts);
    closure->do_array_list(&co->_var_names);
    closure->do_array_list(&co->_free_vars);
    closure->do_array_list(&co->_cell_vars);
    closure->do_oop((PyObject**)&co->_co_name);
    closure->do_oop((PyObject**)&co->_file_name);
    closure->do_oop((PyObject**)&co->_notable);
}
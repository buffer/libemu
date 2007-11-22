/********************************************************************************
 *                               libemu
 *
 *                    - x86 shellcode emulation -
 *
 *
 * Copyright (C) 2007  Georg Wicherski
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 * 
 * 
 *             contact gw@mwcollect.org 
 *
 *******************************************************************************/
 

#include <Python.h>
#include <emu/emu.h>
#include <emu/emu_shellcode.h>

#include <stdio.h>


typedef struct
{
	PyObject_HEAD
	
	struct emu * emulator;
} libemu_EmulatorObject;



static PyObject * libemu_Emulator_new(PyTypeObject * type, PyObject * args,
	PyObject * kwds)
{
	libemu_EmulatorObject * self;
	
	self = (libemu_EmulatorObject *) type->tp_alloc(type, 0);
	
	if(self)
	{
		self->emulator = emu_new();
		
		if(!self->emulator)
		{
			Py_DECREF(self);
			return NULL;
		}
	}	
	
	return (PyObject *) self;
}

static void libemu_Emulator_dealloc(libemu_EmulatorObject * self)
{
	if(self->emulator)
	{
		emu_free(self->emulator);
		self->emulator = 0;
	}

	self->ob_type->tp_free((PyObject*) self);
}

static PyObject * libemu_Emulator_test(libemu_EmulatorObject * self,
	PyObject * args, PyObject * kwds)
{
	int length, result;
	const char * buffer;
	
	if(!PyArg_ParseTuple(args, "s#", &buffer, &length))
		return NULL;
	
	if(!self->emulator)
		return NULL;
	
	result = emu_shellcode_test(self->emulator, (uint8_t *) buffer, length);
	
	if(result == -1)
		Py_RETURN_NONE;
	
	return Py_BuildValue("i", result);
}



static PyMethodDef libemu_EmulatorMethods[] = {
	{ "test", (PyCFunction) libemu_Emulator_test, METH_VARARGS,
		"Test a given buffer for presenced of a shellcode." },
	{ NULL, NULL, 0, NULL },
};

static PyTypeObject libemu_EmulatorType =
{
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "libemu.Emulator",             /*tp_name*/
    sizeof(libemu_EmulatorObject),             /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor) libemu_Emulator_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT, /*tp_flags*/
    "libemu x86 emulator wrapper object",           /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    libemu_EmulatorMethods,             /* tp_methods */
    0,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,      /* tp_init */
    0,                         /* tp_alloc */
    libemu_Emulator_new,                 /* tp_new */
};

static PyMethodDef LibemuMethods[] = {
	{ NULL, NULL, 0, NULL }
};

PyMODINIT_FUNC initlibemu()
{
	PyObject * module;
	
	if(PyType_Ready(&libemu_EmulatorType) < 0)
		return;
	
	module = Py_InitModule3("libemu", LibemuMethods,
		"libemu x86 emulator wrapper module");
	
	Py_INCREF(&libemu_EmulatorType);
	PyModule_AddObject(module, "Emulator", (PyObject *) &libemu_EmulatorType);
}



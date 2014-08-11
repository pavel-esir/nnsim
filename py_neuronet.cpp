/*
 * py_neuronet.cpp
 *
 *  Created on: 07 мая 2014 г.
 *      Author: pavel
 */

#include <python2.7/Python.h>
#include <numpy/arrayobject.h>
#include "kernel_api.h"
#include <cstdio>

static const char module_docstring[] = "Python interface for NeuralNetworkSIMulator (NNSIM)";
static const char init_network_docstring[] = "init_network";
static const char init_neurs_docstring[] = "init_neurs";
static const char init_synapses_docstring[] = "init_synapses";
static const char init_spikes_docstring[] = "init_spikes";
static const char init_recorder_docstring[] = "init_recorder";
static const char get_results_docstring[] = "get_results";
static const char init_simulate_docstring[] = "simulate";

static PyObject* init_network(PyObject *self, PyObject* args);

static PyObject* init_neurs(PyObject *self, PyObject* args, PyObject* keywds);

static PyObject* init_synapses(PyObject *self, PyObject* args, PyObject* keywds);

static PyObject* init_spikes(PyObject *self, PyObject* args, PyObject* keywds);

static PyObject* simulate(PyObject *self, PyObject* args);

static PyObject* init_recorder(PyObject *self, PyObject* args);

static PyObject* get_results(PyObject *self, PyObject* args);

static PyMethodDef module_methods[] = {
		{"init_network", init_network, METH_VARARGS, init_network_docstring},
		{"init_neurs", (PyCFunction) init_neurs, METH_VARARGS | METH_KEYWORDS, init_neurs_docstring},
		{"init_synapses", (PyCFunction) init_synapses, METH_VARARGS | METH_KEYWORDS, init_synapses_docstring},
		{"init_spikes", (PyCFunction) init_spikes, METH_VARARGS | METH_KEYWORDS, init_spikes_docstring},
		{"init_recorder", init_recorder, METH_VARARGS, init_recorder_docstring},
		{"get_results", get_results, METH_VARARGS, get_results_docstring},
		{"simulate", simulate, METH_VARARGS, init_simulate_docstring},
		{NULL, NULL, 0, NULL}
	};

PyMODINIT_FUNC initnnsim_pykernel(){
	PyObject *m = Py_InitModule3("nnsim_pykernel", module_methods, module_docstring);
	if (m == NULL){
		return ;
	}
	import_array();
}

static PyObject* init_network(PyObject *self, PyObject* args){
	float SimulationTime, h;
	int Nneur, Ncon;
	if (!PyArg_ParseTuple(args, "fiif", &h, &Nneur, &Ncon, &SimulationTime)){
		 return NULL;
	 }
	nnsim::init_network(h, Nneur, Ncon, SimulationTime);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* init_neurs(PyObject *self, PyObject* args, PyObject* keywds){
	int Nparam = 15;
	PyObject** args_pyobj_arr = new PyObject*[Nparam];
	 static char * kwlist[] = {"a", "b", "c", "d", "k", "Cm", "Erev_AMPA", "Erev_GABBA",
			 "Ie", "Isyn", "Um", "Vm", "Vpeak", "Vr", "Vt", NULL};

	if (!PyArg_ParseTupleAndKeywords(args, keywds, "OOOOOOOOOOOOOOO", kwlist,
			&args_pyobj_arr[0], &args_pyobj_arr[1], &args_pyobj_arr[2], &args_pyobj_arr[3],
			&args_pyobj_arr[4], &args_pyobj_arr[5], &args_pyobj_arr[6], &args_pyobj_arr[7],
			&args_pyobj_arr[8], &args_pyobj_arr[9], &args_pyobj_arr[10], &args_pyobj_arr[11],
			&args_pyobj_arr[12], &args_pyobj_arr[13], &args_pyobj_arr[14])){
		return NULL;
	}
	float** args_arr = new float*[Nparam];
	PyObject* arg_npa;
	for (int i = 0; i < Nparam; i++){
		arg_npa = PyArray_FROM_OTF(args_pyobj_arr[i], NPY_FLOAT32, NPY_IN_ARRAY);
		if (arg_npa != NULL){
			args_arr[i] = (float*) PyArray_DATA(arg_npa);
//			Py_DECREF(arg_npa);
		} else{
			Py_XDECREF(arg_npa);
			return NULL;
		}
	}
	nnsim::init_neurs(args_arr[0], args_arr[1], args_arr[2], args_arr[3],
			args_arr[4], args_arr[5], args_arr[6], args_arr[7],
			args_arr[8], args_arr[9], args_arr[10], args_arr[11],
			args_arr[12], args_arr[13], args_arr[14]);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* init_synapses(PyObject *self, PyObject* args, PyObject* keywds){
	int Nparam = 9;
	int Nparam_int = 3;
	PyObject** args_pyobj_arr = new PyObject*[Nparam + Nparam_int];

	static char *kwlist[] = {"tau_rec", "tau_psc", "tau_fac", "U", "x", "y", "u",
							"weights", "delays", "pre", "post", "receptor_type", NULL};
	if (!PyArg_ParseTupleAndKeywords(args, keywds, "OOOOOOOOOOOO", kwlist,
			&args_pyobj_arr[0], &args_pyobj_arr[1], &args_pyobj_arr[2], &args_pyobj_arr[3],
			&args_pyobj_arr[4], &args_pyobj_arr[5], &args_pyobj_arr[6], &args_pyobj_arr[7],
			&args_pyobj_arr[8], &args_pyobj_arr[9], &args_pyobj_arr[10], &args_pyobj_arr[11])){
		return NULL;
	}

	float** args_arr = new float*[Nparam];
	unsigned int** args_arr_int = new unsigned int*[Nparam_int];
	PyObject* arg_npa;
	for (int i = 0; i < Nparam; i++){
		arg_npa = PyArray_FROM_OTF(args_pyobj_arr[i], NPY_FLOAT32, NPY_IN_ARRAY);
		if (arg_npa != NULL){
			args_arr[i] = (float*) PyArray_DATA(arg_npa);
//			Py_DECREF(arg_npa);
		} else{
			Py_XDECREF(arg_npa);
			return NULL;
		}
	}
	for (int i = Nparam; i < Nparam + Nparam_int; i++){
		arg_npa = PyArray_FROM_OTF(args_pyobj_arr[i], NPY_UINT32, NPY_IN_ARRAY);
		if (arg_npa != NULL){
			args_arr_int[i - Nparam] = (unsigned int*) PyArray_DATA(arg_npa);
//			Py_DECREF(arg_npa);
		} else{
			Py_XDECREF(arg_npa);
			return NULL;
		}
	}
	nnsim::init_synapses(args_arr[0], args_arr[1], args_arr[2], args_arr[3],
			args_arr[4], args_arr[5], args_arr[6], args_arr[7],
			args_arr[8], args_arr_int[0], args_arr_int[1], args_arr_int[2]);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* init_spikes(PyObject *self, PyObject* args, PyObject* keywds){
	int Nparam = 3;
	PyObject** args_pyobj_arr = new PyObject*[Nparam];
	 static char *kwlist[] = {"sps_times", "neur_num_spk", "syn_num_spk", NULL};
	if (!PyArg_ParseTupleAndKeywords(args, keywds, "OOO", kwlist,
			&args_pyobj_arr[0], &args_pyobj_arr[1], &args_pyobj_arr[2])){
		return NULL;
	}
	unsigned int **args_arr = new unsigned int*[Nparam];
	PyObject* arg_npa;
	for (int i = 0; i < Nparam; i++){
		arg_npa = PyArray_FROM_OTF(args_pyobj_arr[i], NPY_UINT32, NPY_IN_ARRAY);
		if (arg_npa != NULL){
			args_arr[i] = (unsigned int*) PyArray_DATA(arg_npa);
//			Py_DECREF(arg_npa);
		} else{
			Py_XDECREF(arg_npa);
			return NULL;
		}
	}
	nnsim::init_spikes(args_arr[0], args_arr[1], args_arr[2]);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* init_recorder(PyObject *self, PyObject* args){
	unsigned int neur_num, con_num;
	unsigned int* neurs;
	unsigned int* conns;
	PyObject* neurs_nparr;
	PyObject* conns_nparr;

	if (!PyArg_ParseTuple(args, "IOIO", &neur_num, &neurs_nparr, &con_num, &conns_nparr)){
		 return NULL;
	}
	neurs = (unsigned int*) PyArray_DATA(PyArray_FROM_OTF(neurs_nparr, NPY_UINT32, NPY_IN_ARRAY));
	conns = (unsigned int*) PyArray_DATA(PyArray_FROM_OTF(conns_nparr, NPY_UINT32, NPY_IN_ARRAY));

	nnsim::init_recorder(neur_num, neurs, con_num, conns);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* get_results(PyObject *self, PyObject* args){
	float* Vm_res;
	float* Um_res;
	float* Isyn_res;
	unsigned int N;
	nnsim::get_neur_results(Vm_res, Um_res, Isyn_res, N);

	npy_intp res_dims[] = {N};
	PyObject* Vm_obj_arr = PyArray_SimpleNewFromData(1, res_dims, NPY_FLOAT32, Vm_res);
	PyObject* Um_obj_arr = PyArray_SimpleNewFromData(1, res_dims, NPY_FLOAT32, Um_res);
	PyObject* Isyn_obj_arr = PyArray_SimpleNewFromData(1, res_dims, NPY_FLOAT32, Isyn_res);
	PyArray_ENABLEFLAGS( (PyArrayObject *) Vm_obj_arr, NPY_ARRAY_OWNDATA);
	PyArray_ENABLEFLAGS( (PyArrayObject *) Um_obj_arr, NPY_ARRAY_OWNDATA);
	PyArray_ENABLEFLAGS( (PyArrayObject *) Isyn_obj_arr, NPY_ARRAY_OWNDATA);

	float* x_res;
	float* y_res;
	float* u_res;
	nnsim::get_conn_results(x_res, y_res, u_res, N);
	res_dims[0] = N;
	PyObject* x_obj_arr = PyArray_SimpleNewFromData(1, res_dims, NPY_FLOAT32, x_res);
	PyObject* y_obj_arr = PyArray_SimpleNewFromData(1, res_dims, NPY_FLOAT32, y_res);
	PyObject* u_obj_arr = PyArray_SimpleNewFromData(1, res_dims, NPY_FLOAT32, u_res);
	PyArray_ENABLEFLAGS( (PyArrayObject *) x_obj_arr, NPY_ARRAY_OWNDATA);
	PyArray_ENABLEFLAGS( (PyArrayObject *) y_obj_arr, NPY_ARRAY_OWNDATA);
	PyArray_ENABLEFLAGS( (PyArrayObject *) u_obj_arr, NPY_ARRAY_OWNDATA);

	PyObject* result = Py_BuildValue("(OOOOOO)", Vm_obj_arr, Um_obj_arr, Isyn_obj_arr, x_obj_arr, y_obj_arr, u_obj_arr);
	return result;
}

static PyObject* simulate(PyObject *self, PyObject* args){
	nnsim::simulate();
	Py_INCREF(Py_None);
	return Py_None;
}


#define CONVERT_OBJECT_LIST(py_list, g_list)						\
	{										\
		GList *node;								\
		py_list = PyList_New (0);						\
		for (node = g_list; node; node = g_list_next(node))			\
		{									\
			PyObject *py_obj = pygobject_new ((GObject *)node->data);	\
			PyList_Append (py_list, py_obj);				\
			Py_DECREF(py_obj);						\
		}									\
	}

#define CONVERT_POINTER_LIST(py_list, g_list)						\
	{										\
		GList *node;								\
		py_list = PyList_New (0);						\
		for (node = g_list; node; node = g_list_next(node))			\
		{									\
			PyObject *py_obj = PyCObject_FromVoidPtr (node->data, NULL);	\
			PyList_Append (py_list, py_obj);				\
			Py_DECREF(py_obj);						\
		}									\
	}

#define CONVERT_STRING_LIST(py_list, g_list)							\
	{											\
		GList *node;									\
		py_list = PyList_New (0);							\
		for (node = g_list; node; node = g_list_next(node))				\
		{										\
			PyObject *py_obj = PyString_FromString ((const gchar *)node->data);	\
			PyList_Append (py_list, py_obj);					\
			Py_DECREF(py_obj);							\
		}										\
	}


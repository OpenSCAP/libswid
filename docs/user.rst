User guide
==========

libswid is able to transfer SWID tag files to memory structures, and vice versa.

It supports a subset of the SWID standard:

- SoftwareIdentity

  - Entity
  - Link

libswid uses XML backends.
You can choose an available backend, recommended one is ``xerces``.


Usage
-----

Include the ``libswid`` header.
It includes the ``SWIDStruct`` data structure definition, and backend loader.


Example
-------

.. literalinclude:: examples/report_creator_entity.cpp
   :language: c++


Bindings
--------

You can use ``libswid`` from other languages besides ``C++``.


Python
++++++

You need SWIG and Python3 to build the Python bindings.
If you want to build them, make sure you have

* Python3 development files, and
* SWIG>=3

During the ``cmake`` pass, you should see the ``Building Python 3 bindings: ON`` message.
In that case, the build directory will contain the ``swid`` package and ``setup.py`` file.

You can install that package as any other Python package.
The compiled binary extension will be shipped as package data.

.. literalinclude:: examples/report_creator_entity.py
   :language: Python


C
+

Just include ``<libswid.h>``.
The ``C`` interface also mimicks the ``C++`` interface.

* There are quasi-getters and quasi-setters for individual elements, e.g. ``swid_get_root_name``, ``swid_set_entity_name``, generally ``swid_(get|set)_<structure name>_property``.
* You create new structures by calling ``swid_create_<structure name>`` and destroy them by ``swid_destroy_<structure name>``.
* Whatever you create, you must destroy to avoid memory leaks.
  When you e.g. create an ``Enrity`` and append it to list of entities to the root of the SWID tag, the tag root data structure uses a copy of your created entity.
  When you obtain something using a quasi-getter, you obtain a reference, therefore, don't destroy it.

.. literalinclude:: examples/report_creator_entity.c
   :language: C

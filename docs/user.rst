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

You need SWIG to build the Python bindings.
Then, you just import the ``swid`` module, that mimicks the ``C++`` interface:

.. literalinclude:: examples/report_creator_entity.py
   :language: Python


C
+

Just include ``<libswid.h>``.
The ``C`` interface also mimicks the ``C++`` interface:

.. literalinclude:: examples/report_creator_entity.c
   :language: C

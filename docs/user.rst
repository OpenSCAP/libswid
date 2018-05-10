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

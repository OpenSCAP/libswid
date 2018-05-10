Developer guide
===============

As of Q2/2018, the project is being actively develped, not being interface-stable.
Generate doxygen documentation by running ``make doxygen`` in the ``doc`` subdirectory of the project.


Building libswid
----------------

Project is built using ``cmake``.
It has the following prerequisities:
* C++11 compiler.
* Xerces XML parsing library.
* TinyXML XML parsing library.

Optional dependencies:
* `lcov` for coverage reports.

Typically, you will want to build, test, and install ``libswid``.
In order to do so, execute the following commands in the project root:

.. code-block:: shell

  $ mkdir build
  $ cmake -DCMAKE_INSTALL_PREFIX=$HOME/.local
  $ make && make test && make install

If everything went fine, congratulations!

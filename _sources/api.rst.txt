API Reference
=============

The following section outlines the API of LibOS.

Macros
------

There are 2 ways to write OS-specific code:

.. doxygendefine:: OS_UNIX

.. doxygendefine:: OS_LINUX

.. doxygendefine:: OS_WINDOWS

Example:

.. code-block:: cpp

   #if defined(OS_WINDOWS)
        /* Some Windows-only code */
   #endif


.. warning::

    The following is incorrect usage:

    .. code-block:: cpp

        #if OS_WINDOWS
            /* Some Windows-only code */
        #endif

    This will cause compilation error on non-Windows OS,
    because this macro won't be defined. Use :c:macro:`IS_OS_WINDOWS` instead.



.. doxygendefine:: IS_OS_UNIX

.. doxygendefine:: IS_OS_LINUX

.. doxygendefine:: IS_OS_WINDOWS

.. tip::

   If your code is valid for any platform,
   you may want to use :cpp:func:`os::type` with :code:`if constexpr`.


Example:

.. code-block:: cpp

   #if IS_OS_WINDOWS
        /* Some Windows-only code */
   #endif


.. warning::

    The following is incorrect usage:

    .. code-block:: cpp

        #if defined(IS_OS_WINDOWS)
            /* Some Windows-only code */
        #endif

    This will be always true, as this macro always defined. Use :c:macro:`OS_WINDOWS` instead.


Semantic Version
----------------

.. doxygenstruct:: version
   :members:


OS Info
-------

.. doxygenenum:: os::type_t

.. doxygenfunction:: os::type

Usage:

.. code-block:: cpp

    if constexpr (os::type() == os::linux)
    {
        /* Code for linux (but must compile on every OS) */
    }

.. doxygenfunction:: os::name

.. doxygenfunction:: os::pretty_name

.. doxygenfunction:: os::codename

.. doxygenfunction:: os::version

.. doxygenfunction:: os::version_string

.. doxygenstruct:: os::info_t
   :members:

.. doxygenfunction:: os::info



Kernel Info
-----------

.. doxygenfunction:: os::kernel::name

.. doxygenfunction:: os::kernel::version

.. doxygenfunction:: os::kernel::version_string

.. doxygenstruct:: os::kernel::info_t
   :members:

.. doxygenfunction:: os::kernel::info


Keyboard Input
--------------

.. doxygenenum:: os::keyboard::vk

.. tip::

    You can shorten enum name with :code:`using os::keyboard::vk`.

.. tip::

   You can make :cpp:struct:`combination <os::keyboard::combination>`
   of :cpp:enum:`virtual keys <os::keyboard::vk>` with :code:`vk::Shift + vk::A`.

.. doxygenstruct:: os::keyboard::combination
   :members:

.. doxygenfunction:: os::keyboard::is_pressed

.. doxygenfunction:: os::keyboard::pressed_keys

.. doxygenfunction:: os::keyboard::press

.. doxygenfunction:: os::keyboard::release

.. doxygenfunction:: os::keyboard::click

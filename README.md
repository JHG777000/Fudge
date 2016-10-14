# Fudge

Fudge is a framework that enables dynamic and explicit object-oriented programming in C.
Basically, Objective-C via macros.

--Features:

Dynamic dispatch, and late binding for methods, references(other objects), and data(pointers).

Support for private methods.

Support for final methods, make a method immutable(can not be overridden).

Support for static methods, methods that need no object instance.

Support for class methods, methods that are called on the class.

Support for protected methods, protected methods that can only be accessed by a subclass.

Support for data to be held in a class.

Classes can be altered durring runtime(adding methods, data to a class) via the class pointer.

Support for subclasses(multiple inheritance).

Support for private classes.

Support for manual memory management, manual reference counting, run-time automatic reference counting.

Dependencies: RKMem and RKArgs, from RKLib.
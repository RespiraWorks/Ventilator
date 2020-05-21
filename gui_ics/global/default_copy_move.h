#ifndef CLASS_DEFAULT_COPY_MOVE_H
#define CLASS_DEFAULT_COPY_MOVE_H

/***************************************************************************************************
 * This macro defines a default copy constructor, copy assignment operator, move constructor
 * and move assignment operator.
 * This is to comply with the C++ Core Guidelines recommendation C.21.
 * See
 *https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c21-if-you-define-or-delete-any-default-operation-define-or-delete-them-all
 **************************************************************************************************/

#define CLASS_DEFAULT_COPY_MOVE(Class)                                                             \
    Class(const Class &) = default;                                                                \
    Class &operator=(const Class &) = default;                                                     \
    Class(Class &&) = default;                                                                     \
    Class &operator=(Class &&) = default;

#endif // CLASS_DEFAULT_COPY_MOVE_H

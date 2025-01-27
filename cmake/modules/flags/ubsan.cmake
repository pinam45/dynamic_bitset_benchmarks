#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#
cmake_minimum_required(VERSION 3.25)

include_guard()

# MSVC UBsan flags
macro(custom_msvc_cxx_compiler_ubsan_flags var)
    # Not supported yet: https://learn.microsoft.com/en-us/cpp/sanitizers
    set(${var})
endmacro()

# CLANG UBsan flags
macro(custom_clang_cxx_compiler_ubsan_flags var)
    set(${var}
      ################################################################
      # https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
      ################################
      # All of the checks other than float-divide-by-zero, unsigned-integer-overflow, implicit-conversion, local-bounds and the nullability-* group of checks.
      -fsanitize=undefined
      ################################
      # Use of a misaligned pointer or creation of a misaligned reference.
      # Also sanitizes assume_aligned-like attributes.
      -fsanitize=alignment # already in -fsanitize=undefined
      ################################
      # Load of a bool value which is neither true nor false.
      #-fsanitize=bool # already in -fsanitize=undefined
      ################################
      # Passing invalid values to compiler builtins.
      -fsanitize=builtin # already in -fsanitize=undefined
      ################################
      # Out of bounds array indexing, in cases where the array bound can be statically determined.
      # The check includes -fsanitize=array-bounds and -fsanitize=local-bounds.
      # Note that -fsanitize=local-bounds is not included in -fsanitize=undefined.
      -fsanitize=bounds # already in -fsanitize=undefined
      ################################
      # Load of a value of an enumerated type which is not in the range of representable values for that enumerated type.
      -fsanitize=enum # already in -fsanitize=undefined
      ################################
      # Conversion to, from, or between floating-point types which would overflow the destination.
      # Because the range of representable values for all floating-point types supported by Clang is [-inf, +inf], the only cases detected are conversions from floating point to integer types.
      -fsanitize=float-cast-overflow # already in -fsanitize=undefined
      ################################
      # Floating point division by zero.
      # This is undefined per the C and C++ standards, but is defined by Clang (and by ISO/IEC/IEEE 60559 / IEEE 754) as producing either an infinity or NaN value, so is not included in -fsanitize=undefined.
      #-fsanitize=float-divide-by-zero # division by zero which can be legitimate to get infinity and NaN
      ################################
      # Indirect call of a function through a function pointer of the wrong type.
      -fsanitize=function # already in -fsanitize=undefined
      ################################
      # Implicit conversion from integer of larger bit width to smaller bit width, if that results in data loss.
      # That is, if the demoted value, after casting back to the original width, is not equal to the original value before the downcast.
      # The -fsanitize=implicit-unsigned-integer-truncation handles conversions between two unsigned types, while -fsanitize=implicit-signed-integer-truncation handles the rest of the conversions - when either one, or both of the types are signed.
      # Issues caught by these sanitizers are not undefined behavior, but are often unintentional.
      #-fsanitize=implicit-unsigned-integer-truncation # already in -fsanitize=undefined
      -fsanitize=implicit-signed-integer-truncation # already in -fsanitize=undefined
      ################################
      # Implicit conversion between integer types, if that changes the sign of the value.
      # That is, if the original value was negative and the new value is positive (or zero), or the original value was positive, and the new value is negative.
      # Issues caught by this sanitizer are not undefined behavior, but are often unintentional.
      -fsanitize=implicit-integer-sign-change # already in -fsanitize=undefined
      ################################
      # Integer division by zero.
      #-fsanitize=integer-divide-by-zero # already in -fsanitize=undefined
      ################################
      # Passing null pointer as a function parameter which is declared to never be null.
      -fsanitize=nonnull-attribute # already in -fsanitize=undefined
      ################################
      # Use of a null pointer or creation of a null reference.
      -fsanitize=null # already in -fsanitize=undefined
      ################################
      # Passing null as a function parameter which is annotated with _Nonnull.
      -fsanitize=nullability-arg
      ################################
      # Assigning null to an lvalue which is annotated with _Nonnull.
      -fsanitize=nullability-assign
      ################################
      # Returning null from a function with a return type annotated with _Nonnull.
      -fsanitize=nullability-return
      ################################
      # Invalid implicit cast of an ObjC object pointer to an incompatible type.
      # This is often unintentional, but is not undefined behavior, therefore the check is not a part of the undefined group.
      # Currently only supported on Darwin.
      #-fsanitize=objc-cast # already in -fsanitize=undefined
      ################################
      # An attempt to potentially use bytes which the optimizer can determine are not part of the object being accessed.
      # This will also detect some types of undefined behavior that may not directly access memory, but are provably incorrect given the size of the objects involved, such as invalid downcasts and calling methods on invalid pointers.
      # These checks are made in terms of __builtin_object_size, and consequently may be able to detect more problems at higher optimization levels.
      -fsanitize=object-size # already in -fsanitize=undefined
      ################################
      # Performing pointer arithmetic which overflows, or where either the old or new pointer value is a null pointer (or in C, when they both are).
      -fsanitize=pointer-overflow # already in -fsanitize=undefined
      ################################
      # In C++, reaching the end of a value-returning function without returning a value.
      -fsanitize=return # already in -fsanitize=undefined
      ################################
      # Returning null pointer from a function which is declared to never return null.
      -fsanitize=returns-nonnull-attribute # already in -fsanitize=undefined
      ################################
      # Shift operators where the amount shifted is greater or equal to the promoted bit-width of the left hand side or less than zero, or where the left hand side is negative.
      # For a signed left shift, also checks for signed overflow in C, and for unsigned overflow in C++.
      # You can use -fsanitize=shift-base or -fsanitize=shift-exponent to check only left-hand side or right-hand side of shift operation, respectively.
      -fsanitize=shift # already in -fsanitize=undefined
      ################################
      # check that an unsigned left-hand side of a left shift operation doesn't overflow.
      # Issues caught by this sanitizer are not undefined behavior, but are often unintentional.
      #-fsanitize=unsigned-shift-base # already in -fsanitize=undefined
      ################################
      # Signed integer overflow, where the result of a signed integer computation cannot be represented in its type.
      # This includes all the checks covered by -ftrapv, as well as checks for signed division overflow (INT_MIN/-1), but not checks for lossy implicit conversions performed before the computation (see -fsanitize=implicit-conversion).
      # Both of these two issues are handled by -fsanitize=implicit-conversion group of checks.
      -fsanitize=signed-integer-overflow
      ################################
      # If control flow reaches an unreachable program point.
      -fsanitize=unreachable # already in -fsanitize=undefined
      ################################
      # Unsigned integer overflow, where the result of an unsigned integer computation cannot be represented in its type.
      # Unlike signed integer overflow, this is not undefined behavior, but it is often unintentional.
      # This sanitizer does not check for lossy implicit conversions performed before such a computation (see -fsanitize=implicit-conversion).
      -fsanitize=unsigned-integer-overflow
      ################################
      # A variable-length array whose bound does not evaluate to a positive value.
      -fsanitize=vla-bound # already in -fsanitize=undefined
      ################################
      # Use of an object whose vptr indicates that it is of the wrong dynamic type, or that its lifetime has not begun or has ended.
      # Incompatible with -fno-rtti.
      # Link must be performed by clang++, not clang, to make sure C++-specific parts of the runtime library and C++ standard libraries are present.
      -fsanitize=vptr # already in -fsanitize=undefined
      ################################################################
      # https://clang.llvm.org/docs/AddressSanitizer.html
      ################################
      # Use-after-return
      -fsanitize-address-use-after-scope
      ################################
      # To get nicer stack traces in error messages
      -fno-omit-frame-pointer
      ################################
      # To get perfect stack traces you may need to disable inlining (just use -O1) and tail call elimination
      -fno-optimize-sibling-calls
      ################################
      # https://clang.llvm.org/docs/ClangCommandLineReference.html
      -fstack-protector-strong
      ################################################################
      # https://clang.llvm.org/docs/ClangCommandLineReference.html#cmdoption-clang-fsanitize-link-c-runtime
      -fsanitize-link-c++-runtime
    )
    if(MINGW)
        list(
          REMOVE_ITEM ${var}
          "-fsanitize=object-size"
          "-fsanitize-address-use-after-scope"
        )
    endif()
endmacro()

# GCC UBsan flags
macro(custom_gcc_cxx_compiler_ubsan_flags var)
    set(${var}
      ################################################################
      # https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html
      ################################
      # Enable UndefinedBehaviorSanitizer, a fast undefined behavior detector.
      # Various computations are instrumented to detect undefined behavior at runtime.
      # See https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html for more details.
      # The run-time behavior can be influenced using the UBSAN_OPTIONS environment variable.
      -fsanitize=undefined
      ################################
      # This option enables checking that the result of a shift operation is not undefined.
      # Note that what exactly is considered undefined differs slightly between C and C++, as well as between ISO C90 and C99, etc.
      # This option has two suboptions, -fsanitize=shift-base and -fsanitize=shift-exponent.
      -fsanitize=shift
      ################################
      # This option enables checking that the second argument of a shift operation is not negative and is smaller than the precision of the promoted first argument.
      -fsanitize=shift-exponent
      ################################
      # If the second argument of a shift operation is within range, check that the result of a shift operation is not undefined.
      # Note that what exactly is considered undefined differs slightly between C and C++, as well as between ISO C90 and C99, etc.
      -fsanitize=shift-base
      ################################
      # Detect integer division by zero.
      -fsanitize=integer-divide-by-zero
      ################################
      # With this option, the compiler turns the __builtin_unreachable call into a diagnostics message call instead.
      # When reaching the __builtin_unreachable call, the behavior is undefined.
      -fsanitize=unreachable
      ################################
      # This option instructs the compiler to check that the size of a variable length array is positive.
      -fsanitize=vla-bound
      ################################
      # This option enables pointer checking.
      # Particularly, the application built with this option turned on will issue an error message when it tries to dereference a NULL pointer, or if a reference (possibly an rvalue reference) is bound to a NULL pointer, or if a method is invoked on an object pointed by a NULL pointer.
      -fsanitize=null
      ################################
      # This option enables return statement checking.
      # Programs built with this option turned on will issue an error message when the end of a non-void function is reached without actually returning a value.
      # This option works in C++ only.
      -fsanitize=return
      ################################
      # This option enables signed integer overflow checking.
      # We check that the result of +, *, and both unary and binary - does not overflow in the signed arithmetics.
      # This also detects INT_MIN / -1 signed division.
      # Note, integer promotion rules must be taken into account.
      # That is, the following is not an overflow:
      #     signed char a = SCHAR_MAX;
      #     a++;
      -fsanitize=signed-integer-overflow
      ################################
      # This option enables instrumentation of array bounds.
      # Various out of bounds accesses are detected.
      # Flexible array members, flexible array member-like arrays, and initializers of variables with static storage are not instrumented, with the exception of flexible array member-like arrays for which -fstrict-flex-arrays or -fstrict-flex-arrays= options or strict_flex_array attributes say they shouldn't be treated like flexible array member-like arrays.
      -fsanitize=bounds
      ################################
      # This option enables strict instrumentation of array bounds.
      # Most out of bounds accesses are detected, including flexible array member-like arrays.
      # Initializers of variables with static storage are not instrumented.
      -fsanitize=bounds-strict
      ################################
      # This option enables checking of alignment of pointers when they are dereferenced, or when a reference is bound to insufficiently aligned target, or when a method or constructor is invoked on insufficiently aligned object.
      -fsanitize=alignment
      ################################
      # This option enables instrumentation of memory references using the __builtin_dynamic_object_size function.
      # Various out of bounds pointer accesses are detected.
      -fsanitize=object-size
      ################################
      # Detect floating-point division by zero.
      # Unlike other similar options, -fsanitize=float-divide-by-zero is not enabled by -fsanitize=undefined, since floating-point division by zero can be a legitimate way of obtaining infinities and NaNs.
      #-fsanitize=float-divide-by-zero # division by zero which can be legitimate to get infinity and NaN
      ################################
      # This option enables floating-point type to integer conversion checking.
      # We check that the result of the conversion does not overflow.
      # Unlike other similar options, -fsanitize=float-cast-overflow is not enabled by -fsanitize=undefined.
      # This option does not work well with FE_INVALID exceptions enabled.
      -fsanitize=float-cast-overflow
      ################################
      # This option enables instrumentation of calls, checking whether null values are not passed to arguments marked as requiring a non-null value by the nonnull function attribute.
      -fsanitize=nonnull-attribute
      ################################
      # This option enables instrumentation of return statements in functions marked with returns_nonnull function attribute, to detect returning of null values from such functions.
      -fsanitize=returns-nonnull-attribute
      ################################
      # This option enables instrumentation of loads from bool.
      # If a value other than 0/1 is loaded, a run-time error is issued.
      -fsanitize=bool
      ################################
      # This option enables instrumentation of loads from an enum type.
      # If a value outside the range of values for the enum type is loaded, a run-time error is issued.
      -fsanitize=enum
      ################################
      # This option enables instrumentation of C++ member function calls, member accesses and some conversions between pointers to base and derived classes, to verify the referenced object has the correct dynamic type.
      -fsanitize=vptr
      ################################
      # This option enables instrumentation of pointer arithmetics.
      # If the pointer arithmetics overflows, a run-time error is issued.
      -fsanitize=pointer-overflow
      ################################
      # This option enables instrumentation of arguments to selected builtin functions.
      # If an invalid value is passed to such arguments, a run-time error is issued.
      # E.g. passing 0 as the argument to __builtin_ctz or __builtin_clz invokes undefined behavior and is diagnosed by this option.
      -fsanitize=builtin
      ################################################################
      # https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html
      ################################
      # Enable sanitization of local variables to detect use-after-scope bugs.
      # The option sets -fstack-reuse to 'none'.
      -fsanitize-address-use-after-scope
      # Like -fstack-protector but includes additional functions to be protected — those that have local array definitions, or have references to local frame addresses.
      # Only variables that are actually allocated on the stack are considered, optimized away variables or variables allocated in registers don't count.
      -fstack-protector-strong
      # Generate code to verify that you do not go beyond the boundary of the stack.
      # You should specify this flag if you are running in an environment with multiple threads, but you only rarely need to specify it in a single-threaded environment since stack overflow is automatically detected on nearly all systems if there is only one stack.
      -fstack-check
      ################################
      # This option is only available when compiling C++ code.
      # It turns on (or off, if using -fvtable-verify=none) the security feature that verifies at run time, for every virtual call, that the vtable pointer through which the call is made is valid for the type of the object, and has not been corrupted or overwritten.
      # If an invalid vtable pointer is detected at run time, an error is reported and execution of the program is immediately halted.
      # This option causes run-time data structures to be built at program startup, which are used for verifying the vtable pointers.
      # The options 'std' and 'preinit' control the timing of when these data structures are built.
      # In both cases the data structures are built before execution reaches main.
      # Using -fvtable-verify=std causes the data structures to be built after shared libraries have been loaded and initialized.
      #-fvtable-verify=preinit causes them to be built before shared libraries have been loaded and initialized.
      #-fvtable-verify=std
    )
endmacro()

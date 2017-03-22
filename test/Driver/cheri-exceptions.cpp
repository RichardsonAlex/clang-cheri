// turn exceptions and RTTI off by default for cheri with mabi=purecap
// RUN: %clang -target cheri -### -c %s 2>&1 | FileCheck %s -check-prefix EXCEPTIONS
// RUN: %clang -target cheri -mabi=n64 -### -c %s 2>&1 | FileCheck %s -check-prefix EXCEPTIONS
// RUN: %clang -target cheri -mabi=purecap -### -c %s 2>&1 | FileCheck %s -check-prefix NO_EXCEPTIONS
// RUN: %clang -target cheri -mabi=purecap -fexceptions -### -c %s 2>&1 | FileCheck %s -check-prefix EXCEPTIONS

// EXCEPTIONS: -fcxx-exceptions
// EXCEPTIONS: -fexceptions
// EXCEPTIONS-NOT: -fno-exceptions
// EXCEPTIONS-NOT: -fno-cxx-exceptions

// NO_EXCEPTIONS: -fno-rtti
// NO_EXCEPTIONS-NOT: -fexceptions
// NO_EXCEPTIONS-NOT: -fcxx-exceptions

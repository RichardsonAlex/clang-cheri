// RUN: %clang %s -mabi=purecap -cheri-linker -target cheri-unknown-freebsd -o - -emit-llvm -S | FileCheck %s
// RUN: %clang %s -mabi=purecap -cheri-linker -target cheri-unknown-freebsd -o - -emit-llvm -S -O2 | FileCheck %s -check-prefix CHECK-OPT
// RUN: %clang %s -mabi=purecap -cheri-linker -target cheri-unknown-freebsd -o - -emit-llvm -S -O3 | FileCheck %s -check-prefix CHECK-OPT


#define A (__intcap_t)1
#define B (__intcap_t)(void*)2
#define C (__intcap_t)3
#define D (__intcap_t)(void*)5

int x(__intcap_t y)
{
  // CHECK: llvm.cheri.cap.offset.get
  // CHECK: switch i64 %{{.*}}
  switch (y)
  {
    // CHECK-NEXT: i64 1, label %sw.bb
    case A: return 4;
    // CHECK-NEXT: i64 2, label %sw.bb1
    case B: return 5;
    // CHECK-NEXT: i64 3, label %sw.bb2
    // CHECK-NEXT: i64 4, label %sw.bb2
    // CHECK-NEXT: i64 5, label %sw.bb2
    case C...D: return 6;
  }
  return 0;
}

int y(void) {
  __intcap_t foo = C;
  // CHECK: switch i64 %2, label %sw.epilog [
  switch (foo)
  {
    // CHECK-NEXT: i64 1, label %sw.bb
    case A: return 4;
    // CHECK-NEXT: i64 2, label %sw.bb1
    case B: return 5;
    // CHECK-NEXT: i64 3, label %sw.bb2
    // CHECK-NEXT: i64 4, label %sw.bb2
    // CHECK-NEXT: i64 5, label %sw.bb2
    case C...D: return 6;
  }
  // with optimization this switch gets constant folded:
  // CHECK-OPT:      define i32 @y() local_unnamed_addr #2 {
  // CHECK-OPT-NEXT: entry:
  // CHECK-OPT-NEXT:   ret i32 6
  // CHECK-OPT-NEXT: }
  return 0;
}

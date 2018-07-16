// NOTE: Assertions have been autogenerated by utils/update_cc_test_checks.py
// RUN: %cheri_purecap_cc1 -std=c11 %s -emit-llvm -o - -O0 | FileCheck %s
// RUN: %cheri_purecap_cc1 -std=c11 %s -emit-llvm -o - -O2 | FileCheck %s -check-prefix OPT
// Check that we can generate assembly without crashing
// RUN: %cheri_purecap_cc1 -std=c11 %s -S -o - -O2 | FileCheck %s -check-prefix ASM

// This previously crashed in codegen when generating the *p
// CHECK-LABEL: @main(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[RETVAL:%.*]] = alloca i32, align 4, addrspace(200)
// CHECK-NEXT:    [[P:%.*]] = alloca i32 addrspace(200)*, align 16, addrspace(200)
// CHECK-NEXT:    store i32 0, i32 addrspace(200)* [[RETVAL]], align 4
// CHECK-NEXT:    store i32 addrspace(200)* null, i32 addrspace(200)* addrspace(200)* [[P]], align 16
// CHECK-NEXT:    [[ATOMIC_LOAD:%.*]] = load atomic i32 addrspace(200)*, i32 addrspace(200)* addrspace(200)* [[P]] seq_cst, align 16
// CHECK-NEXT:    [[TMP0:%.*]] = load i32, i32 addrspace(200)* [[ATOMIC_LOAD]], align 4
// CHECK-NEXT:    ret i32 [[TMP0]]
//
// OPT-LABEL: @main(
// OPT-NEXT:  entry:
// This uses a crazy alignment value (the maximum) because it is loading a null pointer, not a bug!
// OPT-NEXT:    [[TMP0:%.*]] = load i32, i32 addrspace(200)* null, align 536870912, !tbaa !2
// OPT-NEXT:    ret i32 [[TMP0]]
//
int main(void) {
  _Atomic(int*) p = (int*)0;
  return *p;

  // TODO: why is this not going in the delay slot?
  // ASM-LABEL: main:
  // ASM:      cgetnull	$c1
  // ASM-NEXT: clw	$2, $zero, 0($c1)
  // ASM-NEXT: cjr	$c17
  // ASM-NEXT: nop
}

// This previously crashed in codegen when generating the *p
// CHECK-LABEL: @main2(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[P_ADDR:%.*]] = alloca i32 addrspace(200)*, align 16, addrspace(200)
// CHECK-NEXT:    store i32 addrspace(200)* [[P:%.*]], i32 addrspace(200)* addrspace(200)* [[P_ADDR]], align 16
// CHECK-NEXT:    [[ATOMIC_LOAD:%.*]] = load atomic i32 addrspace(200)*, i32 addrspace(200)* addrspace(200)* [[P_ADDR]] seq_cst, align 16
// CHECK-NEXT:    [[TMP0:%.*]] = load i32, i32 addrspace(200)* [[ATOMIC_LOAD]], align 4
// CHECK-NEXT:    ret i32 [[TMP0]]
//
// OPT-LABEL: @main2(
// OPT-NEXT:  entry:
// OPT-NEXT:    [[TMP0:%.*]] = load i32, i32 addrspace(200)* [[P:%.*]], align 4, !tbaa !2
// OPT-NEXT:    ret i32 [[TMP0]]
//
int main2(_Atomic(int*) p) {
  return *p;

  // ASM-LABEL: main2:
  // TODO: why is this not going in the delay slot?
  // ASM:      clw	$2, $zero, 0($c3)
  // ASM-NEXT: cjr	$c17
  // ASM-NEXT: nop
}

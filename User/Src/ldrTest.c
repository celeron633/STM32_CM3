#include "ldrTest.h"

#include <stdio.h>

void setValue(int *ptr, int val)
{
  asm volatile
  (
    "ldr r0, %1\n"
    "str r0, [%0]\n"
    :
    :"p"(ptr), "m"(val)
    : "r0"
  );
}

void setValue2(int *ptr, int val)
{
  asm volatile
  (
    "mov r0, %1\n"
    "str r0, [%0]\n"
    :
    :"r"(ptr), "r"(val)
    : "r0", "r1"
  );
}

void asmExample(void)
{
      int mspVal;
  int pspVal;
  int inputListVal = 0;

  int xPSRVal = 0;

  // volatile: 汇编不进行优化, 保持原样
  asm volatile (
    "mrs r0, msp\n\t"
    "str r0, %0\n\t"
    "mrs r0, psp\n\t"
    "str r0, %1\n\t"
    "mov r0, #3\n\t"
    "str r0, [%3]\n\t"
    "movs r1, #1\n\t"
    "subs r1, r1, #2\n\t"
  
    "mrs r0, APSR\n\t"
    "str r0, %2"
    // 输出列表
    :"=m"(mspVal), "=m"(pspVal), "=m"(xPSRVal)
    // 输入列表
    :"r"(&inputListVal)
    // 破坏描述
    : "r0", "r1", "memory"
    );

    printf("msp=%08x, psp=%08x\r\n", mspVal, pspVal);
    printf("inputListVal=%08x\r\n", inputListVal);
    printf("xPSR=%08x\r\n", xPSRVal);
}

void ldrTest(void)
{
  uint8_t arr[] = {0x12, 0x34, 0x56, 0x78};
  int outVal = 0;

  printf("&arr=0x%08x\r\n", (int)arr);

  asm volatile
  (
    "mov r0, %1\n"
    "ldr r1, [r0]\n"
    "mov %0, r1\n"
    :"=r"(outVal)
    :"r"(arr)
    :"r0", "r1"
  );
  printf("outVal=0x%08x\r\n", outVal);
  // outVal=0x78563412

  outVal = 0;
  asm volatile
  (
    "mov r0, %1\n"
    "ldrh r1, [r0]\n"
    "mov %0, r1\n"
    :"=r"(outVal)
    :"r"(arr)
    :"r0", "r1"
  );
  printf("outVal=0x%08x\r\n", outVal);
  // outVal=0x00003412

  outVal = 0;
  int arrAddr = (int)arr;
  asm volatile
  (
    "ldr r0, %1\n"
    "ldr r1, [r0]\n"
    "mov %0, r1\n"
    :"=r"(outVal)
    :"m"(arrAddr)
    :"r0"
  );
  printf("outVal=0x%08x\r\n", outVal);
  // outVal=0x78563412

  outVal = 0;
  char *arrAddr2 = (char *)&arr;
  asm volatile
  (
    "ldr r0, %1\n"
    "mov %0, r0\n"
    :"=r"(outVal)
    :"m"(arrAddr2)
    :"r0"
  );
  printf("outVal=0x%08x\r\n", outVal);
  // outVal=0x2000ffd0

}
@echo off
echo AXF to BIN file generation ...

"C:\Program Files\Keil\ARM\ARMCC\bin\fromelf.exe" --bin --output STM3210C_EVAL\STM3210C_EVAL_SysTick.bin STM3210C_EVAL\STM3210C_EVAL.axf 

echo AXF to BIN file generation completed.


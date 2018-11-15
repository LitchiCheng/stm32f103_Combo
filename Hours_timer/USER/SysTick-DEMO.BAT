SET PATH=C:\Keil_v5\ARM\ARMCC\Bin;C:\ProgramData\Oracle\Java\javapath;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files (x86)\PKWARE\pkzipc;C:\Program Files\TortoiseGit\bin;C:\Program Files\Microsoft VS Code\bin;C:\Program Files\Git\cmd;C:\Users\Administrator\AppData\Local\Programs\Python\Python36-32\Scripts\;C:\Users\Administrator\AppData\Local\Programs\Python\Python36-32\;C:\Users\Administrator\AppData\Local\Microsoft\WindowsApps;
SET CPU_TYPE=STM32F103C8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=SysTick-DEMO
SET CPU_CLOCK=0x007A1200
"C:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via ".\startup_stm32f10x_hd._ia"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\main.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_it.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\systick.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\led.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\key.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_gpio.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_rcc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\core_cm3.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\system_stm32f10x.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via ".\STM32-DEMO.lnp"
"C:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" ".\STM32-DEMO.axf" --i32combined --output ".\STM32-DEMO.hex"

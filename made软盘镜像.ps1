# build.ps1
# 最终工程版（无 log，错误即时可见）

$ErrorActionPreference = "Stop"

# ========== 清理旧产物 ==========
Remove-Item *.o, *.bin, *.elf, *.img -ErrorAction SilentlyContinue
Remove-Item driver/*.o -ErrorAction SilentlyContinue

Write-Host "`n=== NASM Compile ===" -ForegroundColor Cyan

nasm -f bin  boot.asm -o boot.bin
nasm -f elf32 entry.asm -o entry.o
nasm -f elf32 stub.asm -o stub.o

Write-Host "`n=== GCC Compile ===" -ForegroundColor Cyan

$GccFlags = @(
    "-m32",
    "-ffreestanding",
    "-nostdlib",
    "-fno-stack-protector",
    "-O0",
    "-g",
    "-Idriver"
)

# 核心内核
i686-elf-gcc @GccFlags -c kernel.c -o kernel.o
i686-elf-gcc @GccFlags -c init_idt.c -o init_idt.o
i686-elf-gcc @GccFlags -c irq.c -o irq.o

# 驱动
i686-elf-gcc @GccFlags -c driver/keyboard.c -o keyboard.o

Write-Host "`n=== Link ===" -ForegroundColor Cyan

i686-elf-ld -m elf_i386 -T linker.ld `
  entry.o init_idt.o irq.o stub.o kernel.o keyboard.o `
  -o kernel.elf

i686-elf-objcopy -O binary kernel.elf kernel.bin

Write-Host "`n=== Symbols ===" -ForegroundColor Cyan
i686-elf-nm kernel.elf

Write-Host "`n=== Create Floppy Image ===" -ForegroundColor Cyan

$boot  = [System.IO.File]::ReadAllBytes("boot.bin")
$kernel = [System.IO.File]::ReadAllBytes("kernel.bin")

$floppy = New-Object byte[] 1474560
[System.Array]::Copy($boot,  $floppy, $boot.Length)
[System.Array]::Copy($kernel, 0, $floppy, 512, $kernel.Length)

$floppy[510] = 0x55
$floppy[511] = 0xAA

[System.IO.File]::WriteAllBytes("floppy.img", $floppy)

Write-Host "`nDone. Kernel size: $($kernel.Length) bytes" -ForegroundColor Green
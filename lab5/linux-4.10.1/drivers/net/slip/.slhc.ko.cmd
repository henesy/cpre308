cmd_drivers/net/slip/slhc.ko := ld -r  -m elf_x86_64 -T ./scripts/module-common.lds --build-id  -o drivers/net/slip/slhc.ko drivers/net/slip/slhc.o drivers/net/slip/slhc.mod.o ;  true

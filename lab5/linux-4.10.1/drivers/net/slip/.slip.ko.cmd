cmd_drivers/net/slip/slip.ko := ld -r  -m elf_x86_64 -T ./scripts/module-common.lds --build-id  -o drivers/net/slip/slip.ko drivers/net/slip/slip.o drivers/net/slip/slip.mod.o ;  true

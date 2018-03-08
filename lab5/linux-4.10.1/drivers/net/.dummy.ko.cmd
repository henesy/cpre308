cmd_drivers/net/dummy.ko := ld -r  -m elf_x86_64 -T ./scripts/module-common.lds --build-id  -o drivers/net/dummy.ko drivers/net/dummy.o drivers/net/dummy.mod.o ;  true

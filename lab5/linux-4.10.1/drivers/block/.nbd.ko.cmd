cmd_drivers/block/nbd.ko := ld -r  -m elf_x86_64 -T ./scripts/module-common.lds --build-id  -o drivers/block/nbd.ko drivers/block/nbd.o drivers/block/nbd.mod.o ;  true

cmd_drivers/block/loop.ko := ld -r  -m elf_x86_64 -T ./scripts/module-common.lds --build-id  -o drivers/block/loop.ko drivers/block/loop.o drivers/block/loop.mod.o ;  true

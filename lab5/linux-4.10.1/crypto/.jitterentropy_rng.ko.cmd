cmd_crypto/jitterentropy_rng.ko := ld -r  -m elf_x86_64 -T ./scripts/module-common.lds --build-id  -o crypto/jitterentropy_rng.ko crypto/jitterentropy_rng.o crypto/jitterentropy_rng.mod.o ;  true

cmd_crypto/hmac.ko := ld -r  -m elf_x86_64 -T ./scripts/module-common.lds --build-id  -o crypto/hmac.ko crypto/hmac.o crypto/hmac.mod.o ;  true

.PHONY: all kernel bootloader clean run

all: kernel bootloader

kernel:
	@$(MAKE) --no-print-directory -C $@

bootloader:
	@$(MAKE) --no-print-directory -C $@

clean:
	@$(MAKE) --no-print-directory -C kernel clean
	@$(MAKE) --no-print-directory -C bootloader clean

run: all
	@./run.sh
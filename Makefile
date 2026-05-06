.PHONY: all
all: kernel bootloader initrd

ISO_NAME := deltaos.iso
ISO_BUILD_DIR := .iso-build
ISO_ROOT := $(ISO_BUILD_DIR)/iso-root
ISO_EFI_DIR := $(ISO_ROOT)/EFI/BOOT
ISO_EFI_IMG := $(ISO_BUILD_DIR)/efiboot.img
ISO_EFI_PADDING_MB := 8
XORRISO := $(shell command -v xorriso 2>/dev/null)
MKISOFS := $(shell command -v mkisofs 2>/dev/null)

.PHONY: kernel
kernel:
	@$(MAKE) --no-print-directory -C $@

.PHONY: bootloader
bootloader:
	@$(MAKE) --no-print-directory -C $@

.PHONY: tools
tools:
	@$(MAKE) --no-print-directory -C tools/darc

.PHONY: user
user:
	@$(MAKE) --no-print-directory -C $@

.PHONY: initrd
initrd: tools user
	@mkdir -p initrd
	@echo "===> Creating initrd.da"
	@./tools/darc/darc create initrd.da initrd

.PHONY: iso
iso: all
	@rm -rf "$(ISO_BUILD_DIR)"
	@mkdir -p "$(ISO_EFI_DIR)"
	@cp bootloader/BOOTX64.EFI "$(ISO_EFI_DIR)/BOOTX64.EFI"
	@cp bootloader/boot/delboot.cfg "$(ISO_EFI_DIR)/delboot.cfg"
	@cp kernel/delta.elf "$(ISO_EFI_DIR)/kernel.bin"
	@cp initrd.da "$(ISO_EFI_DIR)/initrd.da"
	@rm -f "$(ISO_EFI_IMG)" "$(ISO_NAME)"
	@payload_bytes=$$(stat -c %s "$(ISO_EFI_DIR)/BOOTX64.EFI" "$(ISO_EFI_DIR)/delboot.cfg" "$(ISO_EFI_DIR)/kernel.bin" "$(ISO_EFI_DIR)/initrd.da" | awk '{sum += $$1} END {print sum}'); \
		padding_bytes=$$(( $(ISO_EFI_PADDING_MB) * 1024 * 1024 )); \
		total_bytes=$$(( payload_bytes + padding_bytes )); \
		total_sectors=$$(( (total_bytes + 511) / 512 )); \
		if [ $$total_sectors -gt 65535 ]; then \
			echo "error: EFI boot image would exceed the El Torito UEFI size limit (65535 sectors)."; \
			exit 1; \
		fi; \
		truncate -s $$(( total_sectors * 512 )) "$(ISO_EFI_IMG)"
	@mformat -i "$(ISO_EFI_IMG)" ::
	@mmd -i "$(ISO_EFI_IMG)" ::/EFI
	@mmd -i "$(ISO_EFI_IMG)" ::/EFI/BOOT
	@mcopy -i "$(ISO_EFI_IMG)" "$(ISO_EFI_DIR)/BOOTX64.EFI" ::/EFI/BOOT/BOOTX64.EFI
	@mcopy -i "$(ISO_EFI_IMG)" "$(ISO_EFI_DIR)/delboot.cfg" ::/EFI/BOOT/delboot.cfg
	@mcopy -i "$(ISO_EFI_IMG)" "$(ISO_EFI_DIR)/kernel.bin" ::/EFI/BOOT/kernel.bin
	@mcopy -i "$(ISO_EFI_IMG)" "$(ISO_EFI_DIR)/initrd.da" ::/EFI/BOOT/initrd.da
	@cp "$(ISO_EFI_IMG)" "$(ISO_EFI_DIR)/efiboot.img"
	@if [ -n "$(XORRISO)" ]; then \
		echo "===> Creating $(ISO_NAME)"; \
		"$(XORRISO)" -as mkisofs -R -l -J \
			-e EFI/BOOT/efiboot.img \
			-no-emul-boot \
			-o "$(ISO_NAME)" \
			"$(ISO_ROOT)"; \
	elif [ -n "$(MKISOFS)" ]; then \
		echo "===> Creating $(ISO_NAME)"; \
		"$(MKISOFS)" -R \
			-e EFI/BOOT/efiboot.img \
			-no-emul-boot \
			-o "$(ISO_NAME)" \
			"$(ISO_ROOT)"; \
	else \
		echo "error: need xorriso or mkisofs installed to build $(ISO_NAME)."; \
		exit 1; \
	fi

.PHONY: clean
clean:
	@$(MAKE) --no-print-directory -C kernel clean
	@$(MAKE) --no-print-directory -C bootloader clean
	@$(MAKE) --no-print-directory -C user clean
	@$(MAKE) --no-print-directory -C tools/darc clean
	@rm -f initrd.da
	@rm -f "$(ISO_NAME)"
	@rm -rf "$(ISO_BUILD_DIR)"
	@rm -rf initrd/system initrd/init initrd/wm initrd/data.txt

.PHONY: run
run: all
	@./run.sh

.PHONY: web
web:
	@cd web && python -m http.server 8000


FW_SRC_PATH:=fw
FW_DST_PATH:=drivers/net/comcerto
ifdef CONFIG_COMCERTO_DIAG
DIAG_FW_DST_PATH:=diags/utilpe
endif

CLASS_PE_ELF:=class_sbl_c2000_elf
TMU_PE_ELF:=tmu_sbl_c2000_elf
UTIL_PE_ELF:=util_c2000_elf
DIAG_UTIL_PE_ELF:=util_c2000_diag_elf

CLASS_PE_FW:=$(FW_DST_PATH)/$(CLASS_PE_ELF).fw
TMU_PE_FW:=$(FW_DST_PATH)/$(TMU_PE_ELF).fw
UTIL_PE_FW:=$(FW_DST_PATH)/$(UTIL_PE_ELF).fw
ifdef CONFIG_COMCERTO_DIAG
UTIL_PE_DIAG_FW:=$(DIAG_FW_DST_PATH)/$(DIAG_UTIL_PE_ELF).fw
endif

ifdef CONFIG_COMCERTO_DIAG
all: $(CLASS_PE_FW) $(TMU_PE_FW) $(UTIL_PE_FW) $(UTIL_PE_DIAG_FW)
else
all: $(CLASS_PE_FW) $(TMU_PE_FW) $(UTIL_PE_FW) 
endif

$(CLASS_PE_FW): $(FW_SRC_PATH)/$(CLASS_PE_ELF)
ifneq ($(KBUILD_MODULES),)
	$(Q)mkdir -p $(FW_DST_PATH);
endif
	scripts/pfe_fw_convert $(FW_SRC_PATH)/$(CLASS_PE_ELF) $(CLASS_PE_FW)

$(TMU_PE_FW): $(FW_SRC_PATH)/$(TMU_PE_ELF)
ifneq ($(KBUILD_MODULES),)
	$(Q)mkdir -p $(FW_DST_PATH);
endif
	scripts/pfe_fw_convert $(FW_SRC_PATH)/$(TMU_PE_ELF) $(TMU_PE_FW)

$(UTIL_PE_FW): $(FW_SRC_PATH)/$(UTIL_PE_ELF)
ifneq ($(KBUILD_MODULES),)
	$(Q)mkdir -p $(FW_DST_PATH);
endif
	scripts/pfe_fw_convert $(FW_SRC_PATH)/$(UTIL_PE_ELF) $(UTIL_PE_FW)

ifdef CONFIG_COMCERTO_DIAG
$(UTIL_PE_DIAG_FW): $(FW_SRC_PATH)/$(DIAG_UTIL_PE_ELF)
ifneq ($(KBUILD_MODULES),)
	$(Q)mkdir -p $(DIAG_FW_DST_PATH);
endif
	scripts/pfe_fw_convert $(FW_SRC_PATH)/$(DIAG_UTIL_PE_ELF) $(UTIL_PE_DIAG_FW) 
endif

include global.mk

A_CONFIG_SCREEN_MAXIMIZED := 0
A_CONFIG_SCREEN_ZOOM := 4

A_CONFIG_MAKE_CLEAN := genclean

include $(A2X_PATH)/make/dev.mk

Z_DIR_GEN := $(A_DIR_ROOT)/$(A_CONFIG_DIR_SRC)/generated
Z_DIR_GFX := $(A_DIR_ROOT)/assets/gfx
Z_DIR_SFX := $(A_DIR_ROOT)/assets/sfx

Z_GFX_COMPILER := compile-gfx.py
Z_GFX_PNG_FILES := $(notdir $(wildcard $(Z_DIR_GFX)/*.png))
Z_GFX_H_FILES := $(addprefix $(Z_DIR_GEN)/data_gfx_, $(Z_GFX_PNG_FILES:.png=.h))

Z_SFX_COMPILER := compile-sfx.py
Z_SFX_WAV_FILES := $(notdir $(wildcard $(Z_DIR_SFX)/*.wav))
Z_SFX_H_FILES := $(addprefix $(Z_DIR_GEN)/data_sfx_, $(Z_SFX_WAV_FILES:.wav=.h))

$(A_DIR_OBJ_APP)/util_graphics.c.o : $(Z_GFX_H_FILES)
$(A_DIR_OBJ_APP)/util_sound.c.o : $(Z_SFX_H_FILES)

$(Z_DIR_GEN)/data_gfx_%.h: $(Z_DIR_GFX)/%.png $(Z_GFX_COMPILER)
	@ mkdir -p $(@D)
	python3 $(Z_GFX_COMPILER) $(Z_DIR_GFX)/palette.png $< $(patsubst %.png, %, $(notdir $<)) > $@

$(Z_DIR_GEN)/data_sfx_%.h: $(Z_DIR_SFX)/%.wav $(Z_SFX_COMPILER)
	@ mkdir -p $(@D)
	python3 $(Z_SFX_COMPILER) $< $(patsubst %.wav, %, $(notdir $<)) > $@

genclean:
	rm -rf $(Z_DIR_GEN)

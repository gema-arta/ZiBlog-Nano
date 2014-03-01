#----------------------------------------------------------
#	Denis Zheleznyakov aka ZiB @ http://ziblog.ru
#----------------------------------------------------------

SRC_C  = main.c
SRC_C += macros.c
SRC_C += device.c
SRC_C += network.c
SRC_C += access_points.c
SRC_C += mcu_usart.c
SRC_C += stm8_interrupt_vector.c
SRC_C += stm8l15x_it.c
SRC_C += stm8l15x_clk.c
SRC_C += stm8l15x_usart.c
SRC_C += stm8l15x_tim1.c
SRC_C += stm8l15x_tim4.c

CROSS_PATH = D:/Tools/Cosmic/4.3.7/
CROSS = $(CROSS_PATH)/

INCLUDES += -imcu
INCLUDES += -imcu/peripherals
INCLUDES += -imcu/std_lib/inc
INCLUDES += -imcu/std_lib/src
INCLUDES += -iutility
INCLUDES += -idevice

VPATH += mcu
VPATH += mcu/peripherals
VPATH += mcu/std_lib/inc
VPATH += mcu/std_lib/src
VPATH += utility
VPATH += device

FLAGS_C  = +debug +warn +modsl0 -pxp -pp +compact +split -l -gge -pge

all: clean
	@make -s -j job_target
	@echo '-----------------------------------------------------------'
	@$(CROSS)cobj -n target/target.sm8 | $(CROSS)cosmic_format 'mcu/stm8l151k6.lsf'

job_target: target.elf

%.elf: $(SRC_C:%.c=target/%.o)
	@echo '-----------------------------------------------------------'
	@echo linking: $@
	@$(CROSS)clnk -l'$(CROSS)/Lib' -o'target/target.sm8' -m'target/target.map' -sa 'mcu/stm8l151k6.lsf'
	@$(CROSS)cvdwarf target/target.sm8
	@$(CROSS)chex -o target/target.s19 target/target.sm8
	@rm -f target/*.o
	
$(SRC_C:%.c=target/%.o): target/%.o: %.c
	@$(CROSS)cxstm8 $(FLAGS_C) $(INCLUDES) -i. -i$(CROSS)/Hstm8 -cl'target' -co'target' $<

size:
	@echo '-----------------------------------------------------------'
	@$(CROSS)cobj -n target/target.sm8 | $(CROSS_PATH)cosmic_format 'mcu/stm8l151k6.lsf'
	
clean:
	@echo '-----------------------------------------------------------'
	@mkdir -p target
	@rm -f target/*.*

.PHONY : all clean
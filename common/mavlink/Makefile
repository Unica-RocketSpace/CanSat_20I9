
# Каталог с входными файлами
MSGDEFS_DIR = message_definitions/v1.0
# Список имен входных файлов
MSGDEFS_NAMES = UNISAT.xml
# Список путей до входных файлов
MSGDEFS = $(addprefix $(MSGDEFS_DIR)/, $(MSGDEFS_NAMES))

# Базовая строка вызова для сишнего генератора
C_GENERATOR_CALL = python3 -m pymavlink.tools.mavgen --lang=C --wire-protocol=2.0
# Выходной каталог для сишных файлов
C_GENERATOR_OUTDIR = generated/c
# Файл-штамп, использующийся для проверки сишных файлов на устаревание
C_GENERATOR_STAMP = $(C_GENERATOR_OUTDIR)/include/mavlink/protocol.h


$(C_GENERATOR_STAMP): $(MSGDEFS)
	@echo "Генерю сишники для файлов $(MSGDEFS)"
	$(C_GENERATOR_CALL) --output=$(C_GENERATOR_OUTDIR)/include/mavlink $(MSGDEFS)


.PHONY: gen-c all clean 


gen-c: $(C_GENERATOR_STAMP)

all: gen-c

clean:
	rm -rf ./generated

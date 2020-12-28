CC = gcc
CFLAGS = -std=gnu11 -O2 -Wextra -Werror -pedantic -Wno-unused-result
KERNEL_SRC = src-kernel/list.c src-kernel/main.c src-kernel/plugin.c
PLUGIN_TXT_SRC = src-kernel/list.c src-plugin-txt/sort.c src-plugin-txt/ui.c 
PLUGIN_CSV_SRC = src-kernel/list.c src-plugin-csv/sort.c src-plugin-csv/ui.c 
KERNEL = cmplxsort
PLUGIN_TXT = plugins/txt.plugin
PLUGIN_CSV = plugins/csv.plugin
.PHONY: clean

all: $(KERNEL_SRC) $(PLUGIN_TXT_SRC) $(PLUGIN_CSV_SRC)
	@echo "======"
	@echo "cleaning environment..."
	$(RM) *.o *.a $(KERNEL) $(PLUGIN_TXT) $(PLUGIN_CSV) export.csv *.txt

	@echo "======"
	@echo "compiling..."
	$(CC) $(CFLAGS) -o $(KERNEL) $(KERNEL_SRC) -ldl
	$(CC) $(CFLAGS) -shared -fPIC -o $(PLUGIN_TXT) $(PLUGIN_TXT_SRC)
	$(CC) $(CFLAGS) -shared -fPIC -o $(PLUGIN_CSV) $(PLUGIN_CSV_SRC)

	@echo "======"
	@echo "running testcases..."
	./cmplxsort testcases/rand_int.txt inc
	./cmplxsort testcases/test.csv inc

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	$(RM) *.o *.a $(KERNEL) $(PLUGIN_TXT) $(PLUGIN_CSV) export.csv *.txt

kernel: $(KERNEL_SRC)
	$(CC) $(CFLAGS) -o $(KERNEL) $(KERNEL_SRC) -ldl

plugin_txt: $(PLUGIN_TXT_SRC)
	$(CC) $(CFLAGS) -shared -fPIC -o $(PLUGIN_TXT) $(PLUGIN_TXT_SRC)

plugin_csv: $(PLUGIN_CSV_SRC)
	$(CC) $(CFLAGS) -shared -fPIC -o $(PLUGIN_CSV) $(PLUGIN_CSV_SRC)

run_test_txt:
	./cmplxsort testcases/rand_int.txt inc

run_test_csv:
	./cmplxsort testcases/test.csv inc

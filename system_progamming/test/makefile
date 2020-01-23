objects =  vsma-tests.o ../lib/vsma.o
vsma-header = ../include/vsma.h

prog: $(objects) $(vsma-header)
	@gcc -ansi -o prog -pedantic-errors -Wall -Wextra -g \
	$(objects)
	@echo 'creating prog'

vsma-tests.o: vsma-tests.c $(vsma-header)
	@gcc -c -ansi -pedantic-errors -Wall -Wextra -g \
	vsma-tests.c 
	@echo 'creating vsma-tests.o'

../lib/vsma.o: ../lib/vsma.c $(vsma-header)
	@gcc -c -ansi -pedantic-errors -Wall -Wextra -g \
	../lib/vsma.c -o $@
	@echo 'creating vsma.o'

.PHONY: clean
clean: 
	@rm vsma-tests.o ../lib/vsma.o prog
	@echo 'removing all object files'


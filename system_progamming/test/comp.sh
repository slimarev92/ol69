#!/bin/bash

#gcc -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -o prog \
#vsma-tests.c ../lib/vsma.c
#echo 'GC'

#gcc -ansi -o prog -pedantic-errors -Wall -Wextra -g \
#vsma-tests.c ../lib/vsma.c
#echo 'GD'

gcc -ansi -o prog -pedantic-errors -Wall -Wextra -g \
fsma-tests.c ../lib/fsma.c
echo 'GD FSMA'

#gcc -ansi -o prog -pedantic-errors -Wall -Wextra -g \
#vsma-tests.c ../lib/vsma-ben.c
#echo 'GD BEN'



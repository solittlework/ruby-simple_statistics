require 'mkmf'
$CFLAGS << ' -Wall -std=c11'

create_makefile('sstat')
CFLAGS = -Wall -g -I$(ORACLE_HOME)/rdbms/demo -I$(ORACLE_HOME)/network/public
CC=gcc

ocitrace.so: ocitrace.o
	$(LD) -o $@ -shared $<

